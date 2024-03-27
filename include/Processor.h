#pragma once
#include "Config.h"

class Processor
{
public:
	static Processor* GetSingleton()
	{
		static Processor menu;
		return &menu;
	}

	static void AddToConstTranslationStruct(RE::TESForm* form, const std::string& string, Config::SubrecordType SubrecordType, const std::string& editorid)
	{
		m_ConstConfigurationInformationStruct.emplace_back(form, string, SubrecordType, editorid);
	}

	static void AddToMESGITXTTranslationStruct(RE::TESForm* form, const std::string& string, int pos)
	{
		m_MESGITXTInformationStruct.emplace_back(form, string, pos);
	}

	static void AddToPERKEPFDTranslationStruct(RE::TESForm* form, const std::string& string, int pos)
	{
		m_PERKEPFDInformationStruct.emplace_back(form, string, pos);
	}

	void RunConstTranslation();

private:

	template <typename T>
	void SetConstStrings(RE::TESForm* Form, RE::BSFixedString NewString, RE::BSFixedString T::* memberPtr);

	void SetGameSettingString(const std::string& a_name, const std::string& a_NewString);

	void SetMessageBoxButtonStrings();

	void SetRegionDataStrings(RE::TESForm* Form, RE::BSFixedString NewString);

	void SetEntryPointStrings();

	struct ConstConfigurationInformation
	{
		RE::TESForm* Form;
		std::string ReplacerText = "";
		Config::SubrecordType SubrecordType;
		std::string EditorID = "";
	};

	static inline std::vector<ConstConfigurationInformation> m_ConstConfigurationInformationStruct; //Used for const translations

	struct MESGITXTInformation
	{
		RE::TESForm* Form;
		std::string ReplacerText = "";
		int pos;
	};

	static inline std::vector<MESGITXTInformation> m_MESGITXTInformationStruct; //Used for MESG ITXT translations

	static inline std::vector<MESGITXTInformation> m_PERKEPFDInformationStruct; //Used for PERK EPFD translations

	Processor() = default;
	~Processor() = default;
	Processor(const Processor&) = delete;
	Processor(Processor&&) = delete;
	Processor& operator=(const Processor&) = delete;
	Processor& operator=(Processor&&) = delete;
};