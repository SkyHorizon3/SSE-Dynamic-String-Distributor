#include "Config.h"
#include "MiscHooks.h"
#include "DialogueHooks.h"
#include "QuestHooks.h"
#include "Processor.h"
#include "Utils.h"
#include "MergeMapperPluginAPI.h"

std::vector<std::string> Config::GetLoadOrder()
{
	const std::wstring loadOrderPath = Utils::GetPluginTXTFilePath();
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
				loadOrder.emplace_back(line.substr(1)); //Add lines without the *
			}
		}
		file.close();

	}
	else
	{
		SKSE::log::error("The plugins.txt file could not be opened.");
	}

	m_BaseGamePlugins.erase( //Just in case a BaseGamePlugin is inside the plugins.txt
		std::remove_if(m_BaseGamePlugins.begin(), m_BaseGamePlugins.end(), [&](const std::string& BaseGamePlugin)
		{
			return Utils::SearchCompare(loadOrder, BaseGamePlugin);
		}),
		m_BaseGamePlugins.end()
	);

	std::vector<std::string> activePlugins;

	if (!std::filesystem::exists("Data"sv))
	{
		SKSE::log::error("Data folder not found. Please make sure your Skyrim is even installed.");
		return std::vector<std::string>();
	}

	for (const auto& entry : std::filesystem::directory_iterator("Data"))
	{
		if (entry.is_regular_file() && (entry.path().extension() == L".esp" || entry.path().extension() == L".esm" || entry.path().extension() == L".esl"))
		{
			activePlugins.emplace_back(entry.path().filename().string());
		}
	}

	m_BaseGamePlugins.erase(
		std::remove_if(m_BaseGamePlugins.begin(), m_BaseGamePlugins.end(), [&](const std::string& BaseGamePlugin) //Remove plugins not found in data folder from the BaseGamePlugin list.
		{
			return !Utils::SearchCompare(activePlugins, BaseGamePlugin);
		}),
		m_BaseGamePlugins.end()
	);

	loadOrder.insert(loadOrder.begin(), m_BaseGamePlugins.begin(), m_BaseGamePlugins.end());

	return loadOrder;
}

