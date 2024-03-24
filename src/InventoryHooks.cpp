#include "MiscHooks.h"
#include "InventoryHooks.h"
#include "Processor.h"

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
			handleDescriptionItems(itemCard, *a_item);
		};

		static void handleDescriptionItems(RE::ItemCard* itemCard, RE::TESBoundObject* a_item)
		{
			for (const auto& Information : g_ConfigurationInformationStruct)
			{
				switch (Information.SubrecordType)
				{
				case Config::SubrecordType::kCNAM:
				{
					// Replace the ItemCardDescription of books
					if (a_item->formID == Information.Form->formID)
					{
						auto descriptionValue = RE::GFxValue(Information.ReplacerText);
						itemCard->obj.SetMember("description", descriptionValue);

#ifndef NDEBUG
						SKSE::log::debug("Replaced BOOK CNAM {0:08X} with:", a_item->formID);
						SKSE::log::debug("{}", Information.ReplacerText);
#endif
					}
				}
				break;

				default: break;
				}
			}
		}
		static inline REL::Relocation<decltype(thunk)> func;


		static void Install()
		{
			REL::Relocation<std::uintptr_t> target01{ RELOCATION_ID(50005, 50949), REL::VariantOffset(0x80, 0x80, 0x80) };
			stl::write_thunk_call<ItemCardPopulateHook>(target01.address());
			SKSE::log::info("ItemCardPopulateHook hooked at address: {:x} and offset: {:x}", target01.address(), target01.offset());

			REL::Relocation<std::uintptr_t> target02{ RELOCATION_ID(50201, 51130), REL::VariantOffset(0xB2, 0xB2, 0xB2) };
			stl::write_thunk_call<ItemCardPopulateHook>(target02.address());
			SKSE::log::info("ItemCardPopulateHook hooked at address: {:x} and offset: {:x}", target02.address(), target02.offset());

			REL::Relocation<std::uintptr_t> target03{ RELOCATION_ID(50297, 51218), REL::VariantOffset(0x35, 0x35, 0x35) };
			stl::write_thunk_call<ItemCardPopulateHook>(target03.address());
			SKSE::log::info("ItemCardPopulateHook hooked at address: {:x} and offset: {:x}", target03.address(), target03.offset());

			REL::Relocation<std::uintptr_t> target04{ RELOCATION_ID(50674, 51569), REL::VariantOffset(0x80, 0x7A, 0x80) };
			stl::write_thunk_call<ItemCardPopulateHook>(target04.address());
			SKSE::log::info("ItemCardPopulateHook hooked at address: {:x} and offset: {:x}", target04.address(), target04.offset());

			REL::Relocation<std::uintptr_t> target05{ RELOCATION_ID(50973, 51852), REL::VariantOffset(0x80, 0x7A, 0x80) };
			stl::write_thunk_call<ItemCardPopulateHook>(target05.address());
			SKSE::log::info("ItemCardPopulateHook hooked at address: {:x} and offset: {:x}", target05.address(), target05.offset());

			if (REL::Module::IsAE())
			{
				REL::Relocation<std::uintptr_t> target09{ RELOCATION_ID(0, 51458), REL::VariantOffset(0x0, 0x87, 0x0) };
				stl::write_thunk_call<ItemCardPopulateHook>(target09.address());
				SKSE::log::info("ItemCardPopulateHook hooked at address: {:x} and offset: {:x}", target09.address(), target09.offset());
			}

		}
	};

	void InstallInventoryHooks()
	{
		ItemCardPopulateHook::Install();
	}

}