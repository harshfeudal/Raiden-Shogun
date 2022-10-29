#include <spdlog/spdlog.h>

#include "../handler/handler.h"
#include "../handler/btnHandler.h"
#include "../commands/kick.h"

void kick(dpp::cluster& client, const dpp::slashcommand_t& event)
{	
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

	if (!gFind->base_permissions(event.command.member).has(dpp::p_kick_members))
	{
		harshfeudal::SlashMessageReply(
			event, "You have lack of permission to kick", dpp::m_ephemeral, NO_MSG_TYPE
		);

		return;
	}

	if (!clientPermission)
	{
		harshfeudal::SlashMessageReply(
			event, "I have lack of permission to kick", dpp::m_ephemeral, NO_MSG_TYPE
		);

		return;
	}
	
	auto k_Component = dpp::component().set_label("Kick")
		                               .set_type(dpp::cot_button)
		                               .set_style(dpp::cos_danger)
		                               .set_id("k_Id");

	auto cnl_Component = dpp::component().set_label("Cancel")
		                                 .set_type(dpp::cot_button)
		                                 .set_style(dpp::cos_secondary)
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