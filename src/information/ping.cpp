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

#include <spdlog/spdlog.h>
#include <dpp/dpp.h>

#include "../../commands/information/ping.h"
#include "../../handler/handler.h"

void ping(dpp::cluster& client, const dpp::slashcommand_t& event)
{
	double ws            = client.get_shard(0)->websocket_ping;
	const auto RaidenPfp = client.me.get_avatar_url();
	
	std::string content = fmt::format(
		"`{0:.02f} ms`",
		(client.rest_ping + ws) * 1000
	);

	dpp::embed embed;
	embed = dpp::embed().set_color(0xAA7EEE)
                        .set_title("Raiden Shogun latecy")
                        .set_thumbnail(RaidenPfp)
                        .add_field("Latecy checked", content)
                        .set_footer(dpp::embed_footer().set_text(event.command.usr.username)
                                                       .set_icon(event.command.usr.get_avatar_url()))
                        .set_timestamp(time(nullptr));

	
	event.reply(
		dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
	);
}
