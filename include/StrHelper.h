#pragma once
#include "Shared.h"

namespace StrHelper
{
	inline static constexpr const char* EMPTY = " ";

	void fixedStringChange(RE::BSFixedString& to, std::string_view from);
	void report(const RE::TESForm* const form);

	void setGameSettingString(const ConstData& entry);
	void setMessageBoxButtonStrings(RE::TESForm* form, const ConstData& entry);
	void setPerkMessageBoxButtonStrings(RE::TESForm* form, const ConstData& entry);
	void setRegionDataStrings(RE::TESForm* form, const ConstData& entry);
	void setEntryPointStrings(RE::TESForm* form, const ConstData& entry);
	void setQuestObjectiveStrings(RE::TESForm* form, const ConstData& entry);
	void setActivateOverrideStrings(RE::TESForm* form, ConstData& entry, RE::TESObjectREFR* ref);
	void setReferenceStrings(RE::TESForm* form, const ConstData& entry);
}