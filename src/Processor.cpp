#include "../include/Processor.h"
#include "../include/Config.h"


void Processor::RunConstTranslation()
{
	for (const auto& Information : g_ConstConfigurationInformationStruct)
	{
		auto Form = Information.Form;
		auto SubrecordType = Information.SubrecordType;
		auto ReplacerText = Information.ReplacerText;

		if (SubrecordType == "FULL") //DIAL FULL isn't working. Something is very different with DIAL
		{
			SetName<RE::TESFullName>(Form, ReplacerText, &RE::TESFullName::fullName);
		}
		else if (SubrecordType == "SHRT")
		{
			SetName<RE::TESNPC>(Form, ReplacerText, &RE::TESNPC::shortName);
		}
		else if (SubrecordType == "TNAM")
		{
			SetName<RE::TESWordOfPower>(Form, ReplacerText, &RE::TESWordOfPower::translation);
		}
	}
	g_ConstConfigurationInformationStruct.clear(); //Structs only used once, so no need to keep them
}

template <typename T>
void Processor::SetName(RE::TESForm* Form, RE::BSFixedString newName, RE::BSFixedString T::* memberPtr)
{
	T* OrigName = skyrim_cast<T*>(Form);
	if (OrigName)
	{
		OrigName->*memberPtr = newName;
	}
}