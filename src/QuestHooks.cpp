#include "MiscHooks.h"
#include "QuestHooks.h"
#include "Processor.h"
#include "Utils.h"

namespace Hook
{
	/*
	struct QuestObjectiveHook //QUST NNAM text hook for Journal menu
	{
		static void thunk(const RE::BGSInstancedQuestObjective* a_objective, RE::BSString& a_out)
		{
			if (!a_objective)
			{
				return func(a_objective, a_out);
			}
			func(a_objective, a_out);

			auto objective = a_objective->Objective;
			size_t key = Utils::combineHashWithIndex(objective->ownerQuest->formID, objective->index, Utils::GetModName(objective->ownerQuest));

			auto it = g_QUST_NNAM_Map.find(key);
			if (it != g_QUST_NNAM_Map.end())
			{
				a_out = it->second;
				objective->displayText = it->second; //probably not necessary
			}
			else
			{
				a_out = objective->displayText.c_str(); //This function populates the output string. So we want the default if we're not relacing it.
			}

		};
		static inline REL::Relocation<decltype(thunk)> func;

		static void Install()
		{
			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(21951, 22433), REL::VariantOffset(0x147, 0x161, 0x147) };
			stl::write_thunk_call<QuestObjectiveHook>(target1.address());
			SKSE::log::info("QuestObjectiveHook hooked at address: {:x} and offset: {:x}", target1.address(), target1.offset());

			REL::Relocation<std::uintptr_t> target2{ RELOCATION_ID(52181, 53073), REL::VariantOffset(0xD9, 0xD3, 0xE6) };
			stl::write_thunk_call<QuestObjectiveHook>(target2.address());
			SKSE::log::info("QuestObjectiveHook hooked at address: {:x} and offset: {:x}", target2.address(), target2.offset());

			REL::Relocation<std::uintptr_t> target3{ RELOCATION_ID(52276, 53171), REL::VariantOffset(0x208, 0x22F, 0x208) };
			stl::write_thunk_call<QuestObjectiveHook>(target3.address());
			SKSE::log::info("QuestObjectiveHook hooked at address: {:x} and offset: {:x}", target3.address(), target3.offset());
		}
	};

	/*
	struct QuestDescriptionHook //QUST CNAM text hook for Journal menu
	{
		static RE::BSString GetInstanceQuestDescription(RE::TESQuest* a_quest, std::uint32_t a_instanceID)
		{
			using func_t = void (RE::TESQuest::*)(RE::BSString&, std::uint32_t) const;
			REL::Relocation<func_t> funct{ RELOCATION_ID(24549, 25078) };
			RE::BSString CNAM;
			funct(a_quest, CNAM, a_instanceID);
			return CNAM;
		}

		static void thunk(RE::TESQuest* a_quest, RE::BSString& a_out, std::uint32_t a_instanceID)
		{
			if (!a_quest)
			{
				return func(a_quest, a_out, a_instanceID);
			}
			func(a_quest, a_out, a_instanceID);


			SKSE::log::info("Quest: {} - InstanceID: {} - String: {}", a_quest->fullName.c_str(), a_instanceID, a_out.c_str());

			SKSE::log::info("LOOKUP- String:\n {}", GetInstanceQuestDescription(a_quest, a_instanceID).c_str());

			for (auto& text : a_quest->instanceData)
			{
				SKSE::log::info("ID: {} - JournalStage: {} - JournalStageItem: {}", text->id, text->journalStage, text->journalStageItem);

				for (auto& stringData : text->stringData)
				{
					SKSE::log::info("AliasID: {}", stringData.aliasID);

					if (stringData.aliasID == a_instanceID)
					{
						SKSE::log::info("Match!- String:\n {}", GetInstanceQuestDescription(a_quest, stringData.aliasID).c_str());
					}
					else
					{
						SKSE::log::info("No Match!");
					}
				}

			}

		};

		static inline REL::Relocation<decltype(thunk)> func;


		static void Install()
		{
			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(52276, 0), REL::VariantOffset(0x808, 0x0, 0x0) };
			stl::write_thunk_call<QuestDescriptionHook>(target1.address());
			SKSE::log::info("QuestDescriptionHook hooked at address: {:x} and offset: {:x}", target1.address(), target1.offset());
		}
	};
	*/

	struct QuestCNAMTextHook //QUST CNAM
	{
		static void thunk(RE::BSString& a_out, char* a_buffer, std::uint64_t a3)
		{
			func(a_out, a_buffer, a3);

			auto it = g_QUST_CNAM_Map.find(a_out.c_str()); //using original language as key since I couldn't find a way to match the strings without the QuestInstanceID

			if (it != g_QUST_CNAM_Map.end())
			{
				a_out = it->second;
			}

		}
		static inline REL::Relocation<decltype(thunk)> func;


