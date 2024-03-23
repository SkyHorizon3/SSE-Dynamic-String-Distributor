#include "Utils.h"


namespace Utils
{
	size_t combineHash(uint32_t form, const std::string& str)
	{
		size_t strHash = std::hash<std::string>{}(str);
		size_t formHash = std::hash<uint32_t>{}(form);
		return strHash ^ (formHash + 0x9e3779b9 + (strHash << 6) + (strHash >> 2));
	}

	size_t combineHashWithIndex(uint32_t form, int value, const std::string& str)
	{
		size_t strHash = std::hash<std::string>{}(str);
		size_t intValue = static_cast<size_t>(value);
		size_t formHash = std::hash<uint32_t>{}(form);
		return strHash ^ (intValue + formHash + 0x9e3779b9 + (strHash << 6) + (strHash >> 2));
	}

}