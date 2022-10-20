#include <spdlog/spdlog.h>

#include "../commands/ping.h"
#include "../commands/handler.h"

void ping(dpp::cluster& client, const dpp::slashcommand_t& event)
{
	double ws = client.get_shard(0)->websocket_ping;

	std::string content = fmt::format(
		"Raiden Shogun latecy: {0:.02f} ms",
		(client.rest_ping + ws) * 1000
	);
	
	harshfeudal::SlashMessageReply(
		event, content, NO_MSG_FLAG, NO_MSG_TYPE
	);
}
