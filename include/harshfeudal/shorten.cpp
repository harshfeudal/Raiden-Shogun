#include "shorten.h"

void MessageReply(const dpp::slashcommand_t& event, std::string content, dpp::message_flags messageFlag)
{
	event.reply(
		dpp::message()
		.set_flags(messageFlag)
		.set_content(content)
	);
}

void MessageEmbed(const dpp::slashcommand_t& event)
{

}
