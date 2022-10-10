#pragma once

#include <map>

#include "handler.h"
#include "ping.h"

std::map<std::string, command_def> commands
{
	{
		"ping", { "Check Raiden Shogun latecy", ping }
	}
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