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

	static void AddToConstTranslationStruct(RE::TESForm* form, const std::string& string, Config::SubrecordTypes SubrecordTypes, const std::string& editorid)
	{
		m_ConstConfigurationInformationStruct.emplace_back(form, string, SubrecordTypes, editorid);
	}

	void RunConstTranslation();

private:

	template <typename T>
	void SetConstStrings(RE::TESForm* Form, RE::BSFixedString NewString, RE::BSFixedString T::* memberPtr);

	void SetGameSettingString(const std::string& a_name, const std::string& a_NewString);

	struct ConstConfigurationInformation
	{
		RE::TESForm* Form;
		std::string ReplacerText = "";
		Config::SubrecordTypes SubrecordType;
		std::string EditorID = "";
	};

	static inline std::vector<ConstConfigurationInformation> m_ConstConfigurationInformationStruct; //Used for const translations

	Processor() = default;
	~Processor() = default;
	Processor(const Processor&) = delete;
	Processor(Processor&&) = delete;
	Processor& operator=(const Processor&) = delete;
	Processor& operator=(Processor&&) = delete;
};