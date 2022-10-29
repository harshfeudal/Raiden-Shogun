#include <spdlog/spdlog.h>

#include "../commands/ping.h"

void ping(dpp::cluster& client, const dpp::slashcommand_t& event)
{
	double ws = client.get_shard(0)->websocket_ping;
	const auto RaidenPfp = client.me.get_avatar_url();
	
	std::string content = fmt::format(
		"`{0:.02f} ms`",
		(client.rest_ping + ws) * 1000
	);

	dpp::embed embed;
	harshfeudal::SlashReplyEmbed(
		embed, client, event, 
		"Raiden Shogun latecy", 
		client.me.username, BLANK_STRING, RaidenPfp, 
		RaidenPfp, content, 
		event.command.usr.username, event.command.usr.get_avatar_url(), 0xAA7EEE
	);
	
	event.reply(
		dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
	);
}
