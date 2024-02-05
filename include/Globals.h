#pragma once

inline std::shared_ptr<spdlog::logger> g_Logger;

struct ConfigurationInformation
{
	RE::TESForm* Form;
	std::string ReplacerText = "";
	std::string SubrecordType = "";
	std::string RecordType = "";
};

enum class ConstSubrecordType
{
	kFULL,
	kSHRT,
	kTNAM,
	kDATA,

	kUnknown
};

struct ConstConfigurationInformation
{
	RE::TESForm* Form;
	std::string ReplacerText = "";
	ConstSubrecordType SubrecordType;
	std::string EditorID = "";
};

inline std::vector<ConfigurationInformation> g_ConfigurationInformationStruct;
inline std::vector<ConstConfigurationInformation> g_ConstConfigurationInformationStruct; //Used for const translations

inline std::unordered_map<std::string, std::string> g_INFO_NAM1_ITXT_Map;
inline std::unordered_map<std::string, std::string> g_DIAL_FULL_RNAM_Map;
inline std::unordered_map<std::string, std::string> g_QUST_NNAM_CNAM_Map;
inline std::unordered_map<std::string, std::string> g_FLOR_RNAM_RDMP_Map;