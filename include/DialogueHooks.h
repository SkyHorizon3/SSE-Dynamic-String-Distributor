#pragma once

namespace Hook
{
	void InstallDialogueHooks();

	inline ankerl::unordered_dense::map<size_t, std::string> g_INFO_NAM1_Map;
	inline ankerl::unordered_dense::map<std::string, std::string> g_INFO_NAM1_TEMP_Map;
}