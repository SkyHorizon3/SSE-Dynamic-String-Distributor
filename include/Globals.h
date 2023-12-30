#pragma once

inline std::shared_ptr<spdlog::logger> g_Logger;

struct ConfigurationInformation
{
	RE::TESForm* Form;
	std::string ReplacerText = "";
	std::string SubrecordType = "";
	std::string RecordType = "";
};

inline std::vector<ConfigurationInformation> g_ConfigurationInformationStruct;
inline std::vector<ConfigurationInformation> g_ConstConfigurationInformationStruct; //Used for const translations

inline std::unordered_map<std::string, std::string> g_INFO_NAM1_RNAM_Map;
inline std::unordered_map<std::string, std::string> g_QUST_NNAM_CNAM_Map;