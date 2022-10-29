#pragma once

#include <string>

#define NO_MSG_TYPE dpp::mt_default
#define NO_MSG_FLAG dpp::m_default
#define BLANK_STRING ""

namespace harshfeudal
{
	inline void SlashMessageReply(
		const dpp::slashcommand_t& event, std::string content, 
		dpp::message_flags msgFlag,       dpp::message_type msgType,
		dpp::interaction_response_type resType
	)
	{
		event.reply(
			resType,
			dpp::message().set_content(content)
			              .set_flags(msgFlag)
			              .set_type(msgType)
		);
	}

	inline void SlashMessageReply(
		const dpp::slashcommand_t& event, std::string content,
		dpp::message_flags msgFlag,       dpp::message_type msgType
	)
	{
		event.reply(
			dpp::message().set_content(content)
			              .set_flags(msgFlag)
			              .set_type(msgType)
		);
	}

	inline void ButtonReply(
		const dpp::button_click_t& event, std::string content,
		dpp::message_flags msgFlag, dpp::message_type msgType
	)
	{
		event.reply(
			dpp::message().set_content(content)
			.set_flags(msgFlag)
			.set_type(msgType)
		);
	}
}
