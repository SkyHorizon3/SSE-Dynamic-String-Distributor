#include "Manager.h"
#include "Utils.h"

void Manager::LoadINI()
{
	const auto path = std::format("Data/SKSE/Plugins/{}.ini", Plugin::NAME);

	CSimpleIniA ini;
	ini.SetUnicode();
	ini.LoadFile(path.c_str());

	constexpr const char* section = "Debug";
	m_debugLog = ini.GetBoolValue(section, "EnableDebugLog");
	m_debugInfo = ini.GetBoolValue(section, "EnableDebugInfo");
}

void Manager::enumerateLoadOrder()
{
	const auto handler = RE::TESDataHandler::GetSingleton();
	if (!handler)
		return;

	auto& files = handler->files;
	std::uint32_t index = 0;
	for (const auto& file : files)
	{
		if (file && file->compileIndex != 0xFF) // only the files that are truely active
		{
			m_loadOrder.emplace(file->GetFilename(), std::pair{ file, index++ });
		}
	}

	m_loadOrder.emplace("overwrite", std::pair{ nullptr, index });
}

std::vector<std::string> Manager::processFolders()
{
	std::vector<std::string> folders{};

	if (!std::filesystem::exists(DSD_PATH))
		return folders;

	for (const auto& entry : std::filesystem::directory_iterator(DSD_PATH))
	{
		if (!entry.is_directory())
			continue;

		const auto folderName = entry.path().filename().string();
		if (!m_loadOrder.contains(folderName))
		{
			SKSE::log::debug("Ignoring plugin {} since it was not found in the load order!", folderName);
			continue;
		}

		folders.emplace_back(folderName);
	}

	std::ranges::sort(folders.begin(), folders.end(), [&](const std::string& a, const std::string& b)
		{
			const auto itA = m_loadOrder.find(a);
			const auto itB = m_loadOrder.find(b);
			return itA->second.second < itB->second.second;
		});

	return folders;
}

std::vector<std::string> Manager::processFiles(const std::string_view folder)
{
	std::vector<std::string> files{};

	const auto folderPath = std::filesystem::path{ DSD_PATH } / folder;
	if (!std::filesystem::exists(folderPath))
		return files;

	for (const auto& entry : std::filesystem::directory_iterator(folderPath))
	{
		if (entry.is_regular_file())
		{
			const auto ext = string::tolower(entry.path().extension().string());
			if (ext == ".json")
			{
				files.emplace_back(entry.path().string());
			}
		}
	}

	std::ranges::sort(files.begin(), files.end());
	return files;
}

std::tuple<RE::FormID, RE::TESFile*> Manager::extractFormIDAndPlugin(const std::string& formIDWithPlugin)
{
	if (const auto split = string::split(formIDWithPlugin, "|"); split.size() == 2)
	{
		RE::FormID formID = Utils::convertToFormID(split[0]);
		std::string plugin = split[1];

		if (g_mergeMapperInterface)
		{
			auto mergeForm = g_mergeMapperInterface->GetNewFormID(plugin.c_str(), formID);
			plugin = mergeForm.first;
			formID = mergeForm.second;
		}

		const auto it = m_loadOrder.find(plugin);
		if (it != m_loadOrder.end())
		{
			return { formID, it->second.first };
		}
	}

	return { 0, nullptr };
}

