#pragma once

enum class TranslationType : std::uint8_t
{
	kFullName, // XXXX FULL
	kShortName, // XXXX SHRT
	kWordOfPower, // WOOP TNAM
	kGameSetting, // GMST DATA
	kRegion, // REGN RDMP
	kMagicDescription, // MGEF DNAM
	kLoadScreenDescription, // LSCR DESC 
	kQuestObjective, // QUST NNAM
	kButtonText1, // MESG ITXT
	kButtonText2, // PERK EPF2
	kActivationText, // ACTI RNAM, FLOR RNAM
	kReference, // REFR FULL
	kPerkVerb, // PERK EPFD
	kRuntime1,
	kRuntime2,
	kRuntimeLegacy, // QUST CNAM

	kUnknown
};

struct ParseData
{
	std::string form_id{};
	std::string type{};
	std::string string{};
	std::string original{};
	std::optional<std::uint32_t> index{};
	std::optional<std::string> editor_id{};
};

struct ConstTranslationData
{
	RE::FormID runtimeFormID{ 0 };
	std::string replacerText{};
	TranslationType translationType{ TranslationType::kUnknown };
	std::optional<std::uint32_t> index{};
	std::optional<std::string> editor_id{};
};

class Manager : public REX::Singleton<Manager>
{
public:
	void LoadINI();
	bool isDebugLogEnabled() const noexcept { return m_debugLog; }

	void runConstTranslation();

	void enumerateLoadOrder();
	void parseTranslationFiles();
private:
	inline static constexpr const char* DSD_PATH = "Data/SKSE/Plugins/DynamicStringDistributor";

	std::vector<std::string> processFolders();
	std::vector<std::string> processFiles(const std::string_view folder);
	std::tuple<RE::FormID, std::string> extractFormIDAndPlugin(const std::string& formIDWithPlugin);
	TranslationType getTranslationType(std::string_view formType);
	void processEntry(ParseData& entry, const std::string& file);

	void setGameSettingString(const std::string& name, const std::string& newString);
	void setMessageBoxButtonStrings(RE::TESForm* form, const RE::BSFixedString& newString, const std::uint32_t index);
	void setRegionDataStrings(RE::TESForm* form, const RE::BSFixedString& newString);
	void setEntryPointStrings(RE::TESForm* form, const RE::BSFixedString& newString, const std::uint32_t index);
	void setQuestObjectiveStrings(RE::TESForm* form, const RE::BSFixedString& newString, const std::uint32_t index);
	void setActivateOverrideStrings(RE::TESForm* form, const RE::BSFixedString& newString);
	void report(const RE::TESForm* const form);

	StringMap<std::pair<RE::TESFile*, std::uint32_t>> m_loadOrder{};
	std::vector<ConstTranslationData> m_constTranslation{};
	FlatMap<std::uint64_t, std::string> m_runtimeMap1{};
	FlatMap<RE::FormID, std::string> m_runtimeMap2{};
	StringMap<std::string> m_legacyMap{};

	// INI
	bool m_debugLog{ false };
	bool m_debugInfo{ false };
};