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

#pragma once

#include <dpp/dpp.h>

inline void EmbedInfoBuild(dpp::embed& embed, std::string avatar, std::string usrName, std::string usrID, std::string created, std::string BadgeShow, const dpp::user& tgtUser)
{
    embed = dpp::embed();

    embed.set_color(0xAA7EEE)
         .set_title("User Information")
         .set_thumbnail(avatar)
         .add_field("Username", usrName, true).add_field("User ID", usrID, true).add_field("Created", created, true)
         .add_field("Badges", BadgeShow)
         .set_footer(dpp::embed_footer().set_text(tgtUser.username).set_icon(tgtUser.get_avatar_url()))
         .set_timestamp(time(nullptr));
}


inline std::string StringFormatter(std::string &str)
{
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

void infouser(dpp::cluster& client, const dpp::slashcommand_t& event);