TranslationType Manager::getTranslationType(std::string_view formType)
{
	switch (string::const_hash(formType))
	{
	case "ACTI FULL"_h:
	case "ALCH FULL"_h:
	case "AMMO FULL"_h:
	case "APPA FULL"_h:
	case "ARMO FULL"_h:
	case "AVIF FULL"_h:
	case "BOOK FULL"_h:
	case "CELL FULL"_h:
	case "CONT FULL"_h:
	case "CLAS FULL"_h:
	case "DOOR FULL"_h:
	case "ENCH FULL"_h:
	case "EXPL FULL"_h:
	case "FLOR FULL"_h:
	case "FURN FULL"_h:
	case "HAZD FULL"_h:
	case "INGR FULL"_h:
	case "KEYM FULL"_h:
	case "LCTN FULL"_h:
	case "LIGH FULL"_h:
	case "MESG FULL"_h:
	case "MGEF FULL"_h:
	case "MISC FULL"_h:
	case "PERK FULL"_h:
	case "PROJ FULL"_h:
	case "QUST FULL"_h:
	case "RACE FULL"_h:
	case "SCRL FULL"_h:
	case "SHOU FULL"_h:
	case "SLGM FULL"_h:
	case "SPEL FULL"_h:
	case "TACT FULL"_h:
	case "TREE FULL"_h:
	case "WATR FULL"_h:
	case "WEAP FULL"_h:
	case "WOOP FULL"_h:
	case "WRLD FULL"_h:
	{
		return TranslationType::kFullName;
	}
	case "LSCR DESC"_h:
	{
		return TranslationType::kLoadScreenDescription;
	}
	case "MGEF DNAM"_h:
	{
		return TranslationType::kMagicDescription;
	}
	case "NPC_ SHRT"_h:
	{
		return TranslationType::kShortName;
	}
	case "REGN RDMP"_h:
	{
		return TranslationType::kRegion;
	}
	case "WOOP TNAM"_h:
	{
		return TranslationType::kWordOfPower;
	}
	case "MESG ITXT"_h:
	{
		return TranslationType::kButtonText1;
	}
	case "PERK EPF2"_h:
	{
		return TranslationType::kButtonText2;
	}
	case "QUST NNAM"_h:
	{
		return TranslationType::kQuestObjective;
	}
	case "PERK EPFD"_h:
	{
		return TranslationType::kPerkVerb;
	}
	case "ACTI RNAM"_h:
	case "FLOR RNAM"_h:
	{
		return TranslationType::kActivationText;
	}
	case "DIAL FULL"_h:
	case "AMMO DESC"_h:
	case "ARMO DESC"_h:
	case "AVIF DESC"_h:
	case "BOOK DESC"_h:
	case "MESG DESC"_h:
	case "PERK DESC"_h:
	case "RACE DESC"_h:
	case "SCRL DESC"_h:
	case "SHOU DESC"_h:
	case "SPEL DESC"_h:
	case "WEAP DESC"_h:
	{
		return TranslationType::kRuntime1;
	}
	case "GMST DATA"_h:
	{
		return TranslationType::kGameSetting;
	}
	case "REFR FULL"_h:
	{
		return TranslationType::kReference;
	}
	case "QUST CNAM"_h:
	{
		return TranslationType::kRuntimeLegacy;
	}
	case "INFO NAM1"_h:
	case "NPC_ FULL"_h:
	case "BOOK CNAM"_h:
	case "INFO RNAM"_h:
	{
		return TranslationType::kRuntime2;
	}
	default:
	{
		return TranslationType::kUnknown;
	}
	}
}

