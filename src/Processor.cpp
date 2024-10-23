#include "Processor.h"
#include "Config.h"
#include "Utils.h"

void Processor::RunConstTranslation()
{
	if (m_ConstConfigurationInformationStruct.empty())
		return;

	for (const auto& Information : m_ConstConfigurationInformationStruct)
	{

		switch (Information.SubrecordType)
		{
		case Config::SubrecordType::kFULL: //DIAL FULL, REFR FULL aren't working like this.
		{
			SetFullnameStrings(Information.Form, Information.ReplacerText);
		}
		break;
		case Config::SubrecordType::kDESC: //Only LSCR DESC
		{
			SetConstStrings<RE::TESLoadScreen>(Information.Form, Information.ReplacerText, &RE::TESLoadScreen::loadingText);
		}
		break;
		case Config::SubrecordType::kDNAM:
		{
			SetConstStrings<RE::EffectSetting>(Information.Form, Information.ReplacerText, &RE::EffectSetting::magicItemDescription);
		}
		break;
		case Config::SubrecordType::kSHRT:
		{
			SetConstStrings<RE::TESNPC>(Information.Form, Information.ReplacerText, &RE::TESNPC::shortName);
		}
		break;
		case Config::SubrecordType::kTNAM:
		{
			SetConstStrings<RE::TESWordOfPower>(Information.Form, Information.ReplacerText, &RE::TESWordOfPower::translation);
		}
		break;
		case Config::SubrecordType::kDATA:
		{
			SetGameSettingString(Information.EditorID, Information.ReplacerText);
		}
		break;
		case Config::SubrecordType::kITXT:
		{
			SetMessageBoxButtonStrings(Information.Form, Information.ReplacerText, Information.pos);
		}
		break;
		case Config::SubrecordType::kRDMP:
		{
			SetRegionDataStrings(Information.Form, Information.ReplacerText);
		}
		break;
		case Config::SubrecordType::kEPFD:
		{
			SetEntryPointStrings(Information.Form, Information.ReplacerText, Information.pos);
		}
		break;
		case Config::SubrecordType::kNNAM:
		{
			SetQuestObjectiveStrings(Information.Form, Information.ReplacerText, Information.pos);
		}
		break;
		case Config::SubrecordType::kUnknown:
		{
			SKSE::log::info("Unknown record {0:08X} in ConstTranslation", Information.Form->formID);
			SKSE::log::error("out of plugin {}.", Utils::GetModName(Information.Form));
		}
		break;

		default: break;
		}
	}
	m_ConstConfigurationInformationStruct.clear();
	m_ConstConfigurationInformationStruct.shrink_to_fit();

}

template <typename T>
void Processor::SetConstStrings(RE::TESForm* form, const RE::BSFixedString& newString, RE::BSFixedString T::* memberPtr)
{
	if (auto* OrigString = static_cast<T*>(form); OrigString)
	{
		try
		{
			OrigString->*memberPtr = std::move(newString);
		}
		catch (const std::exception& e)
		{
			Report(form);
			SKSE::log::error("Exception occured while changing string above: {}", e.what());
		}
	}
	else
	{
		Report(form);
	}
}

void Processor::SetFullnameStrings(RE::TESForm* form, const std::string& newString)
{
	RE::TESFullName* OrigString = skyrim_cast<RE::TESFullName*>(form);

	if (!OrigString)
	{
		Report(form);
		return;
	}

	OrigString->SetFullName(newString.c_str());
}

void Processor::SetGameSettingString(const std::string& a_name, const std::string& a_NewString)
{
	auto setting = RE::GameSettingCollection::GetSingleton()->GetSetting(a_name.c_str());

	if (setting == nullptr)
	{
		SKSE::log::debug("Failed to set GameSetting string for {}. NOTE: It is normal for this to happen with some settings, they are simply not loaded.", a_name.c_str());
		return;
	}

	if (setting->GetType() == RE::Setting::Type::kString)
	{
		changeSettingString(setting, a_NewString.c_str());
	}
}


