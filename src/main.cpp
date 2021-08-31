#include "Offsets.h"
#include "Serialization.h"
#include "Events.h"
#include "Hooks.h"
#include "Settings.h"
#include "GlobalUIHandler.h"

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface * a_skse, SKSE::PluginInfo * a_info)
{
#ifndef NDEBUG
	auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
	auto path = logger::log_directory();
	if (!path) {
		return false;
	}

	*path /= "Ow.log"sv;
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
	spdlog::set_pattern("%g(%#): [%^%l%$] %v"s);

	logger::info("Ow v0.5.0");

	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = "Ow";
	a_info->version = 1;

	if (a_skse->IsEditor()) {
		logger::critical("Loaded in editor, marking as incompatible"sv);
		return false;
	}

	const auto ver = a_skse->RuntimeVersion();
	if (ver < SKSE::RUNTIME_1_5_39) {
		logger::critical(FMT_STRING("Unsupported runtime version {}"), ver.string());
		return false;
	}
	
	return true;
}

void InitListener(SKSE::MessagingInterface::Message* a_msg)
{
	switch (a_msg->type)
	{
	case SKSE::MessagingInterface::kNewGame :
		Globals::LoadGlobals();
		break;
	case SKSE::MessagingInterface::kDataLoaded :
		Globals::LoadGlobals();
		break;
	}
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface * a_skse)
{
	logger::info("Ow loaded"sv);

	SKSE::Init(a_skse);
	SKSE::AllocTrampoline(14);
	Hooks::Install();
	Events::Register();
	Settings::GetSingleton()->LoadSettings();

	auto messaging = SKSE::GetMessagingInterface();
	if (!messaging->RegisterListener(InitListener))
	{
		return false;
	}

	auto serialization = SKSE::GetSerializationInterface();
	serialization->SetUniqueID(Serialization::kOw);
	serialization->SetSaveCallback(Serialization::SaveCallback);
	serialization->SetLoadCallback(Serialization::LoadCallback);
	return true;
}


