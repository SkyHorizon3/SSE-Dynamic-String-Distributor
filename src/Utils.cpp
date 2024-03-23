#include "Utils.h"
#include "MergeMapperPluginAPI.h"

namespace Utils
{
	size_t combineHash(uint32_t formID, const std::string& str)
	{
		size_t strHash = std::hash<std::string>{}(str);
		size_t formHash = std::hash<uint32_t>{}(formID);
		return strHash ^ (formHash + 0x9e3779b9 + (strHash << 6) + (strHash >> 2));
	}

	size_t combineHashWithIndex(uint32_t formID, int value, const std::string& str)
	{
		size_t strHash = std::hash<std::string>{}(str);
		size_t intValue = static_cast<size_t>(value);
		size_t formHash = std::hash<uint32_t>{}(formID);
		return strHash ^ (intValue + formHash + 0x9e3779b9 + (strHash << 6) + (strHash >> 2));
	}

	std::string GetModName(const RE::TESForm* form)
	{
		if (!form)
		{
			return "";
		}

		const auto array = form->sourceFiles.array;
		if (!array || array->empty())
		{
			return "";
		}

		const auto file = array->front();
		std::string_view filename = file ? file->GetFilename() : "";

		if (g_mergeMapperInterface)
		{
			auto formID = form->GetFormID() & 0xFFFFFF;
			std::tie(filename, formID) = g_mergeMapperInterface->GetOriginalFormID(filename.data(), formID);
		}

		return filename.data();
	}

}