#include "../include/Globals.h"
#include "../include/Hooks.h"
#include "../include/Processor.h"

namespace Hook
{
	//HUGE Credits to Nightfallstorm for some of the hooks!!
	//https://github.com/Nightfallstorm/DescriptionFramework released under GPL-3.0
	//https://github.com/Nightfallstorm/Recipes released under GPL-3.0
	struct ItemCardPopulateHook
	{
		static void thunk(RE::ItemCard* itemCard, RE::TESBoundObject** a_item, char a3)
		{
			if (!itemCard || !a_item || !*a_item)
			{
				return func(itemCard, a_item, a3);
			}

			func(itemCard, a_item, a3);
			handleEffectsItems(itemCard, *a_item);
			handleDescriptionItems(itemCard, *a_item);
		};

		static void handleSpellTomes(RE::ItemCard* itemCard, RE::TESObjectBOOK* book, const ConfigurationInformation& information)
		{
			auto spell = book->GetSpell();
			if (spell && spell->GetAVEffect()->formID == information.Form->formID)
			{
				auto descriptionValue = RE::GFxValue(information.ReplacerText);
				itemCard->obj.SetMember("effects", descriptionValue);
			}
		}

		static void handleMagicItem(RE::ItemCard* itemCard, RE::MagicItem* magicItem, const ConfigurationInformation& information)
		{
			if (magicItem && magicItem->GetAVEffect()->formID == information.Form->formID)
			{
				auto descriptionValue = RE::GFxValue(information.ReplacerText);
				itemCard->obj.SetMember("effects", descriptionValue);
			}
		}

		static void handleEffectsItems(RE::ItemCard* itemCard, RE::TESBoundObject* a_item)
		{
			for (const auto& information : g_ConfigurationInformationStruct)
			{
				if (information.RecordType == "MGEF" && information.SubrecordType == "DNAM")
				{
					if (a_item->IsBook())
					{
						auto book = a_item->As<RE::TESObjectBOOK>();
						handleSpellTomes(itemCard, book, information);
					}
					else
					{
						auto magicItem = a_item->As<RE::MagicItem>();
						handleMagicItem(itemCard, magicItem, information);
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
						// If somebody only translates DESC and the book has CNAM in the esp, than the CNAM description will be lost
						// Nothing we can do. Thats very unlikely to happen. If so, just put the CNAM description into a json.
						auto descriptionValue = RE::GFxValue("");
						itemCard->obj.SetMember("description", descriptionValue);

						// Set ItemCardDescription to empty. This stops the GetDescription hook from replacing also CNAM not only DESC in the UI
					}
				}
			}
		}
		static inline REL::Relocation<decltype(thunk)> func;

	};


	// Track if a book is valid between the two AE hooks
	static bool IsDESC = false;
	std::string SetDescription = "";
	struct ParentDESCHookAE
	{
		// AE inlines GetDescription, so we have to use a new hook right after the check for the parent form
		// so we can validate that parent form is a something with DESC
		static void thunk(RE::BSString* a_out, RE::TESForm* a_parent, std::uint64_t a_unk)
		{
			//Perks, Loading Screens, Race Descriptions and any other DESC in SSEDIT

			func(a_out, nullptr, 0); // Invoke original (BSString::Set(a_out, 0, 0)

			for (const auto& Information : g_ConfigurationInformationStruct)
			{

				if (a_parent && a_parent->formID == Information.Form->formID && Information.SubrecordType == "DESC")
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
			// We can trust rdx will be scratched over when we jump back to the skyirm code
			// This moves the parent form, which is currently in rdi, to rdx so our ValidateBookAE hook can use it
			// rdx is already zeroed out, so we aren't worried about losing any data
			GetParentInsideHook()
			{
				mov(rdx, rdi);
			}
		};
	};


