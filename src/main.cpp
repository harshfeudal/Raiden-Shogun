#include <fstream>
#include <map>

#include <spdlog/spdlog.h>
#include <dpp/nlohmann/json.hpp>
#include <dpp/dpp.h>

#include "../commands/commandBuilder.h"

using json = nlohmann::json;

int main()
{
	json reader;
	{
		std::ifstream reading("config.json");
		reading >> reader;
		reading.close();
	}

	const std::string token = reader["token"];
	dpp::cluster client(token, dpp::i_all_intents);

	client.on_ready([&client](const dpp::ready_t& event) {
		fmt::print("[Started]: {} is online!", client.me.username);
		client.set_presence(dpp::presence(dpp::ps_dnd, dpp::at_game, "Genshin Impact"));

		SlashCommand(client);
	});

	client.on_slashcommand([&client](const dpp::slashcommand_t& event)
		{
			dpp::command_interaction command_data = event.command.get_command_interaction();
			auto command_filter = commands.find(command_data.name);

			if (command_filter != commands.end())
			{
				command_filter->second.function(client, event);
			}
		});


	client.start(false);
	return 0;
}
