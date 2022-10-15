#pragma once

#include <dpp/dpp.h>
#include <spdlog/spdlog.h>

void MessageReply(const dpp::slashcommand_t& event, std::string content, bool ephemeral);
