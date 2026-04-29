#include "Manager.h"
#include "Hooks.h"

void MessageListener(SKSE::MessagingInterface::Message* message)
{
	switch (message->type)
	{
	case SKSE::MessagingInterface::kPostPostLoad:
	{
		MergeMapperPluginAPI::GetMergeMapperInterface001();
		if (g_mergeMapperInterface)
		{
			const auto version = g_mergeMapperInterface->GetBuildNumber();
			SKSE::log::info("Got MergeMapper interface buildnumber {}.", version);
		}
		else
		{
			SKSE::log::info("MergeMapper (https://www.nexusmods.com/skyrimspecialedition/mods/74689) not detected.");
		}

		Hook::InstallHooks();
	}
	break;
	case SKSE::MessagingInterface::kNewGame:
	{
		Manager::GetSingleton()->runConstTranslation();
		//SKSE::log::info("New Game!");
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
		v.AuthorName("SkyHorizon"sv);
		v.UsesAddressLibrary();
		v.UsesNoStructs();
		return v;
	}
();

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface*, SKSE::PluginInfo* pluginInfo)
{
	pluginInfo->name = SKSEPlugin_Version.pluginName;
	pluginInfo->infoVersion = SKSE::PluginInfo::kVersion;
	pluginInfo->version = SKSEPlugin_Version.pluginVersion;
	return true;
}

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
	SKSE::Init(skse, true);
	spdlog::set_pattern("[%H:%M:%S:%e] [%l] %v"s);

	SKSE::log::info("Game version: {}", skse->RuntimeVersion());

	const auto manager = Manager::GetSingleton();
	manager->LoadINI();

	if (manager->isDebugLogEnabled())
	{
		spdlog::set_level(spdlog::level::trace);
		spdlog::flush_on(spdlog::level::trace);
	}
	else
	{
		spdlog::set_level(spdlog::level::info);
		spdlog::flush_on(spdlog::level::info);
	}

	SKSE::AllocTrampoline(144);

	SKSE::GetMessagingInterface()->RegisterListener(MessageListener);

	return true;
}