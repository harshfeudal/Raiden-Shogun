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

	if (!gFind)
		harshfeudal::SlashMessageReply(
			event, "No guild found", dpp::m_ephemeral, NO_MSG_TYPE
		);

	if (gFind)
	{
		auto permissionCheck = gFind->base_permissions(&event.command.usr).has(dpp::p_kick_members);
		if (!permissionCheck)
		{
			harshfeudal::SlashMessageReply(
				event, "You have lack of permission to kick", dpp::m_ephemeral, NO_MSG_TYPE
			);
		}
	}

	dpp::message k_Confirm("Do you want to kick? Press the button below to confirm");

	// harshfeudal::ButtonBasicCreate(k_Confirm, "Kick", dpp::cos_danger, "k_kickId", false, NULL);
	// harshfeudal::ButtonBasicCreate(k_Confirm, "Cancel", dpp::cos_secondary, "k_cancelId", false, NULL);

	k_Confirm.add_component(
		dpp::component().add_component(
			dpp::component()
			.set_label("Kick")
			.set_type(dpp::cot_button)
			.set_style(dpp::cos_danger)
			.set_id("kickId")
		).add_component(
			dpp::component()
			.set_label("Cancel")
			.set_type(dpp::cot_button)
			.set_style(dpp::cos_secondary)
			.set_id("cancelId")
		)
	);

	if (std::holds_alternative<std::string>(trgReason)) 
	{ 
		k_Reason = std::get<std::string>(trgReason); 
	}
	else { k_Reason = "No kick reason provided"; }

	event.reply(
		k_Confirm.set_flags(dpp::m_ephemeral)
	);

	// Working in progress ...
}
