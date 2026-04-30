#pragma once
#include "Shared.h"

class Manager :
	public REX::Singleton<Manager>
{
public:
	void LoadINI();
	bool isDebugLogEnabled() const noexcept { return m_debugLog; }

	void enumerateLoadOrder();
	void parseTranslationFiles();
	void runConstTranslation();
	void reloadConstTranslation(RE::TESForm* form);

	const char* getTranslation(const RE::FormID formID, const std::uint32_t index, const TranslationType type, std::string_view original = {});

private:
	inline static constexpr const char* DSD_PATH = "Data/SKSE/Plugins/DynamicStringDistributor";
	inline static constexpr const char* EMPTY = " ";

	std::vector<std::string> processFolders();
	std::vector<std::string> processFiles(const std::string_view folder);
	std::tuple<RE::FormID, RE::TESFile*> extractFormIDAndPlugin(const std::string& formIDEntry, const std::string& file);
	TranslationType getTranslationType(std::string_view formType);
	void processEntry(ParseData& entry, const std::string& file);

	void fixedStringChange(RE::BSFixedString& to, std::string_view from);
	void setGameSettingString(const std::optional<std::string>& name, const std::string_view newString);
	void setMessageBoxButtonStrings(RE::TESForm* form, std::string_view newString, const std::optional<std::uint32_t>& index);
	void setPerkMessageBoxButtonStrings(RE::TESForm* form, std::string_view newString, const std::optional<std::uint32_t>& index);
	void setRegionDataStrings(RE::TESForm* form, std::string_view newString);
	void setEntryPointStrings(RE::TESForm* form, std::string_view newString, const std::optional<std::uint32_t>& index);
	void setQuestObjectiveStrings(RE::TESForm* form, std::string_view newString, const std::optional<std::uint32_t>& index);
	void setActivateOverrideStrings(RE::TESForm* form, std::string_view newString);
	void setReferenceStrings(RE::TESForm* form, std::string_view newString);
	void report(const RE::TESForm* const form) const;
	void setConstString(RE::TESForm* form, const ConstTranslationData& entry);

	StringMap<std::pair<RE::TESFile*, std::uint32_t>> m_loadOrder;
	FlatMap<RE::FormID, ConstTranslationData> m_constTranslation;
	FlatMap<RE::FormID, std::string> m_runtimeMap1;
	FlatMap<std::uint64_t, std::string> m_runtimeMap2;
	StringMap<std::string> m_legacyMap;

	// INI
	bool m_debugLog{ false };
	bool m_debugInfo{ false };
};