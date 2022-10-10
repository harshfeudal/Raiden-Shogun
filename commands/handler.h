#pragma once

#include <functional>

#include <dpp/dpp.h>

using command_func = std::function<void(dpp::cluster&, const dpp::slashcommand_t&)>;

struct command_def
{
	std::string description;
	command_func function;

	std::vector<dpp::command_option> param = {};
};