#include "Processor.h"
#include "Config.h"

void Processor::RunConstTranslation()
{
	for (const auto& Information : m_ConstConfigurationInformationStruct)
	{

		switch (Information.SubrecordType)
		{
		case Config::SubrecordType::kFULL: //DIAL FULL, REFR FULL aren't working like this.
		{
			SetConstStrings<RE::TESFullName>(Information.Form, Information.ReplacerText, &RE::TESFullName::fullName);
		}
		break;
		case Config::SubrecordType::kDNAM:
		{
			SetConstStrings <RE::EffectSetting>(Information.Form, Information.ReplacerText, &RE::EffectSetting::magicItemDescription);
		}
		break;
		case Config::SubrecordType::kSHRT:
		{
			SetConstStrings<RE::TESNPC>(Information.Form, Information.ReplacerText, &RE::TESNPC::shortName);
		}
		break;
		case Config::SubrecordType::kTNAM:
		{
			SetConstStrings<RE::TESWordOfPower>(Information.Form, Information.ReplacerText, &RE::TESWordOfPower::translation);
		}
		break;
		case Config::SubrecordType::kDATA:
		{
			SetGameSettingString(Information.EditorID, Information.ReplacerText);
		}
		break;
		case Config::SubrecordType::kUnknown:
		{
			SKSE::log::info("Unknown record {0:08X} in ConstTranslation", Information.Form->formID);
		}
		break;

		default: break;
		}
	}
	m_ConstConfigurationInformationStruct.clear(); //Structs only used once, so no need to keep them for fun
	m_ConstConfigurationInformationStruct.shrink_to_fit();

	SetMessageBoxButtonStrings();
}

template <typename T>
void Processor::SetConstStrings(RE::TESForm* Form, RE::BSFixedString NewString, RE::BSFixedString T::* memberPtr)
{
	T* OrigString = skyrim_cast<T*>(Form);
	if (OrigString)
	{
		OrigString->*memberPtr = NewString;
	}
	else
	{
		SKSE::log::error("Issue during ConstTranslation with FormID: {0:08X}.", Form->formID);
	}

}

void Processor::SetGameSettingString(const std::string& a_name, const std::string& a_NewString)
{
	auto setting = RE::GameSettingCollection::GetSingleton()->GetSetting(a_name.c_str());

	if (setting == nullptr)
	{
#ifndef NDEBUG

		SKSE::log::debug("Faild to set GameSetting string for {}. NOTE: It is normal for this to happen with some settings, they are simply not loaded.", a_name.c_str());
#endif
		return;
	}

	if (setting->GetType() == RE::Setting::Type::kString)
	{
		//RE::free(setting->data.s); -> don't know why this is crashing. If you know, please tell me!

		auto length = a_NewString.length() + 1;
		char* NewChar = (char*)RE::MemoryManager::GetSingleton()->Allocate(length, 0, 0);

		if (!NewChar)
		{
			SKSE::log::error("Failed to allocate memory for a new string when setting game setting '{}'. Upgrade your RAM and get rid of Windows XP!", a_name);
			return;
		}

		memcpy(NewChar, a_NewString.c_str(), length);

		setting->data.s = NewChar;
	}
}


void Processor::SetMessageBoxButtonStrings()
{
	if (m_MESGITXTInformationStruct.empty())
	{
		return;
	}

	for (const auto& Information : m_MESGITXTInformationStruct)
	{

		RE::BGSMessage* message = skyrim_cast<RE::BGSMessage*>(Information.Form); //MESG ITXT
		if (message)
		{
			int pos = 0;
			for (auto& button : message->menuButtons)
			{
				//SKSE::log::info("Pos: {} - String: {}", pos, text->text.c_str());

				if (pos == Information.pos)
				{
					button->text = std::move(Information.ReplacerText);
				}

				pos++;
			}

		}
		else
		{
			RE::BGSPerk* perk = skyrim_cast<RE::BGSPerk*>(Information.Form); //PERK EPF2

			if (perk)
			{
				for (auto& entry : perk->perkEntries)
				{
					if (entry->GetType() == RE::PERK_ENTRY_TYPE::kEntryPoint)
					{
						RE::BGSEntryPointPerkEntry* entryPoint = static_cast<RE::BGSEntryPointPerkEntry*>(entry); //This is cruel, but it's working

						if (entryPoint->entryData.function == RE::BGSEntryPointPerkEntry::EntryData::Function::kAddActivateChoice)
						{
							RE::BGSEntryPointFunctionDataActivateChoice* func = static_cast<RE::BGSEntryPointFunctionDataActivateChoice*>(entryPoint->functionData);

							//SKSE::log::info("ID: {} - String: {}", func->id, func->label);

							if (func->id == Information.pos)
							{
								func->label = std::move(Information.ReplacerText);
							}

						}
					}

				}

			}
			else
			{
				SKSE::log::error("Issue during ConstTranslation with FormID: {0:08X}.", Information.Form->formID);
			}
		}

	}
	m_MESGITXTInformationStruct.clear();
	m_MESGITXTInformationStruct.shrink_to_fit();
}
