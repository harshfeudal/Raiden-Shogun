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

#include <map>

#include "handler.h"

#include "../commands/moderation/kick.h"
#include "../commands/moderation/ban.h"
#include "../commands/moderation/prune.h"

#include "../commands/utility/ping.h"
#include "../commands/utility/userInfo.h"
#include "../commands/utility/sha.h"

void SlashCommandCreate(dpp::cluster& client);

inline std::map<std::string, commandDef> commands
{
	{
		"ping", { "Check bot latecy", ping }
	},
	{
		"kick",
			{
				"Kick a member you mentioned", kick,
				{
					dpp::command_option(
						dpp::co_user,
						"member",
						"Mention a member to kick",
						true
					),
					dpp::command_option(
						dpp::co_string,
						"reason",
						"Reason why they got kick",
						false
					)
				}
			}
	},
	{
		"ban",
			{
				"Ban a member you mentioned", ban,
				{
					dpp::command_option(
						dpp::co_user,
						"member",
						"Mention a member to ban",
						true
					),
					dpp::command_option(
						dpp::co_string,
						"reason",
						"Reason why they got ban",
						false
					)
				}
			}
	},
	{
		"userinfo",
			{
				"Show mentioned user info", userInfo,
				{
					dpp::command_option(
						dpp::co_user,
						"id",
						"User ID you would like to know",
						false
					)
				}
			}
	},
	{
		"sha",
			{
				"Convert into your text to SHA256", sha,
				{
					dpp::command_option(
						dpp::co_string,
						"input",
						"Your message to convert",
						true
					)
				}
			}
	},
	{
		"prune",
			{
				"Prune messages", prune,
				{
					dpp::command_option(
						dpp::co_integer,
						"amount",
						"Amount of messages to prune, from 2 up to 99",
						true
					)
				}
			}
	}
};

inline void SlashCommandCreate(dpp::cluster& client)
{
	if (dpp::run_once<struct register_commands>())
	{
		std::vector<dpp::slashcommand> slashCmds;

		for (auto& def : commands)
		{
			dpp::slashcommand cmd;

			cmd.set_name(def.first)
				.set_description(def.second.description)
				.set_application_id(client.me.id);

			cmd.options = def.second.param;
			slashCmds.push_back(cmd);
		}

		client.global_bulk_command_create(slashCmds);
	}
}
