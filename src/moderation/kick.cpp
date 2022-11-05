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

#include "../handler/handler.h"
#include "../handler/btnHandler.h"
#include "../commands/moderation/kick.h"

inline void EmbedBuild(dpp::embed& embed, uint32_t col, std::string title, std::string fieldTitle, std::string fieldDes, const dpp::user& tgtUser);

void kick(dpp::cluster& client, const dpp::slashcommand_t& event)
{
	dpp::embed embed;

	std::string errorTitle = "<:failed:1036206712916553748> Error";
	std::string warnTitle  = "Warning message";

	auto usr       = std::get<dpp::snowflake>(event.get_parameter("member"));
	auto tgtReason = event.get_parameter("reason");
	
	auto source           = event.command.usr.id;
	auto gFind            = dpp::find_guild(event.command.guild_id);
	auto tgtGuild         = event.command.guild_id;
	auto tgtChannel       = event.command.channel_id;
	auto clientPermission = event.command.app_permissions.has(dpp::p_kick_members);

	const auto tgtUser = gFind->members.find(usr);

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

	if (!gFind->base_permissions(event.command.member).has(dpp::p_kick_members))
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "You have lack of permission to prune", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	if (!clientPermission)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "I have lack of permission to prune", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	if (usr == gFind->owner_id)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "You cannot kick the owner", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	if (usr == source)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "You cannot kick yourself", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	if (usr == client.me.id)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "Why do you kick me :(", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	auto k_Component = dpp::component().set_label("Kick")
                                       .set_type(dpp::cot_button)
                                       .set_style(dpp::cos_danger)
                                       .set_emoji("success", 1036206685779398677)
                                       .set_id("k_Id");

	auto cnl_Component = dpp::component().set_label("Cancel")
                                         .set_type(dpp::cot_button)
                                         .set_style(dpp::cos_success)
                                         .set_emoji("failed", 1036206712916553748)
                                         .set_id("k_cnl_Id");

	ButtonBind(k_Component, [&client, tgtGuild, tgtReason, usr, source](const dpp::button_click_t& event)
		{
			if (source != event.command.usr.id)
			{
				return false;
			}

			std::string kContent = fmt::format("<@{}> has been kicked!", usr);
			
			if (std::holds_alternative<std::string>(tgtReason) == true)
			{
				std::string k_Reason = std::get<std::string>(tgtReason);
				client.set_audit_reason(k_Reason);
			}
			else
			{
				std::string k_Reason = "No reason provided";
				client.set_audit_reason(k_Reason);
			}

			client.guild_member_kick(tgtGuild, usr);

			event.reply(
				dpp::interaction_response_type::ir_update_message,
				dpp::message().set_flags(dpp::m_ephemeral)
				              .set_content(kContent)
			);

			return true;
		});

	ButtonBind(cnl_Component, [source](const dpp::button_click_t& event)
		{
			std::string cnlContent = "Cancelled request!";

			if (source != event.command.usr.id)
			{
				return false;
			}

			event.reply(
				dpp::interaction_response_type::ir_update_message,
				dpp::message().set_flags(dpp::m_ephemeral)
				              .set_content(cnlContent)
			);

			return true;
		});

	dpp::message k_Confirm(
		fmt::format("Do you want to kick <@{}>? Press the button below to confirm", usr)
	);

	k_Confirm.add_component(
		dpp::component().add_component(k_Component)
		                .add_component(cnl_Component)
	);

	event.reply(
		k_Confirm.set_flags(dpp::m_ephemeral)
		         .set_channel_id(tgtChannel)
	);
}

inline void EmbedBuild(dpp::embed& embed, uint32_t col, std::string title, std::string fieldTitle, std::string fieldDes, const dpp::user& tgtUser)
{
	embed = dpp::embed().set_color(col)
		.set_title(title)
		.add_field(fieldTitle, fieldDes)
		.set_footer(dpp::embed_footer().set_text(tgtUser.username).set_icon(tgtUser.get_avatar_url()))
		.set_timestamp(time(nullptr));
}
