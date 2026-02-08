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

std::tuple<RE::FormID, std::string> Manager::extractFormIDAndPlugin(const std::string& formIDWithPlugin)
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

		if (m_loadOrder.contains(plugin))
		{
			return { formID, plugin };
		}
	}

	return { 0, "" };
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
	case "INFO RNAM"_h:
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
	if (formID == 0 || plugin.empty())
	{
		SKSE::log::error("Failed to extract formID and plugin for {} out of file {}", entry.form_id, file);
		return;
	}

	if (m_debugInfo)
	{
		entry.string.insert(0, std::format("Debug Info: {:08X}|{}| ", formID, plugin));
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
		const auto filePtr = m_loadOrder.find(plugin)->second.first;
		const auto runtimeFormID = Utils::getRuntimeFormID(filePtr, formID);
		m_constTranslation.emplace_back(runtimeFormID, entry.string, translationType, entry.index, entry.editor_id);
	}
	break;
	case TranslationType::kRuntime1: // add to first runtime map
	{

	}
	break;
	case TranslationType::kRuntime2: // add to second runtime map
	{

	}
	break;
	case TranslationType::kRuntimeLegacy: // add to legacy string key map
	{

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
}

void Manager::setGameSettingString(const std::string& a_name, const std::string& a_newString)
{
	const auto setting = RE::GameSettingCollection::GetSingleton()->GetSetting(a_name.c_str());

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
	const auto message = form->As<RE::BGSMessage>(); //MESG ITXT
	if (message)
	{
		std::uint32_t pos = 0;
		for (const auto& button : message->menuButtons)
		{
			if (pos == index)
			{
				button->text = Utils::validateString(newString);
			}

			pos++;
		}
	}
	else
	{
		const auto perk = form->As<RE::BGSPerk>(); //PERK EPF2

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
			if (func && func->GetID() == index)
			{
				func->label = Utils::validateString(newString);
			}
		}

	}

}

void Manager::setRegionDataStrings(RE::TESForm* form, const RE::BSFixedString& newString) //REGN RDMP
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

		mapData->mapName = Utils::validateString(newString);
	}

}

void Manager::setEntryPointStrings(RE::TESForm* form, const RE::BSFixedString& newString, const std::uint32_t index) //PERK EPFD
{
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

		if (textFuncFound == index)
		{
			const auto func = static_cast<RE::BGSEntryPointFunctionDataText*>(data);
			if (func)
			{
				func->text = Utils::validateString(newString);
			}
		}

		++textFuncFound;
	}
}

void Manager::setQuestObjectiveStrings(RE::TESForm* form, const RE::BSFixedString& newString, const std::uint32_t index) //QUST NNAM
{
	const auto quest = form->As<RE::TESQuest>();

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

void Manager::report(const RE::TESForm* const form)
{
	std::stringstream ss;
	ss << "Tried to cast " << std::format("{0:08X}", form->formID)
		<< " to an invalid form type - Actual Formtype: " << RE::FormTypeToString(form->GetFormType())
		<< " - Plugin: " << Utils::getModName(form);

	SKSE::log::error("{}", ss.str());
}

void Manager::runConstTranslation()
{
	for (const auto& entry : m_constTranslation)
	{
		const auto form = RE::TESForm::LookupByID(entry.runtimeFormID);
		if (!form)
		{
			SKSE::log::error("Couldn't find formID {:08X}!", entry.runtimeFormID);
			continue;
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

			loadScreen->loadingText = entry.replacerText;
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

			effect->magicItemDescription = entry.replacerText;
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

			npc->shortName = entry.replacerText;
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

			word->translation = entry.replacerText;
		}
		break;
		case TranslationType::kButtonText1:
		{
			//setMessageBoxButtonStrings(form, data.replacerText, data.pos);
		}
		break;
		case TranslationType::kButtonText2:
		{
			//setMessageBoxButtonStrings(form, data.replacerText, data.pos);
		}
		break;
		case TranslationType::kQuestObjective:
		{
			//setQuestObjectiveStrings(form, data.replacerText, data.pos);
		}
		break;
		case TranslationType::kPerkVerb:
		{
			//setEntryPointStrings(form, data.replacerText, data.pos);
		}
		break;
		case TranslationType::kActivationText:
		{
			//setActivateOverrideStrings(form, data.replacerText);
		}
		break;
		case TranslationType::kGameSetting:
		{
			//setGameSettingString(data.form_id, data.replacerText);
		}
		break;
		case TranslationType::kReference:
		{

		}
		break;

		default:
			std::unreachable();
		}

	}
}