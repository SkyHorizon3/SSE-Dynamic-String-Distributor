#include "MiscHooks.h"
#include "InventoryHooks.h"
#include "DescriptionHooks.h"
#include "DialogueHooks.h"
#include "QuestHooks.h"
#include "Processor.h"
#include "Utils.h"

#include <detours/detours.h>


namespace Hook
{

	struct MapMarkerDataHook //REFR FULL
	{
		static void thunk(RE::TESObjectREFR* marker)
		{
			if (!marker)
			{
				return func(marker);
			}

			func(marker);

			if (!marker->IsDisabled())
			{
				if (auto mapMarker = marker->extraList.GetByType<RE::ExtraMapMarker>())
				{
					if (mapMarker && mapMarker->mapData)
					{
						size_t key = Utils::combineHash_2(marker->formID, marker->GetFile()->fileName);

						auto it = g_REFR_FULL_Map.find(key);
						if (it != g_REFR_FULL_Map.end())
						{
							mapMarker->mapData->locationName.fullName = it->second;
						}

					}
				}
			}

		};
		static inline REL::Relocation<decltype(thunk)> func;


		static void Install()
		{
			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(18755, 19216), REL::VariantOffset(0xA6, 0xE4, 0xA6) }; //Theres also 40750, 0x176 on AE. But that's leading to random crashes
			stl::write_thunk_call<MapMarkerDataHook>(target1.address());
			SKSE::log::info("MapMarkerDataHook hooked at address: {:x} and offset: {:x}", target1.address(), target1.offset());
		}

	};


	typedef void(WINAPI* MessageBoxDataHook_pFunc)(RE::MessageBoxData* Menu);
	MessageBoxDataHook_pFunc originalFunction01;
	void MessageBoxFunc(RE::MessageBoxData* Menu) //MESG ITXT, PERK EPF2
	{
		if (!Menu && !Menu->buttonText.size())
		{
			return originalFunction01(Menu);
		}

		for (auto& text : Menu->buttonText)
		{
			auto it = g_MESG_ITXT_Map.find(text.c_str());

			if (it != g_MESG_ITXT_Map.end())
			{
				text = it->second;
			}
		}


		return originalFunction01(Menu);
	}

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
				case RE::FormType::ActorCharacter: //PERK EPFD
				case RE::FormType::Reference: //ACTI RNAM, FLOR RNAM, REGN RDMP for doors
				{
					//SKSE::log::info("String: {}", messagedata->text.c_str());

					std::istringstream iss(messagedata->text.c_str());
					std::string line1, line2;

					if (std::getline(iss, line1) && std::getline(iss, line2))
					{
						HandleFormTypeHelper(messagedata, line1, line2);
					}
					else
					{
						auto it1 = g_FLOR_RNAM_RDMP_Map.find(line1);

						if (it1 != g_FLOR_RNAM_RDMP_Map.end())
						{
							messagedata->text = it1->second;
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

		static void HandleFormTypeHelper(RE::HUDData* messagedata, std::string& line1, std::string& line2)
		{
			auto it1 = g_FLOR_RNAM_RDMP_Map.find(line1);
			auto it2 = g_FLOR_RNAM_RDMP_Map.find(line2);

			if (it1 != g_FLOR_RNAM_RDMP_Map.end())
			{
				line1 = it1->second;
			}

			if (it2 != g_FLOR_RNAM_RDMP_Map.end())
			{
				line2 = it2->second;
			}

			messagedata->text = line1 + "\n" + line2;
		}

		static inline REL::Relocation<decltype(thunk)> func;


		static void Install()
		{
			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(39535, 40621), REL::VariantOffset(0x289, 0x280, 0x22E) };
			stl::write_thunk_call<CrosshairTextHook>(target1.address());
			SKSE::log::info("CrosshairTextHook hooked at address: {:x} and offset: {:x}", target1.address(), target1.offset());
		}
	};

	struct AutoExitDoorTextHook
	{
		static void thunk(RE::UIMessageQueue* queue_this, const RE::BSFixedString& menuName, RE::UI_MESSAGE_TYPE type, RE::IUIMessageData* data)
		{
			//REGN RDMP if you're leaving a cave or something like that
			const auto messagedata = data ? static_cast<RE::HUDData*>(data) : nullptr;
			const auto player = RE::PlayerCharacter::GetSingleton();

			if (!messagedata->text.empty() && player && player->GetParentCell() && player->GetParentCell()->IsInteriorCell()) //Exterior is already covered by CELL FULL
			{
				//SKSE::log::info("String: {}", messagedata->text.c_str());

				// Extract text before and after the first space
				std::string text = messagedata->text.c_str();
				size_t spacePos = text.find(' ');
				if (spacePos != std::string::npos)
				{
					std::string before = text.substr(0, spacePos);
					std::string after = text.substr(spacePos + 1);

					auto it = g_FLOR_RNAM_RDMP_Map.find(after);
					if (it != g_FLOR_RNAM_RDMP_Map.end())
					{
						messagedata->text = before + " " + it->second;
					}

				}
			}

			func(queue_this, menuName, type, data);
		};

		static inline REL::Relocation<decltype(thunk)> func;


		static void Install()
		{
			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(50727, 51622), REL::VariantOffset(0xD7, 0x255, 0x101) };//VR: 1408AC8A0
			stl::write_thunk_call<AutoExitDoorTextHook>(target1.address());
			SKSE::log::info("AutoExitDoorTextHook hooked at address: {:x} and offset: {:x}", target1.address(), target1.offset());
		}
	};

	void InstallHooks()
	{
		InstallInventoryHooks();
		InstallDescriptionHooks();
		InstallDialogueHooks();
		InstallQuestHooks();

		Hook::MapMarkerDataHook::Install();
		Hook::CrosshairTextHook::Install();
		Hook::AutoExitDoorTextHook::Install();

		//MessageBoxData Hook
		const auto MessageBoxDataFunc = RELOCATION_ID(51422, 52271).address();


		originalFunction01 = (MessageBoxDataHook_pFunc)MessageBoxDataFunc;


		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(reinterpret_cast<PVOID*>(&originalFunction01), static_cast<PVOID>(&MessageBoxFunc));


		const auto err = DetourTransactionCommit();
		if (err == NO_ERROR)
		{
			SKSE::log::info("Installed MessageBoxDataFunc hook at {0:x}", MessageBoxDataFunc);

		}
		else
		{
			SKSE::log::error("DetourTransactionCommit failed with error code: {}", err);
		}
	}
}