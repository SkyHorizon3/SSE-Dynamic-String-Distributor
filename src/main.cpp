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
	case SKSE::MessagingInterface::kDataLoaded:
	{
		Manager::GetSingleton()->runConstTranslation();

		SKSE::log::info("Data Loaded!");
	}
	break;
	default:
		break;

	}
}

SKSE_PLUGIN_VERSION = []()
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

SKSE_PLUGIN_QUERY(const SKSE::QueryInterface*, SKSE::PluginInfo* pluginInfo)
{
	pluginInfo->name = SKSEPlugin_Version.pluginName;
	pluginInfo->infoVersion = SKSE::PluginInfo::kVersion;
	pluginInfo->version = SKSEPlugin_Version.pluginVersion;
	return true;
}

SKSE_PLUGIN_LOAD(const SKSE::LoadInterface* skse)
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

	SKSE::AllocTrampoline(178);

	SKSE::GetMessagingInterface()->RegisterListener(MessageListener);

	return true;
}