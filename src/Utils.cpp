#include "Utils.h"
#include "Config.h"
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

	std::wstring getPluginTXTFilePath()
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

			if (Config::enableDebugLog)
			{
				std::string_view skyrim = std::filesystem::exists("steam_api64.dll") ? "Skyrim Special Edition" : "Skyrim Special Edition GOG";
				SKSE::log::debug("Directory: {} - Version: {}", skyrim, REL::Module::get().version());
			}
		}

		return loadOrderPath;
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
}