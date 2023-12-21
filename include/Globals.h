#pragma once

inline std::shared_ptr<spdlog::logger> g_Logger;

struct ConfigurationInformation
{
	RE::FormID FormID;
	RE::TESForm* Form;
	std::string ReplacerText = "";
	std::string RecordType = "";
	std::string SubrecordType = "";
};

inline std::vector<ConfigurationInformation> g_ConfigurationInformationStruct;

inline std::unordered_map<std::string, std::string> g_INFO_NAM1_Map;
inline std::unordered_map<std::string, std::string> g_INFO_RNAM_Map; //Not used yet
inline std::unordered_map<std::string, std::string> g_QUST_NNAM_Map;
inline std::unordered_map<std::string, std::string> g_QUST_CNAM_Map;
