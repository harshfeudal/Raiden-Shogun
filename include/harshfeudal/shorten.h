#pragma once

#include <dpp/dpp.h>
#include <spdlog/spdlog.h>
#include <string>

#define NO_MSG_TYPE dpp::mt_default
#define NO_MSG_FLAG dpp::m_default

namespace harshfeudal
{
	inline void SlashMessageReply(
		const dpp::slashcommand_t& event, std::string content, 
		dpp::message_flags msgFlag,       dpp::message_type msgType
	)
	{
		event.reply(
			dpp::message()
			.set_content(content)
			.set_flags(msgFlag)
			.set_type(msgType)
		);
	}

	// Something error here
	inline void ButtonComplexCreate(
		dpp::message message, std::string label, dpp::component_style btnStyle, 
		std::string Id,       bool disable,      const std::string emjName, 
		dpp::snowflake emjId, bool emjAnimated,  std::string btnUrl
	)
	{
		message.add_component(
			dpp::component().add_component(
				dpp::component()
				.set_label(label)
				.set_type(dpp::cot_button)
				.set_style(btnStyle)
				.set_disabled(disable)
				.set_emoji(emjName, emjId, emjAnimated)
				.set_url(btnUrl)
				.set_id(Id)
			)
		);
	}

	// Something error here
	inline void ButtonBasicCreate(
		dpp::message message, std::string label, dpp::component_style btnStyle, 
		std::string Id,       bool disable,      std::string btnUrl
	)
	{
		message.add_component(
			dpp::component().add_component(
				dpp::component()
				.set_label(label)
				.set_type(dpp::cot_button)
				.set_style(btnStyle)
				.set_disabled(disable)
				.set_url(btnUrl)
				.set_id(Id)
			)
		);
	}
}