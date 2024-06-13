#include "MiscHooks.h"
#include "DialogueHooks.h"
#include "Utils.h"
#include "Processor.h"


namespace Hook
{
	struct DialogueResponseHook //INFO NAM1
	{
		static bool SetBSString(RE::BSString& a_str, char* a_buffer, int32_t a3)
		{
			using func_t = decltype(&SetBSString);
			REL::Relocation<func_t> func{ RELOCATION_ID(10979, AE_CHECK(SKSE::RUNTIME_SSE_1_6_1130, 11044, 439876)) };
			return func(a_str, a_buffer, a3);
		}

		struct TrampolineCall : Xbyak::CodeGenerator
		{
			TrampolineCall(std::uintptr_t ret, std::uintptr_t func)
			{
				Xbyak::Label funcLabel;
				Xbyak::Label retnLabel;

				mov(r8, rbp); //r8 would always be 0. So use it for a_topicInfo
				mov(r9, rsi); //a_response

				sub(rsp, 0x20); //allocate for call
				call(ptr[rip + funcLabel]); //call thunk
				add(rsp, 0x20);

				jmp(ptr[rip + retnLabel]); //jump back to original code

				L(funcLabel);
				dq(func);

				L(retnLabel);
				dq(ret);
			}
		};

		static void thunk(RE::BSString& a_str, char* a_buffer, RE::TESTopicInfo* a_topicInfo, RE::TESTopicInfo::ResponseData* a_response)
		{
			const std::string& fileName = Utils::GetModName(a_topicInfo);
			const RE::FormID trimmedFormID = Utils::GetTrimmedFormID(a_topicInfo);

			SKSE::log::debug("Original string: {}", a_response->responseText.c_str());
			SKSE::log::debug("FormID: {0:08X}", a_topicInfo->formID);
			SKSE::log::debug("TrimmedFormID: {0:08X}", trimmedFormID);
			SKSE::log::debug("Number: {}", a_response->responseNumber);
			SKSE::log::debug("Plugin: {}", fileName);

			const auto it = g_INFO_NAM1_Map.find(std::to_string(trimmedFormID) + fileName);
			if (it != g_INFO_NAM1_Map.end())
			{
				const std::uint8_t reponseNumber = a_response->responseNumber;
				const auto& value = it->second;

				auto contains = std::find_if(value.begin(), value.end(),
					[&reponseNumber](const Hook::Value& v) { return v.first == reponseNumber; });

				if (contains != value.end())
				{
					a_str = contains->second;
					char* newText = const_cast<char*>(a_str.c_str());
					SetBSString(a_str, newText, 0);
					return;
				}
			}

			SetBSString(a_str, a_buffer, 0);

		}

		static void Install()
		{
			constexpr auto targetAddress = RELOCATION_ID(34429, 35249);
			REL::Relocation<std::uintptr_t> target{ targetAddress, 0x61 }; //same for all
			auto trampolineJmp = TrampolineCall(target.address() + 0x5, stl::unrestricted_cast<std::uintptr_t>(thunk));

			auto& trampoline = SKSE::GetTrampoline();
			SKSE::AllocTrampoline(trampolineJmp.getSize());
			auto result = trampoline.allocate(trampolineJmp);
			SKSE::AllocTrampoline(14);
			trampoline.write_branch<5>(target.address(), (std::uintptr_t)result);

			REL::safe_write(targetAddress.address() + 0x5B, REL::NOP3, 0x3); // just in case
		}
	};

	struct SharedResponseDataHook // Hacky workaround for records with DNAM (shared response data)
	{
		struct MoveParent : Xbyak::CodeGenerator
		{
			MoveParent() // move RE::TESTopicInfo* as 3rd parameter
			{
				mov(r8, r12);
			}
		};

