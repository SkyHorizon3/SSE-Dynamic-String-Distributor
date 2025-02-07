#include "MiscHooks.h"
#include "Manager.h"
#include "DescriptionHooks.h"
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
		static void thunk(RE::BSString& a_out, char* a_buffer, std::uint64_t a3)
		{
			func(a_out, a_buffer, a3);

			Manager::GetSingleton()->getQUST(a_out.c_str(), a_out);

		}
		static inline REL::Relocation<decltype(thunk)> func;


		static void Install()
		{
			//JournalMenu quest description text Hook
			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(24778, 25259), REL::Relocate(0x21C, 0x221) };//First: 5B ->Don't know; Second: C4 ->Don't know; Third: 221 ->Quest description text
			stl::write_thunk_call<QuestCNAMTextHook>(target1.address());
		}

	};

	static bool IsACTI = false;
	std::string SetActivationText = "";
	struct GetActivateText // ACTI RNAM, FLOR RNAM
	{
		static void thunk(RE::TESObjectACTI* a_this, RE::TESFile* a_file)
		{
			IsACTI = false;

			if (Manager::GetSingleton()->getACTI(Utils::getTrimmedFormID(a_this), Utils::getModName(a_this), SetActivationText))
				IsACTI = true;

			func(a_this, a_file);
		}
		static inline REL::Relocation<decltype(thunk)> func;

		static void Install()
		{
			std::array targets{
				std::make_pair(REL::VariantID(16800, 17181, 0x2244A0), 0x3F),
				std::make_pair(REL::VariantID(16984, 17367, 0x22A450), 0x5F),
				std::make_pair(REL::VariantID(17010, 17393, 0x22AC40), 0x134),
				std::make_pair(REL::VariantID(17010, 17393, 0x22AC40), 0x18E),
				std::make_pair(REL::VariantID(17010, 17393, 0x22AC40), 0x1ED),
				std::make_pair(REL::VariantID(17293, 17694, 0x234D50), 0x36),
			};

			for (auto& [id, offset] : targets)
			{
				REL::Relocation<std::uintptr_t> target(id, offset);
				stl::write_thunk_call<GetActivateText>(target.address());
			}
		}
	};


	struct SetActivateText
	{
		static char** thunk(char** a_this, RE::BSFixedString& a_str)
		{
			if (IsACTI)
			{
				a_str = Utils::validateString(SetActivationText);
			}
			IsACTI = false;

			return func(a_this, a_str);
		}
		static inline REL::Relocation<decltype(thunk)> func;

		static void Install()
		{
			std::array targets{
				std::make_pair(REL::VariantID(17294, 17711, 0x234DC0), REL::Relocate(0x421, 0xF9)),
				std::make_pair(REL::VariantID(17294, 17711, 0x234DC0), REL::Relocate(0x47A, 0x14C)),
				std::make_pair(REL::VariantID(17294, 17711, 0x234DC0), REL::Relocate(0x4AE, 0x178)),
			};

			for (auto& [id, offset] : targets)
			{
				REL::Relocation<std::uintptr_t> target(id, offset);
				stl::write_thunk_call<SetActivateText>(target.address());
			}
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
		static void thunk(RE::TESFullName* a_npcFullname, RE::TESFile* a_file)
		{
			func(a_npcFullname, a_file);

			auto* npc = skyrim_cast<const RE::TESForm*>(a_npcFullname);

			std::string newDescription{};
			if (Manager::GetSingleton()->getACTI(Utils::getTrimmedFormID(npc), Utils::getModName(npc), newDescription))
			{
				a_npcFullname->SetFullName(newDescription.c_str());
			}

		}
		static inline REL::Relocation<decltype(thunk)> func;

		static void Install()
		{
			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(24159, 24663), REL::Relocate(0x7CE, 0x924) };
			stl::write_thunk_call<NpcNameFileStreamHook>(target1.address());
		}
	};

	void InstallPostLoadHooks()
	{
		//NPCLoadFromFile::Install();
		NpcNameFileStreamHook::Install();
		GetActivateText::Install();
		SetActivateText::Install();
		InstallDialogueHooksPostLoad();

		SKSE::log::info("Installed PostLoadHooks!");
	}

	void InstallHooks()
	{
		InstallDescriptionHooks();
		InstallDialogueHooks();

		//MainUpdate::Install();
		QuestCNAMTextHook::Install();
		MapMarkerDataHook::Install();

		SKSE::log::info("Installed DataLoadedHooks!");
	}
}