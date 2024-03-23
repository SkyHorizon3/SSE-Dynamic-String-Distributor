#pragma once
#include "Config.h"

namespace Hook
{
	void InstallHooks();

	struct ConfigurationInformation
	{
		RE::TESForm* Form;
		std::string ReplacerText = "";
		Config::SubrecordType SubrecordType;
	};

	inline std::vector<ConfigurationInformation> g_ConfigurationInformationStruct;

	inline ankerl::unordered_dense::map<RE::FormID, std::string> g_REFR_FULL_Map;
	inline ankerl::unordered_dense::map<std::string, std::string> g_FLOR_RNAM_RDMP_Map;
}