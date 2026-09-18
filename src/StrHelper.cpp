#include "StrHelper.h"
#include "Utils.h"

namespace StrHelper
{
	void fixedStringChange(RE::BSFixedString& to, const char* str, bool update)
	{
		if (!str)
			return;

		const bool isEmpty = str[0] == '\0';
		RE::setBSFixedString(to, isEmpty ? EMPTY : str);

		if (update)
		{
			if (const auto pl = RE::PlayerCharacter::GetSingleton())
			{
				pl->UpdateCrosshairs();
			}
		}
	}

	void report(const RE::TESForm* const form)
	{
		const auto formtype = RE::FormTypeToString(form->GetFormType());
		const auto modname = Utils::getModName(form);
		SKSE::log::error("Tried to cast {:08X} to an invalid form type - Actual Formtype: {} - Plugin: {}", form->formID, formtype, modname);
	}

	void setGameSettingString(const ConstData& entry) // GMST DATA
	{
		const auto& name = entry.editor_id;
		const auto& newString = entry.replacerText;
		if (!name.has_value())
		{
			SKSE::log::error("Couldn't inject string \"{}\"! The editorID is missing!", newString);
			return;
		}

		const auto& settingStr = name.value();
		const auto setting = RE::GameSettingCollection::GetSingleton()->GetSetting(settingStr.c_str());
		if (!setting)
		{
			SKSE::log::debug("Failed to set GameSetting string for {}.", settingStr);
			return;
		}

		if (setting->GetType() == RE::Setting::Type::kString)
		{
			RE::setStringValue(setting, newString.c_str());
		}
	}

	void setMessageBoxButtonStrings(RE::TESForm* form, const ConstData& entry) //MESG ITXT
	{
		const auto& index = entry.index;
		const auto& newString = entry.replacerText;
		if (!index.has_value())
		{
			SKSE::log::error("Couldn't inject string \"{}\"! The index is missing!", newString);
			return;
		}

		const auto message = form->As<RE::BGSMessage>();
		if (!message)
		{
			report(form);
			return;
		}

		std::uint32_t pos = 0;
		for (const auto& button : message->menuButtons)
		{
			if (button && pos == (*index))
			{
				fixedStringChange(button->text, newString.c_str(), false);
			}

			pos++;
		}
	}

	void setPerkMessageBoxButtonStrings(RE::TESForm* form, const ConstData& entry)  //PERK EPF2
	{
		const auto& index = entry.index;
		const auto& newString = entry.replacerText;
		if (!index.has_value())
		{
			SKSE::log::error("Couldn't inject string \"{}\"! The index is missing!", newString);
			return;
		}

		const auto perk = form->As<RE::BGSPerk>();
		if (!perk)
		{
			report(form);
			return;
		}

		for (const auto& perkEntry : perk->perkEntries)
		{
			if (!perkEntry || perkEntry->GetType() != RE::PERK_ENTRY_TYPE::kEntryPoint)
				continue;

			const auto* entryPoint = static_cast<RE::BGSEntryPointPerkEntry*>(perkEntry);
			if (!entryPoint)
				continue;

			const auto data = entryPoint->functionData;
			if (!data || data->GetType() != RE::BGSEntryPointFunctionData::ENTRY_POINT_FUNCTION_DATA::kActivateChoice)
				continue;

			const auto func = static_cast<RE::BGSEntryPointFunctionDataActivateChoice*>(data);
			if (func && func->GetID() == (*index))
			{
				fixedStringChange(func->label, newString.c_str(), false);
			}
		}
	}

	void setRegionDataStrings(RE::TESForm* form, const ConstData& entry) //REGN RDMP
	{
		const auto regionData = form->As<RE::TESRegion>();
		const auto regionDataList = regionData ? regionData->dataList : nullptr;

		if (!regionDataList)
		{
			report(form);
			return;
		}

		for (const auto& region : regionDataList->regionDataList)
		{
			if (!region || region->GetType() != RE::TESRegionData::Type::kMap)
				continue;

			auto* mapData = static_cast<RE::TESRegionDataMap*>(region);
			if (!mapData)
				continue;

			fixedStringChange(mapData->mapName, entry.replacerText.c_str(), false);
		}
	}

	void setEntryPointStrings(RE::TESForm* form, const ConstData& entry) //PERK EPFD
	{
		const auto& index = entry.index;
		const auto& newString = entry.replacerText;
		if (!index.has_value())
		{
			SKSE::log::error("Couldn't inject string \"{}\"! The index is missing!", newString);
			return;
		}

		const auto perk = form->As<RE::BGSPerk>();
		if (!perk)
		{
			report(form);
			return;
		}

		const std::uint32_t entryCount = perk->perkEntries.size();
		std::uint32_t textFuncFound = 0;

		for (std::int32_t i = entryCount - 1; i >= 0; --i)
		{
			const auto& perkEntry = perk->perkEntries[i];
			if (!perkEntry)
				continue;

			if (perkEntry->GetType() != RE::PERK_ENTRY_TYPE::kEntryPoint)
				continue;

			const auto* entryPoint = static_cast<RE::BGSEntryPointPerkEntry*>(perkEntry);
			if (!entryPoint)
				continue;

			const auto data = entryPoint->functionData;
			if (!data || data->GetType() != RE::BGSEntryPointFunctionData::ENTRY_POINT_FUNCTION_DATA::kText)
				continue;

			if (textFuncFound == (*index))
			{
				const auto func = static_cast<RE::BGSEntryPointFunctionDataText*>(data);
				if (func)
				{
					fixedStringChange(func->text, newString.c_str(), false);
				}
			}

			++textFuncFound;
		}
	}

	void setQuestObjectiveStrings(RE::TESForm* form, const ConstData& entry) //QUST NNAM
	{
		const auto& index = entry.index;
		const auto& newString = entry.replacerText;
		if (!index.has_value())
		{
			SKSE::log::error("Couldn't inject string: \"{}\"! The index is missing!", newString);
			return;
		}

		const auto quest = form->As<RE::TESQuest>();
		if (!quest)
		{
			report(form);
			return;
		}

		for (const auto& objective : quest->objectives)
		{
			if (objective && objective->index == (*index))
			{
				fixedStringChange(objective->displayText, newString.c_str(), false);
			}
		}
	}

	void setActivateOverrideStrings(RE::TESForm* form, ConstData& entry, RE::TESObjectREFR* ref)
	{
		auto& overrideMap = RE::getActivateTextOverrideMap();
		const auto& newString = entry.replacerText;

		const auto it = overrideMap.find(form->formID);
		if (it != overrideMap.end())
		{
			auto [str, update] = entry.decideText(ref, it->second, newString);
			fixedStringChange(it->second, str, update);
		}
		else
		{
			RE::BSFixedString temp;
			auto [str, update] = entry.decideText(ref, temp, newString);
			if (str)
			{
				fixedStringChange(temp, str, update);
				overrideMap.emplace(form->formID, temp);
			}
		}
	}

	void setReferenceStrings(RE::TESForm* form, const ConstData& entry)
	{
		const auto ref = form->As<RE::TESObjectREFR>();
		if (!ref)
		{
			report(form);
			return;
		}

		const auto marker = ref ? ref->extraList.GetByType<RE::ExtraMapMarker>() : nullptr;
		const auto data = marker ? marker->mapData : nullptr;
		if (data)
		{
			fixedStringChange(data->locationName.fullName, entry.replacerText.c_str(), false);
		}
	}
}