#pragma once

namespace Utils
{
	std::string getModName(const RE::TESForm* const form);
	RE::FormID getTrimmedFormID(const RE::TESForm* const form);
	RE::BSFixedString validateString(const RE::BSFixedString& toplace);
	RE::FormID convertToFormID(std::string input);
	std::string getAfterSpace(const std::string& types);
	RE::TESActorBase* getFullNameNPC(RE::TESNPC* npc);
}
