#pragma once
#include "../include/Globals.h"

namespace Hook
{
	void InstallHooks();

	/*
	class MessageBoxMenuClass : public RE::MessageBoxMenu
	{

	public:

		static void InstallUIHook()
		{
			REL::Relocation<std::uintptr_t> Vtbl{ RE::VTABLE_MessageBoxMenu[0] };
			func = Vtbl.write_vfunc(0x4, &ProcessMessageHook);
			g_Logger->info("Installed MessageBoxUIHook");
		}


	private:
		RE::UI_MESSAGE_RESULTS ProcessMessageHook(RE::UIMessage& a_message);

		static inline REL::Relocation<decltype(&RE::MessageBoxMenu::ProcessMessage)> func;
	};
	*/
}



/*
//Not used
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
*/

/*
//Not used
	class HudMenu : public RE::HUDMenu
	{

	public:

		static void InstallUIHook()
		{
			REL::Relocation<std::uintptr_t> Vtbl{ RE::VTABLE_HUDMenu[0] };
			func = Vtbl.write_vfunc(0x4, &ProcessMessageHook);
			g_Logger->info("Installed HUDMenuUIHook");
		}


	private:
		std::string RemoveTrailingSpaces(std::string str);
		RE::UI_MESSAGE_RESULTS ProcessMessageHook(RE::UIMessage& a_message);

		static inline REL::Relocation<decltype(&RE::HUDMenu::ProcessMessage)> func;
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