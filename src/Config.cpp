#include "../include/Globals.h"
#include "../include/Config.h"

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

			DEBUG_LOG(g_Logger, "Directory: Skyrim VR", nullptr);
		}
		else
		{
			std::wstring name = std::filesystem::exists("steam_api64.dll") ? L"Skyrim Special Edition" : L"Skyrim Special Edition GOG";
			loadOrderPath += L"\\AppData\\Local\\" + name + L"\\plugins.txt";

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
				if (line[0] == '*') //Check if line starts with *
				{
					loadOrder.emplace_back(line.substr(1)); //Add lines without the *
				}
			}
			file.close();

		}
		else
		{
			g_Logger->info("The plugins.txt file could not be opened.");
		}

		std::vector<std::string> AllPlugins;
		for (const auto& entry : std::filesystem::directory_iterator("Data"))
		{
			if (entry.is_regular_file() &&
				(entry.path().extension() == L".esp" || entry.path().extension() == L".esm" || entry.path().extension() == L".esl"))
			{
				AllPlugins.emplace_back(entry.path().filename().string());
			}
		}


		m_BaseGamePlugins.erase(
			std::remove_if(m_BaseGamePlugins.begin(), m_BaseGamePlugins.end(), [&](const std::string& plugin)
				{
					return std::find_if(AllPlugins.begin(), AllPlugins.end(), [&](const std::string& folderPlugin)
						{
							return caseInsensitiveStringCompare(plugin, folderPlugin);
						}) == AllPlugins.end();
				}),
			m_BaseGamePlugins.end()
		);

		loadOrder.insert(loadOrder.begin(), m_BaseGamePlugins.begin(), m_BaseGamePlugins.end());


		//Most of this is actually a big workaround since I can't find a better way through CLib.

		return loadOrder;
	}

	return std::vector<std::string>();
}

//I don't like the way it looks, but it fulfils its purpose.
void Config::EnumerateFolder() //Get all folders in DynamicStringDistributor directory
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

	std::vector<std::string> loadOrder = GetLoadOrder();

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
			if (std::find_if(loadOrder.begin(), loadOrder.end(), [&](const std::string& loadedPlugin) //Please tell me if there is an easier way of doing this. My eyes and brain are hurting xD
				{
					return caseInsensitiveStringCompare(folder, loadedPlugin);
				}) == loadOrder.end())
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

#ifndef NDEBUG
	static int position = 1;
	for (const auto& Plugin : loadOrder)
	{
		g_Logger->info("Plugin{}: {}", position++, Plugin);
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
		g_Logger->info("Folder{}: {}", position2++, Plugin);
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

	if (types == "INFO NAM1" || types == "MESG ITXT")
	{
		g_INFO_NAM1_ITXT_Map.insert_or_assign(original, stringValue); //update if key already exists. This simulates the esp load order
	}
	else if (types == "QUST NNAM" || types == "QUST CNAM")
	{
		g_QUST_NNAM_CNAM_Map.insert_or_assign(original, stringValue);
	}
	else if (types == "INFO RNAM" || types == "DIAL FULL")
	{
		g_DIAL_FULL_RNAM_Map.insert_or_assign(original, stringValue);
	}
}


void Config::ParseTranslationFiles()
{
	for (const auto& files : m_FilesInPluginFolder)
	{
		DEBUG_LOG(g_Logger, "Parsing file  {}", files);

		try
		{
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
				try
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
					}
					else
					{
						HandleSpecialType(types, entry, stringValue);
					}
				}
				catch (const std::exception& e)
				{
					g_Logger->error("Exception while processing entry in file {}: {}", files, e.what());
				}
			}
		}
		catch (const std::exception& e)
		{
			g_Logger->error("Exception while parsing JSON file {}: {}", files, e.what());
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