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

	const char* getTranslation(const RE::FormID formID, const std::uint32_t index, const TranslationType type, RE::TESObjectREFR* ref, std::string_view originalText);

private:
	inline static constexpr const char* DSD_PATH = "Data/SKSE/Plugins/DynamicStringDistributor";

	void enumerateLoadOrder();
	std::vector<std::string> processFolders();
	std::vector<std::string> processFiles(const std::string_view folder);
	std::tuple<RE::FormID, RE::TESFile*> extractFormIDAndPlugin(const std::string& formIDEntry, const std::string& file);
	TranslationType getTranslationType(std::string_view formType);
	void processEntry(ParseData& entry, const std::string& file);
	bool constTranslationContains(const RE::FormID formID, const TranslationType type, const std::optional<std::uint32_t> index) const;
	void setConstString(RE::TESForm* form, const ConstData& entry);

	StringMap<std::pair<RE::TESFile*, std::uint32_t>> m_loadOrder;
	MultiMap<RE::FormID, ConstData> m_constTranslation;
	FlatMap<RE::FormID, RuntimeData> m_runtimeMap1;
	FlatMap<std::uint64_t, RuntimeData> m_runtimeMap2;
	StringMap<RuntimeData> m_legacyMap;

	// INI
	bool m_debugLog{ false };
	bool m_debugInfo{ false };
};