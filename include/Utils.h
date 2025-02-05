#pragma once
#include "Config.h"

namespace Utils
{
	std::string tolower(std::string_view a_str);
	std::string getModName(const RE::TESForm* form);
	RE::FormID getTrimmedFormID(const RE::TESForm* form);
	std::filesystem::path getPluginTXTFilePath();
	RE::BSFixedString validateString(const RE::BSFixedString& toplace);
	RE::FormID convertToFormID(std::string input);
	std::string getAfterSpace(const std::string& types);
	const RE::TESFile* getFileByFormIDRaw(RE::FormID a_rawFormID);

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

	template <class T>
	inline T to_num(const std::string& a_str, bool a_hex = false)
	{
		const int base = a_hex ? 16 : 10;

		if constexpr (std::is_same_v<T, double>) {
			return static_cast<T>(std::stod(a_str, nullptr));
		}
		else if constexpr (std::is_same_v<T, float>) {
			return static_cast<T>(std::stof(a_str, nullptr));
		}
		else if constexpr (std::is_same_v<T, std::int64_t>) {
			return static_cast<T>(std::stol(a_str, nullptr, base));
		}
		else if constexpr (std::is_same_v<T, std::uint64_t>) {
			return static_cast<T>(std::stoull(a_str, nullptr, base));
		}
		else if constexpr (std::is_signed_v<T>) {
			return static_cast<T>(std::stoi(a_str, nullptr, base));
		}
		else {
			return static_cast<T>(std::stoul(a_str, nullptr, base));
		}
	}

	inline bool iequals(std::string_view a_str1, std::string_view a_str2)
	{
		return std::ranges::equal(a_str1, a_str2, [](unsigned char ch1, unsigned char ch2) {
			return std::toupper(ch1) == std::toupper(ch2);
			});
	}
}

inline namespace literals
{
	constexpr std::uint64_t operator""_h(const char* a_str, std::size_t a_len)
	{
		return Utils::fnv1a_64(std::string_view{ a_str, a_len });
	}
}