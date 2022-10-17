#pragma once

#include <functional>

#include <dpp/dpp.h>

using commandFunc = std::function<void(dpp::cluster&, const dpp::slashcommand_t&)>;

struct commandDef
{
	std::string description;
	commandFunc function;
	std::vector<dpp::command_option> param = {};
};
