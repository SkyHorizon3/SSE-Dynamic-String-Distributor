#include "MiscHooks.h"
#include "QuestHooks.h"
#include "Processor.h"
#include "Utils.h"

namespace Hook
{

	struct QuestObjectiveHook //JournalMenu quest objective text Hook
	{
		static void thunk(const RE::BGSInstancedQuestObjective* a_objective, RE::BSString& a_out)
		{
			if (!a_objective)
			{
				return func(a_objective, a_out);
			}
			func(a_objective, a_out);

			auto index = a_objective->Objective->index;
			auto formID = a_objective->Objective->ownerQuest->formID;
			size_t key = Utils::combineHash(formID, index);

			auto it = g_QUST_NNAM_Map.find(key);
			if (it != g_QUST_NNAM_Map.end())
			{
				a_out = it->second;
				a_objective->Objective->displayText = it->second;
			}
			else
			{
				a_out = a_objective->Objective->displayText.c_str();
			}

		};
		static inline REL::Relocation<decltype(thunk)> func;


		static void Install()
		{
			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(21951, 0), REL::VariantOffset(0x147, 0x0, 0x0) };
			stl::write_thunk_call<QuestObjectiveHook>(target1.address());
			SKSE::log::info("QuestObjectiveHook hooked at address: {:x} and offset: {:x}", target1.address(), target1.offset());

			REL::Relocation<std::uintptr_t> target2{ RELOCATION_ID(52181, 0), REL::VariantOffset(0xD9, 0x0, 0x0) };
			stl::write_thunk_call<QuestObjectiveHook>(target2.address());
			SKSE::log::info("QuestObjectiveHook hooked at address: {:x} and offset: {:x}", target2.address(), target2.offset());

