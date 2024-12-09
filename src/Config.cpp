#include "Config.h"
#include "MiscHooks.h"
#include "DialogueHooks.h"
#include "QuestHooks.h"
#include "Processor.h"
#include "Utils.h"
#include "MergeMapperPluginAPI.h"

std::vector<std::string> Config::getLoadOrder()
{
	const std::wstring loadOrderPath = Utils::getPluginTXTFilePath();
	if (!std::filesystem::exists(loadOrderPath) || loadOrderPath.empty())
		return std::vector<std::string>();

	std::vector<std::string> loadOrder;
	std::ifstream file(loadOrderPath);
	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
		{
			if (line[0] == '*') //Check if line starts with *
			{
				loadOrder.emplace_back(Utils::tolower(line.substr(1))); //Add lines without the *
			}
		}
		file.close();

	}
	else
	{
		SKSE::log::error("The plugins.txt file could not be opened.");
	}

	std::vector<std::string> baseGamePlugins = {
		"skyrim.esm",
		"update.esm",
		"dawnguard.esm",
		"hearthfires.esm",
		"dragonborn.esm",
		"skyrimvr.esm", //For Skyrim VR
		"ccasvsse001-almsivi.esm",
		"ccbgssse001-fish.esm",
		"ccbgssse002-exoticarrows.esl",
		"ccbgssse003-zombies.esl",
		"ccbgssse004-ruinsedge.esl",
		"ccbgssse005-goldbrand.esl",
		"ccbgssse006-stendarshammer.esl",
		"ccbgssse007-chrysamere.esl",
		"ccbgssse010-petdwarvenarmoredmudcrab.esl",
		"ccbgssse011-hrsarmrelvn.esl",
		"ccbgssse012-hrsarmrstl.esl",
		"ccbgssse014-spellpack01.esl",
		"ccbgssse019-staffofsheogorath.esl",
		"ccbgssse020-graycowl.esl",
		"ccbgssse021-lordsmail.esl",
		"ccmtysse001-knightsofthenine.esl",
		"ccqdrsse001-survivalmode.esl",
		"cctwbsse001-puzzledungeon.esm",
		"cceejsse001-hstead.esm",
		"ccqdrsse002-firewood.esl",
		"ccbgssse018-shadowrend.esl",
		"ccbgssse035-petnhound.esl",
		"ccfsvsse001-backpacks.esl",
		"cceejsse002-tower.esl",
		"ccedhsse001-norjewel.esl",
		"ccvsvsse002-pets.esl",
		"ccbgssse037-curios.esl",
		"ccbgssse034-mntuni.esl",
		"ccbgssse045-hasedoki.esl",
		"ccbgssse008-wraithguard.esl",
		"ccbgssse036-petbwolf.esl",
		"ccffbsse001-imperialdragon.esl",
		"ccmtysse002-ve.esl",
		"ccbgssse043-crosselv.esl",
		"ccvsvsse001-winter.esl",
		"cceejsse003-hollow.esl",
		"ccbgssse016-umbra.esm",
		"ccbgssse031-advcyrus.esm",
		"ccbgssse038-bowofshadows.esl",
		"ccbgssse040-advobgobs.esl",
		"ccbgssse050-ba_daedric.esl",
		"ccbgssse052-ba_iron.esl",
		"ccbgssse054-ba_orcish.esl",
		"ccbgssse058-ba_steel.esl",
		"ccbgssse059-ba_dragonplate.esl",
		"ccbgssse061-ba_dwarven.esl",
		"ccpewsse002-armsofchaos.esl",
		"ccbgssse041-netchleather.esl",
		"ccedhsse002-splkntset.esl",
		"ccbgssse064-ba_elven.esl",
		"ccbgssse063-ba_ebony.esl",
		"ccbgssse062-ba_dwarvenmail.esl",
		"ccbgssse060-ba_dragonscale.esl",
		"ccbgssse056-ba_silver.esl",
		"ccbgssse055-ba_orcishscaled.esl",
		"ccbgssse053-ba_leather.esl",
		"ccbgssse051-ba_daedricmail.esl",
		"ccbgssse057-ba_stalhrim.esl",
		"ccbgssse066-staves.esl",
		"ccbgssse067-daedinv.esm",
		"ccbgssse068-bloodfall.esl",
		"ccbgssse069-contest.esl",
		"ccvsvsse003-necroarts.esl",
		"ccvsvsse004-beafarmer.esl",
		"ccbgssse025-advdsgs.esm",
		"ccffbsse002-crossbowpack.esl",
		"ccbgssse013-dawnfang.esl",
		"ccrmssse001-necrohouse.esl",
		"ccedhsse003-redguard.esl",
		"cceejsse004-hall.esl",
		"cceejsse005-cave.esm",
		"cckrtsse001_altar.esl",
		"cccbhsse001-gaunt.esl",
		"ccafdsse001-dwesanctuary.esm",
		"_resourcepack.esl"
	};

	baseGamePlugins.erase(
		std::remove_if(
		baseGamePlugins.begin(),
		baseGamePlugins.end(),
		[&](const std::string& plugin) {
			return std::find(loadOrder.begin(), loadOrder.end(), plugin) != loadOrder.end();
		}
	),
		baseGamePlugins.end()
	);


	std::vector<std::string> activePlugins;
	for (const auto& entry : std::filesystem::directory_iterator("Data"))
	{
		const auto ext = entry.path().extension();
		if (entry.is_regular_file() && (ext == L".esp" || ext == L".esm" || ext == L".esl"))
		{
			activePlugins.emplace_back(Utils::tolower(entry.path().filename().string()));
		}
	}

	baseGamePlugins.erase(
		std::remove_if(
		baseGamePlugins.begin(),
		baseGamePlugins.end(),
		[&](const std::string& plugin) {
			return std::find(activePlugins.begin(), activePlugins.end(), plugin) == activePlugins.end();
		}
	),
		baseGamePlugins.end()
	);

	loadOrder.insert(loadOrder.begin(), baseGamePlugins.begin(), baseGamePlugins.end());

	return loadOrder;
}

