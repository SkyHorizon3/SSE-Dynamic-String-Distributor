#include "Utils.h"
#include "Config.h"
#include "MergeMapperPluginAPI.h"

namespace Utils
{
	template<typename... HashValues>
	size_t combineHashes(HashValues... values)
	{
		size_t combinedHash = 0;
		constexpr size_t constant = 0x9e3779b9;

		((combinedHash ^= (std::hash<std::decay_t<HashValues>>{}(values)+constant + (combinedHash << 6) + (combinedHash >> 2))), ...);

		return combinedHash;
	}

	size_t combineHash(const std::uint32_t formID, const std::string& str)
	{
		return combineHashes(str, formID);
	}

	size_t combineHashWithIndex(const std::uint32_t formID, int value, const std::string& str)
	{
		return combineHashes(str, static_cast<size_t>(value), formID);
	}

	size_t combineHashSubrecord(const std::uint32_t formID, Config::SubrecordType subrecord)
	{
		return combineHashes(formID, static_cast<size_t>(subrecord));
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

	bool char_equals(char a, char b)
	{
		return std::tolower(static_cast<unsigned char>(a)) == std::tolower(static_cast<unsigned char>(b));
	}

	bool CaseInsensitiveStringCompare(std::string_view a, std::string_view b)
	{
		return a.size() == b.size() && std::ranges::equal(a, b, char_equals);
	}

	//return true if str is found in list.
	bool SearchCompare(const std::vector<std::string>& list, const std::string& str)
	{
		return std::any_of(list.begin(), list.end(), [&](const std::string& loadedPlugin)
			{
				return CaseInsensitiveStringCompare(str, loadedPlugin);
			});
	}

	std::wstring GetPluginTXTFilePath()
	{
		wchar_t userDir[MAX_PATH];
		if (FAILED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, userDir)))
		{
			SKSE::log::error("User directory not found!");
			return L"";
		}

		std::wstring loadOrderPath = userDir;
		if (REL::Module::IsVR())
		{
			loadOrderPath += L"\\AppData\\Local\\Skyrim VR\\plugins.txt";
			SKSE::log::debug("Directory: Skyrim VR");
		}
		else
		{
			std::wstring name = std::filesystem::exists("steam_api64.dll") ? L"Skyrim Special Edition" : L"Skyrim Special Edition GOG";
			loadOrderPath += L"\\AppData\\Local\\" + name + L"\\plugins.txt";

			if (Config::EnableDebugLog)
			{
				std::string_view skyrim = std::filesystem::exists("steam_api64.dll") ? "Skyrim Special Edition" : "Skyrim Special Edition GOG";
				SKSE::log::debug("Directory: {} - Version: {}", skyrim, REL::Module::get().version());
			}
		}

		return loadOrderPath;
	}

}