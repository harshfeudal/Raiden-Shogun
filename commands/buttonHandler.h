#pragma once

#include <dpp/dpp.h>

#define COMPONENT_CACHE_DURATION 60 * 10
#define CUSTOM_ID_SPACER "###"

struct Session {
	time_t created_at;
	std::function<bool(const dpp::button_click_t&)> function;

	Session() : created_at(time(nullptr)) {}

	bool isExpired() const {
		return difftime(time(nullptr), created_at) > COMPONENT_CACHE_DURATION;
	}
};

std::unordered_map<uint64_t, Session> cachedSessions;
std::shared_mutex                     cachedSessionsMutex;
uint64_t                              customIdCounter;

void Buttonhandle(const dpp::button_click_t& event)
{
	uint64_t customId;
	time_t   creationTimestamp;

	try
	{
		std::string id = event.custom_id.substr(0, event.custom_id.find(CUSTOM_ID_SPACER));

		std::string creation = event.custom_id.substr(
			event.custom_id.find(CUSTOM_ID_SPACER) + std::strlen(CUSTOM_ID_SPACER),
			std::string::npos
		);

		customId = std::stoul(id);
		creationTimestamp = std::stol(creation);
	}
	catch (std::out_of_range& e)
	{
		harshfeudal::ButtonReply(
			event, "invalid component", dpp::m_ephemeral, NO_MSG_TYPE
		);

		return;
	}
	catch (std::invalid_argument& e)
	{
		event.reply(dpp::message("invalid component").set_flags(dpp::m_ephemeral));
		return;
	}

	std::unique_lock l(cachedSessionsMutex);

	auto existing = cachedSessions.find(customId);
	if (existing != cachedSessions.end() && existing->second.created_at == creationTimestamp)
	{
		bool forget = existing->second.function(event);
		if (forget)
			cachedSessions.erase(existing);
	}
}
