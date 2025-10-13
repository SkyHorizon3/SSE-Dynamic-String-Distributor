#pragma once

namespace Utils
{
	std::string getModName(const RE::TESForm* form);
	RE::FormID getTrimmedFormID(const RE::TESForm* form);
	std::filesystem::path getPluginTXTFilePath();
	RE::BSFixedString validateString(const RE::BSFixedString& toplace);
	RE::FormID convertToFormID(std::string input);
	std::string getAfterSpace(const std::string& types);
	const RE::TESFile* getFileByFormIDRaw(RE::FormID a_rawFormID, RE::TESFile* a_file);
}
