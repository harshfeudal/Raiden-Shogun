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
	std::string      InitTime       = duration;
    std::string      FormatTime;
	
    std::vector<int> DayVector;
    std::vector<int> HourVector;
    std::vector<int> MinuteVector;
    std::vector<int> SecondVector;

    FormatTime = delSpaces(InitTime);

    std::cout << FormatTime << std::endl;

    int DayFormat      = 0;
	int HourFormat     = 0;
	int MinuteFormat   = 0;
	int SecondFormat   = 0;
    int count          = 0;

    bool isDay         = false;
	bool isHour        = false;
	bool isMinute      = false;
	bool isSecond      = false;
	bool time_format_e = false;

    for (char i : FormatTime) 
	{
        if (i == 'd' || i == 'D')
            isDay = true;
        else if (i == 'h'|| i == 'H')
            isHour = true;
        else if (i == 'm' || i == 'M')
            isMinute = true;
        else if (i == 's' || i == 'S')
            isSecond = true;

		if (!isalpha(i) || !isdigit(i))
			time_format_e = true;
    }

	// Working in progress ...

	// Making a function to input number string and automatically output as minute
	// And also check if they put only word but no day format, e.g input: `days` not `2 days`

	if (time_format_e)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "Wrong time format", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}
	
    if (isDay) 
	{
        for (char i : FormatTime) 
		{
            count ++;

            if (isdigit(i)) 
			{
                int Day = i - '0';
                DayVector.push_back(Day);
            }
            
			if (i == 'd' || i == 'D') 
                break;
        }
    }

    if (isHour) 
	{
        for (char i : FormatTime) 
		{
            count ++;

            if (isdigit(FormatTime[i])) 
			{
                int Hour = FormatTime[i] - '0';
                HourVector.push_back(Hour);
            }
			
			if (FormatTime[i] == 'h' || FormatTime[i] == 'H') 
                break;
        }
    }

    if (isMinute) 
	{
        for (char i : FormatTime) 
		{
            count ++;

            if (isdigit(FormatTime[i])) 
			{
                int Minute = FormatTime[i] - '0';
                MinuteVector.push_back(Minute);
            }
            
			if (FormatTime[i] == 'm' || FormatTime[i] == 'M') 
                break;
        }
    }

    if (isSecond) 
	{
        for (char i : FormatTime) 
		{
            count ++;

            if (isdigit(FormatTime[i])) 
			{
                int Second = FormatTime[i] - '0';
                SecondVector.push_back(Second);
            }
        }
    }

    for (int i = 0; i < DayVector.size(); i++)
        DayFormat += DayVector[i] * pow(10, DayVector.size() - i - 1);

    for (int i = 0; i < HourVector.size(); i++) 
        HourFormat += HourVector[i] * pow(10, HourVector.size() - i - 1);
    
    for (int i = 0; i < MinuteVector.size(); i++) 
        MinuteFormat += MinuteVector[i] * pow(10, MinuteVector.size() - i - 1);
    
    for (int i = 0; i < SecondVector.size(); i++) 
        SecondFormat += SecondVector[i] * pow(10, SecondVector.size() - i - 1);
    
    int         TimeFormat  = DayFormat * 86400 + HourFormat * 3600 + MinuteFormat * 60 + SecondFormat;

	// Test
	std::cout << TimeFormat << std::endl;

	const auto  toutContent = fmt::format("<@{}> has been timeout until <t:{}:f>!", usr, time(nullptr) + TimeFormat);
    std::string tout_Reason = "No reason provided";

	// If reason is provided
	if (std::holds_alternative<std::string>(tgtReason))
		tout_Reason = std::get<std::string>(tgtReason);

    client.set_audit_reason(tout_Reason);

	// Timeout the user
	client.guild_member_timeout(tgtGuild, usr, time(nullptr) + TimeFormat);

	event.reply(
		dpp::message().set_flags(dpp::m_ephemeral)
		              .set_content(toutContent)
	);
}
