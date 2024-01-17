#include "../include/Globals.h"
#include "../include/Config.h"
#include "../include/Hooks.h"
#include "../include/Processor.h"

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
	g_Logger = std::make_shared<spdlog::logger>("log", std::move(fileLoggerPtr));
	spdlog::set_default_logger(g_Logger);
	spdlog::set_level(spdlog::level::trace);
	spdlog::flush_on(spdlog::level::trace);
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
			g_Logger->error("po3_Tweaks not found, mod won't work!");
		}

		Config::GetSingleton()->EnumerateFolder();
	}
	break;
	case SKSE::MessagingInterface::kDataLoaded:
	{
		auto startTime = std::chrono::high_resolution_clock::now();

		Config::GetSingleton()->LoadFiles();

		Processor::GetSingleton()->RunConstTranslation();

		Hook::InstallHooks();

		auto endTime = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
		g_Logger->info("The retrieval and parsing of jsons, the execution of ConstTranslation and the installation of hooks took {} milliseconds.", duration.count());

	}
	break;


	default:
		break;

	}
}

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
	SKSE::Init(skse);

	SetupLog();

	SKSE::GetMessagingInterface()->RegisterListener(MessageListener);
	g_Logger->info("{} v{} loaded", Plugin::NAME, Plugin::VERSION);

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