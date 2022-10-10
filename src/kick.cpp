#include <spdlog/spdlog.h>

#include "../commands/kick.h"
#include "../commands/handler.h"

dpp::snowflake guild_target;
dpp::snowflake user_targeted;
std::string reason;

void kick(dpp::cluster& client, const dpp::slashcommand_t& event)
{
	auto target_user = event.get_parameter("member");
	auto target_reason = event.get_parameter("reason");

	auto* guild_find = dpp::find_guild(event.command.guild_id);

	if (!guild_find)
	{
		event.reply(
			dpp::message()
			.set_flags(dpp::m_ephemeral)
			.set_content("Out of guild. Please try again later")
		);
	}
	else if (guild_find)
	{
		if (!(guild_find->base_permissions(&event.command.usr).has(dpp::p_kick_members)))
		{
			event.reply(
				dpp::message()
				.set_flags(dpp::m_ephemeral)
				.set_content("Who are you?")
			);
		}
	}

	dpp::message kick_confirm("Press to confirm");

	kick_confirm.add_component(
		dpp::component()
		.add_component(
			dpp::component()
			.set_label("Kick")
			.set_type(dpp::cot_button)
			.set_style(dpp::cos_danger)
			.set_id("kick_id")
		).add_component(
			dpp::component()
			.set_label("Cancel")
			.set_type(dpp::cot_button)
			.set_style(dpp::cos_secondary)
			.set_id("cancel_id")
		)
	);

	if (std::holds_alternative<std::string>(target_reason))
	{
		reason = std::get<std::string>(target_reason);
	}
	else
	{
		reason = "No kick reason provided";
	}

	event.reply(kick_confirm.set_flags(dpp::m_ephemeral));

	client.on_button_click([&client](const dpp::button_click_t& event) {
		if (event.custom_id == "kick_id")
		{
			client.set_audit_reason(reason);
			client.guild_member_kick(guild_target, user_targeted);
			std::string kick_content = fmt::format("<@{}> has been kicked!", user_targeted);

			event.reply(
				dpp::interaction_response_type::ir_update_message,
				dpp::message()
				.set_flags(dpp::m_ephemeral)
				.set_content(kick_content)
			);
		}
		else if (event.custom_id == "cancel_id")
		{
			std::string cancel_content("Cancelled request!");

			event.reply(
				dpp::interaction_response_type::ir_update_message,
				dpp::message()
				.set_flags(dpp::m_ephemeral)
				.set_content(cancel_content)
			);
		}
		});
}
