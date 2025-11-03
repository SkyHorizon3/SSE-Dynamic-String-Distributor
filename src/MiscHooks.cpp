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

	/*
	//140376DEE - 1.6.640, 14035DCA8 - 1.5.97

	struct NPCLoadFromFile
	{
		struct MoveParent : Xbyak::CodeGenerator
		{
			MoveParent() // move RE::TESNPC* as 3rd parameter
			{
				if (REL::Module::IsAE())
				{
					mov(r8, r12);
				}
				else
				{
					mov(r8, r15);
				}
			}
		};

		static bool thunk(RE::TESFile* a_file, void* a_buf, std::uint64_t a_npc)
		{
			auto result = func(a_file, a_buf, 4u);

			const RE::TESNPC* npc = reinterpret_cast<RE::TESNPC*>(a_npc);
			std::uint32_t dnamFormID = *static_cast<std::uint32_t*>(a_buf);
			const std::uint32_t firstTwoHexDigits = dnamFormID >> 24; // extract first two digits and convert to decimal
			RE::FormID formID = dnamFormID & 0xFFFFFF; // remove file index -> 0x00XXXXXX

			const RE::TESFile* lookupFile = a_file;
			if (firstTwoHexDigits < a_file->masterCount)
			{
				lookupFile = a_file->masterPtrs[firstTwoHexDigits];
			}

			if (lookupFile->IsLight())
			{
				formID &= 0xFFF;
			}

			std::string npcformIDString = std::format("{0:08X}", npc->formID);
			std::string formIDString = std::format("{0:08X}", formID);

			//SKSE::log::info("NPC: {} - FormIDTemplate: {} - PluginTemplate: {}", npcformIDString, formIDString, Utils::getModName(npc));


			//const std::string string = std::to_string(Utils::getTrimmedFormID(npc)) + Utils::getModName(npc);


			return result;
		}

		static inline REL::Relocation<decltype(thunk)> func;

		static void Install()
		{
			constexpr auto targetAddress = RELOCATION_ID(24159, 24663);

			REL::Relocation<std::uintptr_t> swapTarget{ targetAddress, REL::Relocate(0x140D, 0x18D3) };
			REL::safe_fill(swapTarget.address(), REL::NOP, 0x4);

			auto newCode = MoveParent();
			assert(newCode.getSize() <= 0x4);
			REL::safe_write(swapTarget.address(), newCode.getCode(), newCode.getSize());

			REL::Relocation<std::uintptr_t> target{ targetAddress, REL::Relocate(0x1418, 0x18DE) };
			stl::write_thunk_call<NPCLoadFromFile>(target.address());
		}
	};
	*/

	struct QuestCNAMTextHook //QUST CNAM
	{
		static void thunk(RE::BSString& out, char* buffer, std::uint64_t a3)
		{
			func(out, buffer, a3);

			Manager::GetSingleton()->getQUST(out.c_str(), out);

		}
		static inline REL::Relocation<decltype(thunk)> func;


		static void Install()
		{
			//JournalMenu quest description text Hook
			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(24778, 25259), REL::Relocate(0x21C, 0x221) };//First: 5B ->Don't know; Second: C4 ->Don't know; Third: 221 ->Quest description text
			stl::write_thunk_call<QuestCNAMTextHook>(target1.address());
		}

	};

	/*
	struct MainUpdate
	{

		static void thunk()
		{
			func();

			static auto lastCallTime = std::chrono::steady_clock::now();
			auto now = std::chrono::steady_clock::now();

			if (std::chrono::duration_cast<std::chrono::seconds>(now - lastCallTime).count() >= 1)
			{
				lastCallTime = now;

				Manager::GetSingleton()->checkConst();
			}
		};
		static inline REL::Relocation<decltype(thunk)> func;


		static void Install()
		{
			const bool marketplace = REL::Module::get().version() >= SKSE::RUNTIME_SSE_1_6_1130;
			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(35565, 36564), REL::Relocate(0x748, (marketplace ? 0xC2b : 0xC26), 0x7EE) };
			stl::write_thunk_call<MainUpdate>(target1.address());
		}
	};
	*/

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
			func(description, out, parent, chunkID);

			if (!parent)
				return;

			const auto manager = Manager::GetSingleton();
			const auto formID = parent->formID;

			std::string newDescription{};

			// 0x4D414E43 == 'MANC' (CNAM)
			const bool isCNAM = (chunkID == 'MANC');
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
			REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(14401, 14552) };
			stl::hook_function_prologue<GetDescription, 6>(target.address());
		}
	};

	void InstallPostLoadHooks()
	{
		//NPCLoadFromFile::Install();
		NpcNameFileStreamHook::Install();
		InstallDialogueHooksPostLoad();

		SKSE::log::info("Installed PostLoadHooks!");
	}

	void InstallDataLoadedHooks()
	{
		GetDescription::Install();
		InstallDialogueHooks();

		//MainUpdate::Install();
		QuestCNAMTextHook::Install();
		MapMarkerDataHook::Install();

		SKSE::log::info("Installed DataLoadedHooks!");
	}
}