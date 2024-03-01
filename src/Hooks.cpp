#include "Hooks.h"
#include "Processor.h"

#include <detours/detours.h>


namespace Hook
{
	//HUGE Credits to Nightfallstorm for some of the hooks!!
	//https://github.com/Nightfallstorm/DescriptionFramework released under GPL-3.0
	//https://github.com/Nightfallstorm/Recipes released under GPL-3.0
	struct ItemCardPopulateHook //BOOK CNAM, MGEF DNAM
	{
		static void thunk(RE::ItemCard* itemCard, RE::TESBoundObject** a_item, char a3)
		{
			if (!itemCard || !a_item || !*a_item)
			{
				return func(itemCard, a_item, a3);
			}

			func(itemCard, a_item, a3);
			handleEffectsItems(*a_item);
			handleDescriptionItems(itemCard, *a_item);
		};

		static void handleSpellTomes(RE::TESObjectBOOK* book, const ConfigurationInformation& information)
		{

			auto spell = book->GetSpell();
			if (spell && spell->effects.size())
			{
				for (auto& effect : spell->effects)
				{
					if (effect->baseEffect->formID == information.Form->formID)
					{
						effect->baseEffect->magicItemDescription = information.ReplacerText;
					}
				}
			}
		}

		static void handleMagicItem(RE::MagicItem* magicItem, const ConfigurationInformation& information)
		{
			if (magicItem && magicItem->effects.size())
			{
				for (auto& effect : magicItem->effects)
				{
					if (effect->baseEffect->formID == information.Form->formID)
					{
						effect->baseEffect->magicItemDescription = information.ReplacerText;
					}
				}

			}
		}

		static void handleArmor(RE::TESObjectARMO* Item, const ConfigurationInformation& information)
		{
			auto Enchanting = Item->formEnchanting;
			if (Enchanting && Enchanting->effects.size())
			{
				for (auto& effect : Enchanting->effects)
				{
					if (effect->baseEffect->formID == information.Form->formID)
					{
						effect->baseEffect->magicItemDescription = information.ReplacerText;
					}
				}

			}
		}

		static void handleWeapon(RE::TESObjectWEAP* Item, const ConfigurationInformation& information)
		{
			auto Enchanting = Item->formEnchanting;
			if (Enchanting && Enchanting->effects.size())
			{
				for (auto& effect : Enchanting->effects)
				{
					if (effect->baseEffect->formID == information.Form->formID)
					{
						effect->baseEffect->magicItemDescription = information.ReplacerText;
					}
				}

			}
		}

		static void handleEffectsItems(RE::TESBoundObject* a_item)
		{
			for (const auto& information : g_ConfigurationInformationStruct)
			{
				switch (information.SubrecordType)
				{
				case Config::SubrecordTypes::kDNAM:
				{
					if (!a_item->IsMagicItem())
					{
						switch (a_item->GetFormType())
						{
						case RE::FormType::Book:
						{
							auto book = a_item->As<RE::TESObjectBOOK>();
							handleSpellTomes(book, information);
						}
						break;
						case RE::FormType::Armor:
						{
							auto armor = a_item->As<RE::TESObjectARMO>();
							handleArmor(armor, information);
						}
						break;
						case RE::FormType::Weapon:
						{
							auto weapon = a_item->As<RE::TESObjectWEAP>();
							handleWeapon(weapon, information);
						}
						break;

						default:
						{
							break;
						}

						}

					}
					else
					{
						auto magicItem = a_item->As<RE::MagicItem>();
						handleMagicItem(magicItem, information);
					}
				}
				break;

				default: break;
				}
			}
		}

		static void handleDescriptionItems(RE::ItemCard* itemCard, RE::TESBoundObject* a_item)
		{
			for (const auto& Information : g_ConfigurationInformationStruct)
			{
				switch (Information.SubrecordType)
				{
				case Config::SubrecordTypes::kCNAM:
				{
					// Replace the ItemCardDescription of books
					if (a_item->formID == Information.Form->formID)
					{
						auto descriptionValue = RE::GFxValue(Information.ReplacerText);
						itemCard->obj.SetMember("description", descriptionValue);
					}
				}
				break;

				default: break;
				}
			}
		}
		static inline REL::Relocation<decltype(thunk)> func;

	};

