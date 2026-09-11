#include "Hooks.h"
#include "Manager.h"

namespace Hook
{
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
			REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(24778, 25259) };
			stl::hook_function_prologue<GetLogEntryHook, 6>(target.address());
		}
	};

	struct GetDescription
	{
		static void thunk(RE::TESDescription* description, RE::BSString& out, const RE::TESForm* parent, std::uint32_t chunkID)
		{
			// In SE we hook LoadDescriptionFromFile func where the game already castet the description to TESForm
			// AE inlined, we hook the GetDescription func where the game didn't cast it yet
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

				const auto translation = manager->getTranslation(responseTopicInfo->formID, response->responseNumber, TranslationType::kRuntimeIndex);
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
			REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(25083, 25626) };
			stl::hook_function_prologue<GetResponseListHook, 6>(target.address());
		}
	};

	struct DialogueMenuTextHook //DIAL FULL, INFO RNAM
	{
		static void thunk(RE::MenuTopicManager::Dialogue& out, const char* source, std::uint64_t maxLen)
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

	struct DataHandlerInitAllForms
	{
		// This can run multiple times, rebuild our stuff here too (since there can be new plugin files)
		// called directly after all forms are loaded (but not fully initialized yet)
		static void thunk(RE::TESDataHandler* handler)
		{
			Manager::GetSingleton()->parseTranslationFiles();
			func(handler); // first NPCFullNameCopyComponent calls run here
		}
		static inline REL::Relocation<decltype(thunk)> func;

		static void Install()
		{
			// datahandler compile files
			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(13645, 13753), REL::Relocate(0x341, 0x363) };
			stl::write_thunk_call<DataHandlerInitAllForms>(target1.address());

			// plugin hot reload
			REL::Relocation<std::uintptr_t> target2{ RELOCATION_ID(13672, 13785), REL::Relocate(0xB05, 0xB8B) };
			stl::write_thunk_call<DataHandlerInitAllForms>(target2.address());
		}
	};

	struct NPCFullNameCopyComponent
	{
		// TODO: verify that this really covers all templates
		static void thunk(RE::TESFullName* to, RE::BaseFormComponent* from)
		{
			auto fromForm = skyrim_cast<RE::TESForm*>(from);
			if (fromForm)
			{
				// NPCs copy their FullName all the time
				// also reload NPC_ SHRT here, gets copied afterwards
				Manager::GetSingleton()->reloadConstTranslation(fromForm);
			}

			func(to, from); // call original just for compatibility reasons
		}
		static inline REL::Relocation<decltype(thunk)> func;

		static void Install()
		{
			REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(14544, 14716) };
			stl::hook_function_prologue<NPCFullNameCopyComponent, 6>(target.address());
		}
	};

	struct ReconstructForms
	{
		// runs on save load right after finished
		// runs in the big main reset the game performs for example on main menu quit
		static void thunk(RE::BGSConstructFormsInAllFilesMap* data, bool stillLoading)
		{
			func(data, stillLoading);
			if (!data || data->count == 0)
				return;

			const auto mgr = Manager::GetSingleton();
			for (int i = 0; i < 3; i++)
			{
				const auto& list = data->constructedForms.data[i];
				for (const auto& entry : list)
				{
					const auto& form = entry.form;
					if (!form)
						continue;

					mgr->reloadConstTranslation(form);
				}
			}
		}
		static inline REL::Relocation<decltype(thunk)> func;

		static void Install()
		{
			const bool ae2 = REL::Module::get().version() >= SKSE::RUNTIME_SSE_1_7_99;
			REL::Relocation<std::uintptr_t> target1{ REL::VariantID(34644, 35566, 0x581D10), REL::Relocate(0x3AA, ae2 ? 0x265 : 0x25F) };
			stl::write_thunk_call<ReconstructForms>(target1.address());
		}
	};

	/*	struct TESNPCClone3D
		{

			// ref = ref; npc = baseObject
			static RE::NiAVObject* thunk(RE::TESNPC* npcarg, RE::TESObjectREFR* ref)
			{
				auto result = func(npcarg, ref);

				if (ref)
				{
					const auto base = ref->GetBaseObject();
					const auto fullNameBase = RE::getFullNameFormIDForRef(*ref);
					const std::string baseMessage = base ? std::format("{:08X} - {} - FullNameBase: {:08X}", base->GetFormID(), RE::FormTypeToString(base->GetFormType()), fullNameBase) : "Unknown";
					SKSE::log::debug("Found Reference {:08X} - {} with base object {}", ref->GetFormID(), RE::FormTypeToString(ref->GetFormType()), baseMessage);
					const auto npc = RE::TESForm::LookupByID<RE::TESNPC>(fullNameBase);
					if (npc && npc->baseTemplateForm)
					{
						SKSE::log::debug("Found another template {:08X}", npc->baseTemplateForm->formID);
						auto npc2 = npc->baseTemplateForm->As<RE::TESNPC>();
						if (npc2 && npc2->baseTemplateForm)
						{
							SKSE::log::debug("Found another template 2222222 {:08X}", npc2->baseTemplateForm->formID);
						}
					}

				}

				return result;
			}
			static inline REL::Relocation<decltype(thunk)> func;

			static void Install()
			{
				REL::Relocation<std::uintptr_t> Vtbl{ RE::VTABLE_TESNPC[0] };
				func = Vtbl.write_vfunc(0x4A, &thunk);
			}
		};*/

	struct PlayerCharacterUpdate
	{
		static void thunk(RE::PlayerCharacter* player, float delta)
		{
			func(player, delta);
			if (delta <= 0.0f || !player)
				return;

			static float counter = 0.0f;
			counter += delta;
			if (counter < 3.0f) // normally around 3 seconds, changes with time scale which is fine since the refs move
				return;

			counter = 0.0f;

			const auto tes = RE::TES::GetSingleton();
			if (!tes)
				return;

			const auto callback = []([[maybe_unused]] RE::TESObjectREFR* ref) -> RE::BSContainer::ForEachResult
				{
					/*if (ref && ref->IsPersistent())
					{
						const auto base = ref->GetBaseObject();
						const std::string baseMessage = base ? std::format("{:08X} - {}", base->GetFormID(), RE::FormTypeToString(base->GetFormType())) : "Unknown";
						SKSE::log::debug("Found Reference {:08X} - {} with base object {}", ref->GetFormID(), RE::FormTypeToString(ref->GetFormType()), baseMessage);
					}*/

					return RE::BSContainer::ForEachResult::kContinue;
				};


			tes->ForEachReference(callback);
		}
		static inline REL::Relocation<decltype(thunk)> func;

		static void Install()
		{
			REL::Relocation<std::uintptr_t> Vtbl{ RE::VTABLE_PlayerCharacter[0] };
			func = Vtbl.write_vfunc(0xAD, &thunk);
		}
	};

	void InstallHooks()
	{
		DataHandlerInitAllForms::Install();
		NPCFullNameCopyComponent::Install();
		GetDescription::Install();
		GetLogEntryHook::Install();
		GetResponseListHook::Install();
		DialogueMenuTextHook::Install();
		ReconstructForms::Install();
		PlayerCharacterUpdate::Install();
		//TESNPCClone3D::Install();

		SKSE::log::info("{} Done!", __FUNCTION__);
	}
}