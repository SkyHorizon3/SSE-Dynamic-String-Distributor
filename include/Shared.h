#pragma once

enum class TranslationType :
	std::uint8_t
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
	std::string form_id;
	std::string type;
	std::string string;
	std::optional<std::string> original;
	std::optional<std::uint32_t> index;
	std::optional<std::string> editor_id;
};

struct ConstTranslationData
{
	RE::FormID runtimeFormID{ 0 };
	TranslationType translationType{ TranslationType::kUnknown };
	std::string replacerText;
	std::optional<std::uint32_t> index;
	std::optional<std::string> editor_id;
};