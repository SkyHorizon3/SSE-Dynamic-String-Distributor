#include "../include/Globals.h"
#include "../include/Config.h"

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
			loadOrderPath += L"\\AppData\\Local\\Skyrim VR\\loadorder.txt";
			DEBUG_LOG(g_Logger, "Directory: Skyrim VR", nullptr);
		}
		else
		{
			std::wstring name = std::filesystem::exists("steam_api64.dll") ? L"Skyrim Special Edition" : L"Skyrim Special Edition GOG";
			loadOrderPath += L"\\AppData\\Local\\" + name + L"\\loadorder.txt";

#ifndef NDEBUG
			std::string second = std::filesystem::exists("steam_api64.dll") ? "Skyrim Special Edition" : "Skyrim Special Edition GOG";
#endif
			DEBUG_LOG(g_Logger, "Directory: {}", second);
		}

		std::vector<std::string> loadOrder;

		std::ifstream file(loadOrderPath);
		if (file.is_open())
		{
			std::string line;
			while (std::getline(file, line))
			{
				loadOrder.push_back(line);
			}
			file.close();
		}

		return loadOrder;
	}

	return std::vector<std::string>();
}

//I don't like the way it looks, but it fulfils its purpose.
void Config::EnumerateFolder() //Get all folders in DynamicStringReplacer directory
{
	const std::filesystem::path Directory = L"Data\\SKSE\\Plugins\\DynamicStringDistributor\\";
	bool foundOverwriteFolder = false;  // To check if Overwrite folder is found

	for (const auto& entry : std::filesystem::recursive_directory_iterator(Directory))
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


	for (auto it = m_Folders.begin(); it != m_Folders.end();)
	{
		const auto& folder = *it;

		if (folder.find(".esp") == std::string::npos &&
			folder.find(".esm") == std::string::npos &&
			folder.find(".esl") == std::string::npos &&
			folder != "Overwrite")
		{
			g_Logger->error("Unexpected pluginfolder file extension in {}", folder);
			it = m_Folders.erase(it);
		}
		else
		{
			RE::TESDataHandler* handler = RE::TESDataHandler::GetSingleton();
			auto IsInDataFolder = handler->LookupModByName(folder);

			if (!IsInDataFolder)
			{
				g_Logger->info("Plugin {} not found, skipping all files in the folder", folder);
				it = m_Folders.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	// Sort folders based on load order of plugins
	std::vector<std::string> loadOrder = GetLoadOrder();
	std::sort(m_Folders.begin(), m_Folders.end(), [&loadOrder](const std::string& a, const std::string& b)
		{
			auto itA = std::find(loadOrder.begin(), loadOrder.end(), a);
			auto itB = std::find(loadOrder.begin(), loadOrder.end(), b);
			if (itA == loadOrder.end()) return false;
			if (itB == loadOrder.end()) return true;
			return std::distance(loadOrder.begin(), itA) < std::distance(loadOrder.begin(), itB);
		});

	// If Overwrite folder was found, add it at the end
	if (foundOverwriteFolder)
	{
		m_Folders.emplace_back("Overwrite");
	}

}


void Config::EnumerateFilesInFolders(const std::string folders) //Get all files in each of the folders directory
{
	const std::string folderPath = "Data\\SKSE\\Plugins\\DynamicStringDistributor\\" + folders;
	m_Files.clear();

	for (const auto& entry : std::filesystem::recursive_directory_iterator(folderPath))
	{
		if (entry.is_regular_file() && entry.path().extension() == L".json")
		{
			m_Files.emplace_back(entry.path().filename().string());

			/*
			for (const auto& mfile : m_Files)
			{
				DEBUG_LOG(g_Logger, "m_Files: {} ", mfile);
			}
			*/
		}

	}

	std::sort(m_Files.begin(), m_Files.end());//Alphatbet order, just to make sure.

#ifndef NDEBUG
	static int folderCount = 1;
#endif

	for (const auto& file : m_Files)
	{
		m_FilesInPluginFolder.emplace_back(folderPath + "\\" + file);

		DEBUG_LOG(g_Logger, "Datei{}: {}", folderCount++, (folderPath + "\\" + file));
	}
}

bool Config::isSpecialType(const std::string& type)
{
	return type == "INFO NAM1" || type == "QUST CNAM" || type == "QUST NNAM" || type == "INFO RNAM" || type == "DIAL FULL" || type == "MESG ITXT";
}

bool Config::isConstType(const std::string& subrecord)
{
	return subrecord == "FULL" || subrecord == "TNAM" || subrecord == "SHRT";
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

void Config::HandleSpecialType(const std::string& types, const json& entry, const std::string& stringValue)
{
	std::string original = entry["original"];

	if (types == "INFO NAM1" || types == "MESG ITXT") //There shouldn't be a lot ITXT, so it doesn't matter. Creating new map would be heavier
	{
		g_Logger->info("Original: {}");
		g_INFO_NAM1_ITXT_Map.emplace(original, stringValue);
	}
	else if (types == "QUST NNAM" || types == "QUST CNAM")
	{
		g_QUST_NNAM_CNAM_Map.emplace(original, stringValue);
	}
	else if (types == "INFO RNAM" || types == "DIAL FULL")
	{
		g_DIAL_FULL_RNAM_Map.emplace(original, stringValue);
	}
}

void Config::ParseTranslationFiles()
{
	for (const auto& files : m_FilesInPluginFolder)
	{

		DEBUG_LOG(g_Logger, "Parsing file  {}", files);

		std::ifstream file(files);
		if (!file.is_open())
		{
			g_Logger->error("Couldn't open file {}", files);
			continue;
		}

		json jsonData;
		file >> jsonData;

		//Read data out of json file
		for (const auto& entry : jsonData)
		{
			std::string types = entry["type"];
			std::string stringValue = entry["string"];

			if (!isSpecialType(types))
			{
				std::string editorId = entry["editor_id"];

				// extract first four letters to get the record type
				std::string type = types.substr(0, 4);

				// All the other letters are the subrecord type
				std::string subrecord = GetSubrecordType(types);

				RE::TESForm* form = RE::TESForm::LookupByEditorID(editorId);
				if (!form)
				{
					g_Logger->error("Couldn't find Editor ID {} out of file {}", editorId, files);
					continue;
				}


				if (!isConstType(subrecord))
				{
					g_ConfigurationInformationStruct.emplace_back(form, stringValue, subrecord, type);
					//These information will be used in Hooks
					//to get the right description to right place

				}
				else
				{
					g_ConstConfigurationInformationStruct.emplace_back(form, stringValue, subrecord);
					//These information will be used in Processor
				}

				/*
				DEBUG_LOG(g_Logger, "Form ID: {0:08X}", g_ConfigurationInformationStruct.FormID);
				DEBUG_LOG(g_Logger, "String: {}", g_ConfigurationInformationStruct.ReplacerText.c_str());
				DEBUG_LOG(g_Logger, "RecordType: {}", g_ConfigurationInformationStruct.RecordType.c_str());
				DEBUG_LOG(g_Logger, "SubrecordType: {}", g_ConfigurationInformationStruct.SubrecordType.c_str());
				*/
			}
			else
			{
				HandleSpecialType(types, entry, stringValue);
			}
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