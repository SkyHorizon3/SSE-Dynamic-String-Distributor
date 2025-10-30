#include "Utils.h"

namespace Utils
{
	std::string getModName(const RE::TESForm* form)
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

		return string::tolower(filename.data());
	}

	RE::FormID getTrimmedFormID(const RE::TESForm* form)
	{
		if (!form)
		{
			return 0;
		}

		const auto file = form->GetFile(0);
		if (!file)
			return 0;

		RE::FormID formID = form->GetFormID() & 0xFFFFFF; // remove file index -> 0x00XXXXXX
		if (file->IsLight())
		{
			formID &= 0xFFF; // remove ESL index -> 0x00000XXX
		}

		return formID;
	}

	std::filesystem::path getPluginTXTFilePath()
	{
		wchar_t userDir[MAX_PATH];
		if (FAILED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, userDir)))
		{
			SKSE::log::error("User directory not found!");
			return L"";
		}

		std::filesystem::path path = std::filesystem::path(userDir);
		path /= "AppData"sv;
		path /= "Local"sv;
		path /= *REL::Relocation<const char**>(RELOCATION_ID(508778, AE_CHECK(SKSE::RUNTIME_SSE_1_6_1130, 380738, 502114))).get();
		path /= "plugins.txt"sv;

		SKSE::log::debug("Directory with plugins.txt: {}", path.string());

		return path;
	}

	RE::BSFixedString validateString(const RE::BSFixedString& toplace)
	{
		return toplace.empty() ? RE::BSFixedString(" ") : toplace;
	}

	RE::FormID convertToFormID(std::string input)
	{
		const bool has_x = (input.find('x') != std::string::npos) || (input.find('X') != std::string::npos);
		if (!has_x) // If it does not contain 'x', process it.
		{
			if (input.length() == 8)
			{
				if (input.compare(0, 2, "FE") == 0)
				{
					input = "0x" + input.substr(5);
				}
				else
				{
					input = "0x" + input.substr(2);
				}
			}
			else
			{
				input.insert(0, "0x");
			}
		}

		//SKSE::log::info("FormID: {}", input);

		return std::stoul(input, nullptr, 16);
	}

	std::string getAfterSpace(const std::string& types)
	{
		size_t spacePos = types.find(' ');
		if (spacePos != std::string::npos && spacePos + 1 < types.length())
		{
			return types.substr(spacePos + 1);
		}
		return "";
	}

	const RE::TESFile* getFileByFormIDRaw(RE::FormID a_rawFormID, RE::TESFile* a_file)
	{
		if (a_rawFormID - 1 <= 0x7FE)
			return a_file;

		const RE::TESFile* owner = a_file;
		const auto masterIndex = a_rawFormID >> 24;

		if (masterIndex < owner->masterCount && owner->masterPtrs)
			return owner->masterPtrs[masterIndex];

		return owner;
	}
}