#include "MiscHooks.h"
#include "DialogueHooks.h"
#include "Utils.h"
#include "Processor.h"


namespace Hook
{
	struct DialogueConstructHook //INFO NAM1
	{
		static void thunk(RE::TESTopicInfo::ResponseData* a_response, char* a_voiceFilePath, RE::BGSVoiceType* a_voiceType, RE::TESTopic* a_topic, RE::TESTopicInfo* a_topicInfo)
		{
			func(a_response, a_voiceFilePath, a_voiceType, a_topic, a_topicInfo);

			size_t key = Utils::combineHashWithIndex(a_topicInfo->formID, a_response->responseNumber, a_topicInfo->GetFile()->fileName);

			auto it = g_INFO_NAM1_Map.find(key);
			if (it != g_INFO_NAM1_Map.end())
			{
				g_INFO_NAM1_TEMP_Map.emplace(a_response->responseText.c_str(), it->second); //Can't set with responseText = "newText". So setting it with the hook below. The function is called directly after this construct function.
				g_INFO_NAM1_Map.erase(it);
			}
		}
		static inline REL::Relocation<decltype(thunk)> func;


		static void Install()
		{
			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(34429, 35249), REL::VariantOffset(0xDE, 0xDE, 0xDE) };
			stl::write_thunk_call<DialogueConstructHook>(target1.address());
			SKSE::log::info("DialogueConstructHook hooked at address: {:x} and offset: {:x}", target1.address(), target1.offset());
		}

	};

	struct DialogueStreamHook //INFO NAM1
	{
		static void thunk(RE::BSString* a_out, std::uint64_t a2, std::uint64_t a3)
		{
			func(a_out, a2, a3);

			auto it = g_INFO_NAM1_TEMP_Map.find(a_out->c_str());

			if (it != g_INFO_NAM1_TEMP_Map.end())
			{
				*a_out = it->second;
			}

		}
		static inline REL::Relocation<decltype(thunk)> func;


		static void Install()
		{
			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(34429, 35249), REL::VariantOffset(0x61, 0x61, 0x61) };
			stl::write_thunk_call<DialogueStreamHook>(target1.address());
			SKSE::log::info("DialogueStreamHook hooked at address: {:x} and offset: {:x}", target1.address(), target1.offset());
		}

	};

	struct DialogueMenuTextHook //DIAL FULL, INFO RNAM
	{
		static void thunk(RE::MenuTopicManager::Dialogue& a_out, char* a2, std::uint64_t a3) //Skyrim is not only passing BSStrings into this function
		{
			func(a_out, a2, a3);

			size_t key1 = Utils::combineHash(a_out.parentTopic->formID, a_out.parentTopic->GetFile()->fileName);
			auto it1 = g_DIAL_FULL_Map.find(key1);

			if (it1 != g_DIAL_FULL_Map.end())
			{
				a_out.topicText = it1->second;
			}

			size_t key2 = Utils::combineHash(a_out.parentTopicInfo->formID, a_out.parentTopicInfo->GetFile()->fileName);
			auto it2 = g_INFO_RNAM_Map.find(key2); //INFO RNAM always overwrites DIAL FULL of parenttopic

			if (it2 != g_INFO_RNAM_Map.end())
			{
				a_out.topicText = it2->second;
			}

		};
		static inline REL::Relocation<decltype(thunk)> func;


		static void Install()
		{
			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(34434, 35254), REL::VariantOffset(0xCC, 0x226, 0xCC) };
			stl::write_thunk_call<DialogueMenuTextHook>(target1.address());
			SKSE::log::info("DialogueMenuTextHook hooked at address: {:x} and offset: {:x}", target1.address(), target1.offset());

			if (REL::Module::IsAE())
			{
				REL::Relocation<std::uintptr_t> target2{ RELOCATION_ID(0, 35254), REL::VariantOffset(0x0, 0x115, 0x0) };
				stl::write_thunk_call<DialogueMenuTextHook>(target2.address());
				SKSE::log::info("DialogueMenuTextHook hooked at address: {:x} and offset: {:x}", target2.address(), target2.offset());
			}
		}
	};


	void InstallDialogueHooks()
	{
		Hook::DialogueStreamHook::Install();
		Hook::DialogueConstructHook::Install();
		Hook::DialogueMenuTextHook::Install();
	}

}