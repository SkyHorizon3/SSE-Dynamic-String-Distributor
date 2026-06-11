#pragma once
#include "Shared.h"

class Manager :
	public REX::Singleton<Manager>
{
public:
	void LoadINI();
	bool isDebugLogEnabled() const noexcept { return m_debugLog; }

	void parseTranslationFiles();
	void runConstTranslation();
	void reloadConstTranslation(RE::TESForm* form);

	const char* getTranslation(const RE::FormID formID, const std::uint32_t index, const TranslationType type, std::string_view original = {});

private:
	inline static constexpr const char* DSD_PATH = "Data/SKSE/Plugins/DynamicStringDistributor";

	void enumerateLoadOrder();
	std::vector<std::string> processFolders();
	std::vector<std::string> processFiles(const std::string_view folder);
	std::tuple<RE::FormID, RE::TESFile*> extractFormIDAndPlugin(const std::string& formIDEntry, const std::string& file);
	TranslationType getTranslationType(std::string_view formType);
	void processEntry(ParseData& entry, const std::string& file);
	void setConstString(RE::TESForm* form, const ConstTranslationData& entry);

	StringMap<std::pair<RE::TESFile*, std::uint32_t>> m_loadOrder;
	MultiMap<RE::FormID, ConstTranslationData> m_constTranslation;
	FlatMap<RE::FormID, std::string> m_runtimeMap1;
	FlatMap<std::uint64_t, std::string> m_runtimeMap2;
	StringMap<std::string> m_legacyMap;

	// INI
	bool m_debugLog{ false };
	bool m_debugInfo{ false };
};