#include <spdlog/spdlog.h>

#include "../commands/sha.h"

void sha(dpp::cluster& client, const dpp::slashcommand_t& event)
{
	auto inputStr       = std::get<std::string>(event.get_parameter("input"));
	auto encryptStr     = harshfeudal::hash256_hex_string(inputStr);
	std::string content = fmt::format("`{}`", encryptStr);

	dpp::embed embed;
	embed = dpp::embed().set_color(0xAA7EEE)
						.set_title("SHA256 convert result")
						.add_field("Input text", inputStr, true)
						.add_field("Output result", content, true)
						.set_footer(dpp::embed_footer()
							.set_text(event.command.usr.username)
							.set_icon(event.command.usr.get_avatar_url()))
						.set_timestamp(time(nullptr));

	event.reply(
		dpp::message(event.command.channel_id, embed)
	);
}
