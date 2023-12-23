#pragma once

using json = nlohmann::json;

class Config
{
public:
	static Config* GetSingleton()
	{
		static Config menu;
		return &menu;
	}

	void EnumerateFolder();
	void LoadFiles();

private:
	std::vector<std::string> GetLoadOrder();
	void EnumerateFilesInFolders(std::string folders);
	bool isSpecialType(const std::string& type);
	bool isConstType(const std::string& type);
	std::string GetSubrecordType(const std::string& types) const;
	void HandleSpecialType(const std::string& types, const json& entry, const std::string& stringValue);
	void ParseTranslationFiles();

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