#include "Config.h"
#include "MiscHooks.h"
#include "DialogueHooks.h"
#include "QuestHooks.h"
#include "Processor.h"
#include "Utils.h"

//I don't like the way it looks, but it fulfils its purpose...

// Case-insensitive comparison for strings
bool Config::caseInsensitiveStringCompare(const std::string& a, const std::string& b)
{
	return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin(), [](char a, char b)
		{
			return std::tolower(static_cast<unsigned char>(a)) == std::tolower(static_cast<unsigned char>(b));
		});
}

std::vector<std::string> Config::GetLoadOrder()
{
	// Get user directory path
	wchar_t userDir[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, userDir)))
	{
		// Append the rest of the path
		std::wstring loadOrderPath = userDir;

		if (REL::Module::IsVR())
		{
			loadOrderPath += L"\\AppData\\Local\\Skyrim VR\\plugins.txt";

#ifndef NDEBUG
			SKSE::log::debug("Directory: Skyrim VR");
#endif
		}
		else
		{
			std::wstring name = std::filesystem::exists("steam_api64.dll") ? L"Skyrim Special Edition" : L"Skyrim Special Edition GOG";
			loadOrderPath += L"\\AppData\\Local\\" + name + L"\\plugins.txt";

#ifndef NDEBUG
			std::string second = std::filesystem::exists("steam_api64.dll") ? "Skyrim Special Edition" : "Skyrim Special Edition GOG";

			SKSE::log::debug("Directory: {}", second);
#endif
		}

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
					return std::find_if(loadOrder.begin(), loadOrder.end(), [&](const std::string& Plugin)
						{
							return caseInsensitiveStringCompare(BaseGamePlugin, Plugin); //Lambda inside lambda, lovely
						}) != loadOrder.end();
				}),
			m_BaseGamePlugins.end()
		);

		std::vector<std::string> AllPlugins;
		for (const auto& entry : std::filesystem::directory_iterator("Data"))
		{
			if (entry.is_regular_file() && (entry.path().extension() == L".esp" || entry.path().extension() == L".esm" || entry.path().extension() == L".esl"))
			{
				AllPlugins.emplace_back(entry.path().filename().string());
			}
		}

		m_BaseGamePlugins.erase(
			std::remove_if(m_BaseGamePlugins.begin(), m_BaseGamePlugins.end(), [&](const std::string& BaseGamePlugin) //Remove plugins not found in data folder from the BaseGamePlugin list.
				{
					return std::find_if(AllPlugins.begin(), AllPlugins.end(), [&](const std::string& Plugin)
						{
							return caseInsensitiveStringCompare(BaseGamePlugin, Plugin);
						}) == AllPlugins.end();
				}),
			m_BaseGamePlugins.end()
		);

		loadOrder.insert(loadOrder.begin(), m_BaseGamePlugins.begin(), m_BaseGamePlugins.end());


		//Most of this is actually a big and cruel workaround since I can't find a better way through CLib to get all loaded plugins. It seems random, what's counted as loaded in TESDataHandler

		return loadOrder;
	}

	return std::vector<std::string>();
}

void Config::EnumerateFolder() //Get all folders in DynamicStringDistributor directory
{
	const std::filesystem::path Directory = L"Data\\SKSE\\Plugins\\DynamicStringDistributor\\";
	bool foundOverwriteFolder = false;  // To check if Overwrite folder is found

	if (!std::filesystem::exists(Directory))
	{
		return;
	}

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

	std::vector<std::string> loadOrder = GetLoadOrder();

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
			if (std::find_if(loadOrder.begin(), loadOrder.end(), [&](const std::string& loadedPlugin) //Please tell me if there is an easier way of doing this. My eyes and brain are hurting xD
				{
					return caseInsensitiveStringCompare(folder, loadedPlugin);
				}) == loadOrder.end())
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

#ifndef NDEBUG
	static int position = 1;
	for (const auto& Plugin : loadOrder)
	{
		SKSE::log::debug("Plugin{}: {}", position++, Plugin);
	}
#endif

	// Sort folders based on load order of plugins
	std::sort(m_Folders.begin(), m_Folders.end(), [this, &loadOrder](const std::string& a, const std::string& b)
		{
			auto itA = std::find_if(loadOrder.begin(), loadOrder.end(), [&](const std::string& loadedPlugin)
				{
					return caseInsensitiveStringCompare(a, loadedPlugin);
				});

			auto itB = std::find_if(loadOrder.begin(), loadOrder.end(), [&](const std::string& loadedPlugin)
				{
					return caseInsensitiveStringCompare(b, loadedPlugin);
				});

			if (itA == loadOrder.end()) return false;
			if (itB == loadOrder.end()) return true;
			return std::distance(loadOrder.begin(), itA) < std::distance(loadOrder.begin(), itB);
		});


	// If Overwrite folder was found, add it at the end
	if (foundOverwriteFolder)
	{
		m_Folders.emplace_back("Overwrite");
	}

#ifndef NDEBUG
	static int position2 = 1;
	for (const auto& Plugin : m_Folders)
	{
		SKSE::log::debug("Folder{}: {}", position2++, Plugin);
	}
#endif

}


