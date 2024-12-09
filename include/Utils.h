#pragma once
#include "Config.h"

namespace Utils
{
	size_t combineHash(const std::uint32_t formID, const std::string& str);
	size_t combineHashWithIndex(const std::uint32_t formID, int value, const std::string& str);
	size_t combineHashSubrecord(const std::uint32_t formID, Config::SubrecordType subrecord);
	std::string tolower(std::string_view a_str);
	std::string getModName(const RE::TESForm* form);
	RE::FormID getTrimmedFormID(const RE::TESForm* form);
	std::wstring getPluginTXTFilePath();


	// https://github.com/powerof3/CLibUtil/blob/master/include/CLIBUtil/hash.hpp
	template <class T>
	inline constexpr std::uint64_t fnv1a_64(const T& a_toHash)
	{
		constexpr std::uint64_t FNVoffsetBasis = 0xcbf29ce484222325;
		constexpr std::uint64_t FNVprime = 0x00000100000001B3;

		std::uint64_t hash = FNVoffsetBasis;

		for (const auto& ch : a_toHash) {
			hash ^= ch;
			hash *= FNVprime;
		}

		return hash;
	}

	inline constexpr std::uint64_t const_hash(std::string_view a_str)
	{
		return fnv1a_64(a_str);
	}

}

inline namespace literals
{
	constexpr std::uint64_t operator""_h(const char* a_str, std::size_t a_len)
	{
		return Utils::fnv1a_64(std::string_view{ a_str, a_len });
	}
}