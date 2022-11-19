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

#include "../../commands/information/userInfo.h"
#include "../../handler/handler.h"

void userInfo(dpp::cluster& client, const dpp::slashcommand_t& event)
{
	dpp::embed embed;
	
	const auto tgtUser = event.command.usr;

	// If the command user is trying to get another people information
	if (std::holds_alternative<dpp::snowflake>(event.get_parameter("id")) == true)
	{
		const auto usr_id  = event.get_parameter("user");
		const auto tgtId   = dpp::find_user(std::get<dpp::snowflake>(usr_id));

		const auto avatar  = tgtId->get_avatar_url();
		const auto usrID   = fmt::format("{}", tgtId->id);
		const auto created = fmt::format("<t:{}:R>", round(tgtId->get_creation_time()));
		const auto usrName = fmt::format("{}", tgtId->format_username());

		EmbedInfoBuild(embed, avatar, usrName, usrID, created, tgtUser);

		event.reply(
			dpp::message(event.command.channel_id, embed)
		);
	}
	else
	{
		const auto tgtId   = event.command.usr;

		const auto avatar  = tgtId.get_avatar_url();
		const auto usrID   = fmt::format("{}", tgtId.id);
		const auto created = fmt::format("<t:{}:R>", round(tgtId.get_creation_time()));
		const auto usrName = fmt::format("{}", tgtId.format_username());

		EmbedInfoBuild(embed, avatar, usrName, usrID, created, tgtUser);

		event.reply(
			dpp::message(event.command.channel_id, embed)
		);
	}
}
