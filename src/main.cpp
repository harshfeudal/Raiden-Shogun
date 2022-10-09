#include <fstream>
#include <spdlog/spdlog.h>
#include <dpp/nlohmann/json.hpp>
#include <dpp/dpp.h>

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
	});

	client.start(false);
	return 0;
}