	struct SpellItemTextHook //MGEF DNAM for SpellItems
	{
		static void thunk(RE::TESBoundObject* form)
		{
			if (!form)
			{
				return func(form);
			}

			func(form);

			for (const auto& Information : g_ConfigurationInformationStruct)
			{
				switch (Information.SubrecordType)
				{
				case Config::SubrecordTypes::kDNAM:
				{
					if (form->Is(RE::FormType::Spell))
					{
						auto spell = form->As<RE::SpellItem>();

						if (spell && spell->effects.size())
						{
							for (auto& effect : spell->effects)
							{
								if (effect->baseEffect->formID == Information.Form->formID)
								{
									effect->baseEffect->magicItemDescription = Information.ReplacerText;
								}
							}
						}
					}
				}
				break;

				default: break;
				}

			}
		};
		static inline REL::Relocation<decltype(thunk)> func;

	};

	// Track if an object is valid between the two AE hooks
	static bool IsDESC = false;
	std::string SetDescription = "";
	struct ParentDESCHookAE
	{
		static void thunk(RE::BSString* a_out, RE::TESForm* a_parent, std::uint64_t a_unk)
		{
			//Perks, Race Descriptions and any other DESC in xEdit except for loading screens

			UNREFERENCED_PARAMETER(a_unk);

			func(a_out, nullptr, 0); // Invoke original

			for (const auto& Information : g_ConfigurationInformationStruct)
			{
				switch (Information.SubrecordType)
				{
				case Config::SubrecordTypes::kDESC: //So it doesn't set CNAM as DESC if both are in a json
				{
					if (a_parent && a_parent->formID == Information.Form->formID)
					{
						IsDESC = true;
						SetDescription = Information.ReplacerText;
						// Set object valid for GetDescription hook to check
					}
				}
				break;

				default: break;
				}

			}
		}

		static inline REL::Relocation<decltype(thunk)> func;

		struct GetParentInsideHook : Xbyak::CodeGenerator
		{
			// This moves the parent form, which is currently in rdi, to rdx so the other hook can use it
			GetParentInsideHook()
			{
				mov(rdx, rdi);
			}
		};
	};

	struct GetDescriptionHookAE //Nearly any DESC
	{
		static void thunk(RE::BSString* a_out, std::uint64_t a2, std::uint64_t a3)
		{
			func(a_out, a2, a3);

			//g_Logger->info("Output: {}", a_out->c_str());


			if (IsDESC && !a_out->empty()) //Don't replace anything thats empty. This stops the hook from also replacing BOOK CNAM with BOOK DESC if there is no CNAM.
			{
				*a_out = SetDescription;
			}
			IsDESC = false;

		}
		static inline REL::Relocation<decltype(thunk)> func;

	};

	struct GetDescriptionHookSE
	{
		static void thunk(RE::TESDescription* a_description, RE::BSString* a_out, RE::TESForm* a_parent, std::uint32_t unk)
		{
			func(a_description, a_out, a_parent, unk);  // invoke original to get original description string output

			for (const auto& Information : g_ConfigurationInformationStruct)
			{
				switch (Information.SubrecordType)
				{
				case Config::SubrecordTypes::kDESC: //So it doesn't set CNAM as DESC if both are in a json
				{
					if (a_parent && a_parent->formID == Information.Form->formID && !a_out->empty())
					{
						*a_out = Information.ReplacerText;
					}
				}
				break;

				default: break;
				}
			}
		}
		static inline REL::Relocation<decltype(thunk)> func;

	};

	struct DialogueStreamHook //INFO NAM1
	{
		static void thunk(RE::BSString* a_out, std::uint64_t a2, std::uint64_t a3)
		{
			func(a_out, a2, a3);

			auto it = g_INFO_NAM1_ITXT_Map.find(a_out->c_str());

			if (it != g_INFO_NAM1_ITXT_Map.end())
			{
				*a_out = it->second;
			}

		}
		static inline REL::Relocation<decltype(thunk)> func;

	};

