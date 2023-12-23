#include "../include/Processor.h"
#include "../include/Config.h"


void Processor::RunConstTranslation()
{
	for (const auto& Information : g_ConstConfigurationInformationStruct)
	{
		auto Form = Information.Form;
		auto SubrecordType = Information.SubrecordType;
		auto ReplacerText = Information.ReplacerText;

		if (SubrecordType == "FULL")
		{
			SetFULLName(Form, ReplacerText);
		}
		else if (SubrecordType == "SHRT")
		{
			SetSHRTName(Form, ReplacerText);
		}
		else if (SubrecordType == "TNAM")
		{
			SetTNAMName(Form, ReplacerText);
		}
		/*
		else if (SubrecordType == "ITXT")
		{
			CollectITXTTexts(Information);
			SetITXTName(Form);
		}
		*/
	}
	g_ConstConfigurationInformationStruct.clear(); //Structs only used once, so no need to keep them
}

/*
void Processor::CollectITXTTexts(const ConstConfigurationInformation& information)
{

	// Überprüfen, ob die FormID in der ITXTMap vorhanden ist
	auto it = g_ITXTMap.find(information.Form->formID);
	if (it != g_ITXTMap.end())
	{
		// FormID existiert bereits, füge den neuen Replacertext zum vorhandenen Array hinzu
		it->second.push_back(RE::BSString(information.ReplacerText.c_str()));
	}
	else
	{
		RE::BSTArray<RE::BSString> newArray;
		newArray.push_back(RE::BSString(information.ReplacerText.c_str()));
		g_ITXTMap.emplace(information.Form->formID, newArray);
	}


	// Jetzt enthält ITXTMap die gesammelten Replacetexts für jede FormID
	// Sie können darauf zugreifen und weiter verwenden
}
*/
void Processor::SetFULLName(RE::TESForm* Form, RE::BSFixedString newName) //Full names of everything
{
	RE::TESFullName* OrigName = skyrim_cast<RE::TESFullName*>(Form);
	// it's a const string, so directly replacing is working
	if (OrigName)
	{
		OrigName->fullName = newName;
	}

}

void Processor::SetSHRTName(RE::TESForm* Form, RE::BSFixedString newName) //Short names of NPCs
{
	RE::TESNPC* OrigName = skyrim_cast<RE::TESNPC*>(Form);

	if (OrigName)
	{
		OrigName->shortName = newName;
	}

}

void Processor::SetTNAMName(RE::TESForm* Form, RE::BSFixedString newName) //Word of Power translation (TNAM)
{
	RE::TESWordOfPower* OrigName = skyrim_cast<RE::TESWordOfPower*>(Form);

	if (OrigName)
	{
		OrigName->translation = newName;
	}

}

/*
void Processor::SetITXTName(RE::TESForm* Form)
{
	RE::MessageBoxData* OrigName = skyrim_cast<RE::MessageBoxData*>(Form);

	auto it = g_ITXTMap.find(Form->formID);

	if (OrigName && it != g_ITXTMap.end())
	{
		// Set new button text array
		OrigName->buttonText = it->second;
	}
}
*/