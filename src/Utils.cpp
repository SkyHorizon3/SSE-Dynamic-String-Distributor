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

	size_t combineHashSubrecord(uint32_t formID, Config::SubrecordType subrecord)
	{
		size_t intValue = static_cast<size_t>(subrecord);
		size_t formHash = std::hash<uint32_t>{}(formID);
		return formHash ^ (intValue + formHash + 0x9e3779b9 + (formHash << 6) + (formHash >> 2));
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

		return filename.data();
	}

	RE::FormID GetTrimmedFormID(const RE::TESForm* form)
	{
		if (!form)
		{
			return 0;
		}

		const auto array = form->sourceFiles.array;
		if (!array || array->empty())
		{
			return 0;
		}

		RE::FormID formID = form->GetFormID() & 0xFFFFFF; // remove file index -> 0x00XXXXXX
		if (array->front()->IsLight())
		{
			formID = formID & 0xFFF; // remove ESL index -> 0x00000XXX
		}

		return formID;
	}

	bool CaseInsensitiveStringCompare(const std::string& a, const std::string& b)
	{
		return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin(), [](char a, char b)
			{
				return std::tolower(static_cast<unsigned char>(a)) == std::tolower(static_cast<unsigned char>(b));
			});
	}

	//return true if str is found in list.
	bool SearchCompare(const std::vector<std::string>& list, const std::string& str)
	{
		return std::any_of(list.begin(), list.end(), [&](const std::string& loadedPlugin)
			{
				return CaseInsensitiveStringCompare(str, loadedPlugin);
			});
	}

}