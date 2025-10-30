#include "Manager.h"
#include "Utils.h"
#include "RE.h"

SubrecordType Manager::subrecordToEnum(std::string_view type)
{
	switch (string::const_hash(type))
	{
	case "FULL"_h:
		return SubrecordType::kFULL;
	case "SHRT"_h:
		return SubrecordType::kSHRT;
	case "DATA"_h:
		return SubrecordType::kDATA;
	case "TNAM"_h:
		return SubrecordType::kTNAM;
	case "RDMP"_h:
		return SubrecordType::kRDMP;
	case "DESC"_h:
		return SubrecordType::kDESC;
	case "CNAM"_h:
		return SubrecordType::kCNAM;
	case "DNAM"_h:
		return SubrecordType::kDNAM;
	case "NNAM"_h:
		return SubrecordType::kNNAM;
	case "ITXT"_h:
		return SubrecordType::kITXT;
	case "EPFD"_h:
		return SubrecordType::kEPFD;
	case "EPF2"_h:
		return SubrecordType::kITXT;
	case "RNAM"_h:
		return SubrecordType::kRNAM;

	default:
		return SubrecordType::kUnknown;
	}
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

	stringData.subRecordType = subrecordToEnum(Utils::getAfterSpace(data.type));

	if (form != nullptr)
	{
		//SKSE::log::info("FormID: {} - index: {}", std::format("{0:08X}", form->formID), stringData.pos);

		runConstTranslation(form, stringData);
	}
	else
	{
		if (data.editor_id.has_value())
			stringData.form_id = data.editor_id.value();

		m_constTranslationGMST.emplace_back(stringData);
	}
}


void Manager::runGameSettingTranslation()
{
	for (const auto& entry : m_constTranslationGMST)
	{
		setGameSettingString(entry.form_id, entry.replacerText);
	}
	m_constTranslationGMST.clear();
}

void Manager::addDIAL(const RE::FormID formID, const std::string& plugin, const ParseData& data)
{
	m_DIAL_RNAM_Map.insert_or_assign(
		constructKey(formID, plugin),
		StringData{ data.string, data.conditions }
	);
}

bool Manager::getDIAL(const RE::FormID formID, const std::string& plugin, RE::BSString& description)
{
	return getReplacerText(m_DIAL_RNAM_Map, constructKey(formID, plugin), description);
}

void Manager::addINFO_NAM1(const RE::FormID formID, const std::string& plugin, const std::uint32_t responseNumber, const ParseData& data)
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

bool Manager::getINFO_NAM1(const RE::FormID formID, const std::string& plugin, const std::uint8_t responseNumber, RE::BSString& string)
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

std::string Manager::constructKey(const RE::FormID formID, const std::string& plugin)
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

bool Manager::getDESC(const RE::FormID formID, std::string& description)
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

bool Manager::getCNAM(const RE::FormID formID, std::string& description)
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

bool Manager::getREFR(const RE::FormID formID, std::string& description)
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

template <typename T>
void Manager::setConstStrings(RE::TESForm* form, const RE::BSFixedString& newString, RE::BSFixedString T::* memberPtr)
{
	if (auto* OrigString = static_cast<T*>(form); OrigString)
	{
		OrigString->*memberPtr = Utils::validateString(newString);
	}
	else
	{
		report(form);
	}
}

void Manager::setFullnameStrings(RE::TESForm* form, const std::string& newString)
{
	const auto OrigString = form->As<RE::TESFullName>();

	if (!OrigString)
	{
		report(form);
		return;
	}

	OrigString->SetFullName(newString.c_str());
}

void Manager::setGameSettingString(const std::string& a_name, const std::string& a_newString)
{
	auto setting = RE::GameSettingCollection::GetSingleton()->GetSetting(a_name.c_str());

	if (!setting)
	{
		SKSE::log::debug("Failed to set GameSetting string for {}. NOTE: It is normal for this to happen with some settings, they are simply not loaded.", a_name.c_str());
		return;
	}

	if (setting->GetType() == RE::Setting::Type::kString)
	{
		RE::setStringValue(setting, a_newString.c_str());
	}
}

void Manager::setMessageBoxButtonStrings(RE::TESForm* form, const RE::BSFixedString& newString, const std::uint32_t index)
{
	RE::BGSMessage* message = form->As<RE::BGSMessage>(); //MESG ITXT
	if (message)
	{
		std::uint32_t pos = 0;
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
			report(form);
			return;
		}

		for (const auto& entry : perk->perkEntries)
		{
			if (!entry || entry->GetType() != RE::PERK_ENTRY_TYPE::kEntryPoint)
				continue;

			const auto* entryPoint = static_cast<RE::BGSEntryPointPerkEntry*>(entry);
			if (!entryPoint)
				continue;

			if (entryPoint->entryData.function == RE::BGSEntryPointPerkEntry::EntryData::Function::kAddActivateChoice)
			{
				RE::BGSEntryPointFunctionDataActivateChoice* func = static_cast<RE::BGSEntryPointFunctionDataActivateChoice*>(entryPoint->functionData);

				//SKSE::log::info("ID: {} - String: {}", func->id, func->label);

				if (func && func->GetID() == index)
				{
					func->label = Utils::validateString(newString);
				}

			}

		}

	}

}

