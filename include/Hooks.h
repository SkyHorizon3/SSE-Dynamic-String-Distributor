#pragma once
#include "../include/Globals.h"

namespace Hook
{
	void InstallHooks();


	class JournalMenu : public RE::JournalMenu
	{
	public:

		static void InstallUIHook()
		{
			REL::Relocation<std::uintptr_t> Vtbl{ RE::VTABLE_JournalMenu[0] };
			func = Vtbl.write_vfunc(0x4, &ProcessMessageHook);
			g_Logger->info("Installed JournalMenuUIHook");
		}

	private:
		bool IsViewingMiscObjectives() noexcept;
		RE::UI_MESSAGE_RESULTS ProcessMessageHook(RE::UIMessage& a_message);

		static inline REL::Relocation<decltype(&RE::JournalMenu::ProcessMessage)> func;
	};

	/*
	class HudMenu : public RE::HUDNotifications
	{

	public:

		static void InstallUIHook()
		{
			REL::Relocation<std::uintptr_t> Vtbl{ RE::VTABLE_HUDNotifications[0] };
			func = Vtbl.write_vfunc(0x2, &ProcessMessageHook);
			g_Logger->info("Installed HUDNotificationsUIHook");
		}


	private:
		bool ProcessMessageHook(RE::UIMessage* a_message);

		static inline REL::Relocation<decltype(&RE::HUDNotifications::ProcessMessage)> func;
	};
	*/











	/*
	//Not used
	class DialogueMenu : public RE::DialogueMenu
	{

	public:

		static void InstallUIHook()
		{
			REL::Relocation<std::uintptr_t> Vtbl{ RE::VTABLE_DialogueMenu[0] };
			func = Vtbl.write_vfunc(0x6, &ProcessMessageHook);
			g_Logger->info("Installed DialogueMenuUIHook");
		}


	private:
		//bool IsViewingMiscObjectives() noexcept;
		//RE::UI_MESSAGE_RESULTS ProcessMessageHook(RE::UIMessage& a_message);
		void ProcessMessageHook();

		static inline REL::Relocation<decltype(&RE::DialogueMenu::PostDisplay)> func;
	};
	*/
}