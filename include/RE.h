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

	class ResponseList
	{
	public:
		TESTopicInfo::ResponseData* head;
	};
	static_assert(sizeof(ResponseList) == 0x8);
}