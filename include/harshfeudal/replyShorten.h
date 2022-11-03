/*
 * Copyright 2022 harshfeudal and The Harshfeudal Projects contributors
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */

#pragma once

#include <string>

#include <dpp/dpp.h>

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
