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

	const auto errorTitle       = "<:failed:1036206712916553748> Error";
	const auto warnTitle        = "Warning message";

	const auto usr              = std::get<dpp::snowflake>(event.get_parameter("target"));
	const auto gFind            = dpp::find_guild(event.command.guild_id);

	const auto setNickname      = event.get_parameter("nickname");
	const auto tgtGuild         = event.command.guild_id;
	const auto tgtChannel       = event.command.channel_id;
	const auto clientPermission = event.command.app_permissions.has(dpp::p_manage_nicknames);

	const auto tgtUser          = gFind->members.find(usr);

	auto getNicknameEditUsr     = dpp::find_guild_member(event.command.guild_id, usr);

	// If cannot find that member in the server
	if (tgtUser == gFind->members.end())
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "Member not found!", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	// If cannot find the guild to action
	if (gFind == nullptr)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "Guild not found!", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	// If the command user doesn't have any permission
	if (!gFind->base_permissions(event.command.member).has(dpp::p_manage_nicknames))
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "You have lack of permission to change nickname", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	// If the bot doesn't have any permission
	if (!clientPermission)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "I have lack of permission to change nickname", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	// If they try to ban a guild owner
	if (usr == gFind->owner_id)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "You cannot change nickname the owner", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	// If the nickname is set or clear
	if (std::holds_alternative<std::string>(setNickname) == true)
	{
		client.guild_edit_member(
			getNicknameEditUsr.set_nickname(std::get<std::string>(setNickname))
		);

		const auto announce = fmt::format("Nickname {} from <@{}>!", "changed", usr);
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

		const auto announce = fmt::format("Nickname {} from <@{}>!", "cleared", usr);
		event.reply(
			dpp::message().set_content(announce)
			              .set_flags(dpp::m_ephemeral)
		);
	}
}
