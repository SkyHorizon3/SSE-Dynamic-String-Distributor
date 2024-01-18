#include "../include/Globals.h"
#include "../include/Hooks.h"
#include "../include/Processor.h"

#include <detours/detours.h>


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
			handleEffectsItems(*a_item);
			handleDescriptionItems(itemCard, *a_item);
		};

		static void handleSpellTomes(RE::TESObjectBOOK* book, const ConfigurationInformation& information)
		{
			auto spell = book->GetSpell();
			if (spell && spell->GetAVEffect()->formID == information.Form->formID)
			{
				spell->GetAVEffect()->magicItemDescription = information.ReplacerText;
			}
		}

		static void handleMagicItem(RE::MagicItem* magicItem, const ConfigurationInformation& information)
		{
			if (magicItem && magicItem->GetAVEffect()->formID == information.Form->formID)
			{
				magicItem->GetAVEffect()->magicItemDescription = information.ReplacerText;
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
						// If somebody only translates DESC and the book has CNAM in the esp, than the CNAM description will be lost
						// Nothing we can do. Thats very unlikely to happen. If so, just put the CNAM description into a json.
						auto descriptionValue = RE::GFxValue("");
						itemCard->obj.SetMember("description", descriptionValue);

						// Set ItemCardDescription to empty. This stops the GetDescription hook from replacing also CNAM not only DESC in the UI

					//Check if description is empty, if not, keep it...

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

				if (a_parent && a_parent->formID == Information.Form->formID)
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

	struct GetDescriptionHookAE
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
				if (a_parent && a_parent->formID == Information.Form->formID)
				{
					*a_out = Information.ReplacerText;
				}

			}
		}
		static inline REL::Relocation<decltype(thunk)> func;

	};

	struct DialogueStreamHook
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
	struct QuestObjectiveTextHook
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

	struct DialogueMenuTextHook
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
	void MessageBoxThunk(RE::MessageBoxData* Menu)
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
	void LoadScreenFunc(RE::TESLoadScreen* loadscreen, float* a2)
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


	void InstallHooks()
	{

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
		const auto MessageBoxDataFunc_funcAddress = &MessageBoxThunk;

		const auto LoadScreenOffsetFunc = RELOCATION_ID(21368, 21833).address();
		const auto LoadScreenOffsetFunc_funcAddress = &LoadScreenFunc;

		originalFunction01 = (MessageBoxDataHook_pFunc)MessageBoxDataFunc;
		originalFunction02 = (LoadScreen_pFunc)LoadScreenOffsetFunc;

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(reinterpret_cast<PVOID*>(&originalFunction01), static_cast<PVOID>(&MessageBoxThunk));
		DetourAttach(reinterpret_cast<PVOID*>(&originalFunction02), static_cast<PVOID>(&LoadScreenFunc));

		const auto err = DetourTransactionCommit();
		if (err == NO_ERROR)
		{
			g_Logger->info("Installed MessageBoxDataFunc hook at {0:x} with replacement from address {1:x}", MessageBoxDataFunc, reinterpret_cast<uintptr_t>(MessageBoxDataFunc_funcAddress));
			g_Logger->info("Installed LoadScreenFunc hook at {0:x} with replacement from address {1:x}", LoadScreenOffsetFunc, reinterpret_cast<uintptr_t>(LoadScreenOffsetFunc_funcAddress));
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