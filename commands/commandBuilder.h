#pragma once

#include <map>

#include "handler.h"
#include "ping.h"
#include "kick.h"

std::map<std::string, command_def> commands
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
};

void SlashCommand(dpp::cluster& client)
{
	if (dpp::run_once<struct bulk_register>())
	{
		std::vector<dpp::slashcommand> slash_cmds;

		for (auto& def : commands)
		{
			dpp::slashcommand cmd;

			cmd.set_name(def.first)
				.set_description(def.second.description)
				.set_application_id(client.me.id);

			cmd.options = def.second.param;
			slash_cmds.push_back(cmd);
		}

		client.global_bulk_command_create(slash_cmds);
	}
}
