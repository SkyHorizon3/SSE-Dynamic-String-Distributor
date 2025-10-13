#pragma once
#include "Manager.h"

class Config : public REX::Singleton<Config>
{
public:
	void enumerateFolder();
	void parseTranslationFiles();
	void onDataLoad();

	static inline bool enableDebugLog = false;

private:

	std::vector<std::string> getLoadOrder();
	std::vector<std::string> enumerateFilesInFolders(const std::string& folders);
	std::tuple<RE::FormID, std::string> extractFormIDAndPlugin(const std::string& formIDWithPlugin);
	void processEntry(ParseData& entry, const std::string& file);

	std::vector<std::string> m_folders;
	std::vector<std::string> m_loadOrder;
	std::vector<std::string> m_filesInPluginFolder;

	std::vector<ParseData> m_onDataLoad;
};