void Manager::processEntry(ParseData& entry, const std::string& file)
{
	auto [formID, plugin] = extractFormIDAndPlugin(entry.form_id);
	if (formID == 0 || !plugin)
	{
		SKSE::log::error("Failed to extract formID \"{}\" out of file {}!", entry.form_id, file);
		return;
	}

	const auto runtimeFormID = Utils::getRuntimeFormID(plugin, formID);
	if (m_debugInfo)
	{
		entry.string.insert(0, std::format("Debug Info: {:08X}|{}| ", runtimeFormID, plugin->GetFilename()));
	}

	const auto translationType = getTranslationType(entry.type);
	switch (translationType)
	{
	case TranslationType::kFullName:
	case TranslationType::kLoadScreenDescription:
	case TranslationType::kMagicDescription:
	case TranslationType::kShortName:
	case TranslationType::kRegion:
	case TranslationType::kWordOfPower:
	case TranslationType::kButtonText1:
	case TranslationType::kButtonText2:
	case TranslationType::kQuestObjective:
	case TranslationType::kPerkVerb:
	case TranslationType::kActivationText:
	case TranslationType::kGameSetting:
	case TranslationType::kReference: // add to const translation
	{
		m_constTranslation.emplace_back(runtimeFormID, translationType, entry.string, entry.index, entry.editor_id);
	}
	break;
	case TranslationType::kRuntime1: // add to first runtime map
	{
		m_runtimeMap1.insert_or_assign(runtimeFormID, entry.string);
	}
	break;
	case TranslationType::kRuntime2: // add to second runtime map, strings associated with an index
	{
		const auto index = entry.index.has_value() ? entry.index.value() : 0;
		auto combined = hash::szudzik_pair(index, runtimeFormID);
		m_runtimeMap2.insert_or_assign(combined, entry.string);
	}
	break;
	case TranslationType::kRuntimeLegacy: // add to legacy string key map
	{
		if (entry.original.has_value())
		{
			m_legacyMap.insert_or_assign(entry.original.value(), entry.string);
			return;
		}

		const auto index = entry.index.has_value() ? entry.index.value() : 0;
		auto combined = hash::szudzik_pair(index, runtimeFormID);
		m_runtimeMap2.insert_or_assign(combined, entry.string);
	}
	break;
	case TranslationType::kUnknown:
	{
		SKSE::log::error("File {} contains unknown or not visible type: {}", file, entry.type);
	}
	break;

	default:
		std::unreachable();
	}
}

const char* Manager::getTranslation(const RE::FormID formID, const std::uint32_t index, const TranslationType type, std::string_view original)
{
	switch (type)
	{
	case TranslationType::kRuntime1:
	{
		const auto it = m_runtimeMap1.find(formID);
		if (it != m_runtimeMap1.end())
			return it->second.c_str();
	}
	break;
	case TranslationType::kRuntime2:
	{
		auto combined = hash::szudzik_pair(index, formID);
		const auto it = m_runtimeMap2.find(combined);
		if (it != m_runtimeMap2.end())
			return it->second.c_str();
	}
	break;
	case TranslationType::kRuntimeLegacy:
	{
		const auto itL = m_legacyMap.find(original);
		if (itL != m_legacyMap.end())
			return itL->second.c_str();

		auto combined = hash::szudzik_pair(index, formID);
		const auto itR = m_runtimeMap2.find(combined);
		if (itR != m_runtimeMap2.end())
			return itR->second.c_str();
	}
	break;
	default:
		break;
	}

	return nullptr;
}

void Manager::parseTranslationFiles()
{
	const auto folders = processFolders();
	for (const auto& folder : folders)
	{
		const auto files = processFiles(folder);
		for (const auto& file : files)
		{
			std::string buffer{};
			std::vector<ParseData> jsonData{};

			auto err = glz::read_file_json < glz::opts{ .error_on_unknown_keys = false } > (jsonData, file, buffer);
			if (err)
			{
				SKSE::log::error("Error parsing file: {} - Error: {}", file, glz::format_error(err, buffer));
				continue;
			}

			for (auto& entry : jsonData)
			{
				processEntry(entry, file);
			}
		}
	}

	m_loadOrder.clear();
}

void Manager::fixedStringChange(RE::BSFixedString& to, std::string_view from)
{
	RE::setBSFixedString(to, from.empty() ? EMPTY : from.data());
}

void Manager::setGameSettingString(const std::optional<std::string>& name, const std::string_view newString) // GMST DATA
{
	if (!name.has_value())
	{
		SKSE::log::error("Couldn't inject string \"{}\"! The editorID is missing!", newString);
		return;
	}

	const auto settingStr = (*name).c_str();
	const auto setting = RE::GameSettingCollection::GetSingleton()->GetSetting(settingStr);
	if (!setting)
	{
		SKSE::log::debug("Failed to set GameSetting string for {}.", settingStr);
		return;
	}

	if (setting->GetType() == RE::Setting::Type::kString)
	{
		RE::setStringValue(setting, newString.data());
	}
}

