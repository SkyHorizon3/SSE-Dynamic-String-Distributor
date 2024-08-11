#include "MiscHooks.h"
#include "InventoryHooks.h"
#include "DescriptionHooks.h"
#include "DialogueHooks.h"
#include "QuestHooks.h"
#include "Processor.h"
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

			const auto key = Utils::combineHash(marker->formID, Utils::GetModName(marker));

			const auto it = g_REFR_FULL_Map.find(key);
			if (result && it != g_REFR_FULL_Map.end())
			{
				result->fullName = it->second;
			}

			return result;

		};
		static inline REL::Relocation<decltype(thunk)> func;


		static void Install()
		{
			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(18755, 19216), REL::Relocate(0xA6, 0xE4) }; //Theres also 40750, 0x176 on AE. But that's leading to random crashes
			stl::write_thunk_call<MapMarkerDataHook>(target1.address());
		}

	};

	struct NpcNameFileStreamHook //NPC FULL - Why this way? Because templates use one of their templates names and it's horrible to change it
	{
		static void TESFullName__sub_140196D80(RE::TESFullName* a_fullname, RE::TESFile* a_file)
		{
			using func_t = decltype(&TESFullName__sub_140196D80);
			static REL::Relocation<func_t> func{ RELOCATION_ID(14546, 14718) };
			return func(a_fullname, a_file);
		}

		struct TrampolineCall : Xbyak::CodeGenerator
		{
			TrampolineCall(std::uintptr_t retn, std::uintptr_t func)
			{
				Xbyak::Label funcLabel;
				Xbyak::Label retnLabel;

				if (REL::Module::IsAE())
				{
					mov(r8, r12); //TESNPC as 3rd parameter
				}
				else
				{
					mov(r8, r15);
				}

				sub(rsp, 0x20); //allocate for call
				call(ptr[rip + funcLabel]); //call thunk
				add(rsp, 0x20);

				jmp(ptr[rip + retnLabel]); //jump back to original code

				L(funcLabel);
				dq(func);

				L(retnLabel);
				dq(retn);
			}
		};

		static void thunk(RE::TESFullName* a_npcFullname, RE::TESFile* a_file, RE::TESNPC* a_npc) // TODO: Rewrite
		{
			TESFullName__sub_140196D80(a_npcFullname, a_file); //Invoke and get original output

			const auto key = Utils::combineHash(Utils::GetTrimmedFormID(a_npc), Utils::GetModName(a_npc));

			const auto it = g_NPC_FULL_Map.find(key);
			if (it != g_NPC_FULL_Map.end())
			{
				a_npcFullname->fullName = it->second;
			}
		}

		static void Install()
		{
			constexpr auto targetAddress = RELOCATION_ID(24159, 24663);
			REL::Relocation<std::uintptr_t> target{ targetAddress, REL::Relocate(0x7CE, 0x924) };
			auto trampolineJmp = TrampolineCall(target.address() + 0x5, stl::unrestricted_cast<std::uintptr_t>(thunk));

			auto& trampoline = SKSE::GetTrampoline();
			SKSE::AllocTrampoline(trampolineJmp.getSize());
			auto result = trampoline.allocate(trampolineJmp);
			SKSE::AllocTrampoline(14);
			trampoline.write_branch<5>(target.address(), (std::uintptr_t)result);
		}
	};

	static bool IsACTI = false;
	std::string SetActivationText = "";
	struct GetActivateText // ACTI RNAM, FLOR RNAM
	{
		static void thunk(RE::TESObjectACTI* a_this, RE::TESFile* a_file)
		{
			IsACTI = false;

			const auto key = Utils::combineHash(Utils::GetTrimmedFormID(a_this), Utils::GetModName(a_this));
			const auto it = g_ACTI_Map.find(key);

			if (it != g_ACTI_Map.end())
			{
				IsACTI = true;
				SetActivationText = it->second;
			}

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
			if (IsACTI && !a_str.empty())
			{
				a_str = SetActivationText;
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

	void InstallPostLoadHooks()
	{
		NpcNameFileStreamHook::Install();
		GetActivateText::Install();
		SetActivateText::Install();
		InstallDialogueHooksPostLoad();

		SKSE::log::info("Installed PostLoadHooks!");
	}

	void InstallHooks()
	{
		InstallInventoryHooks();
		InstallDescriptionHooks();
		InstallDialogueHooks();
		InstallQuestHooks();

		MapMarkerDataHook::Install();

		SKSE::log::info("Installed DataLoadedHooks!");
	}
}