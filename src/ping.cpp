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
	embed = dpp::embed().set_color(0xAA7EEE)
						.set_title("Raiden Shogun latecy")
						.set_thumbnail(RaidenPfp)
						.set_description(content)
						.set_footer(dpp::embed_footer().set_text(event.command.usr.username)
													   .set_icon(event.command.usr.get_avatar_url()))
						.set_timestamp(time(nullptr));

	
	event.reply(
		dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
	);
}
