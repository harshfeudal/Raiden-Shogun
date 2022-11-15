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

#include "../../commands/utility/sha.h"

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
                        .set_footer(dpp::embed_footer().set_text(event.command.usr.username)
                                                       .set_icon(event.command.usr.get_avatar_url()))
                        .set_timestamp(time(nullptr));

	event.reply(
		dpp::message(event.command.channel_id, embed)
	);
}
