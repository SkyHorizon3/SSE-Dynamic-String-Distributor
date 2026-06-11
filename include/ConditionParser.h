#pragma once

// 90% from copied from https://github.com/powerof3/LightPlacer 

using FUNC_ID = RE::FUNCTION_DATA::FunctionID;
using OP_CODE = RE::CONDITION_ITEM_DATA::OpCode;

using PARAM_TYPE = RE::SCRIPT_PARAM_TYPE;
using PARAMS = std::pair<std::optional<PARAM_TYPE>, std::optional<PARAM_TYPE>>;

class ConditionParser
{
public:
	static void BuildCondition(std::shared_ptr<RE::TESCondition>& a_condition, const std::vector<std::string>& a_conditionList);

private:
	union VOID_PARAM
	{
		char* c;
		std::int32_t i;
		float        f;
		RE::TESForm* ptr;
	};

	static std::int32_t GetFuncID(std::string_view a_str);
	static PARAMS       GetFuncType(FUNC_ID a_funcID);
	static RE::TESForm* LookupForm(const std::string& a_str);
	static bool         ParseVoidParam(const std::string& a_str, VOID_PARAM& a_param, PARAM_TYPE a_type);

};
