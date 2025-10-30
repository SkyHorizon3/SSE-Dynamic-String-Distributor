#pragma once

namespace RE
{
	inline RE::Setting* setStringValue(RE::Setting* setting, const char* str)
	{
		using func_t = decltype(&setStringValue);
		static REL::Relocation<func_t> func{ RELOCATION_ID(73882, 75619) };
		return func(setting, str);
	}

	[[nodiscard]] inline static RE::BSTHashMap<RE::FormID, RE::BSFixedString>& getActivateTextOverrideMap()
	{
		REL::Relocation<RE::BSTHashMap<RE::FormID, RE::BSFixedString>*> map{ RELOCATION_ID(501445, 360165).address() - 0x18 };
		return *map;
	}
}