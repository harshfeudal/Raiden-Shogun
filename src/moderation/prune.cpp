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
#include "../../commands/moderation/prune.h"

void prune(dpp::cluster& client, const dpp::slashcommand_t& event)
{
	dpp::embed embed;

	std::string errorTitle = "<:failed:1036206712916553748> Error";
	std::string warnTitle  = "Warning message";

	auto tgtChannel       = event.command.channel_id;
	auto tgtReason        = event.get_parameter("reason");
	auto gFind            = dpp::find_guild(event.command.guild_id);
	auto amount           = std::get<int64_t>(event.get_parameter("amount"));
	auto clientPermission = event.command.app_permissions.has(dpp::p_manage_messages);
	auto clientViewPerm   = event.command.app_permissions.has(dpp::p_read_message_history);

	if (gFind == nullptr)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "Guild not found!", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	if (!gFind->base_permissions(event.command.member).has(dpp::p_manage_messages))
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "You have lack of permission to delete messages", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	if (!clientPermission && clientViewPerm)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "I have lack of permission to delete messages", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	if (!clientViewPerm && clientPermission)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "I have lack of permission to see message history", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	if (!clientPermission && !clientViewPerm)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "I have lack of permission to delete messages and see message history", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	auto p_Component = dpp::component().set_label("Prune")
                                       .set_type(dpp::cot_button)
                                       .set_style(dpp::cos_danger)
                                       .set_emoji("success", 1036206685779398677)
                                       .set_id("p_Id");

	auto cnl_Component = dpp::component().set_label("Cancel")
                                         .set_type(dpp::cot_button)
                                         .set_style(dpp::cos_success)
                                         .set_emoji("failed", 1036206712916553748)
                                         .set_id("p_cnl_Id");

	ButtonBind(p_Component, [&client, amount, tgtReason](const dpp::button_click_t& event)
		{
			if (std::holds_alternative<std::string>(tgtReason) == true)
			{
				std::string p_Reason = std::get<std::string>(tgtReason);
				client.set_audit_reason(p_Reason);
			}
			else
			{
				std::string p_Reason = "No reason provided";
				client.set_audit_reason(p_Reason);
			}

			client.messages_get(event.command.channel_id, 0, 0, 0, amount, [&client, event](const dpp::confirmation_callback_t& callback)
				{
					std::vector<dpp::snowflake> msgIds;
					const auto msgDelMap = std::get<dpp::message_map>(callback.value);

					for (const auto& msgdel : msgDelMap) 
						msgIds.emplace_back(msgdel.first);

					client.message_delete_bulk(msgIds, event.command.channel_id);
				});

			event.reply(
				dpp::interaction_response_type::ir_update_message,
				dpp::message().set_flags(dpp::m_ephemeral)
				              .set_content("Prune completed!")
			);

			return true;
		});

	ButtonBind(cnl_Component, [](const dpp::button_click_t& event)
		{
			std::string cnlContent = "Cancelled request!";

			event.reply(
				dpp::interaction_response_type::ir_update_message,
				dpp::message().set_flags(dpp::m_ephemeral)
				              .set_content(cnlContent)
			);

			return true;
		});

	if (amount < 2)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle , warnTitle, "Cannot prune less than 2 messages!", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	if (amount > 99)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "Cannot prune more than 99 messages!", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	dpp::message p_Confirm(
		fmt::format("Do you want to prune {} messages? Press the button below to confirm", amount)
	);

	p_Confirm.add_component(
		dpp::component().add_component(p_Component)
		                .add_component(cnl_Component)
	);

	event.reply(
		p_Confirm.set_flags(dpp::m_ephemeral)
		         .set_channel_id(tgtChannel)
	);
}
