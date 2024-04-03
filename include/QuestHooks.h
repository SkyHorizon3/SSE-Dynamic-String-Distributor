#pragma once

namespace Hook
{
	void InstallQuestHooks();

	//inline ankerl::unordered_dense::map<size_t, std::string> g_QUST_NNAM_Map;
	inline ankerl::unordered_dense::map<std::string, std::string> g_QUST_CNAM_Map;
}