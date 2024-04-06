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
		static void thunk(RE::TESObjectREFR* marker)
		{
			if (!marker)
			{
				return func(marker);
			}

			if (!marker->IsDisabled())
			{
				if (auto mapMarker = marker->extraList.GetByType<RE::ExtraMapMarker>())
				{
					if (mapMarker && mapMarker->mapData)
					{

						size_t key = Utils::combineHash(marker->formID, Utils::GetModName(marker));

						auto it = g_REFR_FULL_Map.find(key);

						if (it != g_REFR_FULL_Map.end())
						{
							mapMarker->mapData->locationName.fullName = it->second;
						}

					}
				}
			}

			func(marker);

		};
		static inline REL::Relocation<decltype(thunk)> func;


		static void Install()
		{
			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(18755, 19216), REL::VariantOffset(0xA6, 0xE4, 0xA6) }; //Theres also 40750, 0x176 on AE. But that's leading to random crashes
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

	void InstallHooks()
	{
		InstallInventoryHooks();
		InstallDescriptionHooks();
		InstallDialogueHooks();
		InstallQuestHooks();

		Hook::MapMarkerDataHook::Install();
		Hook::CrosshairTextHook::Install();
	}
}