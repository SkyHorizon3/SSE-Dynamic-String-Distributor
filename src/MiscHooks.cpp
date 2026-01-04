#include "MiscHooks.h"
#include "Manager.h"
#include "DialogueHooks.h"
#include "Utils.h"


namespace Hook
{
	struct MapMarkerDataHook //REFR FULL
	{
		static RE::TESFullName* thunk(RE::TESObjectREFR* marker)
		{
			if (!marker || marker->IsDisabled())
			{
				return func(marker);
			}

			auto result = func(marker);

			std::string newDescription{};
			if (result && Manager::GetSingleton()->getREFR(marker->formID, newDescription))
			{
				result->SetFullName(newDescription.c_str());
			}

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

			return result;
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

			auto* npc = skyrim_cast<const RE::TESForm*>(npcFullname);

			RE::BSString newDescription{};
			if (Manager::GetSingleton()->getDIAL(Utils::getTrimmedFormID(npc), Utils::getModName(npc), newDescription))
			{
				npcFullname->SetFullName(newDescription.c_str());
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

			const auto formID = safeForm->formID;
			std::string newDescription{};

			// 0x4D414E43 == 'MANC' (CNAM)
			const bool isCNAM = (chunkID == 'MANC');
			const auto manager = Manager::GetSingleton();
			const bool result = isCNAM ? manager->getCNAM(formID, newDescription) : manager->getDESC(formID, newDescription);

			if (result)
			{
				out = std::move(newDescription);

				const char* type = isCNAM ? "CNAM" : "DESC";
				SKSE::log::debug("Replaced {} for {:08X} with:", type, formID);
				SKSE::log::debug("{}", out.c_str());
			}

		}
		static inline REL::Relocation<decltype(thunk)> func;

		static void Install()
		{
			REL::Relocation<std::uintptr_t> target{ REL::VariantID(14401, 14552, 0x1A0300) };
			stl::hook_function_prologue<GetDescription, 6>(target.address());
		}
	};

	void InstallPostLoadHooks()
	{
		NpcNameFileStreamHook::Install();
		SKSE::log::info("Installed PostLoadHooks!");
	}

	void InstallDataLoadedHooks()
	{
		GetDescription::Install();
		InstallDialogueHooks();

		GetLogEntryHook::Install();
		MapMarkerDataHook::Install();

		SKSE::log::info("Installed DataLoadedHooks!");
	}
}