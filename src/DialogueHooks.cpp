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
			size_t key = Utils::combineHashWithIndex(trimmedFormID, a_response->responseNumber, fileName);

			SKSE::log::debug("Original string: {}", a_response->responseText.c_str());
			SKSE::log::debug("FormID: {0:08X}", a_topicInfo->formID);
			SKSE::log::debug("TrimmedFormID: {0:08X}", trimmedFormID);
			SKSE::log::debug("Number: {}", a_response->responseNumber);
			SKSE::log::debug("Plugin: {}", fileName);

			auto it = g_INFO_NAM1_Map.find(key);
			if (it != g_INFO_NAM1_Map.end())
			{
				a_str = it->second;
				char* newtext = const_cast<char*>(a_str.c_str());

				SetBSString(a_str, newtext, 0);
			}
			else
			{
				SetBSString(a_str, a_buffer, 0);
			}
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

	struct DialogueMenuTextHook //DIAL FULL, INFO RNAM
	{
		static void thunk(RE::MenuTopicManager::Dialogue& a_out, char* a2, std::uint64_t a3) //Skyrim is not only passing BSStrings into this function
		{
			func(a_out, a2, a3);

			size_t key1 = Utils::combineHash(a_out.parentTopic->formID, Utils::GetModName(a_out.parentTopic));
			auto it1 = g_DIAL_FULL_Map.find(key1);

			if (it1 != g_DIAL_FULL_Map.end())
			{
				a_out.topicText = it1->second;
			}

			size_t key2 = Utils::combineHash(a_out.parentTopicInfo->formID, Utils::GetModName(a_out.parentTopicInfo));
			auto it2 = g_INFO_RNAM_Map.find(key2); //INFO RNAM always overwrites DIAL FULL of parenttopic

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


	void InstallDialogueHooks()
	{
		DialogueResponseHook::Install();
		DialogueMenuTextHook::Install();
	}

}