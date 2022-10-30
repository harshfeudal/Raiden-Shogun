#include <spdlog/spdlog.h>

#include "../commands/userInfo.h"
#include "../handler/handler.h"

void EmbedBuild(dpp::embed& embed, std::string content, const dpp::user& tgtUser)
{
	embed = dpp::embed().set_color(0xAA7EEE)
							.set_title("User Information")
							.set_thumbnail(tgtUser.get_avatar_url())
							.set_description(content)
							.set_footer(dpp::embed_footer().set_text(tgtUser.username)
														   .set_icon(tgtUser.get_avatar_url()))
							.set_timestamp(time(nullptr));
}

// I will soon add more details

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

		EmbedBuild(embed, content, tgtUser);
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

		EmbedBuild(embed, content, tgtUser);
		event.reply(
			dpp::message(event.command.channel_id, embed)
		);
	}
}
