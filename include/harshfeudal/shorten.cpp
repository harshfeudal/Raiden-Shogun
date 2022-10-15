#include "shorten.h"

void MessageReply(const dpp::slashcommand_t& event, std::string content, bool ephemeral)
{
	if (ephemeral == true)
	{
		event.reply(
			dpp::message()
			.set_flags(dpp::m_ephemeral)
			.set_content(content)
		);
	}
	else
	{
		event.reply(
			dpp::message()
			.set_content(content)
		);
	}
}
