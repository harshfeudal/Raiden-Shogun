/*
 * Copyright 2022 harshfeudal and The Harshfeudal Projects contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <spdlog/spdlog.h>
#include <dpp/dpp.h>

#include "../../handler/handler.h"
#include "../../commands/moderation/timeout.h"

void timeout(dpp::cluster& client, const dpp::slashcommand_t& event)
{
	dpp::embed embed;

	const auto errorTitle       = "<:failed:1036206712916553748> Error!";
	const auto successTitle     = "<:success:1036206685779398677> Success!";
	const auto warnTitle        = "Warning message";
	
	const auto duration         = std::get<std::string>(event.get_parameter("duration"));
	const auto usr              = std::get<dpp::snowflake>(event.get_parameter("member"));
	const auto tgtReason        = event.get_parameter("reason");

	const auto source           = event.command.usr.id;
	const auto tgtGuild         = event.command.guild_id;
	const auto tgtChannel       = event.command.channel_id;
	const auto clientPermission = event.command.app_permissions.has(dpp::p_moderate_members);

	const auto gFind            = dpp::find_guild(event.command.guild_id);
	const auto tgtUser          = gFind->members.find(usr);

	if (tgtUser == gFind->members.end())
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "Member not found!", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	if (gFind == nullptr)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "Guild not found!", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	if (!gFind->base_permissions(event.command.member).has(dpp::p_moderate_members))
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "You have lack of permission to timeout", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	if (!clientPermission)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "I have lack of permission to timeout", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	if (usr == gFind->owner_id)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "You cannot timeout the owner", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	if (usr == source)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "You cannot timeout yourself", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	if (usr == client.me.id)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "Why do you timeout me :(", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	// Time format engine
	// Working in progress ...

	// Making a function to input number string and automatically output as minute

	std::string   input   = duration;

    uint64_t sec          = 0;
    uint64_t temp         = 0;

    bool     bSyntax      = 0;
	bool     error        = 0;

	int DayCount          = 0;
	int HourCount         = 0;
	int MinuteCount       = 0;
	int SecondCount       = 0;

	// Automatically convert if no day format
	if (isNumber(input))
	{
		sec += temp * 60;
        temp = 0;
        bSyntax = 0;
	}

    for(int a : input)
    {
        a = tolower(a);

        if('0' <= a && a <= '9')
        {
            temp = temp * 10 + (a - '0');
            bSyntax = 1;
        }
        else if(a == 'd' && bSyntax)
        {
            sec += temp * 86400;
            temp = 0;
            bSyntax = 0;
        }
        else if(a == 'h' && bSyntax)
        {
            sec += temp * 3600;
            temp = 0;
            bSyntax = 0;
        }
        else if(a == 'm' && bSyntax)
        {
            sec += temp * 60;
            temp = 0;
            bSyntax = 0;
        }
        else if(a == 's' && bSyntax)
        {
            sec += temp;
            temp = 0;
            bSyntax = 0;
        }
		else
			error = true;
    }

	for (int i = 0; i < input.size(); i++)
	{
		if (isdigit(input[i]))
			if (input[i + 1] == 'd')
				DayCount++;
			else if (input[i + 1] == 'h')
				HourCount++;
			else if (input[i + 1] == 'm')
				MinuteCount++;
			else if (input[i + 1] == 's')
				SecondCount++;
			else
				break;
	}

	std::cout << DayCount << std::endl;
	std::cout << HourCount << std::endl;
	std::cout << MinuteCount << std::endl;
	std::cout << SecondCount << std::endl;

	// Check all error cases occur
	if (std::isdigit(input[input.size() - 1]) && sec > 0)
		error = true;

	if (DayCount > 1 || HourCount > 1 || MinuteCount > 1 || SecondCount > 1)
		error = true;

	if (error)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "Wrong time format", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	if (sec > 2419200)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "Cannot timeout user over 28 days!", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	auto        toutContent = fmt::format("<@{}> has been timeouted until <t:{}:F>!", usr, time(nullptr) + sec);
    std::string tout_Reason = "No reason provided";

	if (sec == 0)
		toutContent = fmt::format("<@{}> has been un-timeouted!", usr);

	// If reason is provided
	if (std::holds_alternative<std::string>(tgtReason))
		tout_Reason = std::get<std::string>(tgtReason);

    client.set_audit_reason(tout_Reason);

	// Timeout the user
	client.guild_member_timeout(tgtGuild, usr, time(nullptr) + sec);

	event.reply(
		dpp::message().set_flags(dpp::m_ephemeral)
		              .set_content(toutContent)
	);
}
