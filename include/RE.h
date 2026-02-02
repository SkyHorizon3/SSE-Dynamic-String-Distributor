#pragma once

namespace RE
{
	inline RE::Setting* setStringValue(RE::Setting* setting, const char* str)
	{
		using func_t = decltype(&setStringValue);
		static REL::Relocation<func_t> func{ RELOCATION_ID(73882, 75619) };
		return func(setting, str);
	}

	[[nodiscard]] inline static RE::BSTHashMap<RE::FormID, RE::BSFixedString>& getActivateTextOverrideMap() // ACTI RNAM, FLOR RNAM
	{
		REL::Relocation<RE::BSTHashMap<RE::FormID, RE::BSFixedString>*> map{ REL::VariantID(501445, 360165, 0x1E71EA0), -0x18 }; // ptr to sentinal - offset to start of hashmap
		return *map;
	}

	// case sensitive BSFixedString change
	inline void setBSFixedString(RE::BSFixedString& str, const char* data)
	{
		using func_t = decltype(&setBSFixedString);
		static REL::Relocation<func_t> func{ REL::VariantID(15291, 15453, 0x0) };
		func(str, data);
	}
}