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

#pragma once

#include <spdlog/spdlog.h>

#include "../../commands/moderation/nickname.h"
#include "../../handler/handler.h"

void nickname(dpp::cluster& client, const dpp::slashcommand_t& event)
{
	dpp::embed embed;

	std::string errorTitle = "<:failed:1036206712916553748> Error";
	std::string warnTitle = "Warning message";

	auto usr              = std::get<dpp::snowflake>(event.get_parameter("target"));
	auto setNickname      = event.get_parameter("nickname");
	auto gFind            = dpp::find_guild(event.command.guild_id);
	auto tgtGuild         = event.command.guild_id;
	auto tgtChannel       = event.command.channel_id;
	auto clientPermission = event.command.app_permissions.has(dpp::p_manage_nicknames);
	const auto tgtUser    = gFind->members.find(usr);

	if (tgtUser == gFind->members.end())
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "Member not found!", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	if (gFind == nullptr)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "Guild not found!", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	if (!gFind->base_permissions(event.command.member).has(dpp::p_manage_nicknames))
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "You have lack of permission to change nickname", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	if (!clientPermission)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "I have lack of permission to change nickname", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	if (usr == gFind->owner_id)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "You cannot change nickname the owner", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	auto getNicknameEditUsr = dpp::find_guild_member(event.command.guild_id, usr);

	if (std::holds_alternative<std::string>(setNickname) == true)
	{
		client.guild_edit_member(
			getNicknameEditUsr.set_nickname(std::get<std::string>(setNickname))
		);

		std::string announce = fmt::format("Nickname {} from <@{}>!", "changed", usr);
		event.reply(
			dpp::message().set_content(announce)
			              .set_flags(dpp::m_ephemeral)
		);
	}
	else
	{
		client.guild_edit_member(
			getNicknameEditUsr.set_nickname(dpp::find_user(usr)->username)
		);

		std::string announce = fmt::format("Nickname {} from <@{}>!", "cleared", usr);
		event.reply(
			dpp::message().set_content(announce)
			              .set_flags(dpp::m_ephemeral)
		);
	}
}
