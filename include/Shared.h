#pragma once

enum class TranslationType :
	std::uint8_t
{
	kFullName, // XXXX FULL
	kShortName, // NPC_ SHRT
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
	kRuntime1, // XXXX DESC, DIAL FULL
	kRuntime2, // INFO RNAM, BOOK CNAM
	kRuntimeIndex, // INFO NAM1
	kRuntimeLegacy, // QUST CNAM

	kUnknown
};

struct ParseData
{
	std::string form_id;
	std::string type;
	std::string string;
	std::optional<std::string> original;
	std::optional<std::uint32_t> index;
	std::optional<std::string> editor_id;
	std::optional<std::vector<std::string>> conditions;
};

struct ConditionData
{
	bool buildConditions(const std::optional<std::vector<std::string>>& conditionList);
	const char* decideText(RE::TESObjectREFR* ref, std::string_view currentText, std::string_view replacerText);

	std::string originalText;
	std::shared_ptr<RE::TESCondition> conditions;
	bool lastConditionState{ false };
};

struct ConstData
{
	explicit ConstData(const TranslationType type, const ParseData& entry);

	TranslationType translationType;
	std::string replacerText;
	std::optional<std::uint32_t> index;
	std::optional<std::string> editor_id;
	ConditionData data;
};

struct RuntimeData
{
	explicit RuntimeData(const ParseData& entry);

	std::string replacerText;
	ConditionData data;
};