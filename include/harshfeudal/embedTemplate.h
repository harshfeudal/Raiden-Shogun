#pragma once

namespace harshfeudal
{
	inline void SlashReplyEmbed(
		dpp::embed& embed,    dpp::cluster& client, const dpp::slashcommand_t& event, std::string title, 
		std::string username, std::string url,      std::string avtURL,               std::string thumbnail, 
		std::string content,  std::string text,     std::string icon,                 uint32_t color
	)
	{
		embed = dpp::embed().set_color(color)
			.set_title(title)
			.set_author(username, url, avtURL)
			.set_thumbnail(thumbnail)
			.set_description(content)
			.set_footer(dpp::embed_footer().set_text(text)
				.set_icon(icon))
			.set_timestamp(time(nullptr));
	}
}