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
			auto result = func(marker);

			if (!marker || marker->IsDisabled())
			{
				return result;
			}

			size_t key = Utils::combineHash(marker->formID, Utils::GetModName(marker));

			auto it = g_REFR_FULL_Map.find(key);

			if (it != g_REFR_FULL_Map.end())
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
			SKSE::log::info("MapMarkerDataHook hooked at address: {:x} and offset: {:x}", target1.address(), target1.offset());
		}

	};

	//Credits to po3 https://github.com/powerof3/SimpleActivateSKSE released under MIT
	//Still compatible
	struct CrosshairTextHook //Could also hook the calls of 1400F9E60... Unfortunately there are only 148
	{
		static void thunk(RE::UIMessageQueue* queue_this, const RE::BSFixedString& menuName, RE::UI_MESSAGE_TYPE type, RE::IUIMessageData* data)
		{
			const auto messagedata = data ? static_cast<RE::HUDData*>(data) : nullptr;
			const auto crosshairRef = messagedata ? messagedata->crossHairRef.get() : RE::TESObjectREFRPtr();

			if (messagedata && crosshairRef)
			{
				switch (crosshairRef->GetFormType())
				{
				case RE::FormType::Reference: //ACTI RNAM, FLOR RNAM
				{
					//SKSE::log::info("String: {}", messagedata->text.c_str());

					std::istringstream iss(messagedata->text.c_str());
					std::string line1;

					if (std::getline(iss, line1))
					{
						auto it1 = g_FLOR_RNAM_RDMP_Map.find(line1);
						if (it1 != g_FLOR_RNAM_RDMP_Map.end())
						{
							std::string restOfText, line;
							while (std::getline(iss, line))
							{
								restOfText += line + "\n";
							}

							if (!restOfText.empty())
							{
								restOfText.pop_back(); // remove useless \n
							}

							messagedata->text = it1->second + "\n" + restOfText;
						}
					}
				}
				break;

				default:
					break;
				}
			}

			func(queue_this, menuName, type, data);
		};
		static inline REL::Relocation<decltype(thunk)> func;


		static void Install()
		{
			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(39535, 40621), REL::VariantOffset(0x289, 0x280, 0x22E) };
			stl::write_thunk_call<CrosshairTextHook>(target1.address());
			SKSE::log::info("CrosshairTextHook hooked at address: {:x} and offset: {:x}", target1.address(), target1.offset());
		}
	};

	struct NpcNameFileStreamHook //NPC FULL - Why this way? Because templates use one of their templates names and it's horrible to change it
	{
		static void TESFullName__sub_140196D80(RE::TESFullName* a_fullname, RE::TESFile* a_file)
		{
			using func_t = decltype(&TESFullName__sub_140196D80);
			REL::Relocation<func_t> func{ RELOCATION_ID(14546, 14718) };
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

		static void thunk(RE::TESFullName* a_npcFullname, RE::TESFile* a_file, RE::TESNPC* a_npc)
		{
			TESFullName__sub_140196D80(a_npcFullname, a_file); //Invoke and get original output

			size_t key = Utils::combineHash(a_npc->formID & 0x00FFFFFF, Utils::GetModName(a_npc));

			auto it = g_NPC_FULL_Map.find(key);
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

	void InstallPostLoadHooks()
	{
		NpcNameFileStreamHook::Install();
	}

	void InstallHooks()
	{
		InstallInventoryHooks();
		InstallDescriptionHooks();
		InstallDialogueHooks();
		InstallQuestHooks();

		MapMarkerDataHook::Install();
		CrosshairTextHook::Install();
	}
}