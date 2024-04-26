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
	void SetConstStrings(RE::TESForm* Form, const RE::BSFixedString& NewString, RE::BSFixedString T::* memberPtr);

	void SetGameSettingString(const std::string& a_name, const std::string& a_NewString);

	void SetMessageBoxButtonStrings(RE::TESForm* Form, const RE::BSFixedString& NewString, const int& index);

	void SetRegionDataStrings(RE::TESForm* Form, const RE::BSFixedString& NewString);

	void SetEntryPointStrings(RE::TESForm* Form, const RE::BSFixedString& NewString, const int& index);

	void SetQuestObjectiveStrings(RE::TESForm* Form, const RE::BSFixedString& NewString, const int& index);

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