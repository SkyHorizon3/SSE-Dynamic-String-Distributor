#pragma once
#include "Config.h"

namespace Utils
{
	size_t combineHash(const std::uint32_t formID, const std::string& str);
	size_t combineHashWithIndex(const std::uint32_t formID, int value, const std::string& str);
	size_t combineHashSubrecord(const std::uint32_t formID, Config::SubrecordType subrecord);
	std::string GetModName(const RE::TESForm* form);
	RE::FormID GetTrimmedFormID(const RE::TESForm* form);
	bool char_equals(char a, char b);
	bool CaseInsensitiveStringCompare(std::string_view a, std::string_view b);
	bool SearchCompare(const std::vector<std::string>& list, const std::string& str);
	std::wstring GetPluginTXTFilePath();
}