			REL::Relocation<std::uintptr_t> target3{ RELOCATION_ID(52276, 0), REL::VariantOffset(0x208, 0x0, 0x0) };
			stl::write_thunk_call<QuestObjectiveHook>(target3.address());
			SKSE::log::info("QuestObjectiveHook hooked at address: {:x} and offset: {:x}", target3.address(), target3.offset());
		}
	};

	struct QuestDescriptionHook //JournalMenu quest description text Hook
	{
		static void thunk(RE::TESQuest* a_quest, RE::BSString& a_out, std::uint32_t a_instanceID)
		{
			if (!a_quest)
			{
				return func(a_quest, a_out, a_instanceID);
			}
			func(a_quest, a_out, a_instanceID);

			//			auto stage = a_quest->GetCurrentStageID();

			if (a_quest->objConditions)
			{
				auto head = a_quest->objConditions.head;
				if (head->data.flags.global)
				{
					head->data.comparisonValue.g->formEditorID;

				}
				else
				{
					head->data.comparisonValue.f;

				}

			}


			//a_out = "test";
			//SKSE::log::info("String: {}", a_out.c_str());
		};

		static inline REL::Relocation<decltype(thunk)> func;


		static void Install()
		{
			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(52276, 0), REL::VariantOffset(0x808, 0x0, 0x0) };
			stl::write_thunk_call<QuestDescriptionHook>(target1.address());
			SKSE::log::info("QuestDescriptionHook hooked at address: {:x} and offset: {:x}", target1.address(), target1.offset());
		}
	};

	struct HudMenuQuestObjectiveHook //HUDMenu quest objective text Hook
	{
		static void thunk(std::uint32_t a1, char* a_out, RE::TESQuest* a_quest, std::uint64_t a4)
		{
			if (!a_quest || !a_out)
			{
				return func(a1, a_out, a_quest, a4);
			}

			for (auto& objective : a_quest->objectives)
			{
				std::string origText = objective->displayText.c_str();
				auto index = objective->index;
				auto formID = objective->ownerQuest->formID;
				size_t key = Utils::combineHash(formID, index);

				auto it = g_QUST_NNAM_Map.find(key);
				if (it != g_QUST_NNAM_Map.end())
				{
					std::string orig(a_out);
					if (origText == orig)
					{
						char* newtext = const_cast<char*>(it->second.c_str());
						return func(a1, newtext, a_quest, a4);
					}
				}
			}

			return func(a1, a_out, a_quest, a4);

		};

		static inline REL::Relocation<decltype(thunk)> func;


		static void Install()
		{
			/*
			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(24472, 0), REL::VariantOffset(0x72, 0x0, 0x0) };
			stl::write_thunk_call<HudMenuQuestObjectiveHook>(target1.address());
			SKSE::log::info("HudMenuQuestObjectiveHook hooked at address: {:x} and offset: {:x}", target1.address(), target1.offset());
			*/

			REL::Relocation<std::uintptr_t> target2{ RELOCATION_ID(23467, 0), REL::VariantOffset(0x107, 0x0, 0x0) }; //Objective
			stl::write_thunk_call<HudMenuQuestObjectiveHook>(target2.address());
			SKSE::log::info("HudMenuQuestObjectiveHook hooked at address: {:x} and offset: {:x}", target2.address(), target2.offset());

			REL::Relocation<std::uintptr_t> target6{ RELOCATION_ID(23467, 0), REL::VariantOffset(0xAC, 0x0, 0x0) }; //Multiple objectives 
			stl::write_thunk_call<HudMenuQuestObjectiveHook>(target6.address());
			SKSE::log::info("HudMenuQuestObjectiveHook hooked at address: {:x} and offset: {:x}", target6.address(), target6.offset());

			REL::Relocation<std::uintptr_t> target3{ RELOCATION_ID(23467, 0), REL::VariantOffset(0x153, 0x0, 0x0) }; //Multiple objectives 
			stl::write_thunk_call<HudMenuQuestObjectiveHook>(target3.address());
			SKSE::log::info("HudMenuQuestObjectiveHook hooked at address: {:x} and offset: {:x}", target3.address(), target3.offset());

			/*
			REL::Relocation<std::uintptr_t> target4{ RELOCATION_ID(24469, 0), REL::VariantOffset(0x60, 0x0, 0x0) };
			stl::write_thunk_call<HudMenuQuestObjectiveHook>(target4.address());
			SKSE::log::info("HudMenuQuestObjectiveHook hooked at address: {:x} and offset: {:x}", target4.address(), target4.offset());

			REL::Relocation<std::uintptr_t> target5{ RELOCATION_ID(40554, 0), REL::VariantOffset(0x2b4, 0x0, 0x0) };
			stl::write_thunk_call<HudMenuQuestObjectiveHook>(target5.address());
			SKSE::log::info("HudMenuQuestObjectiveHook hooked at address: {:x} and offset: {:x}", target5.address(), target5.offset());
			*/

		}
	};





	//TODO: Add INFO RNAM, DIAL FULL hook
	void InstallQuestHooks()
	{
		Hook::QuestObjectiveHook::Install();
		//Hook::QuestDescriptionHook::Install();
		Hook::HudMenuQuestObjectiveHook::Install();
	}




	/*
	Old hooks:

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


		static void Install()
		{
			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(34434, 35254), REL::VariantOffset(0xCC, 0x226, 0xCC) };
			stl::write_thunk_call<DialogueMenuTextHook>(target1.address());
			SKSE::log::info("DialogueMenuTextHook hooked at address: {:x} and offset: {:x}", target1.address(), target1.offset());

			if (REL::Module::IsAE())
			{
				REL::Relocation<std::uintptr_t> target2{ RELOCATION_ID(0, 35254), REL::VariantOffset(0x0, 0x115, 0x0) };
				stl::write_thunk_call<DialogueMenuTextHook>(target2.address());
				SKSE::log::info("DialogueMenuTextHook hooked at address: {:x} and offset: {:x}", target2.address(), target2.offset());
			}
		}
	};

		//Hook::QuestObjectiveTextHook::Install();
		//Hook::DialogueMenuTextHook::Install();
	*/
}