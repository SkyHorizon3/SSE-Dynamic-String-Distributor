#pragma once


namespace Utils
{
	size_t combineHash(uint32_t form, const std::string& str);
	size_t combineHashWithIndex(uint32_t form, int value, const std::string& str);
	std::string GetModName(const RE::TESForm* a_form);
}