void Manager::setMessageBoxButtonStrings(RE::TESForm* form, std::string_view newString, const std::optional<std::uint32_t>& index) //MESG ITXT
{
	if (!index.has_value())
	{
		SKSE::log::error("Couldn't inject string \"{}\"! The index is missing!", newString);
		return;
	}

	const auto message = form->As<RE::BGSMessage>();
	if (!message)
	{
		report(form);
		return;
	}

	std::uint32_t pos = 0;
	for (const auto& button : message->menuButtons)
	{
		if (button && pos == (*index))
		{
			fixedStringChange(button->text, newString);
		}

		pos++;
	}
}

void Manager::setPerkMessageBoxButtonStrings(RE::TESForm* form, std::string_view newString, const std::optional<std::uint32_t>& index)  //PERK EPF2
{
	if (!index.has_value())
	{
		SKSE::log::error("Couldn't inject string \"{}\"! The index is missing!", newString);
		return;
	}

	const auto perk = form->As<RE::BGSPerk>();
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

		const auto data = entryPoint->functionData;
		if (!data || data->GetType() != RE::BGSEntryPointFunctionData::ENTRY_POINT_FUNCTION_DATA::kActivateChoice)
			continue;

		const auto func = static_cast<RE::BGSEntryPointFunctionDataActivateChoice*>(data);
		if (func && func->GetID() == (*index))
		{
			fixedStringChange(func->label, newString);
		}
	}
}

void Manager::setRegionDataStrings(RE::TESForm* form, std::string_view newString) //REGN RDMP
{
	const auto regionData = form->As<RE::TESRegion>();

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

		fixedStringChange(mapData->mapName, newString);
	}
}

void Manager::setEntryPointStrings(RE::TESForm* form, std::string_view newString, const std::optional<std::uint32_t>& index) //PERK EPFD
{
	if (!index.has_value())
	{
		SKSE::log::error("Couldn't inject string \"{}\"! The index is missing!", newString);
		return;
	}

	const auto perk = form->As<RE::BGSPerk>();
	if (!perk)
	{
		report(form);
		return;
	}

	const std::uint32_t entryCount = perk->perkEntries.size();
	std::uint32_t textFuncFound = 0;

	for (std::int32_t i = entryCount - 1; i >= 0; --i)
	{
		const auto& entry = perk->perkEntries[i];
		if (!entry)
			continue;

		if (entry->GetType() != RE::PERK_ENTRY_TYPE::kEntryPoint)
			continue;

		const auto* entryPoint = static_cast<RE::BGSEntryPointPerkEntry*>(entry);
		if (!entryPoint)
			continue;

		const auto data = entryPoint->functionData;
		if (!data || data->GetType() != RE::BGSEntryPointFunctionData::ENTRY_POINT_FUNCTION_DATA::kText)
			continue;

		if (textFuncFound == (*index))
		{
			const auto func = static_cast<RE::BGSEntryPointFunctionDataText*>(data);
			if (func)
			{
				fixedStringChange(func->text, newString);
			}
		}

		++textFuncFound;
	}
}

void Manager::setQuestObjectiveStrings(RE::TESForm* form, std::string_view newString, const std::optional<std::uint32_t>& index) //QUST NNAM
{
	if (!index.has_value())
	{
		SKSE::log::error("Couldn't inject string: \"{}\"! The index is missing!", newString);
		return;
	}

	const auto quest = form->As<RE::TESQuest>();
	if (!quest)
	{
		report(form);
		return;
	}

	for (const auto& objective : quest->objectives)
	{
		if (objective && objective->index == (*index))
		{
			fixedStringChange(objective->displayText, newString);
		}
	}
}

void Manager::setActivateOverrideStrings(RE::TESForm* form, std::string_view newString)
{
	auto& overrideMap = RE::getActivateTextOverrideMap();

	const auto it = overrideMap.find(form->formID);
	if (it != overrideMap.end())
	{
		fixedStringChange(it->second, newString);
	}
	else
	{
		RE::BSFixedString temp;
		fixedStringChange(temp, newString);
		overrideMap.emplace(form->formID, temp);
	}
}