void Config::EnumerateFolder() //Get all folders in DynamicStringDistributor directory
{
	constexpr auto Directory = L"Data\\SKSE\\Plugins\\DynamicStringDistributor\\";
	bool foundOverwriteFolder = false;  // To check if Overwrite folder is found

	if (!std::filesystem::exists(Directory))
		return;


	for (const auto& entry : std::filesystem::directory_iterator(Directory))
	{
		if (entry.is_directory())
		{
			const auto& folder = entry.path().filename().string();

			// Check if it's the Overwrite folder
			if (folder == "Overwrite")
			{
				foundOverwriteFolder = true;
			}
			else
			{
				m_Folders.emplace_back(folder);
			}
		}
	}

	m_LoadOrder = GetLoadOrder();


	for (auto it = m_Folders.begin(); it != m_Folders.end();)
	{
		const auto& folder = *it;

		if (folder.find(".esp") == std::string::npos &&
			folder.find(".esm") == std::string::npos &&
			folder.find(".esl") == std::string::npos &&
			folder != "Overwrite")
		{
			SKSE::log::error("Unexpected pluginfolder file extension in {}", folder);
			it = m_Folders.erase(it);
		}
		else
		{
			if (!Utils::SearchCompare(m_LoadOrder, folder))
			{
				SKSE::log::info("Plugin {} not found, skipping all files in the folder", folder);
				it = m_Folders.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	/*
	if (EnableDebugLog)
	{
		static int position = 0;
		for (const auto& Plugin : m_LoadOrder)
		{
			SKSE::log::debug("Plugin{}: {}", position++, Plugin);
		}
	}
	*/

	// Sort folders based on load order of plugins
	std::sort(m_Folders.begin(), m_Folders.end(), [this](const std::string& a, const std::string& b)
		{
			auto itA = std::find_if(m_LoadOrder.begin(), m_LoadOrder.end(), [&](const std::string& loadedPlugin)
				{
					return Utils::CaseInsensitiveStringCompare(a, loadedPlugin);
				});

			auto itB = std::find_if(m_LoadOrder.begin(), m_LoadOrder.end(), [&](const std::string& loadedPlugin)
				{
					return Utils::CaseInsensitiveStringCompare(b, loadedPlugin);
				});

			if (itA == m_LoadOrder.end()) return false;
			if (itB == m_LoadOrder.end()) return true;
			return std::distance(m_LoadOrder.begin(), itA) < std::distance(m_LoadOrder.begin(), itB);
		});


	// If Overwrite folder was found, add it at the end
	if (foundOverwriteFolder)
	{
		m_Folders.emplace_back("Overwrite");
	}

	if (m_Folders.empty())
		return;

	if (EnableDebugLog)
	{
		static int position2 = 0;
		for (const auto& Plugin : m_Folders)
		{
			SKSE::log::debug("Folder{}: {}", position2++, Plugin);
		}
	}

	for (const auto& folders : m_Folders)
		EnumerateFilesInFolders(folders);

}


void Config::EnumerateFilesInFolders(const std::string& folders) //Get all files in each of the folders directory
{
	const std::string& folderPath = "Data\\SKSE\\Plugins\\DynamicStringDistributor\\" + folders;
	if (!std::filesystem::exists(folderPath))
		return;

    std::vector<std::string> files;
    std::vector<std::string> translatedFiles;

	for (const auto& entry : std::filesystem::recursive_directory_iterator(folderPath))
	{
        if (entry.is_regular_file() && entry.path().extension() == L".json")
        {
            files.emplace_back(entry.path().filename().string());
        }
        else if (entry.is_regular_file() && entry.path().extension() == L"." + Config::OverwritingLanguage)
        {
            translatedFiles.emplace_back(entry.path().filename().string());
        }

	}

    //Alphabetic order, just to make sure.
    std::sort(files.begin(), files.end());
    std::sort(translatedFiles.begin(), translatedFiles.end());

	static int num = 0;
    for (const auto& file : files)
    {
        m_FilesInPluginFolder.emplace_back(folderPath + "\\" + file);
        SKSE::log::debug("File{}: {}", num++, (folderPath + "\\" + file));
    }
    for (const auto& file : translatedFiles)
    {
        m_FilesInPluginFolder.emplace_back(folderPath + "\\" + file);
        SKSE::log::debug("File{}: {}", num++, (folderPath + "\\" + file));
    }

}

std::string Config::GetSubrecordType(const std::string& types) const
{
	size_t spacePos = types.find(' ');
	if (spacePos != std::string::npos && spacePos + 1 < types.length())
	{
		return types.substr(spacePos + 1);
	}
	return "";
}

std::tuple<RE::FormID, std::string> Config::ExtractFormIDAndPlugin(const std::string& formIDWithPlugin)
{
	size_t separatorPos = formIDWithPlugin.find('|');
	if (separatorPos == std::string::npos)
	{
		// if | is not found
		return std::make_tuple(0, "");
	}

	RE::FormID formID = ConvertToFormID(formIDWithPlugin.substr(0, separatorPos));
	std::string plugin = formIDWithPlugin.substr(separatorPos + 1);

	if (!Utils::SearchCompare(m_LoadOrder, plugin))
	{
		return std::make_tuple(0, "");
	}

	if (g_mergeMapperInterface)
	{
		auto mergeForm = g_mergeMapperInterface->GetNewFormID(plugin.c_str(), formID);
		plugin = mergeForm.first;
		formID = mergeForm.second;
	}

	//SKSE::log::info("FormID: {0:08X}.", formID);


	return std::make_tuple(formID, plugin);
}

RE::FormID Config::ConvertToFormID(std::string input)
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

Config::RecordType Config::GetRecordType_map(const std::string& type)
{
	static const ankerl::unordered_dense::map<std::string_view, RecordType> typeMap = {
		{"ACTI FULL"sv, RecordType::kConst_Translation},
		{"ACTI RNAM"sv, RecordType::kACTI_RNAM},
		{"ALCH FULL"sv, RecordType::kConst_Translation},
		{"AMMO FULL"sv, RecordType::kConst_Translation},
		{"APPA FULL"sv, RecordType::kConst_Translation},
		{"ARMO FULL"sv, RecordType::kConst_Translation},
		{"AVIF FULL"sv, RecordType::kConst_Translation},
		{"BOOK FULL"sv, RecordType::kConst_Translation},
		{"BPTD BPTN"sv, RecordType::kNotVisible},
		{"CELL FULL"sv, RecordType::kConst_Translation},
		{"CLAS FULL"sv, RecordType::kNotVisible},
		{"CLFM FULL"sv, RecordType::kNotVisible},
		{"CONT FULL"sv, RecordType::kConst_Translation},
		{"DIAL FULL"sv, RecordType::kDIAL_FULL},
		{"DOOR FULL"sv, RecordType::kConst_Translation},
		{"ENCH FULL"sv, RecordType::kConst_Translation},
		{"EXPL FULL"sv, RecordType::kConst_Translation},
		{"EYES FULL"sv, RecordType::kNotVisible},
		{"FACT FNAM"sv, RecordType::kNotVisible},
		{"FACT FULL"sv, RecordType::kNotVisible},
		{"FACT MNAM"sv, RecordType::kNotVisible},
		{"FLOR FULL"sv, RecordType::kConst_Translation},
		{"FLOR RNAM"sv, RecordType::kFLOR_RNAM},
		{"FURN FULL"sv, RecordType::kConst_Translation},
		{"GMST DATA"sv, RecordType::kGMST_DATA},
		{"HAZD FULL"sv, RecordType::kConst_Translation},
		{"HDPT FULL"sv, RecordType::kNotVisible},
		{"INFO RNAM"sv, RecordType::kINFO_RNAM},
		{"INGR FULL"sv, RecordType::kConst_Translation},
		{"KEYM FULL"sv, RecordType::kConst_Translation},
		{"LCTN FULL"sv, RecordType::kConst_Translation},
		{"LIGH FULL"sv, RecordType::kConst_Translation},
		{"LSCR DESC"sv, RecordType::kConst_Translation},
		{"MESG FULL"sv, RecordType::kConst_Translation},
		{"MESG ITXT"sv, RecordType::kMESG_ITXT},
		{"MGEF DNAM"sv, RecordType::kConst_Translation},
		{"MGEF FULL"sv, RecordType::kConst_Translation},
		{"MISC FULL"sv, RecordType::kConst_Translation},
		{"NPC_ FULL"sv, RecordType::kNPC__FULL},
		{"NPC_ SHRT"sv, RecordType::kConst_Translation},
		{"PERK EPF2"sv, RecordType::kMESG_ITXT},
		{"PERK EPFD"sv, RecordType::kPERK_EPFD},
		{"PERK FULL"sv, RecordType::kConst_Translation},
		{"PROJ FULL"sv, RecordType::kConst_Translation},
		{"QUST FULL"sv, RecordType::kConst_Translation},
		{"QUST NNAM"sv, RecordType::kQUST_NNAM},
		{"RACE FULL"sv, RecordType::kConst_Translation},
		{"REFR FULL"sv, RecordType::kREFR_FULL},
		{"REGN RDMP"sv, RecordType::kConst_Translation},
		{"SCRL FULL"sv, RecordType::kConst_Translation},
		{"SHOU FULL"sv, RecordType::kConst_Translation},
		{"SLGM FULL"sv, RecordType::kConst_Translation},
		{"SNCT FULL"sv, RecordType::kNotVisible},
		{"SPEL FULL"sv, RecordType::kConst_Translation},
		{"TACT FULL"sv, RecordType::kConst_Translation},
		{"TREE FULL"sv, RecordType::kConst_Translation},
		{"WATR FULL"sv, RecordType::kConst_Translation},
		{"WEAP FULL"sv, RecordType::kConst_Translation},
		{"WOOP FULL"sv, RecordType::kConst_Translation},
		{"WOOP TNAM"sv, RecordType::kConst_Translation},
		{"WRLD FULL"sv, RecordType::kConst_Translation},

		{"AMMO DESC"sv, RecordType::kNormal_Translation}, //DLStrings
		{"ARMO DESC"sv, RecordType::kNormal_Translation},
		{"AVIF DESC"sv, RecordType::kNormal_Translation},
		{"BOOK CNAM"sv, RecordType::kNormal_Translation},
		{"BOOK DESC"sv, RecordType::kNormal_Translation},
		{"COLL DESC"sv, RecordType::kNotVisible},
		{"MESG DESC"sv, RecordType::kNormal_Translation},
		{"PERK DESC"sv, RecordType::kNormal_Translation},
		{"QUST CNAM"sv, RecordType::kQUST_CNAM},
		{"RACE DESC"sv, RecordType::kNormal_Translation},
		{"SCRL DESC"sv, RecordType::kNormal_Translation},
		{"SHOU DESC"sv, RecordType::kNormal_Translation},
		{"SPEL DESC"sv, RecordType::kNormal_Translation},
		{"WEAP DESC"sv, RecordType::kNormal_Translation},

		{"INFO NAM1"sv, RecordType::kINFO_NAM1},//ILStrings
	};

	const auto it = typeMap.find(type);
	return (it != typeMap.end()) ? it->second : RecordType::kUnknown;
}

Config::SubrecordType Config::GetSubrecordType_map(const std::string& type)
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

void Config::ProcessEntry(const std::string& files, const json& entry, const RecordType& recordType)
{
	const std::string& formIDEntry = entry["form_id"].get<std::string>();
	auto [formID, plugin] = ExtractFormIDAndPlugin(formIDEntry);

	if (formID == 0 && plugin.empty())
		return;

	const std::string& stringValue = entry["string"];

	RE::TESForm* form = nullptr;

	switch (recordType)
	{
	case RecordType::kINFO_RNAM:
	case RecordType::kINFO_NAM1:
	case RecordType::kGMST_DATA: //Don't look them up because they aren't loaded
		break;

	default:
	{
		form = RE::TESDataHandler::GetSingleton()->LookupForm(formID, plugin);

		if (form == nullptr)
		{
			SKSE::log::error("Couldn't find FormID {} with record type {} in file: {}", formIDEntry, entry["type"].get<std::string>(), files);
			return;
		}
	}

	}

	switch (recordType)
	{
	case RecordType::kREFR_FULL:
	case RecordType::kDIAL_FULL:
	{
		const size_t key = Utils::combineHash(form->formID, plugin);
		if (recordType == RecordType::kREFR_FULL)
		{
			Hook::g_REFR_FULL_Map.insert_or_assign(key, stringValue);
		}
		else if (recordType == RecordType::kDIAL_FULL)
		{
			Hook::g_DIAL_FULL_Map.insert_or_assign(key, stringValue);
		}
	}
	break;
	case RecordType::kQUST_NNAM:
	case RecordType::kMESG_ITXT:
	case RecordType::kPERK_EPFD:
		Processor::AddToConstTranslationStruct(form, stringValue, GetSubrecordType_map(GetSubrecordType(entry["type"])), entry["index"].get<int>(), "");
		break;
	case RecordType::kConst_Translation:
		Processor::AddToConstTranslationStruct(form, stringValue, GetSubrecordType_map(GetSubrecordType(entry["type"])), 0, "");
		break;
	case RecordType::kNormal_Translation:
	{
		const size_t key = Utils::combineHashSubrecord(form->formID, GetSubrecordType_map(GetSubrecordType(entry["type"])));
		Hook::g_DESC_CNAM_Map.emplace(key, stringValue);
	}
	break;
	case RecordType::kNotVisible:
		SKSE::log::info("File {} contains not visible type: {}", files, entry["type"].get<std::string>());
		break;
	case RecordType::kUnknown:
		SKSE::log::info("File {} contains unknown type: {}", files, entry["type"].get<std::string>());
		break;

	default: break;
	}
}

void Config::ProcessEntryPreload(const json& entry, const RecordType& recordType)
{
	const std::string& formIDEntry = entry["form_id"].get<std::string>();
	auto [formID, plugin] = ExtractFormIDAndPlugin(formIDEntry);

	if (formID == 0 && plugin.empty())
		return;

	const std::string& stringValue = entry["string"];
	const size_t key = Utils::combineHash(formID, plugin);

	auto insertIntoMap = [&](auto& map) {
		map.insert_or_assign(key, stringValue);
		};

	switch (recordType)
	{
	case RecordType::kACTI_RNAM:
	case RecordType::kFLOR_RNAM:
		insertIntoMap(Hook::g_ACTI_Map);
		break;
	case RecordType::kNPC__FULL:
		insertIntoMap(Hook::g_NPC_FULL_Map);
		break;
	case RecordType::kINFO_RNAM:
		insertIntoMap(Hook::g_INFO_RNAM_Map);
		break;
	case RecordType::kINFO_NAM1:
	{
		auto& valueList = Hook::g_INFO_NAM1_Map[std::to_string(formID) + plugin];
		Hook::Value value = { entry["index"].get<std::uint8_t>(), stringValue };

		auto it = std::find_if(valueList.begin(), valueList.end(),
			[&value](const Hook::Value& v) { return v.first == value.first; });

		if (it != valueList.end())
			it->second = value.second;
		else
			valueList.emplace_back(value);

	}
	break;
	case RecordType::kQUST_CNAM:
		Hook::g_QUST_CNAM_Map.insert_or_assign(entry["original"], stringValue);
		break;
	case RecordType::kGMST_DATA:
		Processor::AddToConstTranslationStruct(nullptr, stringValue, GetSubrecordType_map(GetSubrecordType(entry["type"])), 0, entry["editor_id"]);
		break;

	default: break;
	}
}

void Config::ParseTranslationFiles(bool preload)
{
	if (m_FilesInPluginFolder.empty())
		return;

	for (const auto& file : m_FilesInPluginFolder)
	{
		//SKSE::log::debug("Parsing file {}", file);

		try
		{
			std::ifstream jsonfile(file);
			if (!jsonfile.is_open())
			{
				SKSE::log::error("Couldn't open file {}", file);
				continue;
			}

			json jsonData;
			jsonfile >> jsonData;

			//Read data out of json file
			for (const auto& entry : jsonData)
			{
				try
				{
					RecordType recordType = GetRecordType_map(entry["type"]);

					if (preload)
					{
						ProcessEntryPreload(entry, recordType);
					}
					else
					{
						ProcessEntry(file, entry, recordType);
					}
				}
				catch (const std::exception& e)
				{
					SKSE::log::error("Exception while processing entry in file {}: {}", file, e.what());
				}
			}
		}
		catch (const std::exception& e)
		{
			SKSE::log::error("Exception while parsing JSON file {}: {}", file, e.what());
		}
	}
}


void Config::LoadFiles(bool preload)
{
	if (m_Folders.empty())
		return;

	ParseTranslationFiles(preload);
}