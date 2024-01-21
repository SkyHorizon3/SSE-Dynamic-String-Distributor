#include "../include/Globals.h"
#include "../include/Hooks.h"
#include "../include/Processor.h"

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

		//Spells missing
		//Test entering caves

		static void handleSpellTomes(RE::TESObjectBOOK* book, const ConfigurationInformation& information)
		{

			auto spell = book->GetSpell();
			if (spell && spell->effects.size())
			{
				auto OrigSize = spell->effects.size();

				for (RE::BSTArrayBase::size_type i = 0; i < OrigSize; ++i)
				{
					if (spell->effects[i]->baseEffect->formID == information.Form->formID)
					{
						spell->effects[i]->baseEffect->magicItemDescription = information.ReplacerText;
					}
				}

			}
		}

		static void handleMagicItem(RE::MagicItem* magicItem, const ConfigurationInformation& information)
		{
			if (magicItem && magicItem->effects.size())
			{
				auto OrigSize = magicItem->effects.size();

				for (RE::BSTArrayBase::size_type i = 0; i < OrigSize; ++i)
				{

					if (magicItem->effects[i]->baseEffect->formID == information.Form->formID)
					{
						magicItem->effects[i]->baseEffect->magicItemDescription = information.ReplacerText;
					}

				}

			}
		}

		static void handleArmor(RE::TESObjectARMO* Item, const ConfigurationInformation& information)
		{
			auto Enchanting = Item->formEnchanting;
			if (Enchanting && Enchanting->effects.size())
			{
				auto OrigSize = Enchanting->effects.size();

				for (RE::BSTArrayBase::size_type i = 0; i < OrigSize; ++i)
				{
					if (Enchanting->effects[i]->baseEffect->formID == information.Form->formID)
					{
						Enchanting->effects[i]->baseEffect->magicItemDescription = information.ReplacerText;
					}
				}

			}
		}

		static void handleWeapon(RE::TESObjectWEAP* Item, const ConfigurationInformation& information)
		{
			auto Enchanting = Item->formEnchanting;
			if (Enchanting && Enchanting->effects.size())
			{
				auto OrigSize = Enchanting->effects.size();

				for (RE::BSTArrayBase::size_type i = 0; i < OrigSize; ++i)
				{
					if (Enchanting->effects[i]->baseEffect->formID == information.Form->formID)
					{
						Enchanting->effects[i]->baseEffect->magicItemDescription = information.ReplacerText;
					}
				}

			}
		}

		static void handleEffectsItems(RE::TESBoundObject* a_item)
		{
			for (const auto& information : g_ConfigurationInformationStruct)
			{
				if (information.RecordType == "MGEF" && information.SubrecordType == "DNAM")
				{
					if (a_item->IsBook())
					{
						auto book = a_item->As<RE::TESObjectBOOK>();
						handleSpellTomes(book, information);
					}
					else if (a_item->IsArmor())
					{
						auto armor = a_item->As<RE::TESObjectARMO>();
						handleArmor(armor, information);
					}
					else if (a_item->IsWeapon())
					{
						auto weapon = a_item->As<RE::TESObjectWEAP>();
						handleWeapon(weapon, information);
					}
					else
					{
						auto magicItem = a_item->As<RE::MagicItem>();
						handleMagicItem(magicItem, information);
					}
				}
			}
		}

		static void handleDescriptionItems(RE::ItemCard* itemCard, RE::TESBoundObject* a_item)
		{
			for (const auto& Information : g_ConfigurationInformationStruct)
			{
				if (a_item->formID == Information.Form->formID)
				{
					if (Information.SubrecordType == "CNAM") // Replace the ItemCardDescription of books
					{
						auto descriptionValue = RE::GFxValue(Information.ReplacerText);
						itemCard->obj.SetMember("description", descriptionValue);
					}
					else if (Information.SubrecordType == "DESC")
					{
						// If someone only translates DESC and the book has CNAM in the esp, then the CNAM description is lost
						// There is nothing I can do. It is very unlikely that this will happen. If it does, just write the CNAM description to a json file.
						// The problem is that DESC from the GetDescription hook also replaces CNAM before I can get the original string (CNAM) in the UI.
						// If you also put CNAM in a json, GetDescription will put DESC in CNAM, but with the ItemCardHook we will put the description after.

						auto descriptionValue = RE::GFxValue("");
						itemCard->obj.SetMember("description", descriptionValue);

					}
				}
			}
		}
		static inline REL::Relocation<decltype(thunk)> func;

	};


	// Track if an object is valid between the two AE hooks
	static bool IsDESC = false;
	std::string SetDescription = "";
	struct ParentDESCHookAE
	{
		static void thunk(RE::BSString* a_out, RE::TESForm* a_parent, std::uint64_t a_unk)
		{
			//Perks, Loading Screens, Race Descriptions and any other DESC in SSEDIT

			UNREFERENCED_PARAMETER(a_unk);

			func(a_out, nullptr, 0); // Invoke original

			for (const auto& Information : g_ConfigurationInformationStruct)
			{

				if (a_parent && a_parent->formID == Information.Form->formID && Information.SubrecordType == "DESC") //So it doesn't set CNAM as DESC if both are in a json
				{
					IsDESC = true;
					SetDescription = Information.ReplacerText;
					// Set object valid for GetDescription hook to check

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

			if (IsDESC)
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
				if (a_parent && a_parent->formID == Information.Form->formID && Information.SubrecordType == "DESC")
				{
					*a_out = Information.ReplacerText;
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

	typedef void(WINAPI* MessageBoxDataHook_pFunc)(RE::MessageBoxData* Menu);
	MessageBoxDataHook_pFunc originalFunction01;
	void MessageBoxFunc(RE::MessageBoxData* Menu) //MESG ITXT
	{
		if (!Menu)
		{
			return originalFunction01(Menu);
		}

		RE::BSTArray<RE::BSString, RE::BSTArrayHeapAllocator>::size_type originalSize = Menu->buttonText.size();

		for (RE::BSTArray<RE::BSString, RE::BSTArrayHeapAllocator>::size_type i = 0; i < originalSize; ++i)
		{
			//g_Logger->info("Processing buttonText[{}]", i);

			if (i < Menu->buttonText.size()) //Just to make sure
			{
				auto it = g_INFO_NAM1_ITXT_Map.find(Menu->buttonText[i].c_str());

				if (it != g_INFO_NAM1_ITXT_Map.end())
				{
					Menu->buttonText[i] = it->second;
				}
			}
		}

		return originalFunction01(Menu);
	}


	typedef void(WINAPI* LoadScreen_pFunc)(RE::TESLoadScreen* loadscreen, float* a2);
	LoadScreen_pFunc originalFunction02;
	void LoadScreenFunc(RE::TESLoadScreen* loadscreen, float* a2) //LSCR DESC
	{

		for (const auto& Information : g_ConfigurationInformationStruct)
		{
			if (loadscreen && loadscreen->formID == Information.Form->formID)
			{
				loadscreen->loadingText = Information.ReplacerText;
			}
		}

		return originalFunction02(loadscreen, a2);
	}

	typedef void(WINAPI* MapMarker_pFunc)(RE::TESObjectREFR* marker);
	MapMarker_pFunc originalFunction03;
	void MapMarkerFunc(RE::TESObjectREFR* marker) //REFR FULL in Hudmenu and map
	{
		if (auto mapMarker = marker->extraList.GetByType<RE::ExtraMapMarker>())
		{
			if (auto data = mapMarker->mapData)
			{
				try
				{
					data->locationName.fullName = g_FLOR_RNAM_RDMP_Map.at(data->locationName.fullName.c_str());
				}
				catch (const std::out_of_range&)
				{
					//Thats very unsafe, but I can't figure out the problem
				}

			}
		}

		return originalFunction03(marker);
	}

	//Credits to po3 https://github.com/powerof3/SimpleActivateSKSE released under MIT
	//Still compatible
	struct RNAM_RDMP_TextHook
	{
		static void thunk(RE::UIMessageQueue* queue_this, const RE::BSFixedString& menuName, RE::UI_MESSAGE_TYPE type, RE::IUIMessageData* data)
		{
			const auto messagedata = data ? static_cast<RE::HUDData*>(data) : nullptr;
			const auto crosshairRef = messagedata ? messagedata->crossHairRef.get() : RE::TESObjectREFRPtr();

			if (messagedata && crosshairRef)
			{
				switch (crosshairRef->GetBaseObject()->GetFormType())
				{
				case RE::FormType::Flora:
				case RE::FormType::Tree: //FLOR RNAM
				{
					std::istringstream iss(messagedata->text.c_str());
					std::string line1, line2;

					if (std::getline(iss, line1) && std::getline(iss, line2))
					{

						auto it = g_FLOR_RNAM_RDMP_Map.find(line1);

						if (it != g_FLOR_RNAM_RDMP_Map.end())
						{
							line1 = it->second;

							messagedata->text = line1 + "\n" + line2;
						}

					}

				}
				break;
				case RE::FormType::Door: //REGN RDMP for doors
				{
					std::istringstream iss(messagedata->text.c_str());
					std::string line1, line2;

					//g_Logger->info("String: {}", messagedata->text.c_str());

					if (std::getline(iss, line1) && std::getline(iss, line2))
					{
						auto it = g_FLOR_RNAM_RDMP_Map.find(line2);

						if (it != g_FLOR_RNAM_RDMP_Map.end())
						{
							line2 = it->second;

							messagedata->text = line1 + "\n" + line2;
						}
					}
				}
				break;

				default:
				{
					break;
				}

				}

			}

			func(queue_this, menuName, type, data);
		};
		static inline REL::Relocation<decltype(thunk)> func;

	};

	bool HudMenu::containsOnlySpace(const std::string& str)
	{
		return std::all_of(str.begin(), str.end(), [](unsigned char c) { return std::isspace(c); });
	}

	RE::UI_MESSAGE_RESULTS HudMenu::ProcessMessageHook(RE::UIMessage& a_message) //REGN RDMP if you're leaving a cave or something like that
	{

		if (a_message.type == RE::UI_MESSAGE_TYPE::kUpdate)
		{

			RE::GFxValue ObjectiveText;

			auto ui = RE::UI::GetSingleton();
			auto menu = ui->GetMenu(RE::HUDMenu::MENU_NAME);

			menu->uiMovie->GetVariable(&ObjectiveText, "HUDMovieBaseInstance.RolloverText");

			if (ObjectiveText.GetType() != RE::GFxValue::ValueType::kUndefined)
			{
				RE::GFxValue Text;
				ObjectiveText.GetMember("text", &Text);
				std::string OrigString = Text.GetString();

				if (!OrigString.empty() && !containsOnlySpace(OrigString))
				{
					//Look if it's only one line
					size_t newlinePos = OrigString.find('\n');
					if (newlinePos == std::string::npos)
					{
						// Extract text before and after the first space
						size_t spacePos = OrigString.find(' ');
						if (spacePos != std::string::npos)
						{
							std::string before = OrigString.substr(0, spacePos);
							std::string after = OrigString.substr(spacePos + 1);


							auto it = g_FLOR_RNAM_RDMP_Map.find(after);
							if (it != g_FLOR_RNAM_RDMP_Map.end())
							{
								std::string NewString = before + " " + it->second;
								RE::GFxValue newDes(NewString);
								ObjectiveText.SetMember("text", newDes);

								//g_Logger->info("NewString: {}", NewString);
							}

						}
					}
				}
			}
		}

		return func(this, a_message);
	}

	void InstallHooks()
	{
		//RNAM_RDMP_TextHook
		REL::Relocation<std::uintptr_t> target17{ RELOCATION_ID(39535, 40621), REL::VariantOffset(0x289, 0x280, 0x289) };
		stl::write_thunk_call<RNAM_RDMP_TextHook>(target17.address());
		g_Logger->info("RNAM_RDMP_TextHook hooked at address: {:x} and offset: {:x}", target17.address(), target17.offset());

		//DialogueMenu text hook
		REL::Relocation<std::uintptr_t> target16{ RELOCATION_ID(34434, 35254), REL::VariantOffset(0xCC, 0x226, 0xCC) };
		stl::write_thunk_call<DialogueMenuTextHook>(target16.address());
		g_Logger->info("DialogueMenuTextHook hooked at address: {:x} and offset: {:x}", target16.address(), target16.offset());

		//JournalMenu quest description text Hook
		REL::Relocation<std::uintptr_t> target14{ RELOCATION_ID(24778, 25259), REL::VariantOffset(0x21C, 0x221, 0x21C) };//First: 5B ->Don't know; Second: C4 ->Don't know; Third: 221 ->Quest description text
		stl::write_thunk_call<QuestObjectiveTextHook>(target14.address());
		g_Logger->info("QuestObjectiveTextHook hooked at address: {:x} and offset: {:x}", target14.address(), target14.offset());

		//HudMenu quest objective text Hook
		REL::Relocation<std::uintptr_t> target13{ RELOCATION_ID(50751, 51646), REL::VariantOffset(0xA6, 0xA6, 0xA6) };//First: A6 ->All NNAM sentences are passing here; (Second: F7 ->"or" (depends on language) if needed; Third: 143 ->Second sentence after "or" if available)
		stl::write_thunk_call<QuestObjectiveTextHook>(target13.address());
		g_Logger->info("QuestObjectiveTextHook hooked at address: {:x} and offset: {:x}", target13.address(), target13.offset());

		//JournalMenu quest objective text Hook
		REL::Relocation<std::uintptr_t> target12{ RELOCATION_ID(23229, 23684), REL::VariantOffset(0x21, 0x21, 0x21) };
		stl::write_thunk_call<QuestObjectiveTextHook>(target12.address());
		g_Logger->info("QuestObjectiveTextHook hooked at address: {:x} and offset: {:x}", target12.address(), target12.offset());

		//DialogueStream (subtitles) Hook
		REL::Relocation<std::uintptr_t> target11{ RELOCATION_ID(34429, 35249), REL::VariantOffset(0x61, 0x61, 0x61) };
		stl::write_thunk_call<DialogueStreamHook>(target11.address());
		g_Logger->info("DialogueStreamHook hooked at address: {:x} and offset: {:x}", target11.address(), target11.offset());

		//ItemCard Hook
		REL::Relocation<std::uintptr_t> target04{ RELOCATION_ID(50005, 50949), REL::VariantOffset(0x80, 0x80, 0x80) };
		stl::write_thunk_call<ItemCardPopulateHook>(target04.address());
		g_Logger->info("ItemCardPopulateHook hooked at address: {:x} and offset: {:x}", target04.address(), target04.offset());

		REL::Relocation<std::uintptr_t> target05{ RELOCATION_ID(50201, 51130), REL::VariantOffset(0xB2, 0xB2, 0xB2) };
		stl::write_thunk_call<ItemCardPopulateHook>(target05.address());
		g_Logger->info("ItemCardPopulateHook hooked at address: {:x} and offset: {:x}", target05.address(), target05.offset());

		REL::Relocation<std::uintptr_t> target06{ RELOCATION_ID(50297, 51218), REL::VariantOffset(0x35, 0x35, 0x35) };
		stl::write_thunk_call<ItemCardPopulateHook>(target06.address());
		g_Logger->info("ItemCardPopulateHook hooked at address: {:x} and offset: {:x}", target06.address(), target06.offset());

		REL::Relocation<std::uintptr_t> target07{ RELOCATION_ID(50674, 51569), REL::VariantOffset(0x80, 0x7A, 0x80) };
		stl::write_thunk_call<ItemCardPopulateHook>(target07.address());
		g_Logger->info("ItemCardPopulateHook hooked at address: {:x} and offset: {:x}", target07.address(), target07.offset());

		REL::Relocation<std::uintptr_t> target08{ RELOCATION_ID(50973, 51852), REL::VariantOffset(0x80, 0x7A, 0x80) };
		stl::write_thunk_call<ItemCardPopulateHook>(target08.address());
		g_Logger->info("ItemCardPopulateHook hooked at address: {:x} and offset: {:x}", target08.address(), target08.offset());

		//MessageBoxData Hook SE
		const auto MessageBoxDataFunc = RELOCATION_ID(51422, 52271).address();
		const auto LoadScreenOffsetFunc = RELOCATION_ID(21368, 21833).address(); //Not tested on SE
		const auto MapMarkerOffsetFunc = RELOCATION_ID(19814, 20219).address(); //Not tested on SE

		originalFunction01 = (MessageBoxDataHook_pFunc)MessageBoxDataFunc;
		originalFunction02 = (LoadScreen_pFunc)LoadScreenOffsetFunc;
		originalFunction03 = (MapMarker_pFunc)MapMarkerOffsetFunc;

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(reinterpret_cast<PVOID*>(&originalFunction01), static_cast<PVOID>(&MessageBoxFunc));
		DetourAttach(reinterpret_cast<PVOID*>(&originalFunction02), static_cast<PVOID>(&LoadScreenFunc));
		DetourAttach(reinterpret_cast<PVOID*>(&originalFunction03), static_cast<PVOID>(&MapMarkerFunc));

		const auto err = DetourTransactionCommit();
		if (err == NO_ERROR)
		{
			g_Logger->info("Installed MessageBoxDataFunc hook at {0:x}", MessageBoxDataFunc);
			g_Logger->info("Installed LoadScreenFunc hook at {0:x}", LoadScreenOffsetFunc);
			g_Logger->info("Installed MapMarkerFunc hook at {0:x}", MapMarkerOffsetFunc);

		}
		else
		{
			g_Logger->error("DetourTransactionCommit failed with error code: {}", err);
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
			g_Logger->info("ParentDESCHookAE hooked at address: {:x} and offset: {:x}", target01.address(), target01.offset());

			//GetDescription Hook AE
			REL::Relocation<std::uintptr_t> target02{ RELOCATION_ID(0, 14552), REL::VariantOffset(0x0, 0xFB, 0x0) };
			stl::write_thunk_call<GetDescriptionHookAE>(target02.address());
			g_Logger->info("GetDescriptionHookAE hooked at address: {:x} and offset: {:x}", target02.address(), target02.offset());

			REL::Relocation<std::uintptr_t> target03{ RELOCATION_ID(0, 14552), REL::VariantOffset(0x0, 0x174, 0x0) };
			stl::write_thunk_call<GetDescriptionHookAE>(target03.address());
			g_Logger->info("GetDescriptionHookAE hooked at address: {:x} and offset: {:x}", target03.address(), target03.offset());

			//ItemCard Hook AE
			REL::Relocation<std::uintptr_t> target09{ RELOCATION_ID(0, 51458), REL::VariantOffset(0x0, 0x87, 0x0) };
			stl::write_thunk_call<ItemCardPopulateHook>(target09.address());
			g_Logger->info("ItemCardPopulateHook hooked at address: {:x} and offset: {:x}", target09.address(), target09.offset());

			//DialogueMenu text hook AE
			REL::Relocation<std::uintptr_t> target15{ RELOCATION_ID(0, 35254), REL::VariantOffset(0x0, 0x115, 0x0) };
			stl::write_thunk_call<DialogueMenuTextHook>(target15.address());
			g_Logger->info("DialogueMenuTextHook hooked at address: {:x} and offset: {:x}", target15.address(), target15.offset());
		}

		if (REL::Module::IsSE() || REL::Module::IsVR())
		{
			//GetDescription Hook SE
			REL::Relocation<std::uintptr_t> target10{ RELOCATION_ID(14399, 0), REL::VariantOffset(0x53, 0x0, 0x53) };
			stl::write_thunk_call<GetDescriptionHookSE>(target10.address());
			g_Logger->info("GetDescriptionHookSE hooked at address: {:x} and offset: {:x}", target10.address(), target10.offset());
		}

	}
}