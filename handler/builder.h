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
#include "../commands/moderation/unban.h"
#include "../commands/moderation/prune.h"
#include "../commands/moderation/timeout.h"
#include "../commands/moderation/nickname.h"
#include "../commands/moderation/disconnect.h"
#include "../commands/moderation/move.h"
#include "../commands/moderation/deafen.h"
#include "../commands/moderation/undeafen.h"
#include "../commands/moderation/mute.h"

#include "../commands/information/ping.h"
#include "../commands/information/infouser.h"
#include "../commands/information/about.h"

void SlashCommandCreate(dpp::cluster& client);

// Slash coomand builder
inline std::map<std::string, commandDef> commands
{
	{
		"ping", { "Check bot latency", ping }
	},
	{
		"kick",
			{
				"Kick a member you mentioned", kick,
				{
					dpp::command_option(dpp::co_user,   "member", "Mention a member to kick", true),
					dpp::command_option(dpp::co_string, "reason", "Reason why they got kick", false)
				}
			}
	},
	{
		"ban",
			{
				"Ban a member you mentioned", ban,
				{
					dpp::command_option(dpp::co_user,   "member", "Mention a member to ban", true),
					dpp::command_option(dpp::co_string, "reason", "Reason why they got ban", false)
				}
			}
	},
	{
		"unban",
			{
				"Ban a member you mentioned", unban,
				{
					dpp::command_option(dpp::co_user,   "member", "Mention a member to unban",  true),
					dpp::command_option(dpp::co_string, "reason", "Reason why they remove ban", false)
				}
			}
	},
	{
		"infouser",
			{
				"Show mentioned user info", infouser,
				{
					dpp::command_option(dpp::co_user, "user", "User you would like to know", false)
				}
			}
	},
	{
		"prune",
			{
				"Prune messages", prune,
				{
					dpp::command_option(dpp::co_integer, "amount", "Amount of messages to prune, from 2 up to 99", true)
				}
			}
	},
	{
		"timeout",
			{
				"Timeout member", timeout,
				{
					dpp::command_option(dpp::co_user,    "member",   "Mention a member to timeout", true),
					dpp::command_option(dpp::co_string,  "duration", "Timeout duration",            true),
					dpp::command_option(dpp::co_string,  "reason",   "Reason why they got timeout", false)
				}
			}
	},
    {
        "disconnect",
            {
                "Disconnect a user in the voice call", disconnect,
                {
                    dpp::command_option(dpp::co_user,   "user",   "User that you want to disconnect",  true),
                    dpp::command_option(dpp::co_string, "reason", "Reason why they gain a disconnect", false)
                }
            }
    },
	{
        "move",
            {
                "Move a user to another voice call", move,
                {
                    dpp::command_option(dpp::co_user,    "user",    "User that you want to move",  true),
                    dpp::command_option(dpp::co_channel, "channel", "New voice channel",           true),
                    dpp::command_option(dpp::co_string,  "reason",  "Reason why they gain a move", false)
                }
            }
    },
	{
        "deafen",
            {
                "Deafen a user in the voice call", deafen,
                {
                    dpp::command_option(dpp::co_user,  "user",   "User that you want to deafen",  true)
                }
            }
    },
	{
        "undeafen",
            {
                "Uneafen a user in the voice call", undeafen,
                {
                    dpp::command_option(dpp::co_user,  "user",   "User that you want to undeafen",  true)
                }
            }
    },
	{
        "mute",
            {
                "Mute a user in the voice call", mute,
                {
                    dpp::command_option(dpp::co_user,  "user",   "User that you want to mute",  true)
                }
            }
    },
	{
        "unmute",
            {
                "Unmute a user in the voice call", mute,
                {
                    dpp::command_option(dpp::co_user,  "user",   "User that you want to unmute",  true)
                }
            }
    },
	{
		"nick",
			{
				"Change member nickname or reset to their original username", nickname,
				{
					dpp::command_option(dpp::co_user,   "target",   "Member needs to change nickname",                  true),
					dpp::command_option(dpp::co_string, "nickname", "Set their new nickname, blank to reset as normal", false)
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

			// Create slash command template
			cmd.set_name(def.first)
			   .set_description(def.second.description)
			   .set_application_id(client.me.id);

			cmd.options = def.second.param;

			// Pushing all commands
			slashCmds.push_back(cmd);
		}

		// Create a global slash commands
		client.global_bulk_command_create(slashCmds);
	}
}