		static bool thunk(RE::TESFile* a_file, void* a_buf, std::uint64_t a_topicInfo)
		{
			auto result = func(a_file, a_buf, 4u); // invoke with 4u from original code

			const RE::TESTopicInfo* parentInfo = reinterpret_cast<RE::TESTopicInfo*>(a_topicInfo);
			std::uint32_t dnamFormID = *static_cast<std::uint32_t*>(a_buf);
			const std::uint32_t firstTwoHexDigits = dnamFormID >> 24; // extract first two digits and convert to decimal
			RE::FormID formID = dnamFormID & 0xFFFFFF; // remove file index -> 0x00XXXXXX

			const RE::TESFile* lookupFile = a_file;
			if (firstTwoHexDigits < a_file->masterCount)
			{
				lookupFile = a_file->masterPtrs[firstTwoHexDigits];
			}

			if (lookupFile == nullptr)
			{
				SKSE::log::error("INFO DNAM error, please report it in our Discord!");
				SKSE::log::error("FormID - Parent: {0:08X}", parentInfo->formID);
				SKSE::log::error("FormID - DNAM: {0:08X}", dnamFormID);
				SKSE::log::error("FormID - ID: {}", firstTwoHexDigits);
				SKSE::log::error("File - Parent: {}", a_file->GetFilename());
				return result;
			}

			if (lookupFile->IsLight())
			{
				formID &= 0xFFF;
			}

			const auto it = g_INFO_NAM1_Map.find(std::to_string(formID) + lookupFile->GetFilename().data());
			if (it != g_INFO_NAM1_Map.end())
			{
				const std::string newKey = std::to_string(Utils::GetTrimmedFormID(parentInfo)) + Utils::GetModName(parentInfo);
				g_INFO_NAM1_Map.insert({ newKey, it->second });
			}

			return result;
		}

		static inline REL::Relocation<decltype(thunk)> func;

		static void Install()
		{
			constexpr auto targetAddress = REL::VariantID(25058, 25597, 0x3A1310);

			REL::Relocation<std::uintptr_t> swapTarget{ targetAddress, REL::Relocate(0x332, 0x39F) };
			REL::safe_fill(swapTarget.address(), REL::NOP, 0x6);

			auto newCode = MoveParent();
			assert(newCode.getSize() <= 0x6);
			REL::safe_write(swapTarget.address(), newCode.getCode(), newCode.getSize());

			REL::Relocation<std::uintptr_t> target{ targetAddress, REL::Relocate(0x33F, 0x3AC) };
			stl::write_thunk_call<SharedResponseDataHook>(target.address());
		}
	};

	struct DialogueMenuTextHook //DIAL FULL, INFO RNAM
	{
		static void thunk(RE::MenuTopicManager::Dialogue& a_out, char* a2, std::uint64_t a3) //Skyrim is not only passing BSStrings into this function
		{
			func(a_out, a2, a3);

			const auto key1 = Utils::combineHash(a_out.parentTopic->formID, Utils::GetModName(a_out.parentTopic));
			const auto it1 = g_DIAL_FULL_Map.find(key1);

			if (it1 != g_DIAL_FULL_Map.end())
			{
				a_out.topicText = it1->second;
			}

			const auto key2 = Utils::combineHash(a_out.parentTopicInfo->formID, Utils::GetModName(a_out.parentTopicInfo));
			const auto it2 = g_INFO_RNAM_Map.find(key2); //INFO RNAM always overwrites DIAL FULL of parenttopic

			if (it2 != g_INFO_RNAM_Map.end())
			{
				a_out.topicText = it2->second;
			}

		};
		static inline REL::Relocation<decltype(thunk)> func;


		static void Install()
		{
			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(34434, 35254), REL::Relocate(0xCC, 0x226) };
			stl::write_thunk_call<DialogueMenuTextHook>(target1.address());

			if (REL::Module::IsAE())
			{
				REL::Relocation<std::uintptr_t> target2{ RELOCATION_ID(0, 35254), REL::VariantOffset(0x0, 0x115, 0x0) };
				stl::write_thunk_call<DialogueMenuTextHook>(target2.address());
			}
		}
	};

	void InstallDialogueHooksPostLoad()
	{
		SharedResponseDataHook::Install();
	}

	void InstallDialogueHooks()
	{
		DialogueResponseHook::Install();
		DialogueMenuTextHook::Install();
	}

}