void Manager::setReferenceStrings(RE::TESForm* form, std::string_view newString) // TODO: find out if there are map markers that are not persistent
{
	const auto ref = form->As<RE::TESObjectREFR>();
	if (!ref)
	{
		report(form);
		return;
	}

	const auto marker = ref ? ref->extraList.GetByType<RE::ExtraMapMarker>() : nullptr;
	const auto data = marker ? marker->mapData : nullptr;
	if (data)
	{
		data->locationName.SetFullName(newString.data());
	}
}

void Manager::report(const RE::TESForm* const form) const
{
	SKSE::log::error("Tried to cast {:08X} to an invalid form type - Actual Formtype: {} - Plugin: {}", form->formID, RE::FormTypeToString(form->GetFormType()), Utils::getModName(form));
}

void Manager::runConstTranslation()
{
	for (const auto& entry : m_constTranslation)
	{
		RE::TESForm* form = nullptr;

		if (entry.translationType != TranslationType::kGameSetting)
		{
			form = RE::TESForm::LookupByID(entry.runtimeFormID);
			if (!form)
			{
				SKSE::log::error("Couldn't find formID {:08X}!", entry.runtimeFormID);
				continue;
			}
		}

		switch (entry.translationType)
		{
		case TranslationType::kFullName:
		{
			const auto OrigString = form->As<RE::TESFullName>();
			if (!OrigString)
			{
				report(form);
				continue;
			}

			OrigString->SetFullName(entry.replacerText.c_str());
		}
		break;
		case TranslationType::kLoadScreenDescription: // TODO: add formType checks
		{
			auto loadScreen = form->As<RE::TESLoadScreen>();
			if (!loadScreen)
			{
				report(form);
				continue;
			}

			fixedStringChange(loadScreen->loadingText, entry.replacerText);
		}
		break;
		case TranslationType::kMagicDescription:
		{
			auto effect = form->As<RE::EffectSetting>();
			if (!effect)
			{
				report(form);
				continue;
			}

			fixedStringChange(effect->magicItemDescription, entry.replacerText);
		}
		break;
		case TranslationType::kShortName:
		{
			auto npc = form->As<RE::TESNPC>();
			if (!npc)
			{
				report(form);
				continue;
			}

			fixedStringChange(npc->shortName, entry.replacerText);
		}
		break;
		case TranslationType::kRegion:
		{
			setRegionDataStrings(form, entry.replacerText);
		}
		break;
		case TranslationType::kWordOfPower:
		{
			auto word = form->As<RE::TESWordOfPower>();
			if (!word)
			{
				report(form);
				continue;
			}

			fixedStringChange(word->translation, entry.replacerText);
		}
		break;
		case TranslationType::kButtonText1:
		{
			setMessageBoxButtonStrings(form, entry.replacerText, entry.index);
		}
		break;
		case TranslationType::kButtonText2:
		{
			setPerkMessageBoxButtonStrings(form, entry.replacerText, entry.index);
		}
		break;
		case TranslationType::kQuestObjective:
		{
			setQuestObjectiveStrings(form, entry.replacerText, entry.index);
		}
		break;
		case TranslationType::kPerkVerb:
		{
			setEntryPointStrings(form, entry.replacerText, entry.index);
		}
		break;
		case TranslationType::kActivationText:
		{
			setActivateOverrideStrings(form, entry.replacerText);
		}
		break;
		case TranslationType::kReference:
		{
			setReferenceStrings(form, entry.replacerText);
		}
		break;
		case TranslationType::kGameSetting:
		{
			setGameSettingString(entry.editor_id, entry.replacerText);
		}
		break;
		default:
			std::unreachable();
		}

	}

	m_constTranslation.clear();
	m_constTranslation.shrink_to_fit();
}