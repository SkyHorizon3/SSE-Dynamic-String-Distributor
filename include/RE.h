#pragma once

namespace RE
{
	[[nodiscard]] inline static RE::BSTHashMap<RE::FormID, RE::BSFixedString>& getActivateTextOverrideMap() // ACTI RNAM, FLOR RNAM
	{
		REL::Relocation<RE::BSTHashMap<RE::FormID, RE::BSFixedString>*> map{ REL::VariantID(501445, 360165, 0x1E71EA0), -0x18 }; // ptr to sentinal - offset to start of hashmap
		return *map;
	}

	RE::Setting* setStringValue(RE::Setting* setting, const char* str);
	// case sensitive BSFixedString change
	void setBSFixedString(RE::BSFixedString& str, const char* data);

	// gets the last base of the actor reference that has the BaseData flag. But this can still be a template that inherited the full name
	RE::FormID getFullNameFormIDForRef(const RE::TESObjectREFR& actor);

	RE::OBJECT_TYPE stringToObjectType(std::string_view str);
}