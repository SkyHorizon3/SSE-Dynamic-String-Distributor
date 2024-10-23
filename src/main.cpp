#include "Config.h"
#include "MiscHooks.h"
#include "Processor.h"
#include "MergeMapperPluginAPI.h"

void LoadINI()
{
	const auto path = std::format("Data/SKSE/Plugins/{}.ini", Plugin::NAME);

	CSimpleIniA ini;
	ini.SetUnicode();
	ini.LoadFile(path.c_str());

	Config::EnableDebugLog = ini.GetBoolValue("Debug", "EnableDebugLog");
}

void MessageListener(SKSE::MessagingInterface::Message* message)
{
	switch (message->type)
	{
		// https://github.com/ianpatt/skse64/blob/09f520a2433747f33ae7d7c15b1164ca198932c3/skse64/PluginAPI.h#L193-L212
	case SKSE::MessagingInterface::kPostPostLoad:
	{
		if (!GetModuleHandle(L"po3_Tweaks"))
		{
			SKSE::log::critical("po3_Tweaks not found, mod won't work!");
		}

		MergeMapperPluginAPI::GetMergeMapperInterface001();
		if (g_mergeMapperInterface)
		{
			SKSE::log::info("Got MergeMapper interface buildnumber {}", g_mergeMapperInterface->GetBuildNumber());
		}
		else
		{
			SKSE::log::info("MergeMapper not detected");
		}

		Hook::InstallPostLoadHooks();

		Config::GetSingleton()->EnumerateFolder();
		Config::GetSingleton()->LoadFiles(true);
	}
	break;
	case SKSE::MessagingInterface::kDataLoaded:
	{
		auto startTime = std::chrono::high_resolution_clock::now();

		Config::GetSingleton()->LoadFiles(false);

		Processor::GetSingleton()->RunConstTranslation();

		Hook::InstallHooks();

		auto endTime = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
		SKSE::log::info("The parsing of jsons, the execution of ConstTranslation and the installation of hooks took {} milliseconds.", duration.count());

	}
	break;


	default:
		break;

	}
}

#define DLLEXPORT __declspec(dllexport)

extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []()
	{
		SKSE::PluginVersionData v;
		v.PluginName(Plugin::NAME);
		v.PluginVersion(Plugin::VERSION);
		v.AuthorName("SkyHorizon");
		v.UsesAddressLibrary();
		v.UsesNoStructs();
		return v;
	}
();

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface*, SKSE::PluginInfo * pluginInfo)
{
	pluginInfo->name = SKSEPlugin_Version.pluginName;
	pluginInfo->infoVersion = SKSE::PluginInfo::kVersion;
	pluginInfo->version = SKSEPlugin_Version.pluginVersion;
	return true;
}

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
	SKSE::Init(skse, true);

	SKSE::AllocTrampoline(140);

	LoadINI();

	spdlog::set_pattern("[%H:%M:%S:%e] [%l] %v"s);

	if (Config::EnableDebugLog)
	{
		spdlog::set_level(spdlog::level::trace);
		spdlog::flush_on(spdlog::level::trace);
	}
	else
	{
		spdlog::set_level(spdlog::level::info);
		spdlog::flush_on(spdlog::level::info);
	}

	SKSE::GetMessagingInterface()->RegisterListener(MessageListener);

	return true;
}