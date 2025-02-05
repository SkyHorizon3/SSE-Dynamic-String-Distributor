#include "Utils.h"
#include "MergeMapperPluginAPI.h"

namespace Utils
{
	std::string tolower(std::string_view a_str)
	{
		std::string result(a_str);
		std::ranges::transform(result, result.begin(), [](unsigned char ch) { return static_cast<unsigned char>(std::tolower(ch)); });
		return result;
	}

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

		return tolower(filename.data());
	}

	RE::FormID getTrimmedFormID(const RE::TESForm* form)
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
		if (input.find('x') == std::string::npos) // If it does not contain 'x', process it.
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

	const RE::TESFile* getFileByFormIDRaw(RE::FormID a_rawFormID)
	{
		const auto handler = RE::TESDataHandler::GetSingleton();

		const auto* expectedFile = (a_rawFormID & 0xFF000000) == 0xFE000000 ?
			handler->LookupLoadedLightModByIndex(
				static_cast<uint16_t>((0x00FFF000 & a_rawFormID) >> 12)) :
			handler->LookupLoadedModByIndex(
				static_cast<uint8_t>((0xFF000000 & a_rawFormID) >> 24));

		return expectedFile;
	}
}