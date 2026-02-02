#include "MiscHooks.h"
#include "DialogueHooks.h"
#include "Config.h"
#include "Utils.h"
#include "Manager.h"

namespace Hook
{
	struct GetResponseListHook //INFO NAM1
	{
		static RE::TESTopicInfo::TESResponseList* thunk(RE::TESTopicInfo* topicInfo, RE::TESTopicInfo::TESResponseList* list)
		{
			auto result = func(topicInfo, list);
			if (!topicInfo || !result)
				return result;

			auto responseTopicInfo = topicInfo;
			auto linkedResponseInfo = topicInfo->dataInfo;

			if (linkedResponseInfo)
			{
				for (auto i = linkedResponseInfo; i; i = i->dataInfo)
				{
					responseTopicInfo = i;
				}
			}

			const std::string fileName = Utils::getModName(responseTopicInfo);
			const RE::FormID trimmedFormID = Utils::getTrimmedFormID(responseTopicInfo);
			const auto manager = Manager::GetSingleton();

			for (auto response = result->head; response; response = response->next)
			{
				SKSE::log::debug("Original string: {} - TopicInfoFormID: {:08X} - LinkedResponseFormID: {:08X} - ResponseNumber: {} - Plugin: {}", response->responseText.c_str(), topicInfo->formID, responseTopicInfo->formID, response->responseNumber, fileName);

				manager->getINFO_NAM1(trimmedFormID, fileName, response->responseNumber, response->responseText);
			}

			return result;
		}
		static inline REL::Relocation<decltype(thunk)> func;

		static void Install()
		{
			REL::Relocation<std::uintptr_t> target{ REL::VariantID(25083, 0, 0x0) };
			stl::hook_function_prologue<GetResponseListHook, 6>(target.address());
		}
	};

	struct DialogueMenuTextHook //DIAL FULL, INFO RNAM
	{
		static void thunk(RE::MenuTopicManager::Dialogue& a_out, const char* source, std::uint64_t maxLen) //Skyrim is not only passing BSStrings into this function
		{
			func(a_out, source, maxLen);

			const auto manager = Manager::GetSingleton();
			manager->getDIAL(Utils::getTrimmedFormID(a_out.parentTopic), Utils::getModName(a_out.parentTopic), a_out.topicText);
			manager->getDIAL(Utils::getTrimmedFormID(a_out.parentTopicInfo), Utils::getModName(a_out.parentTopicInfo), a_out.topicText); //INFO RNAM always overwrites DIAL FULL of parenttopic

		};
		static inline REL::Relocation<decltype(thunk)> func;

		static void Install()
		{
			constexpr auto address = RELOCATION_ID(34434, 35254);

			REL::Relocation<std::uintptr_t> target1{ address, REL::Relocate(0xCC, 0x226) };
			stl::write_thunk_call<DialogueMenuTextHook>(target1.address());

			if (REL::Module::IsAE())
			{
				REL::Relocation<std::uintptr_t> target2{ address, 0x115 };
				stl::write_thunk_call<DialogueMenuTextHook>(target2.address());
			}
		}
	};

	void InstallDialogueHooks()
	{
		GetResponseListHook::Install();
		DialogueMenuTextHook::Install();
	}

}