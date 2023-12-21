#pragma once

class Config
{
public:
	static Config* GetSingleton()
	{
		static Config menu;
		return &menu;
	}
	std::vector<std::string> GetLoadOrder();
	void EnumerateFolder();
	void EnumerateFilesInFolders(std::string folders);
	bool isSpecialType(const std::string& type);
	void ParseTranslationFiles();
	void LoadFiles();

private:
	Config() = default;
	~Config() = default;
	Config(const Config&) = delete;
	Config(Config&&) = delete;
	Config& operator=(const Config&) = delete;
	Config& operator=(Config&&) = delete;

	std::vector<std::string> m_Folders;
	std::vector<std::string> m_Files;
	std::vector<std::string> m_FilesInPluginFolder;

};