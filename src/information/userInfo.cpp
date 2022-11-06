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

#include "../commands/information/userInfo.h"
#include "../handler/handler.h"

void EmbedBuild(dpp::embed& embed, std::string avatar, std::string usrName, std::string usrID, std::string created, const dpp::user& tgtUser);

void userInfo(dpp::cluster& client, const dpp::slashcommand_t& event)
{
	auto tgtUser = event.command.usr;
	dpp::embed embed;

	if (std::holds_alternative<dpp::snowflake>(event.get_parameter("id")) == true)
	{
		auto usr_id = event.get_parameter("user");
		auto tgtId  = dpp::find_user(std::get<dpp::snowflake>(usr_id));

		std::string avatar  = tgtId->get_avatar_url();
		std::string usrID   = fmt::format("{}", tgtId->id);
		std::string created = fmt::format("<t:{}:R>", round(tgtId->get_creation_time()));
		std::string usrName = fmt::format("{}", tgtId->format_username());

		EmbedBuild(embed, avatar, usrName, usrID, created, tgtUser);
		event.reply(
			dpp::message(event.command.channel_id, embed)
		);
	}
	else
	{
		auto tgtId = event.command.usr;

		std::string avatar  = tgtId.get_avatar_url();
		std::string usrID   = fmt::format("{}", tgtId.id);
		std::string created = fmt::format("<t:{}:R>", round(tgtId.get_creation_time()));
		std::string usrName = fmt::format("{}", tgtId.format_username());

		EmbedBuild(embed, avatar, usrName, usrID, created, tgtUser);
		event.reply(
			dpp::message(event.command.channel_id, embed)
		);
	}
}

void EmbedBuild(dpp::embed& embed, std::string avatar, std::string usrName, std::string usrID, std::string created, const dpp::user& tgtUser)
{
	embed = dpp::embed().set_color(0xAA7EEE)
                        .set_title("User Information")
                        .set_thumbnail(avatar)
                        .add_field("Username", usrName, true).add_field("User ID", usrID, true).add_field("Created", created, true)
                        .set_footer(dpp::embed_footer().set_text(tgtUser.username).set_icon(tgtUser.get_avatar_url()))
                        .set_timestamp(time(nullptr));
}
