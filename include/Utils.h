#pragma once

namespace Utils
{
	const RE::TESFile* getFormBasePlugin(const RE::TESForm* const form);
	std::string getModName(const RE::TESForm* const form);
	RE::FormID getTrimmedFormID(const RE::TESForm* const form);
	RE::FormID getRuntimeFormID(const RE::TESFile* file, const RE::FormID raw);
	RE::BSFixedString validateString(const RE::BSFixedString& toplace);
	RE::FormID convertToFormID(std::string input);
}