void Manager::setRegionDataStrings(RE::TESForm* form, const RE::BSFixedString& newString) //REGN RDMP
{
	RE::TESRegion* regionData = form->As<RE::TESRegion>();

	if (!regionData || !regionData->dataList)
	{
		report(form);
		return;
	}

	for (const auto& region : regionData->dataList->regionDataList)
	{
		if (!region || region->GetType() != RE::TESRegionData::Type::kMap)
			continue;

		auto* mapData = static_cast<RE::TESRegionDataMap*>(region);
		if (!mapData)
			continue;

		mapData->mapName = Utils::validateString(newString);
	}

}

void Manager::setEntryPointStrings(RE::TESForm* form, const RE::BSFixedString& newString, const std::uint32_t index) //PERK EPFD
{
	RE::BGSPerk* perk = form->As<RE::BGSPerk>();
	if (!perk)
	{
		report(form);
		return;
	}

	const std::uint32_t entryCount = perk->perkEntries.size();
	for (std::uint32_t i = entryCount; i-- > 0;)
	{
		const auto& entry = perk->perkEntries[i];
		if (!entry || entry->GetType() != RE::PERK_ENTRY_TYPE::kEntryPoint)
			continue;

		const auto* entryPoint = static_cast<RE::BGSEntryPointPerkEntry*>(entry);
		if (!entryPoint)
			continue;

		if (entryPoint->entryData.function == RE::BGSEntryPointPerkEntry::EntryData::Function::kSetText)
		{
			auto* func = static_cast<RE::BGSEntryPointFunctionDataText*>(entryPoint->functionData);

			if (func && entryCount - 1 - i == index)
			{
				func->text = Utils::validateString(newString);
			}
		}
	}
}

void Manager::setQuestObjectiveStrings(RE::TESForm* form, const RE::BSFixedString& newString, const std::uint32_t index) //QUST NNAM
{
	RE::TESQuest* quest = form->As<RE::TESQuest>();

	if (!quest)
	{
		report(form);
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

void Manager::setActivateOverrideStrings(RE::TESForm* form, const RE::BSFixedString& newString)
{
	auto& overrideMap = RE::getActivateTextOverrideMap();

	const auto it = overrideMap.find(form->formID);
	if (it != overrideMap.end())
	{
		it->second = newString;
	}
	else
	{
		overrideMap.emplace(form->formID, newString);
	}
}

void Manager::report(const RE::TESForm* form)
{
	std::stringstream ss;
	ss << "Tried to cast " << std::format("{0:08X}", form->formID)
		<< " to an invalid form type - Actual Formtype: " << RE::FormTypeToString(form->GetFormType())
		<< " - Plugin: " << Utils::getModName(form);

	SKSE::log::error("{}", ss.str());
}

void Manager::runConstTranslation(RE::TESForm* form, const StringData& data)
{
	switch (data.subRecordType)
	{
	case SubrecordType::kFULL: //DIAL FULL, REFR FULL aren't working like this.
	{
		setFullnameStrings(form, data.replacerText);
	}
	break;
	case SubrecordType::kDESC: //Only LSCR DESC
	{
		setConstStrings<RE::TESLoadScreen>(form, data.replacerText, &RE::TESLoadScreen::loadingText);
	}
	break;
	case SubrecordType::kDNAM:
	{
		setConstStrings<RE::EffectSetting>(form, data.replacerText, &RE::EffectSetting::magicItemDescription);
	}
	break;
	case SubrecordType::kSHRT:
	{
		setConstStrings<RE::TESNPC>(form, data.replacerText, &RE::TESNPC::shortName);
	}
	break;
	case SubrecordType::kTNAM:
	{
		setConstStrings<RE::TESWordOfPower>(form, data.replacerText, &RE::TESWordOfPower::translation);
	}
	break;
	case SubrecordType::kDATA:
	{
		setGameSettingString(data.form_id, data.replacerText);
	}
	break;
	case SubrecordType::kITXT:
	{
		setMessageBoxButtonStrings(form, data.replacerText, data.pos);
	}
	break;
	case SubrecordType::kRDMP:
	{
		setRegionDataStrings(form, data.replacerText);
	}
	break;
	case SubrecordType::kEPFD:
	{
		setEntryPointStrings(form, data.replacerText, data.pos);
	}
	break;
	case SubrecordType::kNNAM:
	{
		setQuestObjectiveStrings(form, data.replacerText, data.pos);
	}
	break;
	case SubrecordType::kRNAM:
	{
		setActivateOverrideStrings(form, data.replacerText);
	}
	break;
	case SubrecordType::kUnknown:
	{
		const auto formID = std::format("{0:08X}", form->formID);
		SKSE::log::error("Unknown record {} in ConstTranslation out of plugin {}", formID, Utils::getModName(form));
	}
	break;

	default: break;
	}
}