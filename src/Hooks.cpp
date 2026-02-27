#include "Hooks.h"
#include "Manager.h"

namespace Hook
{
	// TODO: Port this to constTranslation
	struct MapMarkerDataHook //REFR FULL
	{
		static RE::TESFullName* thunk(RE::TESObjectREFR* marker)
		{
			auto result = func(marker);

			if (marker && !marker->IsPersistent())
			{
				SKSE::log::info("Not persistent: {:08X}", marker->formID);
			}

			/*std::string newDescription{};
			if (result && Manager::GetSingleton()->getREFR(marker->formID, newDescription))
			{
				result->SetFullName(newDescription.c_str());
			}*/

			return result;
		};
		static inline REL::Relocation<decltype(thunk)> func;


		static void Install()
		{
			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(18755, 19216), REL::Relocate(0xA6, 0xE4) };
			stl::write_thunk_call<MapMarkerDataHook>(target1.address());
		}
	};

	struct GetLogEntryHook //QUST CNAM
	{
		static const char* thunk(RE::TESQuestStageItem* item, const RE::TESQuest* ownerQuest)
		{
			auto result = func(item, ownerQuest);

			const char* translation = nullptr;
			if (ownerQuest && item)
			{
				const std::uint32_t uniqueID = item->index + ownerQuest->currentStage;
				translation = Manager::GetSingleton()->getTranslation(ownerQuest->formID, uniqueID, TranslationType::kRuntimeLegacy, result);
			}

			return translation == nullptr ? result : translation;
		}
		static inline REL::Relocation<decltype(thunk)> func;

		static void Install()
		{
			REL::Relocation<std::uintptr_t> target{ REL::VariantID(24778, 25259, 0x0) };
			stl::hook_function_prologue<GetLogEntryHook, 6>(target.address());
		}
	};

	struct NpcNameFileStreamHook //NPC FULL - Why this way? Because templates use one of their templates names and it's horrible to change it based on formID
	{
		static void thunk(RE::TESFullName* npcFullname, RE::TESFile* file)
		{
			func(npcFullname, file);

			static std::once_flag flag;
			std::call_once(flag, [&]() {
				// Run our parser here, DataHandler already build the file list. 
				// The reason is, that there is no reliable way in which I could tell which template the NPC copied from at DataLoaded
				const auto manager = Manager::GetSingleton();
				manager->enumerateLoadOrder();
				manager->parseTranslationFiles();
				});

			if (!npcFullname)
				return;

			const auto npc = skyrim_cast<const RE::TESForm*>(npcFullname);
			const char* translation = nullptr;
			if (npc)
			{
				translation = Manager::GetSingleton()->getTranslation(npc->formID, 0, TranslationType::kRuntime2);
			}

			if (translation)
			{
				npcFullname->SetFullName(translation);
			}
		}
		static inline REL::Relocation<decltype(thunk)> func;

		static void Install()
		{
			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(24159, 24663), REL::Relocate(0x7CE, 0x924) };
			stl::write_thunk_call<NpcNameFileStreamHook>(target1.address());
		}
	};

	struct GetDescription
	{
		static void thunk(RE::TESDescription* description, RE::BSString& out, const RE::TESForm* parent, std::uint32_t chunkID)
		{
			// In SE we hook LoadDescriptionFromFile func where the game already castet the description to TESForm
			// AE inlined, we hook the GetDescription where the game didn't cast it yet
			const auto safeForm = parent ? parent : skyrim_cast<const RE::TESForm*>(description);

			func(description, out, safeForm, chunkID); // call original func with our cast, so we don't cast twice at least
			if (!safeForm)
				return;

			const char* translation = nullptr;

			// 0x4D414E43 == 'MANC' (CNAM)
			const bool isCNAM = chunkID == 'MANC';
			const bool isDESC = chunkID == 'CSED';
			if (isDESC || isCNAM) // skip garbage data, not caused by Skyrim but other modders
			{
				const auto type = isDESC ? TranslationType::kRuntime1 : TranslationType::kRuntime2;
				translation = Manager::GetSingleton()->getTranslation(safeForm->formID, 0, type);
			}

			if (translation)
			{
				out = translation;
			}
		}
		static inline REL::Relocation<decltype(thunk)> func;

		static void Install()
		{
			REL::Relocation<std::uintptr_t> target{ REL::VariantID(14401, 14552, 0x1A0300) };
			stl::hook_function_prologue<GetDescription, 6>(target.address());
		}
	};

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

			const auto manager = Manager::GetSingleton();

			for (auto response = result->head; response; response = response->next)
			{
				if (!response)
					continue;

				SKSE::log::debug("Original string: {} - TopicInfoFormID: {:08X} - LinkedResponseFormID: {:08X} - ResponseNumber: {}", response->responseText.c_str(), topicInfo->formID, responseTopicInfo->formID, response->responseNumber);

				const auto translation = manager->getTranslation(responseTopicInfo->formID, response->responseNumber, TranslationType::kRuntime2);
				if (translation)
				{
					RE::setBSFixedString(response->responseText, translation);
				}
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
		static void thunk(RE::MenuTopicManager::Dialogue& out, const char* source, std::uint64_t maxLen) //Skyrim is not only passing BSStrings into this function
		{	
			const auto manager = Manager::GetSingleton();
			const char* translation = nullptr;

			const auto parent = out.parentTopic;
			if (parent)
			{
				translation = manager->getTranslation(parent->formID, 0, TranslationType::kRuntime1);
			}

			const auto parentInfo = out.parentTopicInfo;
			if (parentInfo)
			{
				const auto rnamTranslation = manager->getTranslation(parentInfo->formID, 0, TranslationType::kRuntime2);
				if (rnamTranslation)
				{
					translation = rnamTranslation;
				}
			}

			func(out, translation == nullptr ? source : translation, maxLen);	
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

	void InstallHooks()
	{
		NpcNameFileStreamHook::Install();
		GetDescription::Install();
		GetLogEntryHook::Install();
		//MapMarkerDataHook::Install();
		GetResponseListHook::Install();
		DialogueMenuTextHook::Install();

		SKSE::log::info("Installed Hooks!");
	}
}