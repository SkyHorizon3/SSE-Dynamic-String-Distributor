#include "Processor.h"
#include "Config.h"
#include "Utils.h"

void Processor::RunConstTranslation()
{
	if (!m_ConstConfigurationInformationStruct.empty())
	{
		for (const auto& Information : m_ConstConfigurationInformationStruct)
		{

			switch (Information.SubrecordType)
			{
			case Config::SubrecordType::kFULL: //DIAL FULL, REFR FULL aren't working like this.
			{
				SetConstStrings<RE::TESFullName>(Information.Form, Information.ReplacerText, &RE::TESFullName::fullName);
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
		m_ConstConfigurationInformationStruct.clear(); //Structs only used once, so no need to keep them for fun
		m_ConstConfigurationInformationStruct.shrink_to_fit();
	}

}

template <typename T>
void Processor::SetConstStrings(RE::TESForm* Form, RE::BSFixedString NewString, RE::BSFixedString T::* memberPtr)
{
	T* OrigString = skyrim_cast<T*>(Form);
	if (OrigString)
	{
		OrigString->*memberPtr = std::move(NewString);
	}
	else
	{
		SKSE::log::error("Issue during ConstTranslation with FormID: {0:08X}", Form->formID);
		SKSE::log::error("out of plugin {}.", Utils::GetModName(Form));
	}

}

void Processor::SetGameSettingString(const std::string& a_name, const std::string& a_NewString)
{
	auto setting = RE::GameSettingCollection::GetSingleton()->GetSetting(a_name.c_str());

	if (setting == nullptr)
	{
#ifndef NDEBUG

		SKSE::log::debug("Failed to set GameSetting string for {}. NOTE: It is normal for this to happen with some settings, they are simply not loaded.", a_name.c_str());
#endif
		return;
	}

	if (setting->GetType() == RE::Setting::Type::kString)
	{
		//RE::free(setting->data.s); -> don't know why this is crashing. If you know, please tell me!

		auto length = a_NewString.length() + 1;
		char* NewChar = (char*)RE::MemoryManager::GetSingleton()->Allocate(length, 0, 0);

		if (!NewChar)
		{
			SKSE::log::error("Failed to allocate memory for a new string when setting game setting '{}'. Upgrade your RAM and get rid of Windows XP!", a_name);
			return;
		}

		memcpy(NewChar, a_NewString.c_str(), length);

		setting->data.s = NewChar;
	}
}


void Processor::SetMessageBoxButtonStrings(RE::TESForm* Form, RE::BSFixedString NewString, int index)
{
	RE::BGSMessage* message = skyrim_cast<RE::BGSMessage*>(Form); //MESG ITXT
	if (message)
	{
		int pos = 0;
		for (auto& button : message->menuButtons)
		{
			//SKSE::log::info("Pos: {} - String: {}", pos, text->text.c_str());

			if (pos == index)
			{
				button->text = std::move(NewString);
			}

			pos++;
		}

	}
	else
	{
		RE::BGSPerk* perk = skyrim_cast<RE::BGSPerk*>(Form); //PERK EPF2

		if (perk)
		{
			for (auto& entry : perk->perkEntries)
			{
				if (entry->GetType() == RE::PERK_ENTRY_TYPE::kEntryPoint)
				{
					RE::BGSEntryPointPerkEntry* entryPoint = skyrim_cast<RE::BGSEntryPointPerkEntry*>(entry);

					if (entryPoint->entryData.function == RE::BGSEntryPointPerkEntry::EntryData::Function::kAddActivateChoice)
					{
						RE::BGSEntryPointFunctionDataActivateChoice* func = skyrim_cast<RE::BGSEntryPointFunctionDataActivateChoice*>(entryPoint->functionData);

						//SKSE::log::info("ID: {} - String: {}", func->id, func->label);

						if (func->id == index)
						{
							func->label = std::move(NewString);
						}

					}
				}

			}

		}
		else
		{
			SKSE::log::error("Issue during ConstTranslation with FormID: {0:08X}", Form->formID);
			SKSE::log::error("out of plugin {}.", Utils::GetModName(Form));
		}
	}

}

void Processor::SetRegionDataStrings(RE::TESForm* Form, RE::BSFixedString NewString) //REGN RDMP
{
	RE::TESRegion* regionData = skyrim_cast<RE::TESRegion*>(Form);

	if (regionData && regionData->dataList)
	{
		for (const auto& region : regionData->dataList->regionDataList)
		{
			if (region->GetType() == RE::TESRegionData::Type::kMap)
			{
				RE::TESRegionDataMap* mapData = skyrim_cast<RE::TESRegionDataMap*>(region);
				mapData->mapName = std::move(NewString);
			}
		}
	}
	else
	{
		SKSE::log::error("Issue during ConstTranslation with FormID: {0:08X}", Form->formID);
		SKSE::log::error("out of plugin {}.", Utils::GetModName(Form));
	}
}

void Processor::SetEntryPointStrings(RE::TESForm* Form, RE::BSFixedString NewString, int index) //PERK EPFD
{
	RE::BGSPerk* perk = skyrim_cast<RE::BGSPerk*>(Form);

	if (perk)
	{
		int pos = 0;
		for (auto& entry : perk->perkEntries)
		{
			if (entry->GetType() == RE::PERK_ENTRY_TYPE::kEntryPoint)
			{
				RE::BGSEntryPointPerkEntry* entryPoint = skyrim_cast<RE::BGSEntryPointPerkEntry*>(entry);

				if (entryPoint->entryData.function == RE::BGSEntryPointPerkEntry::EntryData::Function::kSetText)
				{
					RE::BGSEntryPointFunctionDataText* func = skyrim_cast<RE::BGSEntryPointFunctionDataText*>(entryPoint->functionData);

					if (pos == index)
					{
						func->text = std::move(NewString);
					}

					pos++;
				}
			}

		}
	}
	else
	{
		SKSE::log::error("Issue during ConstTranslation with FormID: {0:08X}", Form->formID);
		SKSE::log::error("out of plugin {}.", Utils::GetModName(Form));
	}
}


void Processor::SetQuestObjectiveStrings(RE::TESForm* Form, RE::BSFixedString NewString, int index) //QUST NNAM
{
	RE::TESQuest* quest = skyrim_cast<RE::TESQuest*>(Form);

	if (quest)
	{
		for (const auto& objective : quest->objectives)
		{
			if (objective->index == index)
			{
				objective->displayText = std::move(NewString);
			}
		}
	}
	else
	{
		SKSE::log::error("Issue during ConstTranslation with FormID: {0:08X}", Form->formID);
		SKSE::log::error("out of plugin {}.", Utils::GetModName(Form));
	}
}
