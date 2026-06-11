#pragma once

namespace StrHelper
{
	inline static constexpr const char* EMPTY = " ";

	void fixedStringChange(RE::BSFixedString& to, std::string_view from);
	void report(const RE::TESForm* const form);

	void setGameSettingString(const std::optional<std::string>& name, const std::string_view newString);
	void setMessageBoxButtonStrings(RE::TESForm* form, std::string_view newString, const std::optional<std::uint32_t>& index);
	void setPerkMessageBoxButtonStrings(RE::TESForm* form, std::string_view newString, const std::optional<std::uint32_t>& index);
	void setRegionDataStrings(RE::TESForm* form, std::string_view newString);
	void setEntryPointStrings(RE::TESForm* form, std::string_view newString, const std::optional<std::uint32_t>& index);
	void setQuestObjectiveStrings(RE::TESForm* form, std::string_view newString, const std::optional<std::uint32_t>& index);
	void setActivateOverrideStrings(RE::TESForm* form, std::string_view newString);
	void setReferenceStrings(RE::TESForm* form, std::string_view newString);
}