		static void Install()
		{
			//JournalMenu quest description text Hook
			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(24778, 25259), REL::VariantOffset(0x21C, 0x221, 0x21C) };//First: 5B ->Don't know; Second: C4 ->Don't know; Third: 221 ->Quest description text
			stl::write_thunk_call<QuestCNAMTextHook>(target1.address());
			SKSE::log::info("QuestCNAMTextHook hooked at address: {:x} and offset: {:x}", target1.address(), target1.offset());
		}

	};

	/*
	struct HudMenuQuestObjectiveHook //QUST NNAM text hook for Hudmenu
	{
		static void thunk(std::uint32_t a_HUDDataType, char* a_out, RE::TESQuest* a_quest, std::uint64_t a4)
		{
			if (!a_quest || !a_out)
			{
				return func(a_HUDDataType, a_out, a_quest, a4);
			}

			if (a_HUDDataType != 14 && a_HUDDataType != 15 && a_HUDDataType != 16) // kQuestStarted = 14, kQuestComplete = 15, kQuestFailed = 16,
			{
				for (const auto& objective : a_quest->objectives)
				{
					size_t key = Utils::combineHashWithIndex(objective->ownerQuest->formID, objective->index, Utils::GetModName(objective->ownerQuest));

					auto it = g_QUST_NNAM_Map.find(key);
					if (it != g_QUST_NNAM_Map.end())
					{
						char* newtext = const_cast<char*>(it->second.c_str());
						return func(a_HUDDataType, newtext, a_quest, a4);
					}
				}
			}

			return func(a_HUDDataType, a_out, a_quest, a4);

		};

		static inline REL::Relocation<decltype(thunk)> func;


		static void Install()
		{

			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(24472, 24991), REL::VariantOffset(0x72, 0x7D, 0x72) };
			stl::write_thunk_call<HudMenuQuestObjectiveHook>(target1.address());
			SKSE::log::info("HudMenuQuestObjectiveHook hooked at address: {:x} and offset: {:x}", target1.address(), target1.offset());

			REL::Relocation<std::uintptr_t> target2{ RELOCATION_ID(23467, 23933), REL::VariantOffset(0x107, 0x17F, 0x107) }; //Objective
			stl::write_thunk_call<HudMenuQuestObjectiveHook>(target2.address());
			SKSE::log::info("HudMenuQuestObjectiveHook hooked at address: {:x} and offset: {:x}", target2.address(), target2.offset());

			REL::Relocation<std::uintptr_t> target6{ RELOCATION_ID(23467, 23933), REL::VariantOffset(0xAC, 0x1D5, 0xAC) }; //Multiple objectives
			stl::write_thunk_call<HudMenuQuestObjectiveHook>(target6.address());
			SKSE::log::info("HudMenuQuestObjectiveHook hooked at address: {:x} and offset: {:x}", target6.address(), target6.offset());

			REL::Relocation<std::uintptr_t> target3{ RELOCATION_ID(23467, 23933), REL::VariantOffset(0x153, 0x221, 0x153) }; //Multiple objectives
			stl::write_thunk_call<HudMenuQuestObjectiveHook>(target3.address());
			SKSE::log::info("HudMenuQuestObjectiveHook hooked at address: {:x} and offset: {:x}", target3.address(), target3.offset());

			REL::Relocation<std::uintptr_t> target4{ RELOCATION_ID(24469, 24988), REL::VariantOffset(0x60, 0x5E, 0x60) };
			stl::write_thunk_call<HudMenuQuestObjectiveHook>(target4.address());
			SKSE::log::info("HudMenuQuestObjectiveHook hooked at address: {:x} and offset: {:x}", target4.address(), target4.offset());

			REL::Relocation<std::uintptr_t> target5{ RELOCATION_ID(40554, 41561), REL::VariantOffset(0x2b4, 0x35C, 0x2b4) };
			stl::write_thunk_call<HudMenuQuestObjectiveHook>(target5.address());
			SKSE::log::info("HudMenuQuestObjectiveHook hooked at address: {:x} and offset: {:x}", target5.address(), target5.offset());

		}
	};
	*/

	void InstallQuestHooks()
	{
		//Hook::QuestObjectiveHook::Install();
		//Hook::QuestDescriptionHook::Install();
		Hook::QuestCNAMTextHook::Install();
		//Hook::HudMenuQuestObjectiveHook::Install();
	}




	/*
	Old hook:

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


		static void Install()
		{
			//JournalMenu quest description text Hook
			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(24778, 25259), REL::VariantOffset(0x21C, 0x221, 0x21C) };//First: 5B ->Don't know; Second: C4 ->Don't know; Third: 221 ->Quest description text
			stl::write_thunk_call<QuestObjectiveTextHook>(target1.address());
			SKSE::log::info("QuestObjectiveTextHook hooked at address: {:x} and offset: {:x}", target1.address(), target1.offset());

			//HudMenu quest objective text Hook
			REL::Relocation<std::uintptr_t> target2{ RELOCATION_ID(50751, 51646), REL::VariantOffset(0xA6, 0xA6, 0xA6) };//First: A6 ->All NNAM sentences are passing here; (Second: F7 ->"or" (depends on language) if needed; Third: 143 ->Second sentence after "or" if available)
			stl::write_thunk_call<QuestObjectiveTextHook>(target2.address());
			SKSE::log::info("QuestObjectiveTextHook hooked at address: {:x} and offset: {:x}", target2.address(), target2.offset());

			//JournalMenu quest objective text Hook
			REL::Relocation<std::uintptr_t> target3{ RELOCATION_ID(23229, 23684), REL::VariantOffset(0x21, 0x21, 0x21) };
			stl::write_thunk_call<QuestObjectiveTextHook>(target3.address());
			SKSE::log::info("QuestObjectiveTextHook hooked at address: {:x} and offset: {:x}", target3.address(), target3.offset());
		}

	};
		//Hook::QuestObjectiveTextHook::Install();
	*/
}