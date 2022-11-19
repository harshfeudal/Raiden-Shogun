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

#include "../../handler/handler.h"
#include "../../handler/btnHandler.h"
#include "../../commands/moderation/timeout.h"

void timeout(dpp::cluster& client, const dpp::slashcommand_t& event)
{
	dpp::embed embed;

	const auto errorTitle       = "<:failed:1036206712916553748> Error!";
	const auto successTitle     = "<:success:1036206685779398677> Success!";
	const auto warnTitle        = "Warning message";
	
	const auto duration         = std::get<int64_t>(event.get_parameter("duration"));
	const auto usr              = std::get<dpp::snowflake>(event.get_parameter("member"));
	const auto tgtReason        = event.get_parameter("reason");

	const auto source           = event.command.usr.id;
	const auto tgtGuild         = event.command.guild_id;
	const auto tgtChannel       = event.command.channel_id;
	const auto clientPermission = event.command.app_permissions.has(dpp::p_moderate_members);

	const auto gFind            = dpp::find_guild(event.command.guild_id);
	const auto tgtUser          = gFind->members.find(usr);

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

	if (!gFind->base_permissions(event.command.member).has(dpp::p_moderate_members))
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "You have lack of permission to timeout", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	if (!clientPermission)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "I have lack of permission to timeout", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	if (usr == gFind->owner_id)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "You cannot timeout the owner", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	if (usr == source)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "You cannot timeout yourself", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	if (usr == client.me.id)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "Why do you timeout me :(", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	auto tout_Component = dpp::component();
	auto cnl_Component  = dpp::component();

	tout_Component.set_label("Timeout").set_type(dpp::cot_button).set_style(dpp::cos_danger).set_emoji("success", 1036206685779398677).set_id("tout_Id");
	cnl_Component.set_label("Cancel").set_type(dpp::cot_button).set_style(dpp::cos_success).set_emoji("failed", 1036206712916553748).set_id("tout_cnl_Id");

	// Button for muting user (timeout)
	ButtonBind(tout_Component, [&client, tgtGuild, tgtReason, usr, source, duration](const dpp::button_click_t& event)
		{
			// If not the user who request that interaction
			if (source != event.command.usr.id)
				return false;

			const auto toutContent = fmt::format("<@{}> has been timeout until <t:{}:f>!", usr, time(nullptr) + duration);
			
			// If reason is provided
			if (std::holds_alternative<std::string>(tgtReason) == true)
			{
				const auto tout_Reason = std::get<std::string>(tgtReason);
				client.set_audit_reason(tout_Reason);
			}
			else
			{
				const auto tout_Reason = "No reason provided";
				client.set_audit_reason(tout_Reason);
			}

			// Timeout the user
			client.guild_member_timeout(tgtGuild, usr, time(nullptr) + duration);

			event.reply(
				dpp::interaction_response_type::ir_update_message,
				dpp::message().set_flags(dpp::m_ephemeral)
				              .set_content(toutContent)
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

	dpp::message tout_Confirm(
		fmt::format("Do you want to timeout <@{}> until <t:{}:f>? Press the button below to confirm", usr, time(nullptr) + duration)
	);

	tout_Confirm.add_component(
		dpp::component().add_component(tout_Component)
		                .add_component(cnl_Component)
	);

	event.reply(
		tout_Confirm.set_flags(dpp::m_ephemeral)
		            .set_channel_id(tgtChannel)
	);
}
