#pragma once

class Config : public REX::Singleton<Config>
{
public:
	std::vector<std::string> getFolders();
	void parseTranslationFiles();
	void onDataLoad();

	static inline bool enableDebugLog = false;
	static inline bool enableDebugInfo = false;

	std::vector<std::string_view> getLoadOrder();
private:
	inline static constexpr std::string_view DSD_PATH = "Data/SKSE/Plugins/DynamicStringDistributor";

	std::vector<std::string> enumerateFilesInFolders(const std::string& folders);
	std::tuple<RE::FormID, std::string> extractFormIDAndPlugin(const std::string& formIDWithPlugin);
	void processEntry(ParseData& entry, const std::string& file);

	std::vector<std::string_view> m_loadOrder;

	std::vector<ParseData> m_onDataLoad;
};