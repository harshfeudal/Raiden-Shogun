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

#include "../../commands/information/userinfo.h"
#include "../../handler/handler.h"

// TO DO
// 1. Try hard to fix the pointer, since it is getting error
// 2. Make the code "shorter" if possible

void userInfo(dpp::cluster& client, const dpp::slashcommand_t& event)
{   
	dpp::embed embed;

    const auto errorTitle            = "<:failed:1036206712916553748> Error";
	const auto warnTitle             = "Warning message";
	
	const auto cmdUser               = event.command.usr;
    auto       usrId                 = cmdUser.id;

    const auto StaffBadge            = "<:BadgeStaff:1043810278564970568>";
    const auto PartnerBadge          = "<:BadgePartner:1043810326862381078>";
    const auto CertifiedMod          = "<:BadgeCertifiedMod:1043811850925658162>";
    
    const auto EventBadge            = "<:BadgeHypeSquadEvents:1043817210499584050>";
    const auto EarlyVerifiedBotDev   = "<:BadgeEarlyVerifiedBotDeveloper:1043820046318846062>";
    const auto EarlySupporter        = "<:BadgeEarlySupporter:1043820083186778162>";
    
    const auto NitroSubscriber       = "<:BadgeNitro:1043797639050834060>";
    const auto DiscordBugHunterGreen = "<:BadgeBugHunter:1043820183581638686>";
    const auto DiscordBugHunterGold  = "<:BadgeBugHunterLvl2:1043820241517563956>";
    
    const auto HypesquadBravery      = "<:BadgeBravery:1043798197908291645>";
    const auto HypesquadBalance      = "<:BadgeBalance:1043797533060767835>";
    const auto HypesquadBrilliance   = "<:BadgeBrilliance:1043798261137408060>";
    
    auto       hasStaffBadge         = "";
    auto       hasPartnerBadge       = "";
    auto       hasModBadge           = "";
               
    auto       hasEventBadge         = "";
    auto       hasHouseBadge         = "";
    auto       hasBugHunterBadge     = "";
               
    auto       hasBotDevBadge        = "";
    auto       hasEarlySupBadge      = "";
    auto       hasNitroBadge         = "";
               
    auto       hasBoostBadge         = "";

    // If the command user is trying to get another people information
    if (std::holds_alternative<dpp::snowflake>(event.get_parameter("user")) == true)
        usrId = std::get<dpp::snowflake>(event.get_parameter("user"));

    const auto tgtId                 = dpp::find_user(usrId);

    // If cannot find the user
	if (!tgtId)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "User not found!", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);
    
		return;
	}

    // This is under-investigation [Library bug]
    if (tgtId->is_discord_employee())
        hasStaffBadge = StaffBadge;
    
    if (tgtId->is_partnered_owner())
        hasPartnerBadge = PartnerBadge;
    
    if (tgtId->is_certified_moderator())
        hasModBadge = CertifiedMod;
    
    if (tgtId->has_hypesquad_events())
        hasEventBadge = EventBadge;

    // Only HypeSquad works
    if (tgtId->is_house_balance())
        hasHouseBadge = HypesquadBalance;
    else if (tgtId->is_house_brilliance())
        hasHouseBadge = HypesquadBrilliance;
    else if (tgtId->is_house_bravery())
        hasHouseBadge = HypesquadBravery;
    
    if (tgtId->is_bughunter_1())
        hasBugHunterBadge = DiscordBugHunterGreen;
    else if (tgtId->is_bughunter_2())
        hasBugHunterBadge = DiscordBugHunterGold;
    
    if (tgtId->is_verified_bot_dev())
        hasBotDevBadge = EarlyVerifiedBotDev;
    
    if (tgtId->is_early_supporter())
        hasEarlySupBadge = EarlySupporter;

    if (tgtId->has_nitro_basic() || tgtId->has_nitro_classic() || tgtId->has_nitro_full())
        hasNitroBadge = NitroSubscriber;

    const auto avatar    = tgtId->get_avatar_url();
    const auto usrID     = fmt::format("{}", tgtId->id);
    const auto created   = fmt::format("<t:{}:R>", round(tgtId->get_creation_time()));
    const auto usrName   = fmt::format("{}", tgtId->format_username());
    
    // The badge cannot show now because the pointer is error ... will fix it ASAP
    auto       BadgeShow = fmt::format("{}{}{}{}{}{}{}{}{}{}",
        hasStaffBadge,     hasPartnerBadge,  hasModBadge,      hasEventBadge, hasHouseBadge,
        hasBugHunterBadge, hasBotDevBadge,   hasEarlySupBadge, hasNitroBadge, hasBoostBadge
    );

    // Check if the user doesn't have any badge
    if (BadgeShow == "")
        BadgeShow = "No badge found!";

    // Add view profile linking button
    auto       linkComponent = dpp::component();
    const auto profileURL    = fmt::format("discord://-/users/{}", tgtId->id);

    linkComponent.set_label("View profile")
                 .set_type(dpp::cot_button)
                 .set_emoji("MessageLink", 1045584835420368896)
                 .set_style(dpp::cos_link)
                 .set_url(profileURL);

    EmbedInfoBuild(embed, avatar, usrName, usrID, created, BadgeShow, cmdUser);

    event.reply(
        dpp::message().add_embed(embed).add_component(dpp::component().add_component(linkComponent))
    );
}
