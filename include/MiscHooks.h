#pragma once
#include "Config.h"

namespace Hook
{
	void InstallHooks();

	struct ConfigurationInformation
	{
		RE::TESForm* Form;
		std::string ReplacerText = "";
		Config::SubrecordTypes SubrecordType;
	};

	inline std::vector<ConfigurationInformation> g_ConfigurationInformationStruct;

	inline ankerl::unordered_dense::map<std::string, std::string> g_MESG_ITXT_Map;


	inline ankerl::unordered_dense::map<std::string, std::string> g_DIAL_FULL_RNAM_Map;
	inline ankerl::unordered_dense::map<std::string, std::string> g_FLOR_RNAM_RDMP_Map;
}