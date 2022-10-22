#include <spdlog/spdlog.h>

#include "../commands/handler.h"
#include "../commands/kick.h"

void kick(dpp::cluster& client, const dpp::slashcommand_t& event)
{
	dpp::snowflake tgtGuild;
	dpp::snowflake tgtUser;
	std::string    k_Reason;

	auto trgUser   = event.get_parameter("member");
	auto trgReason = event.get_parameter("reason");
	auto gFind     = dpp::find_guild(event.command.guild_id);

	if (gFind)
	{
		auto permissionCheck = gFind->base_permissions(&event.command.usr).has(dpp::p_kick_members);
		if (!permissionCheck)
		{
			harshfeudal::SlashMessageReply(
				event, "You have lack of permission to kick", dpp::m_ephemeral, NO_MSG_TYPE, dpp::ir_default
			);
		}
	}

	dpp::message k_Confirm("Do you want to kick? Press the button below to confirm");

	k_Confirm.add_component(
		dpp::component().add_component(
			dpp::component().set_label("Kick")
			                .set_type(dpp::cot_button)
			                .set_style(dpp::cos_danger)
			                .set_id("k_Id")
		).add_component(
			dpp::component().set_label("Cancel")
			                .set_type(dpp::cot_button)
			                .set_style(dpp::cos_secondary)
			                .set_id("cnl_Id")
		)
	);

	event.reply(
		k_Confirm.set_flags(dpp::m_ephemeral)
	);

	if (!std::holds_alternative<std::string>(trgReason)) 
		k_Reason = "No kick reason provided";

	k_Reason = std::get<std::string>(trgReason);

	client.on_button_click([&client, k_Reason, tgtGuild, tgtUser](const dpp::button_click_t& btnEvent)
		{
			if (btnEvent.custom_id == "kick_id")
			{
				client.set_audit_reason(k_Reason);
				client.guild_member_kick(tgtGuild, tgtUser);
				std::string kContent = fmt::format("<@{}> has been kicked!", tgtUser);

				harshfeudal::ButtonMessageReply(
					btnEvent, kContent, dpp::m_ephemeral, NO_MSG_TYPE, dpp::ir_update_message
				);
			}
			else if (btnEvent.custom_id == "cancel_id")
			{
				std::string cnlContent("Cancelled request!");

				harshfeudal::ButtonMessageReply(
					btnEvent, cnlContent, dpp::m_ephemeral, NO_MSG_TYPE, dpp::ir_update_message
				);
			}
		});

	// Working in progress ...
}
