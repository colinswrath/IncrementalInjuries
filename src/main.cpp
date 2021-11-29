#include "Cache.h"
#include "Serialization.h"
#include "Events.h"
#include "Hooks.h"
#include "Settings.h"
#include "GlobalUIHandler.h"
#include "PapyrusPLayerAV.h"
#include "Version.h"

void InitLogger()
{
#ifndef NDEBUG
	auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
	auto path = logger::log_directory();
	if (!path) {
		return;
	}

	*path /= fmt::format("{}.log"sv, Version::PROJECT);
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

#ifndef NDEBUG
	log->set_level(spdlog::level::trace);
#else
	log->set_level(spdlog::level::info);
	log->flush_on(spdlog::level::info);
#endif

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("%s(%#): [%^%l%$] %v"s);

	logger::info(FMT_STRING("{} v{}"), Version::PROJECT, Version::NAME);
}

extern "C" DLLEXPORT constexpr auto SKSEPlugin_Version =
[]() {
	SKSE::PluginVersionData v{};
	v.PluginVersion(Version::MAJOR);
	v.PluginName(Version::PROJECT);
	v.AuthorName("colinswrath"sv);
	v.UsesAddressLibrary(true);
	return v;
}();


void InitListener(SKSE::MessagingInterface::Message* a_msg)
{
	switch (a_msg->type)
	{
	case SKSE::MessagingInterface::kNewGame :
		Globals::LoadGlobals();
		break;
	case SKSE::MessagingInterface::kDataLoaded :
		Globals::LoadGlobals();
		//PlayerAVDamageManager::SetPlayerProperty(RE::PlayerCharacter::GetSingleton());
		Cache::SetPlayerSingletonAddress();
		PlayerAVDamageManager::InstallVHook();
		break;
	}
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface * a_skse)
{
	InitLogger();
	logger::info("IncrementalInjuries loaded"sv);

	SKSE::Init(a_skse);
	SKSE::AllocTrampoline(14);
	Hooks::Install();
	Events::Register();
	Settings::LoadSettings();
	
	auto messaging = SKSE::GetMessagingInterface();
	if (!messaging->RegisterListener(InitListener))
	{
		return false;
	}

	auto papyrus = SKSE::GetPapyrusInterface();
	papyrus->Register(PapyrusPlayerAV::Register);
	logger::info("Papyrus functions registered.");

	auto serialization = SKSE::GetSerializationInterface();
	serialization->SetUniqueID(Serialization::kOw);
	serialization->SetSaveCallback(Serialization::SaveCallback);
	serialization->SetLoadCallback(Serialization::LoadCallback);
	serialization->SetRevertCallback(Serialization::RevertCallback);
	return true;
}


