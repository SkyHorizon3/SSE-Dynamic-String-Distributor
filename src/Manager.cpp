#include "Manager.h"
#include "Utils.h"
#include "Conditions.h"

/*
void Manager::buildConditions()
{
	auto processEntry = [](auto& pair) {
		auto& entry = pair.second;
		if (!entry.conditionString.empty()) {
			ConditionParser::BuildCondition(entry.conditions, entry.conditionString);
			entry.conditionString.clear();
		}
		};

	std::for_each(std::execution::par, m_REFR.begin(), m_REFR.end(), processEntry);
	std::for_each(std::execution::par, m_constTranslation.begin(), m_constTranslation.end(), processEntry);


}
*/

SubrecordType Manager::getSubrecordType_map(std::string_view type)
{
	static constexpr frozen::unordered_map<std::string_view, SubrecordType, 12> typeMap = {
		{"FULL"sv, SubrecordType::kFULL},
		{"SHRT"sv, SubrecordType::kSHRT},
		{"DATA"sv, SubrecordType::kDATA},
		{"TNAM"sv, SubrecordType::kTNAM},
		{"RDMP"sv, SubrecordType::kRDMP},

		{"DESC"sv, SubrecordType::kDESC},
		{"CNAM"sv, SubrecordType::kCNAM},
		{"DNAM"sv, SubrecordType::kDNAM},
		{"NNAM"sv, SubrecordType::kNNAM},
		{"ITXT"sv, SubrecordType::kITXT},
		{"EPFD"sv, SubrecordType::kEPFD},
		{"EPF2"sv, SubrecordType::kITXT},
	};

	const auto it = typeMap.find(type);
	return (it != typeMap.end()) ? it->second : SubrecordType::kUnknown;
}

template <typename Map, typename Key, typename replace>
bool Manager::getReplacerText(const Map& map, const Key& key, replace& string)
{
	const auto it = map.find(key);
	if (it != map.end())
	{
		string = it->second.replacerText;
		return true;
	}
	return false;
}

void Manager::addToConst(RE::TESForm* form, const ParseData& data)
{
	StringData stringData{ data.string, data.conditions };

	if (data.index.has_value())
		stringData.pos = data.index.value();

	stringData.subRecordType = getSubrecordType_map(Utils::getAfterSpace(data.type));

	if (form != nullptr)
	{
		//SKSE::log::info("FormID: {} - index: {}", std::format("{0:08X}", form->formID), stringData.pos);

		runConstTranslation(form, stringData);

		//m_constTranslation.insert_or_assign(
		//	form->formID, // not unique, can be more than one replacement per form
		//	stringData
		//);
	}
	else
	{
		if (data.editor_id.has_value())
			stringData.form_id = data.editor_id.value();

		m_constTranslationGMST.emplace_back(stringData);
	}
}


void Manager::checkConst()
{
	for (const auto& entry : m_constTranslationGMST)
	{
		SetGameSettingString(entry.form_id, entry.replacerText);
	}
	m_constTranslationGMST.clear();
}

void Manager::addDIAL(const RE::FormID& formID, const std::string& plugin, const ParseData& data)
{
	m_DIAL_RNAM_Map.insert_or_assign(
		constructKey(formID, plugin),
		StringData{ data.string, data.conditions }
	);
}

bool Manager::getDIAL(const RE::FormID& formID, const std::string& plugin, RE::BSString& description)
{
	return getReplacerText(m_DIAL_RNAM_Map, constructKey(formID, plugin), description);
}

void Manager::addINFO_NAM1(const RE::FormID& formID, const std::string& plugin, const int& responseNumber, const ParseData& data)
{
	auto& valueList = m_INFO_NAM1_Map[constructKey(formID, plugin)];

	auto it = std::find_if(valueList.begin(), valueList.end(),
		[&responseNumber](const StringData& v) { return v.pos == responseNumber; });

	if (it != valueList.end())
	{
		it->replacerText = data.string;
		it->conditionString = data.conditions;
	}
	else
	{
		StringData stringData{ data.string, data.conditions };
		stringData.pos = responseNumber;
		valueList.emplace_back(stringData);
	}
}

bool Manager::getINFO_NAM1(const RE::FormID& formID, const std::string& plugin, const std::uint8_t& responseNumber, RE::BSString& string)
{
	const auto it = m_INFO_NAM1_Map.find(constructKey(formID, plugin));
	if (it != m_INFO_NAM1_Map.end())
	{
		const auto& value = it->second;
		auto contains = std::find_if(value.begin(), value.end(),
			[&responseNumber](const StringData& v) { return v.pos == responseNumber; });

		if (contains != value.end())
		{
			string = contains->replacerText;
			return true;
		}
	}

	return false;
}

std::string Manager::constructKey(const RE::FormID& formID, const std::string& plugin)
{
	if (formID == 0 || plugin.empty())
		return {};

	return std::to_string(formID) + plugin;
}

void Manager::addDESC(RE::TESForm* form, const ParseData& data) // no duplicates should arrive here
{
	m_DESC.insert_or_assign(
		form->formID,
		StringData{ data.string, data.conditions }
	);
}

bool Manager::getDESC(const RE::FormID& formID, std::string& description)
{
	return getReplacerText(m_DESC, formID, description);
}

void Manager::addCNAM(RE::TESForm* form, const ParseData& data)
{
	m_CNAM.insert_or_assign(
		form->formID,
		StringData{ data.string, data.conditions }
	);
}

bool Manager::getCNAM(const RE::FormID& formID, std::string& description)
{
	return getReplacerText(m_CNAM, formID, description);
}

