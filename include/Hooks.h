#pragma once
#include "../include/Globals.h"

namespace Hook
{
	void InstallHooks();

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
		bool containsOnlySpace(const std::string& str);
		RE::UI_MESSAGE_RESULTS ProcessMessageHook(RE::UIMessage& a_message);

		static inline REL::Relocation<decltype(&RE::HUDMenu::ProcessMessage)> func;
	};
}