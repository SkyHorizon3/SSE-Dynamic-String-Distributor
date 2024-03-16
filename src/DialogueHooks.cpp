#include "MiscHooks.h"
#include "DialogueHooks.h"
#include "Utils.h"
#include "Processor.h"


namespace Hook
{
	struct DialogueConstructHook //INFO NAM1
	{
		static void thunk(RE::TESTopicInfo::ResponseData* response, char* voicePath, RE::BGSVoiceType* voiceType, RE::TESTopic* topic, RE::TESTopicInfo* topicInfo)
		{
			func(response, voicePath, voiceType, topic, topicInfo);

			size_t key = Utils::combineHash(topicInfo->formID, response->responseNumber);

			auto it = g_INFO_NAM1_Map.find(key);
			if (it != g_INFO_NAM1_Map.end())
			{
				g_INFO_NAM1_TEMP_Map.emplace(response->responseText.c_str(), it->second);
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

	void InstallDialogueHooks()
	{
		Hook::DialogueStreamHook::Install();
		Hook::DialogueConstructHook::Install();
	}

}