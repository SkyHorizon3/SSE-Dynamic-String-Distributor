#pragma once


namespace Utils
{
	size_t combineHash(uint32_t formID, const std::string& str);
	size_t combineHashWithIndex(uint32_t formID, int value, const std::string& str);
	std::string GetModName(const RE::TESForm* form);
	bool CaseInsensitiveStringCompare(const std::string& a, const std::string& b);
	bool SearchCompare(const std::vector<std::string>& list, const std::string& str);
}