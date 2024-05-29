#pragma once

namespace Hook
{
	void InstallDialogueHooksPostLoad();
	void InstallDialogueHooks();

	using Value = std::pair<std::uint8_t, std::string>;
	inline ankerl::unordered_dense::map <std::string, std::vector<Value>> g_INFO_NAM1_Map;

	inline ankerl::unordered_dense::map<size_t, std::string> g_DIAL_FULL_Map;
	inline ankerl::unordered_dense::map<size_t, std::string> g_INFO_RNAM_Map;
}