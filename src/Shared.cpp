#include "Shared.h"
#include "ConditionParser.h"

bool ConditionData::buildConditions(const std::optional<std::vector<std::string>>& conditionList)
{
	if (conditionList && !conditionList->empty())
	{
		ConditionParser::BuildCondition(conditions, *conditionList);
		return true;
	}
	return false;
}

const char* ConditionData::decideText(RE::TESObjectREFR* ref, std::string_view currentText, std::string_view replacerText)
{
	if (!conditions)
		return replacerText.data();

	const bool conditionTrue = ref && conditions->IsTrue(ref, ref);
	if (conditionTrue)
	{
		if (!lastConditionState)
		{
			originalText = currentText;
		}

		lastConditionState = true;
		return replacerText.data();
	}

	if (lastConditionState)
	{
		lastConditionState = false;
		return originalText.c_str();
	}

	return nullptr;
}

ConstData::ConstData(const TranslationType type, const ParseData& entry) : translationType(type)
{
	switch (type)
	{
	case TranslationType::kGameSetting: // add to const translation (editorID is needed)
	{
		replacerText = entry.string;
		editor_id = entry.editor_id;
	}
	break;
	case TranslationType::kFullName:
	case TranslationType::kLoadScreenDescription:
	case TranslationType::kMagicDescription:
	case TranslationType::kShortName:
	case TranslationType::kRegion:
	case TranslationType::kWordOfPower:
	case TranslationType::kActivationText:
	case TranslationType::kReference: // add to const translation (no index)
	{
		replacerText = entry.string;
	}
	break;
	case TranslationType::kButtonText1:
	case TranslationType::kButtonText2:
	case TranslationType::kQuestObjective:
	case TranslationType::kPerkVerb: // add to const translation (index)
	{
		replacerText = entry.string;
		index = entry.index;
	}
	break;
	default:
	{
		translationType = TranslationType::kUnknown;
	}
	break;
	}

	data.buildConditions(entry.conditions);
}

RuntimeData::RuntimeData(const ParseData& entry) : replacerText(entry.string)
{
	data.buildConditions(entry.conditions);
}