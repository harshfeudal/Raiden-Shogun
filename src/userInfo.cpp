#include <spdlog/spdlog.h>

#include "../commands/userInfo.h"
#include "../handler/handler.h"

void userInfo(dpp::cluster& client, const dpp::slashcommand_t& event)
{
	auto tgtUser = event.command.usr;
	dpp::embed embed;

	if (std::holds_alternative<dpp::snowflake>(event.get_parameter("id")) == true)
	{
		auto usr_id = event.get_parameter("id");
		auto tgtId  = dpp::find_user(std::get<dpp::snowflake>(usr_id));

		std::string content = fmt::format(
			"**User ID:** {}\n**Username:** {}\n**Created:** <t:{}:f>",
			tgtId->id, tgtId->format_username(), round(tgtId->get_creation_time())
		);

		harshfeudal::SlashReplyEmbed(
			embed, client, event,
			"User Information",
			BLANK_STRING, BLANK_STRING, BLANK_STRING,
			tgtId->get_avatar_url(), content,
			tgtUser.username, tgtUser.get_avatar_url(), 0xAA7EEE
		);

		event.reply(
			dpp::message(event.command.channel_id, embed)
		);
	}
	else
	{
		std::string content = fmt::format(
			"**User ID:** {}\n**Username:** {}\n**Created:** <t:{}:f>",
			tgtUser.id, tgtUser.format_username(), round(tgtUser.get_creation_time())
		);

		harshfeudal::SlashReplyEmbed(
			embed, client, event,
			"User Information",
			BLANK_STRING, BLANK_STRING, BLANK_STRING,
			tgtUser.get_avatar_url(), content,
			tgtUser.username, tgtUser.get_avatar_url(), 0xAA7EEE
		);

		event.reply(
			dpp::message(event.command.channel_id, embed)
		);
	}
}
