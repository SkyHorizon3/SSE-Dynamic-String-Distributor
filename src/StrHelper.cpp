#include "StrHelper.h"
#include "Utils.h"

namespace StrHelper
{
	void fixedStringChange(RE::BSFixedString& to, std::string_view from)
	{
		RE::setBSFixedString(to, from.empty() ? EMPTY : from.data());
	}

	void report(const RE::TESForm* const form)
	{
		const auto formtype = RE::FormTypeToString(form->GetFormType());
		const auto modname = Utils::getModName(form);
		SKSE::log::error("Tried to cast {:08X} to an invalid form type - Actual Formtype: {} - Plugin: {}", form->formID, formtype, modname);
	}

	void setGameSettingString(const std::optional<std::string>& name, const std::string_view newString) // GMST DATA
	{
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
			RE::setStringValue(setting, newString.data());
		}
	}

	void setMessageBoxButtonStrings(RE::TESForm* form, std::string_view newString, const std::optional<std::uint32_t>& index) //MESG ITXT
	{
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
				fixedStringChange(button->text, newString);
			}

			pos++;
		}
	}

	void setPerkMessageBoxButtonStrings(RE::TESForm* form, std::string_view newString, const std::optional<std::uint32_t>& index)  //PERK EPF2
	{
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

		for (const auto& entry : perk->perkEntries)
		{
			if (!entry || entry->GetType() != RE::PERK_ENTRY_TYPE::kEntryPoint)
				continue;

			const auto* entryPoint = static_cast<RE::BGSEntryPointPerkEntry*>(entry);
			if (!entryPoint)
				continue;

			const auto data = entryPoint->functionData;
			if (!data || data->GetType() != RE::BGSEntryPointFunctionData::ENTRY_POINT_FUNCTION_DATA::kActivateChoice)
				continue;

			const auto func = static_cast<RE::BGSEntryPointFunctionDataActivateChoice*>(data);
			if (func && func->GetID() == (*index))
			{
				fixedStringChange(func->label, newString);
			}
		}
	}

	void setRegionDataStrings(RE::TESForm* form, std::string_view newString) //REGN RDMP
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

			fixedStringChange(mapData->mapName, newString);
		}
	}

	void setEntryPointStrings(RE::TESForm* form, std::string_view newString, const std::optional<std::uint32_t>& index) //PERK EPFD
	{
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
			const auto& entry = perk->perkEntries[i];
			if (!entry)
				continue;

			if (entry->GetType() != RE::PERK_ENTRY_TYPE::kEntryPoint)
				continue;

			const auto* entryPoint = static_cast<RE::BGSEntryPointPerkEntry*>(entry);
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
					fixedStringChange(func->text, newString);
				}
			}

			++textFuncFound;
		}
	}

	void setQuestObjectiveStrings(RE::TESForm* form, std::string_view newString, const std::optional<std::uint32_t>& index) //QUST NNAM
	{
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
				fixedStringChange(objective->displayText, newString);
			}
		}
	}

	void setActivateOverrideStrings(RE::TESForm* form, std::string_view newString)
	{
		auto& overrideMap = RE::getActivateTextOverrideMap();

		const auto it = overrideMap.find(form->formID);
		if (it != overrideMap.end())
		{
			fixedStringChange(it->second, newString);
		}
		else
		{
			RE::BSFixedString temp;
			fixedStringChange(temp, newString);
			overrideMap.emplace(form->formID, temp);
		}
	}

	void setReferenceStrings(RE::TESForm* form, std::string_view newString)
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
			data->locationName.SetFullName(newString.data());
		}
	}
}