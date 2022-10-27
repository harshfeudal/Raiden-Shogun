#pragma once

#include <map>

#include "handler.h"
#include "../commands/ping.h"
#include "../commands/kick.h"
#include "../commands/ban.h"
#include "../commands/userInfo.h"

void SlashCommandCreate(dpp::cluster& client);

inline std::map<std::string, commandDef> commands
{
	{
		"ping", { "Check Raiden Shogun latecy", ping }
	},
	{
		"kick",
			{
				"Raiden Shogun will kick a member you mentioned", kick,
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
				"Raiden Shogun will ban a member you mentioned", ban,
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
				"Raiden Shogun will show mentioned user info", userInfo,
				{
					dpp::command_option(
						dpp::co_user,
						"id",
						"User ID you would like to know",
						false
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
