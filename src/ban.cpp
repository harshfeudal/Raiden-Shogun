#include <spdlog/spdlog.h>

#include "../handler/handler.h"
#include "../handler/btnHandler.h"
#include "../commands/ban.h"

void ban(dpp::cluster& client, const dpp::slashcommand_t& event)
{
	auto usr       = std::get<dpp::snowflake>(event.get_parameter("member"));
	auto tgtReason = event.get_parameter("reason");

	auto source           = event.command.usr.id;
	auto gFind            = dpp::find_guild(event.command.guild_id);
	auto tgtGuild         = event.command.guild_id;
	auto tgtChannel       = event.command.channel_id;
	auto clientPermission = event.command.app_permissions.has(dpp::p_ban_members);

	const auto tgtUser = gFind->members.find(usr);

	if (tgtUser == gFind->members.end())
	{
		harshfeudal::SlashMessageReply(
			event, "Member not found!", dpp::m_ephemeral, NO_MSG_TYPE
		);

		return;
	}

	if (gFind == nullptr)
	{
		harshfeudal::SlashMessageReply(
			event, "Guild not found!", dpp::m_ephemeral, NO_MSG_TYPE
		);

		return;
	}

	if (!gFind->base_permissions(event.command.member).has(dpp::p_ban_members))
	{
		harshfeudal::SlashMessageReply(
			event, "You have lack of permission to ban", dpp::m_ephemeral, NO_MSG_TYPE
		);

		return;
	}

	if (!clientPermission)
	{
		harshfeudal::SlashMessageReply(
			event, "I have lack of permission to ban", dpp::m_ephemeral, NO_MSG_TYPE
		);

		return;
	}

	// Checking higher roles ...
	if (usr == gFind->owner_id)
	{
		harshfeudal::SlashMessageReply(
			event, "You cannot ban the owner!", dpp::m_ephemeral, NO_MSG_TYPE
		);

		return;
	}
	
	if (usr == event.command.usr.id)
	{
		harshfeudal::SlashMessageReply(
			event, "You cannot self-ban!", dpp::m_ephemeral, NO_MSG_TYPE
		);

		return;
	}

	if (usr == client.me.id)
	{
		harshfeudal::SlashMessageReply(
			event, "You can not ban me :(", dpp::m_ephemeral, NO_MSG_TYPE
		);

		return;
	}
	
	auto b_Component = dpp::component()
						.set_label("Ban")
						.set_type(dpp::cot_button)
						.set_style(dpp::cos_danger)
						.set_emoji("Rtick", 1036206685779398677)
						.set_id("b_Id");

	auto cnl_Component = dpp::component()
						.set_label("Cancel")
						.set_type(dpp::cot_button)
						.set_style(dpp::cos_secondary)
						.set_emoji("Rcross", 1036206712916553748)
						.set_id("b_cnl_Id");

	ButtonBind(b_Component, [&client, tgtGuild, tgtReason, usr, source](const dpp::button_click_t& event)
		{
			if (source != event.command.usr.id)
			{
				return false;
			}

			std::string bContent = fmt::format("<@{}> has been banned!", usr);

			if (std::holds_alternative<std::string>(tgtReason) == true)
			{
				std::string b_Reason = std::get<std::string>(tgtReason);
				client.set_audit_reason(b_Reason);
			}
			else
			{
				std::string b_Reason = "No reason provided";
				client.set_audit_reason(b_Reason);
			}

			client.guild_ban_add(tgtGuild, usr);

			event.reply(
				dpp::interaction_response_type::ir_update_message,
				dpp::message().set_flags(dpp::m_ephemeral)
				              .set_content(bContent)
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

	dpp::message b_Confirm(
		fmt::format("Do you want to ban <@{}>? Press the button below to confirm", usr)
	);

	b_Confirm.add_component(
		dpp::component().add_component(b_Component)
		                .add_component(cnl_Component)
	);

	event.reply(
		b_Confirm.set_flags(dpp::m_ephemeral)
		         .set_channel_id(tgtChannel)
	);
}