void Processor::SetMessageBoxButtonStrings(RE::TESForm* form, const RE::BSFixedString& newString, const int& index)
{
	RE::BGSMessage* message = form->As<RE::BGSMessage>(); //MESG ITXT
	if (message)
	{
		int pos = 0;
		for (const auto& button : message->menuButtons)
		{
			//SKSE::log::info("Pos: {} - String: {}", pos, text->text.c_str());

			if (pos == index)
			{
				button->text = std::move(newString);
			}

			pos++;
		}

	}
	else
	{
		RE::BGSPerk* perk = form->As<RE::BGSPerk>(); //PERK EPF2

		if (!perk)
		{
			Report(form);
			return;
		}

		for (const auto& entry : perk->perkEntries)
		{
			if (entry->GetType() == RE::PERK_ENTRY_TYPE::kEntryPoint)
			{
				RE::BGSEntryPointPerkEntry* entryPoint = static_cast<RE::BGSEntryPointPerkEntry*>(entry);

				if (entryPoint->entryData.function == RE::BGSEntryPointPerkEntry::EntryData::Function::kAddActivateChoice)
				{
					RE::BGSEntryPointFunctionDataActivateChoice* func = static_cast<RE::BGSEntryPointFunctionDataActivateChoice*>(entryPoint->functionData);

					//SKSE::log::info("ID: {} - String: {}", func->id, func->label);

					if (func->id == index)
					{
						func->label = std::move(newString);
					}

				}
			}

		}

	}

}

void Processor::SetRegionDataStrings(RE::TESForm* form, const RE::BSFixedString& newString) //REGN RDMP
{
	RE::TESRegion* regionData = form->As<RE::TESRegion>();

	if (!regionData || !regionData->dataList)
	{
		Report(form);
		return;
	}

	for (const auto& region : regionData->dataList->regionDataList)
	{
		if (region->GetType() == RE::TESRegionData::Type::kMap)
		{
			RE::TESRegionDataMap* mapData = static_cast<RE::TESRegionDataMap*>(region);
			mapData->mapName = std::move(newString);
		}
	}

}

void Processor::SetEntryPointStrings(RE::TESForm* form, const RE::BSFixedString& newString, const int& index) //PERK EPFD
{
	RE::BGSPerk* perk = form->As<RE::BGSPerk>();
	if (!perk)
	{
		Report(form);
		return;
	}

	int entryCount = perk->perkEntries.size();
	for (int i = entryCount - 1; i >= 0; --i)
	{
		const auto& entry = perk->perkEntries[i];

		if (entry->GetType() == RE::PERK_ENTRY_TYPE::kEntryPoint)
		{
			RE::BGSEntryPointPerkEntry* entryPoint = static_cast<RE::BGSEntryPointPerkEntry*>(entry);

			if (entryPoint->entryData.function == RE::BGSEntryPointPerkEntry::EntryData::Function::kSetText)
			{
				RE::BGSEntryPointFunctionDataText* func = static_cast<RE::BGSEntryPointFunctionDataText*>(entryPoint->functionData);

				if (entryCount - 1 - i == index)
				{
					func->text = std::move(newString);
				}
			}
		}

	}
}


void Processor::SetQuestObjectiveStrings(RE::TESForm* form, const RE::BSFixedString& newString, const int& index) //QUST NNAM
{
	RE::TESQuest* quest = form->As<RE::TESQuest>();

	if (!quest)
	{
		Report(form);
		return;
	}

	for (const auto& objective : quest->objectives)
	{
		if (objective->index == index)
		{
			objective->displayText = std::move(newString);
		}
	}
}

void Processor::Report(const RE::TESForm* form)
{
	if (!form) // Should not happen, because it's only added if valid
		return;

	std::stringstream ss;
	ss << "Issue during ConstTranslation with FormID: " << std::format("{0:08X}", form->formID)
		<< " - Formtype: " << RE::FormTypeToString(form->GetFormType())
		<< " - Plugin: " << Utils::GetModName(form);

	SKSE::log::error("{}", ss.str());

}
