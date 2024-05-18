#include "Config.h"
#include "MiscHooks.h"
#include "Processor.h"
#include "Parser.h"
#include "MergeMapperPluginAPI.h"

// Setup logger for plugin
void SetupLog()
{
	auto logsFolder = SKSE::log::log_directory();
	if (!logsFolder)
	{
		SKSE::stl::report_and_fail("SKSE log_directory not provided, logs disabled.");
	}

	auto pluginName = SKSE::PluginDeclaration::GetSingleton()->GetName();
	auto logFilePath = *logsFolder / std::format("{}.log", pluginName);
	auto fileLoggerPtr = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath.string(), true);
	auto loggerPtr = std::make_shared<spdlog::logger>("log", std::move(fileLoggerPtr));

	if (Config::EnableDebugLog)
	{
		loggerPtr->set_level(spdlog::level::trace);
		loggerPtr->flush_on(spdlog::level::trace);
	}
	else
	{
		loggerPtr->set_level(spdlog::level::info);
		loggerPtr->flush_on(spdlog::level::info);
	}

	spdlog::set_default_logger(std::move(loggerPtr));
}

void LoadINI()
{
	CSimpleIniA ini;
	ini.SetUnicode(false);
	ini.LoadFile(L"Data\\SKSE\\Plugins\\DynamicStringDistributor.ini");

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

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
	SKSE::Init(skse);

	LoadINI();
	SetupLog();

	//Parser::GetSingleton()->load("Data\\parseTest.esp");

	SKSE::GetMessagingInterface()->RegisterListener(MessageListener);
	SKSE::log::info("{} v{} loaded", Plugin::NAME, Plugin::VERSION);

	return true;
}

#define DLLEXPORT __declspec(dllexport)

extern "C" DLLEXPORT const auto SKSEPlugin_Version = []() noexcept {
	SKSE::PluginVersionData v;
	v.PluginName(Plugin::NAME.data());
	v.PluginVersion(Plugin::VERSION);
	v.UsesAddressLibrary(true);
	v.HasNoStructUse();
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