void Config::enumerateFolder() //Get all folders in DynamicStringDistributor directory
{
	constexpr auto directory = L"Data\\SKSE\\Plugins\\DynamicStringDistributor\\";
	bool foundOverwriteFolder = false;  // To check if Overwrite folder is found

	if (!std::filesystem::exists(directory))
		return;


	for (const auto& entry : std::filesystem::directory_iterator(directory))
	{
		if (entry.is_directory())
		{
			const auto& folder = Utils::tolower(entry.path().filename().string());

			// Check if it's the Overwrite folder
			if (folder == "overwrite")
			{
				foundOverwriteFolder = true;
			}
			else
			{
				m_folders.emplace_back(folder);
			}
		}
	}

	m_loadOrder = getLoadOrder();

	for (auto it = m_folders.begin(); it != m_folders.end();)
	{
		const auto& folder = *it;

		if (folder.find(".esp") == std::string::npos &&
			folder.find(".esm") == std::string::npos &&
			folder.find(".esl") == std::string::npos &&
			folder != "overwrite")
		{
			SKSE::log::error("Unexpected pluginfolder file extension in {}", folder);
			it = m_folders.erase(it);
		}
		else
		{
			if (std::find(m_loadOrder.begin(), m_loadOrder.end(), folder) == m_loadOrder.end())
			{
				SKSE::log::info("Plugin {} not found, skipping all files in the folder", folder);
				it = m_folders.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	// Sort folders based on load order of plugins
	std::sort(m_folders.begin(), m_folders.end(), [this](const std::string& a, const std::string& b)
		{
			auto itA = std::find(m_loadOrder.begin(), m_loadOrder.end(), a);
			auto itB = std::find(m_loadOrder.begin(), m_loadOrder.end(), b);

			if (itA == m_loadOrder.end()) return false;
			if (itB == m_loadOrder.end()) return true;
			return std::distance(m_loadOrder.begin(), itA) < std::distance(m_loadOrder.begin(), itB);
		});


	// If Overwrite folder was found, add it at the end
	if (foundOverwriteFolder)
	{
		m_folders.emplace_back("overwrite");
	}

	if (m_folders.empty())
		return;

	if (enableDebugLog)
	{
		static int position2 = 0;
		for (const auto& Plugin : m_folders)
		{
			SKSE::log::debug("Folder{}: {}", position2++, Plugin);
		}
	}

	for (const auto& folder : m_folders)
	{
		const auto files = enumerateFilesInFolders(folder);
		m_filesInPluginFolder.insert(m_filesInPluginFolder.end(), files.begin(), files.end());
	}

	if (enableDebugLog)
	{
		static int num = 0;
		for (const auto& file : m_filesInPluginFolder)
		{
			SKSE::log::debug("File{}: {}", num++, file);
		}
	}
}

std::vector<std::string> Config::enumerateFilesInFolders(const std::string& folders) //Get all files in each of the folders directory
{
	std::vector<std::string> files;
	const std::string folderPath = "Data\\SKSE\\Plugins\\DynamicStringDistributor\\" + folders;
	if (!std::filesystem::exists(folderPath))
		return files;

	for (const auto& entry : std::filesystem::recursive_directory_iterator(folderPath))
	{
		if (entry.is_regular_file())
		{
			const auto ext = entry.path().extension();
			if (ext == L".json")
			{
				files.emplace_back(entry.path().string());
			}
		}
	}

	std::sort(files.begin(), files.end(),
		[](const std::string& a, const std::string& b) {
			auto aExt = std::filesystem::path(a).extension().string();
			auto bExt = std::filesystem::path(b).extension().string();

			// JSON files to the front, other files to the end
			if (aExt == ".json" && bExt != ".json")
				return true;
			if (aExt != ".json" && bExt == ".json")
				return false;

			return std::filesystem::path(a).filename().string() < std::filesystem::path(b).filename().string();
		});

	return files;
}

std::string Config::getSubrecordType(const std::string& types) const
{
	size_t spacePos = types.find(' ');
	if (spacePos != std::string::npos && spacePos + 1 < types.length())
	{
		return types.substr(spacePos + 1);
	}
	return "";
}

std::tuple<RE::FormID, std::string> Config::extractFormIDAndPlugin(const std::string& formIDWithPlugin)
{
	const size_t separatorPos = formIDWithPlugin.find('|');
	if (separatorPos == std::string::npos)
	{
		// if | is not found
		return std::make_tuple(0, "");
	}

	RE::FormID formID = convertToFormID(formIDWithPlugin.substr(0, separatorPos));
	std::string plugin = formIDWithPlugin.substr(separatorPos + 1);

	if (g_mergeMapperInterface)
	{
		auto mergeForm = g_mergeMapperInterface->GetNewFormID(plugin.c_str(), formID);
		plugin = mergeForm.first;
		formID = mergeForm.second;
	}

	plugin = Utils::tolower(plugin);

	if (std::find(m_loadOrder.begin(), m_loadOrder.end(), plugin) == m_loadOrder.end())
	{
		return std::make_tuple(0, "");
	}

	return std::make_tuple(formID, plugin);
}

RE::FormID Config::convertToFormID(std::string input)
{
	if (input.find('x') == std::string::npos) // If it does not contain 'x', process it.
	{
		if (input.length() == 8)
		{
			if (input.compare(0, 2, "FE") == 0)
			{
				input = "0x" + input.substr(5);
			}
			else
			{
				input = "0x" + input.substr(2);
			}
		}
		else
		{
			input.insert(0, "0x");
		}
	}

	//SKSE::log::info("FormID: {}", input);

	return std::stoul(input, nullptr, 16);
}

Config::SubrecordType Config::getSubrecordType_map(std::string_view type)
{
	static const ankerl::unordered_dense::map<std::string_view, SubrecordType> typeMap = {
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

void Config::onDataLoad()
{
	for (const auto& entry : m_onDataLoad)
	{
		auto [formID, plugin] = extractFormIDAndPlugin(entry.form_id);
		if (formID == 0 && plugin.empty())
			continue;

		//SKSE::log::info("FormID: {} - switch: {} - type: {} - string: {}", entry.form_id, entry.original, entry.type, entry.string);

		const auto form = RE::TESDataHandler::GetSingleton()->LookupForm(formID, plugin);
		if (form == nullptr)
		{
			SKSE::log::error("Couldn't find FormID {} with record type {}", entry.form_id, entry.type);
			return;
		}

		switch (Utils::const_hash(entry.original))
		{
		case "const"_h:
		{
			Processor::AddToConstTranslationStruct(form, entry.string, getSubrecordType_map(getSubrecordType(entry.type)), 0, "");
		}
		break;
		case "index"_h:
		{
			Processor::AddToConstTranslationStruct(form, entry.string, getSubrecordType_map(getSubrecordType(entry.type)), entry.index.value(), "");
		}
		break;
		case "refr"_h:
		{
			const size_t key = Utils::combineHash(form->formID, plugin);
			Hook::g_REFR_FULL_Map.insert_or_assign(key, entry.string);
		}
		break;
		case "desc"_h:
		{
			const size_t key = Utils::combineHashSubrecord(form->formID, getSubrecordType_map(getSubrecordType(entry.type)));
			Hook::g_DESC_CNAM_Map.insert_or_assign(key, entry.string);
		}
		break;


		}
	}
}

void Config::processEntry(Data& entry, const std::string& file)
{
	auto [formID, plugin] = extractFormIDAndPlugin(entry.form_id);
	if (formID == 0 && plugin.empty())
		return;

	const size_t key = Utils::combineHash(formID, plugin);

	auto insertIntoMap = [&](auto& map) {
		map.insert_or_assign(key, entry.string);
		};

	switch (Utils::const_hash(entry.type))
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
	case "LSCR DESC"_h:
	case "MESG FULL"_h:
	case "MGEF DNAM"_h:
	case "MGEF FULL"_h:
	case "MISC FULL"_h:
	case "NPC_ SHRT"_h:
	case "PERK FULL"_h:
	case "PROJ FULL"_h:
	case "QUST FULL"_h:
	case "RACE FULL"_h:
	case "REGN RDMP"_h:
	case "SCRL FULL"_h:
	case "SHOU FULL"_h:
	case "SLGM FULL"_h:
	case "SPEL FULL"_h:
	case "TACT FULL"_h:
	case "TREE FULL"_h:
	case "WATR FULL"_h:
	case "WEAP FULL"_h:
	case "WOOP FULL"_h:
	case "WOOP TNAM"_h:
	case "WRLD FULL"_h:
	{
		entry.original = "const";
		m_onDataLoad.emplace_back(entry);
	}
	break;
	case "ACTI RNAM"_h:
	case "FLOR RNAM"_h:
	{
		insertIntoMap(Hook::g_ACTI_Map);
	}
	break;
	case "DIAL FULL"_h:
	{
		insertIntoMap(Hook::g_DIAL_FULL_Map);
	}
	break;
	case "GMST DATA"_h:
	{
		Processor::AddToConstTranslationStruct(nullptr, entry.string, getSubrecordType_map(getSubrecordType(entry.type)), 0, entry.editor_id.value());
	}
	break;
	case "INFO RNAM"_h:
	{
		insertIntoMap(Hook::g_INFO_RNAM_Map);
	}
	break;
	case "MESG ITXT"_h:
	case "PERK EPF2"_h:
	case "QUST NNAM"_h:
	case "PERK EPFD"_h:
	{
		entry.original = "index";
		m_onDataLoad.emplace_back(entry);
	}
	break;
	case "NPC_ FULL"_h:
	{
		insertIntoMap(Hook::g_NPC_FULL_Map);
	}
	break;
	case "REFR FULL"_h:
	{
		entry.original = "refr";
		m_onDataLoad.emplace_back(entry);
	}
	break;
	case "QUST CNAM"_h:
	{
		Hook::g_QUST_CNAM_Map.insert_or_assign(entry.original, entry.string);
	}
	break;
	case "INFO NAM1"_h:
	{
		auto& valueList = Hook::g_INFO_NAM1_Map[std::to_string(formID) + plugin];
		Hook::Value value = { entry.index.value(), entry.string };

		auto it = std::find_if(valueList.begin(), valueList.end(),
			[&value](const Hook::Value& v) { return v.first == value.first; });

		if (it != valueList.end())
			it->second = value.second;
		else
			valueList.emplace_back(value);
	}
	break;
	case "AMMO DESC"_h:
	case "ARMO DESC"_h:
	case "AVIF DESC"_h:
	case "BOOK CNAM"_h:
	case "BOOK DESC"_h:
	case "MESG DESC"_h:
	case "PERK DESC"_h:
	case "RACE DESC"_h:
	case "SCRL DESC"_h:
	case "SHOU DESC"_h:
	case "SPEL DESC"_h:
	case "WEAP DESC"_h:
	{
		entry.original = "desc";
		m_onDataLoad.emplace_back(entry);
	}
	break;

	default:
		SKSE::log::error("File {} contains unknown or not visible type: {}", file, entry.type);
		break;

	}
}

void Config::parseTranslationFiles()
{
	if (m_folders.empty() || m_filesInPluginFolder.empty())
		return;

	for (const auto& file : m_filesInPluginFolder)
	{
		std::string buffer{};
		std::vector<Data> jsonData{};

		auto err = glz::read_file_json < glz::opts{ .partial_read_nested = true } > (jsonData, file, buffer);
		if (err)
		{
			const std::string descriptive_error = glz::format_error(err, buffer);
			SKSE::log::error("Error parsing JSON: {}", descriptive_error);
			continue;
		}

		for (auto& entry : jsonData)
		{
			processEntry(entry, file);
		}
	}
}