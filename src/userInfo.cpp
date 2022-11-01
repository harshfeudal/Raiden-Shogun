#include <spdlog/spdlog.h>

#include "../commands/userInfo.h"
#include "../handler/handler.h"

void EmbedBuild(dpp::embed& embed, std::string avatar, std::string usrName, std::string usrID, std::string created, const dpp::user& tgtUser)
{
	embed = dpp::embed().set_color(0xAA7EEE)
						.set_title("User Information")
						.set_thumbnail(avatar)
						.add_field("Username", usrName, true).add_field("User ID", usrID, true).add_field("Created", created, true)
						.set_footer(dpp::embed_footer().set_text(tgtUser.username).set_icon(tgtUser.get_avatar_url()))
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

		std::string avatar  = tgtId->get_avatar_url();
		std::string usrID   = fmt::format("{}", tgtId->id);
		std::string created = fmt::format("<t:{}:R>", round(tgtId->get_creation_time()));
		std::string usrName = fmt::format("{}", tgtId->format_username());

		EmbedBuild(embed, avatar, usrName,usrID, created, tgtUser);
		event.reply(
			dpp::message(event.command.channel_id, embed)
		);
	}
	else
	{
		auto tgtId = event.command.usr;

		std::string avatar  = tgtId.get_avatar_url();
		std::string usrID   = fmt::format("{}", tgtId.id);
		std::string created = fmt::format("<t:{}:R>", round(tgtId.get_creation_time()));
		std::string usrName = fmt::format("{}", tgtId.format_username());

		EmbedBuild(embed, avatar, usrName, usrID, created, tgtUser);
		event.reply(
			dpp::message(event.command.channel_id, embed)
		);
	}
}