void Manager::addREFR(RE::TESForm* form, const ParseData& data)
{
	m_REFR.insert_or_assign(
		form->formID,
		StringData{ data.string, data.conditions }
	);
}

bool Manager::getREFR(const RE::FormID& formID, std::string& description)
{
	return getReplacerText(m_REFR, formID, description);
}

void Manager::addQUST(const std::string& original, const ParseData& data)
{
	m_QUST_CNAM_Map.insert_or_assign(
		original,
		StringData{ data.string, data.conditions }
	);
}

bool Manager::getQUST(const std::string& original, RE::BSString& description)
{
	return getReplacerText(m_QUST_CNAM_Map, original, description);
}

void Manager::addACTI(const RE::FormID& formID, const std::string& plugin, const ParseData& data)
{
	m_ACTI_Map.insert_or_assign(
		constructKey(formID, plugin),
		StringData{ data.string, data.conditions }
	);
}

bool Manager::getACTI(const RE::FormID& formID, const std::string& plugin, std::string& description)
{
	return getReplacerText(m_ACTI_Map, constructKey(formID, plugin), description);
}

template <typename T>
void Manager::SetConstStrings(RE::TESForm* form, const RE::BSFixedString& newString, RE::BSFixedString T::* memberPtr)
{
	if (auto* OrigString = static_cast<T*>(form); OrigString)
	{
		OrigString->*memberPtr = Utils::validateString(newString);
	}
	else
	{
		Report(form);
	}
}

void Manager::SetFullnameStrings(RE::TESForm* form, const std::string& newString)
{
	const auto OrigString = form->As<RE::TESFullName>();

	if (!OrigString)
	{
		Report(form);
		return;
	}

	OrigString->SetFullName(newString.c_str());
}

void Manager::SetGameSettingString(const std::string& a_name, const std::string& a_newString)
{
	auto setting = RE::GameSettingCollection::GetSingleton()->GetSetting(a_name.c_str());

	if (setting == nullptr)
	{
		SKSE::log::debug("Failed to set GameSetting string for {}. NOTE: It is normal for this to happen with some settings, they are simply not loaded.", a_name.c_str());
		return;
	}

	if (setting->GetType() == RE::Setting::Type::kString)
	{
		changeSettingString(setting, a_newString.c_str());
	}
}


void Manager::SetMessageBoxButtonStrings(RE::TESForm* form, const RE::BSFixedString& newString, const int& index)
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
				button->text = Utils::validateString(newString);
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
						func->label = Utils::validateString(newString);
					}

				}
			}

		}

	}

}

void Manager::SetRegionDataStrings(RE::TESForm* form, const RE::BSFixedString& newString) //REGN RDMP
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
			mapData->mapName = Utils::validateString(newString);
		}
	}

}

void Manager::SetEntryPointStrings(RE::TESForm* form, const RE::BSFixedString& newString, const int& index) //PERK EPFD
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
					func->text = Utils::validateString(newString);
				}
			}
		}

	}
}


void Manager::SetQuestObjectiveStrings(RE::TESForm* form, const RE::BSFixedString& newString, const int& index) //QUST NNAM
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
			objective->displayText = Utils::validateString(newString);
		}
	}
}

void Manager::Report(const RE::TESForm* form)
{
	if (!form) // Should not happen, because it's only added if valid
		return;


	std::stringstream ss;
	ss << "Issue during ConstTranslation with FormID: " << std::format("{0:08X}", form->formID)
		<< " - Formtype: " << RE::FormTypeToString(form->GetFormType())
		<< " - Plugin: " << Utils::getModName(form);

	SKSE::log::error("{}", ss.str());

}


void Manager::runConstTranslation(RE::TESForm* form, const StringData& data)
{
	switch (data.subRecordType)
	{
	case SubrecordType::kFULL: //DIAL FULL, REFR FULL aren't working like this.
	{
		SetFullnameStrings(form, data.replacerText);
	}
	break;
	case SubrecordType::kDESC: //Only LSCR DESC
	{
		SetConstStrings<RE::TESLoadScreen>(form, data.replacerText, &RE::TESLoadScreen::loadingText);
	}
	break;
	case SubrecordType::kDNAM:
	{
		SetConstStrings<RE::EffectSetting>(form, data.replacerText, &RE::EffectSetting::magicItemDescription);
	}
	break;
	case SubrecordType::kSHRT:
	{
		SetConstStrings<RE::TESNPC>(form, data.replacerText, &RE::TESNPC::shortName);
	}
	break;
	case SubrecordType::kTNAM:
	{
		SetConstStrings<RE::TESWordOfPower>(form, data.replacerText, &RE::TESWordOfPower::translation);
	}
	break;
	case SubrecordType::kDATA:
	{
		SetGameSettingString(data.form_id, data.replacerText);
	}
	break;
	case SubrecordType::kITXT:
	{
		SetMessageBoxButtonStrings(form, data.replacerText, data.pos);
	}
	break;
	case SubrecordType::kRDMP:
	{
		SetRegionDataStrings(form, data.replacerText);
	}
	break;
	case SubrecordType::kEPFD:
	{
		SetEntryPointStrings(form, data.replacerText, data.pos);
	}
	break;
	case SubrecordType::kNNAM:
	{
		SetQuestObjectiveStrings(form, data.replacerText, data.pos);
	}
	break;
	case SubrecordType::kUnknown:
	{
		SKSE::log::info("Unknown record {0:08X} in ConstTranslation", form->formID);
		SKSE::log::error("out of plugin {}.", Utils::getModName(form));
	}
	break;

	default: break;
	}
}