void Config::EnumerateFilesInFolders(const std::string folders) //Get all files in each of the folders directory
{
	const std::string folderPath = "Data\\SKSE\\Plugins\\DynamicStringDistributor\\" + folders;
	m_Files.clear();
	m_Files.shrink_to_fit(); //I'm not sure if that does anything useful, but it doesn't really matter :D

	for (const auto& entry : std::filesystem::recursive_directory_iterator(folderPath))
	{
		if (entry.is_regular_file() && entry.path().extension() == L".json")
		{
			m_Files.emplace_back(entry.path().filename().string());
		}

	}

	std::sort(m_Files.begin(), m_Files.end());//Alphatbet order, just to make sure.

#ifndef NDEBUG
	static int folderCount = 1;
#endif

	for (const auto& file : m_Files)
	{
		m_FilesInPluginFolder.emplace_back(folderPath + "\\" + file);

#ifndef NDEBUG
		SKSE::log::debug("File{}: {}", folderCount++, (folderPath + "\\" + file));
#endif

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

std::string Config::ExtractContentInBrackets(const std::string& input)
{
	size_t startPos = input.find('[');
	size_t endPos = input.find(']');

	if (startPos != std::string::npos && endPos != std::string::npos && startPos < endPos)
	{
		return input.substr(startPos + 1, endPos - startPos - 1);
	}

	return "";
}

RE::FormID Config::ConvertToFormID(std::string input)
{
	if (input.substr(0, 2) == "FE")
	{
		if (input.length() >= 5)
		{
			input[3] = 'x';
			input.erase(0, 2);
		}
	}
	else
	{
		if (input.length() >= 3)
		{
			input[1] = 'x';
		}
	}

	return std::stoul(input, nullptr, 16);
}

Config::RecordTypes Config::GetRecordType_map(const std::string& type)
{
	static const std::unordered_map<std::string, RecordTypes> typeMap = {
		{"ACTI FULL", RecordTypes::kConst_Translation},
		{"ACTI RNAM", RecordTypes::kACTI_RNAM},
		{"ALCH FULL", RecordTypes::kConst_Translation},
		{"AMMO FULL", RecordTypes::kConst_Translation},
		{"APPA FULL", RecordTypes::kConst_Translation},
		{"ARMO FULL", RecordTypes::kConst_Translation},
		{"AVIF FULL", RecordTypes::kConst_Translation},
		{"BOOK FULL", RecordTypes::kConst_Translation},
		{"BPTD BPTN", RecordTypes::kNotVisible},
		{"CELL FULL", RecordTypes::kConst_Translation},
		{"CLAS FULL", RecordTypes::kNotVisible},
		{"CLFM FULL", RecordTypes::kNotVisible},
		{"CONT FULL", RecordTypes::kConst_Translation},
		{"DIAL FULL", RecordTypes::kDIAL_FULL},
		{"DOOR FULL", RecordTypes::kConst_Translation},
		{"ENCH FULL", RecordTypes::kConst_Translation},
		{"EXPL FULL", RecordTypes::kConst_Translation},
		{"EYES FULL", RecordTypes::kNotVisible},
		{"FACT FNAM", RecordTypes::kNotVisible}, //Seems like this is not visible, but I'm not sure
		{"FACT FULL", RecordTypes::kNotVisible},
		{"FACT MNAM", RecordTypes::kNotVisible},
		{"FLOR FULL", RecordTypes::kConst_Translation},
		{"FLOR RNAM", RecordTypes::kFLOR_RNAM},
		{"FURN FULL", RecordTypes::kConst_Translation},
		{"GMST DATA", RecordTypes::kConst_Translation},
		{"HAZD FULL", RecordTypes::kConst_Translation},
		{"HDPT FULL", RecordTypes::kNotVisible},
		{"INFO RNAM", RecordTypes::kINFO_RNAM},
		{"INGR FULL", RecordTypes::kConst_Translation},
		{"KEYM FULL", RecordTypes::kConst_Translation},
		{"LCTN FULL", RecordTypes::kConst_Translation},
		{"LIGH FULL", RecordTypes::kConst_Translation},
		{"LSCR DESC", RecordTypes::kNormal_Translation},
		{"MESG FULL", RecordTypes::kConst_Translation},
		{"MESG ITXT", RecordTypes::kMESG_ITXT},
		{"MGEF DNAM", RecordTypes::kNormal_Translation},
		{"MGEF FULL", RecordTypes::kConst_Translation},
		{"MISC FULL", RecordTypes::kConst_Translation},
		{"NPC_ FULL", RecordTypes::kConst_Translation},
		{"NPC_ SHRT", RecordTypes::kConst_Translation},
		{"PERK EPF2", RecordTypes::kMESG_ITXT}, //BGSEntryPoint.h
		{"PERK EPFD", RecordTypes::kACTI_RNAM},
		{"PERK FULL", RecordTypes::kConst_Translation},
		{"PROJ FULL", RecordTypes::kConst_Translation},
		{"QUST FULL", RecordTypes::kConst_Translation},
		{"QUST NNAM", RecordTypes::kQUST_NNAM},
		{"RACE FULL", RecordTypes::kConst_Translation},
		{"REFR FULL", RecordTypes::kREFR_FULL},
		{"REGN RDMP", RecordTypes::kREGN_RDMP},
		{"SCRL FULL", RecordTypes::kConst_Translation},
		{"SHOU FULL", RecordTypes::kConst_Translation},
		{"SLGM FULL", RecordTypes::kConst_Translation},
		{"SNCT FULL", RecordTypes::kNotVisible},
		{"SPEL FULL", RecordTypes::kConst_Translation},
		{"TACT FULL", RecordTypes::kConst_Translation},
		{"TREE FULL", RecordTypes::kConst_Translation},
		{"WATR FULL", RecordTypes::kConst_Translation},
		{"WEAP FULL", RecordTypes::kConst_Translation},
		{"WOOP FULL", RecordTypes::kConst_Translation},
		{"WOOP TNAM", RecordTypes::kConst_Translation},
		{"WRLD FULL", RecordTypes::kConst_Translation},

		{"AMMO DESC", RecordTypes::kNormal_Translation}, //DLStrings
		{"ARMO DESC", RecordTypes::kNormal_Translation},
		{"AVIF DESC", RecordTypes::kNormal_Translation},
		{"BOOK CNAM", RecordTypes::kNormal_Translation},
		{"BOOK DESC", RecordTypes::kNormal_Translation},
		{"COLL DESC", RecordTypes::kNotVisible},
		{"MESG DESC", RecordTypes::kNormal_Translation},
		{"PERK DESC", RecordTypes::kNormal_Translation},
		{"QUST CNAM", RecordTypes::kQUST_CNAM},
		{"RACE DESC", RecordTypes::kNormal_Translation},
		{"SCRL DESC", RecordTypes::kNormal_Translation},
		{"SHOU DESC", RecordTypes::kNormal_Translation},
		{"SPEL DESC", RecordTypes::kNormal_Translation},
		{"WEAP DESC", RecordTypes::kNormal_Translation},

		{"INFO NAM1", RecordTypes::kINFO_NAM1},//ILStrings
	};

	auto it = typeMap.find(type);
	return (it != typeMap.end()) ? it->second : RecordTypes::kUnknown;
}

Config::SubrecordTypes Config::GetSubrecordType_map(const std::string& type)
{
	static const std::unordered_map<std::string, SubrecordTypes> typeMap = {
		{"FULL", SubrecordTypes::kFULL},
		{"SHRT", SubrecordTypes::kSHRT},
		{"DATA", SubrecordTypes::kDATA},
		{"TNAM", SubrecordTypes::kTNAM},

		{"DESC", SubrecordTypes::kDESC},
		{"CNAM", SubrecordTypes::kCNAM},
		{"DNAM", SubrecordTypes::kDNAM},
	};

	auto it = typeMap.find(type);
	return (it != typeMap.end()) ? it->second : SubrecordTypes::kUnknown;
}

void Config::ParseTranslationFiles()
{
	for (const auto& files : m_FilesInPluginFolder)
	{
#ifndef NDEBUG
		SKSE::log::debug("Parsing file {}", files);
#endif

		try
		{
			std::ifstream file(files);
			if (!file.is_open())
			{
				SKSE::log::error("Couldn't open file {}", files);
				continue;
			}

			json jsonData;
			file >> jsonData;

			//Read data out of json file
			for (const auto& entry : jsonData)
			{
				try
				{
					const std::string& types = entry["type"];
					const std::string& stringValue = entry["string"];

					RecordTypes RecordType = GetRecordType_map(types);

					switch (RecordType)
					{
					case RecordTypes::kACTI_RNAM:
					case RecordTypes::kFLOR_RNAM:
					case RecordTypes::kREFR_FULL: //For REFR FULL we could also use the other way, but most of REFR don't have a EditorID
					case RecordTypes::kREGN_RDMP:
					{
						Hook::g_FLOR_RNAM_RDMP_Map.insert_or_assign(entry["original"], stringValue); //update if key already exists. This simulates the esp load order
					}
					break;
					case RecordTypes::kDIAL_FULL:
					case RecordTypes::kINFO_RNAM:
					{
						Hook::g_DIAL_FULL_RNAM_Map.insert_or_assign(entry["original"], stringValue);
					}
					break;
					case RecordTypes::kQUST_CNAM:
					{
						RE::TESForm* form = RE::TESForm::LookupByEditorID(entry["editor_id"]);
						if (!form)
						{
							SKSE::log::error("Couldn't find Editor ID {} out of file {}", entry["editor_id"].get<std::string>(), files);
							continue;
						}


						//Extract index -> umrechnen -> combine hash -> key


						//Hook::g_QUST_CNAM_Map.insert_or_assign(key, stringValue);
					}
					break;
					case RecordTypes::kQUST_NNAM:
					{
						RE::TESForm* form = RE::TESForm::LookupByEditorID(entry["editor_id"]);
						if (!form)
						{
							SKSE::log::error("Couldn't find Editor ID {} out of file {}", entry["editor_id"].get<std::string>(), files);
							continue;
						}

						size_t key = Utils::combineHash(form->formID, entry["index"].get<int>());

						Hook::g_QUST_NNAM_Map.insert_or_assign(key, stringValue);
					}
					break;
					case RecordTypes::kMESG_ITXT:
					{
						Hook::g_MESG_ITXT_Map.insert_or_assign(entry["original"], stringValue);
					}
					break;
					case RecordTypes::kINFO_NAM1:
					{
						const std::string& stringFormID = ExtractContentInBrackets(entry["editor_id"]);
						RE::FormID form = ConvertToFormID(stringFormID);
						size_t key = Utils::combineHash(form, entry["index"].get<int>());

						Hook::g_INFO_NAM1_Map.insert_or_assign(key, stringValue);
					}
					break;
					case RecordTypes::kConst_Translation:
					{
						const std::string& editorId = entry["editor_id"];

						const std::string& subrecord = GetSubrecordType(types);

						RE::TESForm* form = RE::TESForm::LookupByEditorID(editorId);
						if (!form && subrecord != "DATA")
						{
							SKSE::log::error("Couldn't find Editor ID {} out of file {}", editorId, files);
							continue;
						}

						SubrecordTypes ConstSubrecordType = GetSubrecordType_map(subrecord);


						Processor::AddToConstTranslationStruct(form, stringValue, ConstSubrecordType, editorId);
					}
					break;
					case RecordTypes::kNormal_Translation:
					{
						const std::string& editorId = entry["editor_id"];

						RE::TESForm* form = RE::TESForm::LookupByEditorID(editorId);
						if (!form)
						{
							SKSE::log::error("Couldn't find Editor ID {} out of file {}", editorId, files);
							continue;
						}

						SubrecordTypes SubrecordType = GetSubrecordType_map(GetSubrecordType(types));

						Hook::g_ConfigurationInformationStruct.emplace_back(form, stringValue, SubrecordType);
					}
					break;
					case RecordTypes::kNotVisible:
					{
						SKSE::log::info("File {} contains not visible type: {}", files, types);
					}
					break;
					case RecordTypes::kUnknown:
					{
						SKSE::log::info("File {} contains unkown type: {}", files, types);
					}
					break;



					default: break;
					}
				}
				catch (const std::exception& e)
				{
					SKSE::log::error("Exception while processing entry in file {}: {}", files, e.what());
				}
			}
		}
		catch (const std::exception& e)
		{
			SKSE::log::error("Exception while parsing JSON file {}: {}", files, e.what());
		}
	}
}


void Config::LoadFiles()
{
	for (const auto& folders : m_Folders)
	{

		EnumerateFilesInFolders(folders);

	}

	ParseTranslationFiles();

}