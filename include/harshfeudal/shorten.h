#pragma once

#include <dpp/dpp.h>
#include <spdlog/spdlog.h>

#define NOMSGTYPE dpp::mt_default
#define NOMSGFLAG dpp::m_default

namespace harshfeudal
{
	inline void SlashMessageReply(const dpp::slashcommand_t& event, std::string content, dpp::message_flags msgFlag, dpp::message_type msgType)
	{
		event.reply(
			dpp::message()
			.set_content(content)
			.set_flags(msgFlag)
			.set_type(msgType)
		);
	}
}