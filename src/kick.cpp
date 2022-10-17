#include <spdlog/spdlog.h>

#include "../commands/handler.h"
#include "../commands/kick.h"

void kick(dpp::cluster& client, const dpp::slashcommand_t& event)
{
	dpp::snowflake guild_target;
	dpp::snowflake user_targeted;
	std::string reason;

	auto trgUser = event.get_parameter("member");
	auto trgReason = event.get_parameter("reason");

	auto* gFind = dpp::find_guild(event.command.guild_id);

	if (!gFind)
	{
		harshfeudal::SlashMessageReply(
			event, "No guild found", dpp::m_ephemeral, dpp::mt_default
		);
	}

	auto permissionCheck = gFind->base_permissions(&event.command.usr).has(dpp::p_kick_members);
	if (!permissionCheck)
	{
		harshfeudal::SlashMessageReply(
			event, "You have lack of permission to kick", dpp::m_ephemeral, NOMSGTYPE
		);
	}

	dpp::message kickConfirm("Do you want to kick? Press the button below to confirm");

	kickConfirm.add_component(
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
		reason = std::get<std::string>(trgReason); 
	}
	else { reason = "No kick reason provided"; }

	event.reply(
		kickConfirm.set_flags(dpp::m_ephemeral)
	);

	// Working in progress ...
}
