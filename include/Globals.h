#pragma once

inline std::shared_ptr<spdlog::logger> g_Logger;

struct ConfigurationInformation
{
	RE::TESForm* Form;
	std::string ReplacerText = "";
	std::string RecordType = "";
	std::string SubrecordType = "";
};

struct ConstConfigurationInformation
{
	RE::TESForm* Form;
	std::string ReplacerText = "";
	std::string SubrecordType = "";
};

inline std::vector<ConfigurationInformation> g_ConfigurationInformationStruct;
inline std::vector<ConstConfigurationInformation> g_ConstConfigurationInformationStruct; //Used for const translations

inline std::unordered_map<std::string, std::string> g_INFO_NAM1_Map;
inline std::unordered_map<std::string, std::string> g_INFO_RNAM_Map; //Not used yet
inline std::unordered_map<std::string, std::string> g_QUST_NNAM_Map;
inline std::unordered_map<std::string, std::string> g_QUST_CNAM_Map;
inline std::unordered_map<RE::FormID, RE::BSTArray<RE::BSString>> g_ITXTMap;