	//Objective text in Journal menu. Not doing this via UI, because it's possible to add multiple NNAMs in one textfield.
	//And objective text in HUDmenu. Not doing this via UI, because SetObjectiveDisplayed(index) in papyrus doesn't set text in the UI. So I can't get the text then...
	//And quest description text in JournalMenu. Not doing this via UI, because I found the populate hook.
	struct QuestObjectiveTextHook //QUST NNAM, QUST CNAM
	{
		static void thunk(RE::BSString* a_out, std::uint64_t a2, std::uint64_t a3)
		{
			func(a_out, a2, a3);

			auto it = g_QUST_NNAM_CNAM_Map.find(a_out->c_str());

			if (it != g_QUST_NNAM_CNAM_Map.end())
			{
				*a_out = it->second;
			}

		}
		static inline REL::Relocation<decltype(thunk)> func;

	};

	struct DialogueMenuTextHook //DIAL FULL, INFO RNAM
	{
		static void thunk(RE::BSString* a_out, std::uint64_t a2, std::uint64_t a3)
		{
			func(a_out, a2, a3);

			auto it = g_DIAL_FULL_RNAM_Map.find(a_out->c_str());

			if (it != g_DIAL_FULL_RNAM_Map.end())
			{
				*a_out = it->second;
			}

		};
		static inline REL::Relocation<decltype(thunk)> func;

	};


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
						auto it = g_FLOR_RNAM_RDMP_Map.find(mapMarker->mapData->locationName.fullName.c_str());