	struct GetDescriptionHookAE
	{
		static void thunk(RE::BSString* a_out, std::uint64_t a_unk, std::uint64_t a_unk2)
		{
			func(a_out, a_unk, a_unk2);

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

	struct DialogueStreamHook
	{
		static void thunk(RE::BSString* a_out, std::uint64_t a_unk, std::uint64_t a_unk2)
		{
			func(a_out, a_unk, a_unk2);

			auto it = g_INFO_NAM1_Map.find(a_out->c_str());

			if (it != g_INFO_NAM1_Map.end())
			{
				*a_out = it->second;
			}

		}
		static inline REL::Relocation<decltype(thunk)> func;

	};

	bool JournalMenu::IsViewingMiscObjectives() noexcept
	{
		RE::GFxValue root, boolvalue;
		this->uiMovie->GetVariable(&root, "QuestJournalFader.Menu_mc.QuestsFader.Page_mc");
		return (root.Invoke("isViewingMiscObjectives", &boolvalue) && boolvalue.GetBool());
	}

	RE::UI_MESSAGE_RESULTS JournalMenu::ProcessMessageHook(RE::UIMessage& a_message)
	{

		if (a_message.type == RE::UI_MESSAGE_TYPE::kUpdate)
		{
			RE::GFxValue DescriptionText, ObjectiveText;
			this->uiMovie->GetVariable(&DescriptionText, "QuestJournalFader.Menu_mc.QuestsFader.Page_mc.DescriptionText"); //CNAM Quests in Journal Menu

			if (DescriptionText.GetType() != RE::GFxValue::ValueType::kUndefined && !IsViewingMiscObjectives())
			{
				RE::GFxValue Text;
				DescriptionText.GetMember("htmlText", &Text);
				std::string OrigDesString = Text.GetString();
				//g_Logger->info("Text: {}", string.c_str());

				auto it = g_QUST_CNAM_Map.find(OrigDesString.c_str());

				if (it != g_QUST_CNAM_Map.end())
				{
					RE::GFxValue newDes(it->second);
					DescriptionText.SetMember("htmlText", newDes);
				}
			}

			for (int entryIndex = 0; entryIndex <= 11; ++entryIndex)
			{
				std::string entryVariableName = "QuestJournalFader.Menu_mc.QuestsFader.Page_mc.ObjectiveList.Entry" + std::to_string(entryIndex) + ".textField";

				this->uiMovie->GetVariable(&ObjectiveText, entryVariableName.c_str()); //NNAM (objective text) from Quests in Journal Menu

				if (ObjectiveText.GetType() != RE::GFxValue::ValueType::kUndefined)
				{
					RE::GFxValue Text;
					ObjectiveText.GetMember("text", &Text);
					std::string string = Text.GetString();
					//g_Logger->info("Text: {}", string.c_str());

					auto it = g_QUST_NNAM_Map.find(string.c_str());

					if (it != g_QUST_NNAM_Map.end())
					{
						RE::GFxValue newDes(it->second);
						ObjectiveText.SetMember("text", newDes);
					}
				}
			}

		}

		return func(this, a_message);
	}

	/*
	bool HudMenu::ProcessMessageHook(RE::UIMessage* a_message)
	{

		if (a_message->type == RE::UI_MESSAGE_TYPE::kUpdate)
		{
			RE::GFxValue ObjectiveLineInstance, ObjectiveTextFieldInstance, TextFieldInstance, textField;

			//"ObjectiveLineInstance.ObjectiveTextFieldInstance.TextFieldInstance.text"

			auto ui = RE::UI::GetSingleton();
			auto menu = ui->GetMenu(RE::HUDMenu::MENU_NAME);

			bool success = menu->uiMovie->GetVariable(&ObjectiveLineInstance, "_root.HUDMovieBaseInstance.ObjectiveLineInstance."); //.objective1.ObjectiveTextFieldInstance.TextFieldInstance.text

			if (!success)
			{
				g_Logger->info("Couldn't get ObjectiveLine_mc");
			}
			else
			{
				g_Logger->info("Got ObjectiveLine_mc");
			}

			/*
			success = ObjectiveLineInstance.GetMember("text", &textField);
			if (!success)
			{
				g_Logger->info("Couldn't get text");
			}
			else
			{
				g_Logger->info("Got text");

			}
			*/

			/*
			bool success = this->root.GetMember("ObjectiveLineInstance", &ObjectiveLineInstance);
			if (!success)
			{
				g_Logger->info("Couldn't get ObjectiveLineInstance");
			}
			else
			{
				g_Logger->info("Got ObjectiveLineInstance");

			}


			success = ObjectiveLineInstance.GetMember("ObjectiveTextFieldInstance", &ObjectiveTextFieldInstance);
			if (!success)
			{
				g_Logger->info("Couldn't get ObjectiveTextFieldInstance"); //MovieClipsA
			}
			else
			{
				g_Logger->info("Got ObjectiveTextFieldInstance");

			}



			success = ObjectiveTextFieldInstance.GetMember("TextFieldInstance", &TextFieldInstance);
			if (!success)
			{
				g_Logger->info("Couldn't get TextFieldInstance");
			}
			else
			{
				g_Logger->info("Got TextFieldInstance");

			}


			success = TextFieldInstance.GetMember("htmlText", &textField);
			if (!success)
			{
				g_Logger->info("Couldn't get htmlText");
			}
			else
			{
				g_Logger->info("Got htmlText");

			}


			std::string string = textField.GetString();
			g_Logger->info("Text: {}", string.c_str());


			//RE::GFxValue newDes("Es wurde ersetzt.");
			//TextFieldInstance.SetMember("text", newDes);


			//ObjectiveLineInstance.ObjectiveTextFieldInstance.TextFieldInstance.text

}
return func(this, a_message);
	}
	*/

	/*
	//Not used, because it looks like shit
	void DialogueMenu::ProcessMessageHook()
	{

		func(this);

		for (int entryIndex = 0; entryIndex <= 10; ++entryIndex)
		{
			std::string entryVariableName = "DialogueMenu_mc.TopicList.Entry" + std::to_string(entryIndex) + ".textField";

			RE::GFxValue DialogueText;
			this->uiMovie->GetVariable(&DialogueText, entryVariableName.c_str());

			if (DialogueText.GetType() != RE::GFxValue::ValueType::kUndefined)
			{
				RE::GFxValue Text;
				DialogueText.GetMember("text", &Text);
				std::string string = Text.GetString();

				RE::GFxValue newDes("replaced");
				DialogueText.SetMember("text", newDes);

				g_Logger->info("Entry {}: {}", entryIndex, string.c_str());
			}
		}

	}
	*/

	//RNAM in INFO; 
	//NNAM in HudMenu
	//ITXT Message
	//Overwrite Ordner

	void InstallHooks()
	{

		//DialogueStream Hook
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