#include <spdlog/spdlog.h>

#include "../handler/handler.h"
#include "../handler/btnHandler.h"
#include "../commands/moderation/prune.h"

void prune(dpp::cluster& client, const dpp::slashcommand_t& event)
{
	auto tgtChannel       = event.command.channel_id;
	auto gFind            = dpp::find_guild(event.command.guild_id);
	auto clientPermission = event.command.app_permissions.has(dpp::p_manage_messages);

	if (gFind == nullptr)
	{
		harshfeudal::SlashMessageReply(
			event, "Guild not found!", dpp::m_ephemeral, NO_MSG_TYPE
		);

		return;
	}

	if (!gFind->base_permissions(event.command.member).has(dpp::p_manage_messages))
	{
		harshfeudal::SlashMessageReply(
			event, "You have lack of permission to prune", dpp::m_ephemeral, NO_MSG_TYPE
		);

		return;
	}

	if (!clientPermission)
	{
		harshfeudal::SlashMessageReply(
			event, "I have lack of permission to prune", dpp::m_ephemeral, NO_MSG_TYPE
		);

		return;
	}

	auto p_Component = dpp::component().set_label("Prune")
						.set_type(dpp::cot_button)
						.set_style(dpp::cos_danger)
						.set_emoji("Rtick", 1036206685779398677)
						.set_id("p_Id");

	auto cnl_Component = dpp::component().set_label("Cancel")
						.set_type(dpp::cot_button)
						.set_style(dpp::cos_secondary)
						.set_emoji("Rcross", 1036206712916553748)
						.set_id("p_cnl_Id");

	ButtonBind(p_Component, [&client](const dpp::button_click_t& event)
		{
			

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

	// working in progress ...
	dpp::message p_Confirm(
		fmt::format("Do you want to prune {} message(s)? Press the button below to confirm")	// Grammar check soon!
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