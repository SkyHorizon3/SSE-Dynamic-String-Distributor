#include "Shared.h"
#include "ConditionParser.h"

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

	if (const auto& cond = entry.conditions; cond && !cond->empty())
	{
		ConditionParser::BuildCondition(conditions, *cond);
	}
}

RuntimeData::RuntimeData(const ParseData& entry) : replacerText(entry.string)
{
	if (const auto& cond = entry.conditions; cond && !cond->empty())
	{
		ConditionParser::BuildCondition(conditions, *cond);
	}
}