						if (it != g_FLOR_RNAM_RDMP_Map.end())
						{
							mapMarker->mapData->locationName.fullName = it->second;
						}

					}
				}
			}

		};
		static inline REL::Relocation<decltype(thunk)> func;

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
			auto it = g_INFO_NAM1_ITXT_Map.find(text.c_str());

			if (it != g_INFO_NAM1_ITXT_Map.end())
			{
				text = it->second;
			}
		}

		return originalFunction01(Menu);
	}

	struct LoadScreenTextHook //LSCR DESC
	{
		static void thunk(RE::TESLoadScreen* loadscreen, float* a2)
		{
			if (!loadscreen && !loadscreen->loadingText.size())
			{
				return func(loadscreen, a2);
			}

			func(loadscreen, a2);

			for (const auto& Information : g_ConfigurationInformationStruct)
			{
				switch (Information.SubrecordType)
				{
				case Config::SubrecordTypes::kDESC:
				{
					if (loadscreen && loadscreen->formID == Information.Form->formID)
					{
						loadscreen->loadingText = Information.ReplacerText;
					}
				}
				break;

				default: break;
				}
			}

		};
		static inline REL::Relocation<decltype(thunk)> func;

	};

	//Credits to po3 https://github.com/powerof3/SimpleActivateSKSE released under MIT
	//Still compatible
	struct CrosshairTextHook
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
	};

	void InstallHooks()
	{
		//loadscreen text hook
		REL::Relocation<std::uintptr_t> target21{ RELOCATION_ID(51048, 51929), REL::VariantOffset(0x2BB, 0x1BC, 0x2BB) };
		stl::write_thunk_call<LoadScreenTextHook>(target21.address());
		SKSE::log::info("LoadScreenTextHook hooked at address: {:x} and offset: {:x}", target21.address(), target21.offset());

		//AutoExitDoorTextHook
		REL::Relocation<std::uintptr_t> target20{ RELOCATION_ID(50727, 51622), REL::VariantOffset(0xD7, 0x255, 0xD7) };
		stl::write_thunk_call<AutoExitDoorTextHook>(target20.address());
		SKSE::log::info("AutoExitDoorTextHook hooked at address: {:x} and offset: {:x}", target20.address(), target20.offset());

		//REFR FULL Text Hook
		REL::Relocation<std::uintptr_t> target19{ RELOCATION_ID(18755, 19216), REL::VariantOffset(0xA6, 0xE4, 0xA6) }; //Theres also 40750, 0x176 on AE. But that's leading to random crashes
		stl::write_thunk_call<MapMarkerDataHook>(target19.address());
		SKSE::log::info("MapMarkerDataHook hooked at address: {:x} and offset: {:x}", target19.address(), target19.offset());

		//SpellItem Text Hook
		REL::Relocation<std::uintptr_t> target18{ RELOCATION_ID(14163, 14271), REL::VariantOffset(0x1C, 0x1A, 0x1C) };
		stl::write_thunk_call<SpellItemTextHook>(target18.address());
		SKSE::log::info("SpellItemTextHook hooked at address: {:x} and offset: {:x}", target18.address(), target18.offset());

		//CrosshairTextHook
		REL::Relocation<std::uintptr_t> target17{ RELOCATION_ID(39535, 40621), REL::VariantOffset(0x289, 0x280, 0x289) };
		stl::write_thunk_call<CrosshairTextHook>(target17.address());
		SKSE::log::info("CrosshairTextHook hooked at address: {:x} and offset: {:x}", target17.address(), target17.offset());

		//DialogueMenu text hook
		REL::Relocation<std::uintptr_t> target16{ RELOCATION_ID(34434, 35254), REL::VariantOffset(0xCC, 0x226, 0xCC) };
		stl::write_thunk_call<DialogueMenuTextHook>(target16.address());
		SKSE::log::info("DialogueMenuTextHook hooked at address: {:x} and offset: {:x}", target16.address(), target16.offset());

		//JournalMenu quest description text Hook
		REL::Relocation<std::uintptr_t> target14{ RELOCATION_ID(24778, 25259), REL::VariantOffset(0x21C, 0x221, 0x21C) };//First: 5B ->Don't know; Second: C4 ->Don't know; Third: 221 ->Quest description text
		stl::write_thunk_call<QuestObjectiveTextHook>(target14.address());
		SKSE::log::info("QuestObjectiveTextHook hooked at address: {:x} and offset: {:x}", target14.address(), target14.offset());

		//HudMenu quest objective text Hook
		REL::Relocation<std::uintptr_t> target13{ RELOCATION_ID(50751, 51646), REL::VariantOffset(0xA6, 0xA6, 0xA6) };//First: A6 ->All NNAM sentences are passing here; (Second: F7 ->"or" (depends on language) if needed; Third: 143 ->Second sentence after "or" if available)
		stl::write_thunk_call<QuestObjectiveTextHook>(target13.address());
		SKSE::log::info("QuestObjectiveTextHook hooked at address: {:x} and offset: {:x}", target13.address(), target13.offset());

		//JournalMenu quest objective text Hook
		REL::Relocation<std::uintptr_t> target12{ RELOCATION_ID(23229, 23684), REL::VariantOffset(0x21, 0x21, 0x21) };
		stl::write_thunk_call<QuestObjectiveTextHook>(target12.address());
		SKSE::log::info("QuestObjectiveTextHook hooked at address: {:x} and offset: {:x}", target12.address(), target12.offset());

		//DialogueStream (subtitles) Hook
		REL::Relocation<std::uintptr_t> target11{ RELOCATION_ID(34429, 35249), REL::VariantOffset(0x61, 0x61, 0x61) };
		stl::write_thunk_call<DialogueStreamHook>(target11.address());
		SKSE::log::info("DialogueStreamHook hooked at address: {:x} and offset: {:x}", target11.address(), target11.offset());

		//ItemCard Hook
		REL::Relocation<std::uintptr_t> target04{ RELOCATION_ID(50005, 50949), REL::VariantOffset(0x80, 0x80, 0x80) };
		stl::write_thunk_call<ItemCardPopulateHook>(target04.address());
		SKSE::log::info("ItemCardPopulateHook hooked at address: {:x} and offset: {:x}", target04.address(), target04.offset());

		REL::Relocation<std::uintptr_t> target05{ RELOCATION_ID(50201, 51130), REL::VariantOffset(0xB2, 0xB2, 0xB2) };
		stl::write_thunk_call<ItemCardPopulateHook>(target05.address());
		SKSE::log::info("ItemCardPopulateHook hooked at address: {:x} and offset: {:x}", target05.address(), target05.offset());

		REL::Relocation<std::uintptr_t> target06{ RELOCATION_ID(50297, 51218), REL::VariantOffset(0x35, 0x35, 0x35) };
		stl::write_thunk_call<ItemCardPopulateHook>(target06.address());
		SKSE::log::info("ItemCardPopulateHook hooked at address: {:x} and offset: {:x}", target06.address(), target06.offset());

		REL::Relocation<std::uintptr_t> target07{ RELOCATION_ID(50674, 51569), REL::VariantOffset(0x80, 0x7A, 0x80) };
		stl::write_thunk_call<ItemCardPopulateHook>(target07.address());
		SKSE::log::info("ItemCardPopulateHook hooked at address: {:x} and offset: {:x}", target07.address(), target07.offset());

		REL::Relocation<std::uintptr_t> target08{ RELOCATION_ID(50973, 51852), REL::VariantOffset(0x80, 0x7A, 0x80) };
		stl::write_thunk_call<ItemCardPopulateHook>(target08.address());
		SKSE::log::info("ItemCardPopulateHook hooked at address: {:x} and offset: {:x}", target08.address(), target08.offset());

		//MessageBoxData Hook SE
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

		if (REL::Module::IsAE())
		{
			//GetDescriptionParent Hook AE
			REL::Relocation<std::uintptr_t> codeSwap{ RELOCATION_ID(0, 14552), REL::VariantOffset(0x0, 0x8B, 0x0) }; //14019C91B xor     r8d, r8d
			REL::safe_fill(codeSwap.address(), REL::NOP, 0x5);
			auto newCode = ParentDESCHookAE::GetParentInsideHook();
			assert(newCode.getSize() <= 0x5);
			REL::safe_write(codeSwap.address(), newCode.getCode(), newCode.getSize());

			REL::Relocation<std::uintptr_t> target01{ RELOCATION_ID(0, 14552), REL::VariantOffset(0x0, 0x93, 0x0) };
			stl::write_thunk_call<ParentDESCHookAE>(target01.address());
			SKSE::log::info("ParentDESCHookAE hooked at address: {:x} and offset: {:x}", target01.address(), target01.offset());

			//GetDescription Hook AE
			REL::Relocation<std::uintptr_t> target02{ RELOCATION_ID(0, 14552), REL::VariantOffset(0x0, 0xFB, 0x0) };
			stl::write_thunk_call<GetDescriptionHookAE>(target02.address());
			SKSE::log::info("GetDescriptionHookAE hooked at address: {:x} and offset: {:x}", target02.address(), target02.offset());

			REL::Relocation<std::uintptr_t> target03{ RELOCATION_ID(0, 14552), REL::VariantOffset(0x0, 0x174, 0x0) };
			stl::write_thunk_call<GetDescriptionHookAE>(target03.address());
			SKSE::log::info("GetDescriptionHookAE hooked at address: {:x} and offset: {:x}", target03.address(), target03.offset());

			//ItemCard Hook AE
			REL::Relocation<std::uintptr_t> target09{ RELOCATION_ID(0, 51458), REL::VariantOffset(0x0, 0x87, 0x0) };
			stl::write_thunk_call<ItemCardPopulateHook>(target09.address());
			SKSE::log::info("ItemCardPopulateHook hooked at address: {:x} and offset: {:x}", target09.address(), target09.offset());

			//DialogueMenu text hook AE
			REL::Relocation<std::uintptr_t> target15{ RELOCATION_ID(0, 35254), REL::VariantOffset(0x0, 0x115, 0x0) };
			stl::write_thunk_call<DialogueMenuTextHook>(target15.address());
			SKSE::log::info("DialogueMenuTextHook hooked at address: {:x} and offset: {:x}", target15.address(), target15.offset());
		}

		if (REL::Module::IsSE() || REL::Module::IsVR())
		{
			//GetDescription Hook SE
			REL::Relocation<std::uintptr_t> target10{ RELOCATION_ID(14399, 0), REL::VariantOffset(0x53, 0x0, 0x53) };
			stl::write_thunk_call<GetDescriptionHookSE>(target10.address());
			SKSE::log::info("GetDescriptionHookSE hooked at address: {:x} and offset: {:x}", target10.address(), target10.offset());
		}

	}
}