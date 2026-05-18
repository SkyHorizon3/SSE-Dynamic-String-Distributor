#pragma once

namespace Utils
{
	const RE::TESFile* getFormBasePlugin(const RE::TESForm* const form);
	std::string getModName(const RE::TESForm* const form);
	RE::FormID getTrimmedFormID(const RE::TESForm* const form);
	RE::FormID getRuntimeFormID(const RE::TESFile* file, const RE::FormID raw);
	RE::FormID convertToFormID(std::string input);

	inline constexpr std::uint64_t packU32(const std::uint32_t a, const std::uint32_t b) noexcept
	{
		return (static_cast<std::uint64_t>(a) << 32) | b;
	}

	inline constexpr std::uint32_t highU32(const std::uint64_t v) noexcept
	{
		return static_cast<std::uint32_t>(v >> 32);
	}

	inline constexpr std::uint32_t lowU32(const std::uint64_t v) noexcept
	{
		return static_cast<uint32_t>(v);
	}
}
