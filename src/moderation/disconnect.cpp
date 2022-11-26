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

#include "../../commands/moderation/disconnect.h"
#include "../../handler/handler.h"
#include "../../handler/btnHandler.h"

void disconnect(dpp::cluster& client, const dpp::slashcommand_t& event)
{
	dpp::embed embed;

	const auto errorTitle       = "<:failed:1036206712916553748> Error";
	const auto warnTitle        = "Warning message";

	const auto usr              = std::get<dpp::snowflake>(event.get_parameter("user"));
	const auto gFind            = dpp::find_guild(event.command.guild_id);

	const auto tgtReason        = event.get_parameter("reason");
	const auto source           = event.command.usr.id;
	const auto tgtGuild         = event.command.guild_id;
	const auto tgtChannel       = event.command.channel_id;
	const auto clientPermission = event.command.app_permissions.has(dpp::p_move_members);

	const auto tgtUser          = gFind->members.find(usr);

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
	if (!gFind->base_permissions(event.command.member).has(dpp::p_move_members))
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "You have lack of permission to disconnect member", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	// If the bot doesn't have any permission
	if (!clientPermission)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "I have lack of permission to disconnect member", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	// If they try to disconnect a guild owner
	if (usr == gFind->owner_id)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "You cannot disconnect the owner", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	// If they disconnect theirselves
	if (usr == source)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "You cannot disconnect yourself", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	// If they try to disconnect the bot
	if (usr == client.me.id)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "Why do you disconnect me :(", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	auto d_Component   = dpp::component();
	auto cnl_Component = dpp::component();

	d_Component.set_label("Disconnect").set_type(dpp::cot_button).set_style(dpp::cos_danger).set_emoji("success", 1036206685779398677).set_id("d_Id");
	cnl_Component.set_label("Cancel").set_type(dpp::cot_button).set_style(dpp::cos_success).set_emoji("failed", 1036206712916553748).set_id("d_cnl_Id");

	// Button for disconnecting
	ButtonBind(d_Component, [&client, tgtGuild, tgtReason, usr, source](const dpp::button_click_t& event)
		{
			// If not the user who request that interaction
			if (source != event.command.usr.id)
				return false;

			const auto  dContent = fmt::format("<@{}> has been disconnected!", usr);
            std::string d_Reason = "No reason provided";

			// If reason is provided
			if (std::holds_alternative<std::string>(tgtReason))
                d_Reason = std::get<std::string>(tgtReason);
			
			client.set_audit_reason(d_Reason);

            // Disconnect the target user
            client.guild_member_move(0, tgtGuild, usr);

			event.reply(
				dpp::interaction_response_type::ir_update_message,
				dpp::message().set_flags(dpp::m_ephemeral)
							  .set_content(dContent)
			);

			return true;
		});

	// Button for cancelling
	ButtonBind(cnl_Component, [source](const dpp::button_click_t& event)
		{
			// If not the user who request that interaction
			if (source != event.command.usr.id)
				return false;

			const auto cnlContent = "Cancelled request!";

			event.reply(
				dpp::interaction_response_type::ir_update_message,
				dpp::message().set_flags(dpp::m_ephemeral)
				              .set_content(cnlContent)
			);

			return true;
		});

	dpp::message d_Confirm(
		fmt::format("Do you want to disconnect <@{}>? Press the button below to confirm", usr)
	);

	d_Confirm.add_component(
		dpp::component().add_component(d_Component)
		                .add_component(cnl_Component)
	);

	event.reply(
		d_Confirm.set_flags(dpp::m_ephemeral)
		         .set_channel_id(tgtChannel)
	);
}
