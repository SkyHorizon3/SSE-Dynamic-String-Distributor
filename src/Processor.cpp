#include "../include/Processor.h"
#include "../include/Config.h"

void Processor::RunConstTranslation()
{
	for (const auto& Information : g_ConstConfigurationInformationStruct)
	{

		switch (Information.SubrecordType)
		{
		case ConstSubrecordType::kFULL: //DIAL FULL, REFR FULL aren't working like this.
		{
			SetConstStrings<RE::TESFullName>(Information.Form, Information.ReplacerText, &RE::TESFullName::fullName);
		}
		break;
		case ConstSubrecordType::kSHRT:
		{
			SetConstStrings<RE::TESNPC>(Information.Form, Information.ReplacerText, &RE::TESNPC::shortName);
		}
		break;
		case ConstSubrecordType::kTNAM:
		{
			SetConstStrings<RE::TESWordOfPower>(Information.Form, Information.ReplacerText, &RE::TESWordOfPower::translation);
		}
		break;
		case ConstSubrecordType::kDATA:
		{
			SetGameSettingString(Information.EditorID, Information.ReplacerText);
		}
		break;
		case ConstSubrecordType::kUnknown:
		{
			g_Logger->info("Unknown type {} in ConstTranslation", Information.EditorID);
		}
		break;

		default: break;
		}
	}
	g_ConstConfigurationInformationStruct.clear(); //Structs only used once, so no need to keep them for fun
	g_ConstConfigurationInformationStruct.shrink_to_fit();
}

template <typename T>
void Processor::SetConstStrings(RE::TESForm* Form, RE::BSFixedString NewString, RE::BSFixedString T::* memberPtr)
{
	T* OrigString = skyrim_cast<T*>(Form);
	if (OrigString)
	{
		OrigString->*memberPtr = NewString;
	}
	else
	{
		g_Logger->info("Issue during ConstTranslation with FormID: {0:08X}.", Form->formID);
	}

}

void Processor::SetGameSettingString(const std::string& a_name, const std::string& a_NewString)
{
	auto setting = RE::GameSettingCollection::GetSingleton()->GetSetting(a_name.c_str());

	//g_Logger->info("String: {}", std::string(setting->data.s));

	if (setting->GetType() == RE::Setting::Type::kString)
	{
		//RE::free(setting->data.s); -> don't know why this is crashing. If you know, please tell me!

		auto length = a_NewString.length() + 1;
		char* NewChar = (char*)RE::MemoryManager::GetSingleton()->Allocate(length, 0, 0);

		if (!NewChar)
		{
			g_Logger->error("Failed to allocate memory for a new string when setting game setting '{}'. Upgrade your RAM and get rid of Windows XP!", a_name);
			return;
		}

		memcpy(NewChar, a_NewString.c_str(), length);

		setting->data.s = NewChar;
	}
}