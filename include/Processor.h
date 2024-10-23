#pragma once
#include "Config.h"

class Processor : public ISingleton<Processor>
{
public:
	static void AddToConstTranslationStruct(RE::TESForm* form, const std::string& string, Config::SubrecordType SubrecordType, int pos, const std::string& editorid)
	{
		if (editorid.empty())
		{
			m_ConstConfigurationInformationStruct.emplace_back(form, string, SubrecordType, pos);
		}
		else
		{
			m_ConstConfigurationInformationStruct.emplace_back(form, string, SubrecordType, pos, editorid);
		}

	}

	void RunConstTranslation();

private:

	template <typename T>
	void SetConstStrings(RE::TESForm* form, const RE::BSFixedString& newString, RE::BSFixedString T::* memberPtr);

	void SetFullnameStrings(RE::TESForm* form, const std::string& newString);

	void SetGameSettingString(const std::string& name, const std::string& newString);

	void SetMessageBoxButtonStrings(RE::TESForm* form, const RE::BSFixedString& newString, const int& index);

	void SetRegionDataStrings(RE::TESForm* form, const RE::BSFixedString& newString);

	void SetEntryPointStrings(RE::TESForm* form, const RE::BSFixedString& newString, const int& index);

	void SetQuestObjectiveStrings(RE::TESForm* form, const RE::BSFixedString& newString, const int& index);

	void Report(const RE::TESForm* form);

	static RE::Setting* changeSettingString(RE::Setting* a_setting, const char* a_str)
	{
		using func_t = decltype(&changeSettingString);
		static REL::Relocation<func_t> func{ RELOCATION_ID(73882, 75619) };
		return func(a_setting, a_str);
	}

	struct ConstConfigurationInformation
	{
		RE::TESForm* Form;
		std::string ReplacerText = "";
		Config::SubrecordType SubrecordType;
		int pos;
		std::string EditorID = "";
	};

	static inline std::vector<ConstConfigurationInformation> m_ConstConfigurationInformationStruct; //Used for const translations
};