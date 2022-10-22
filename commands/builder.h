#pragma once

#include <map>

#include "handler.h"
#include "ping.h"
#include "kick.h"
#include "ban.h"
#include "userInfo.h"

void SlashCommandCreate(dpp::cluster& client);

std::map<std::string, commandDef> commands
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
						dpp::co_mentionable,
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
						dpp::co_mentionable,
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
				"Raiden Shogun will show your info or someone info", userInfo,
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

void SlashCommandCreate(dpp::cluster& client)
{
	if (dpp::run_once<struct bulk_register>())
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
