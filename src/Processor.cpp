#include "../include/Globals.h"
#include "../include/Processor.h"
#include "../include/Config.h"
#include "../include/Utils.h"

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
			SetGameSettingsStrings(Information.EditorID, Information.ReplacerText);
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

void Processor::SetGameSettingsStrings(const std::string& EditorID, const std::string& NewString)
{
	Utils::RunConsoleCommand("setgs " + EditorID + " \"" + NewString + "\"");
}