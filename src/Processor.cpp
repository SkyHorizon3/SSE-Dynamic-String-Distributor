#include "../include/Processor.h"
#include "../include/Config.h"


void Processor::RunConstTranslation()
{
	for (size_t i = 0; i < g_ConfigurationInformationStruct.size(); i++)
	{
		const auto& Information = g_ConfigurationInformationStruct[i];

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

	}

}

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

/*
//Stupid experiments :D
void Processor::TEST()
{
	auto FormID = 0x0AB816;
	RE::TESDataHandler* handler = RE::TESDataHandler::GetSingleton();
	auto form = handler->LookupForm(FormID, "Skyrim.esm");


	//RE::TESForm* form = RE::TESForm::LookupByID(FormID);

	if (form)
	{
		RE::TESFullName* OrigName = skyrim_cast<RE::TESFullName*>(form);

		if (OrigName)
		{
			g_Logger->info("Name: {}", OrigName->fullName.c_str());
		}
		else
		{
			g_Logger->info("Couldn't get OrigName");
		}

	}
	else
	{

		g_Logger->info("Couldn't get form");

	}



}
*/