#include "Utils.h"

namespace Utils
{
	// Get the file that defines the record through the formID index
	// The problem is that the internal file array is wrong in an edge case with REFR forms:
	// It returns wrong file for REFR records defined in esm files when another esm file adds the persistent flag in an override
	// It's not bug, just bullshit caused by the plugin system since it obviously can't know if 
	// a record is persistent if the flag is added later in another plugin
	const RE::TESFile* getFormBasePlugin(const RE::TESForm* const form)
	{
		const auto handler = RE::TESDataHandler::GetSingleton();

		if (!form || !handler)
			return nullptr;

		const auto formID = form->formID;
		const auto expected = (formID & 0xFF000000) == 0xFE000000 ?
			handler->LookupLoadedLightModByIndex(static_cast<std::uint16_t>((0x00FFF000 & formID) >> 12)) :
			handler->LookupLoadedModByIndex(static_cast<std::uint8_t>((0xFF000000 & formID) >> 24));

		return expected;
	}

	std::string getModName(const RE::TESForm* const form)
	{
		if (!form)
			return {};

		const auto file = getFormBasePlugin(form);
		if (!file)
			return {};

		std::string_view filename = file ? file->GetFilename() : "";
		return string::tolower(filename.data());
	}

	RE::FormID getTrimmedFormID(const RE::TESForm* const form)
	{
		if (!form)
			return 0;

		const auto file = getFormBasePlugin(form);
		if (!file)
			return 0;

		RE::FormID formID = form->GetFormID() & 0xFFFFFF; // remove file index -> 0x00XXXXXX
		if (file->IsLight())
		{
			formID &= 0xFFF; // remove ESL index -> 0x00000XXX
		}

		return formID;
	}

	RE::BSFixedString validateString(const RE::BSFixedString& toplace)
	{
		return toplace.empty() ? RE::BSFixedString(" ") : toplace;
	}

	RE::TESActorBase* getFullNameNPC(RE::TESNPC* npc)
	{
		if (!npc)
			return nullptr;

		//const auto& refName = npc->fullName;
		RE::TESActorBase* current = npc;

		std::uint16_t guard = 0;

		while (current && current->baseTemplateForm && guard++ < 20)
		{
			auto base = skyrim_cast<RE::TESActorBase*>(current->baseTemplateForm);

			if (base && !base->IsPlayer())
				base->SetFullName("HEYYY!");

			if (!base)
			{
				SKSE::log::info("Cast failed for {:08X}!", current->baseTemplateForm->formID);
				break;
			}

			RE::TESFullName;

			RE::TESLeveledList;

			/*if (base->fullName != refName)
				break;*/

			current = base;
		}

		if (guard >= 20)
		{
			SKSE::log::critical("NPC {:08X} uses more than 20 base templates. Report to DSD modpage!", npc->formID);
		}

		return current;
	}

	RE::FormID convertToFormID(std::string input)
	{
		const bool has_x = (input.find('x') != std::string::npos) || (input.find('X') != std::string::npos);
		if (!has_x) // If it does not contain 'x', process it.
		{
			if (input.length() == 8)
			{
				if (input.compare(0, 2, "FE") == 0)
				{
					input = "0x" + input.substr(5);
				}
				else
				{
					input = "0x" + input.substr(2);
				}
			}
			else
			{
				input.insert(0, "0x");
			}
		}

		return std::stoul(input, nullptr, 16);
	}
}