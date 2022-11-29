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

#include <fstream>
#include <map>
#include <spdlog/spdlog.h>
#include <dpp/nlohmann/json.hpp>
#include <harshfeudal/harshfeudal.h>
#include <dpp/dpp.h>
#include <Windows.h>

#include "../handler/builder.h"
#include "../handler/btnHandler.h"

using json = nlohmann::json;

int main()
{
	// Reading JSON configuration file
	json reader;
	{
		std::ifstream reading("config.json");

		// If cannot read find the configuration file to read
		if (!reading)
		{
			// Send a warning on console log
			std::cout << "No configuration file found! Please add it with the name \"config.json\"!";
			std::cin.get();

			// Stop the project immediately
			exit(0);
		}
		
		// Navigate and read
		reading >> reader;
	}

	// Encrypt the token - Base64
	const auto token = harshfeudal::Base64::Decode(reader["token"]);

	// Client variable builder
	dpp::cluster client(token, dpp::i_all_intents);

	client.on_ready([&client](const dpp::ready_t& event) 
		{
			// Set presence for the bot
			client.set_presence(
				dpp::presence(dpp::ps_dnd, dpp::at_game, "Genshin Impact")
			);
			
			// Slash command registration
			SlashCommandCreate(client);

			// Confirm logger
			fmt::print("[{} - STARTED]: {} is online!\n", dpp::utility::current_date_time(), client.me.format_username());
			fmt::print("[{} - REGISTERED]: Successfully registered slash commands!\n", dpp::utility::current_date_time());
		});

	client.on_slashcommand([&client](const dpp::slashcommand_t& event)
		{
			dpp::command_interaction commandData   = event.command.get_command_interaction();
			auto                     commandFilter = commands.find(commandData.name);

			// Check commands
			if (commandFilter != commands.end())
				commandFilter->second.function(client, event);
		});

	client.on_button_click([](const dpp::button_click_t& event)
		{
			// Handling the button event
			ButtonHandle(event);
		});
	
	// Console log prettier
	SetConsoleTitle(TEXT("[BETA] v0.1.6.0 - Raiden Shogun Discord Bot - The Harshfeudal Projects"));

	// Use this logger to check why the code is error (remove when done)
	client.on_log(dpp::utility::cout_logger());

	// Starting the bot
	client.start(dpp::st_wait);
	
	return 0;
}
