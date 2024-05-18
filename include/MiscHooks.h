#pragma once
#include "Config.h"

namespace Hook
{
	void InstallPostLoadHooks();
	void InstallHooks();

	inline ankerl::unordered_dense::map<size_t, std::string> g_DESC_CNAM_Map;

	inline ankerl::unordered_dense::map<size_t, std::string> g_REFR_FULL_Map;
	inline ankerl::unordered_dense::map<size_t, std::string> g_ACTI_Map;
	inline ankerl::unordered_dense::map<size_t, std::string> g_NPC_FULL_Map;
}