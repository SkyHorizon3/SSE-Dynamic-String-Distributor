#include "../include/Processor.h"
#include "../include/Config.h"


void Processor::RunConstTranslation()
{
	for (const auto& Information : g_ConstConfigurationInformationStruct)
	{
		auto& Form = Information.Form;
		auto& SubrecordType = Information.SubrecordType;
		auto& ReplacerText = Information.ReplacerText;

		if (SubrecordType == "FULL") //DIAL FULL isn't working like this. Something is very different with DIAL
		{
			SetConstStrings<RE::TESFullName>(Form, ReplacerText, &RE::TESFullName::fullName);
		}
		else if (SubrecordType == "SHRT")
		{
			SetConstStrings<RE::TESNPC>(Form, ReplacerText, &RE::TESNPC::shortName);
		}
		else if (SubrecordType == "TNAM")
		{
			SetConstStrings<RE::TESWordOfPower>(Form, ReplacerText, &RE::TESWordOfPower::translation);
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
}