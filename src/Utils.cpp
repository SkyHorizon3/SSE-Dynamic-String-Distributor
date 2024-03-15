#include "Utils.h"


namespace Utils
{
	size_t combineHash(uint32_t form, int value)
	{
		size_t strHash = std::hash<uint32_t>{}(form);
		size_t intValue = static_cast<size_t>(value);
		return strHash ^ (intValue + 0x9e3779b9 + (strHash << 6) + (strHash >> 2));
	}

}