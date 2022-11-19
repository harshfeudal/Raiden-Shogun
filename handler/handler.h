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

#include <functional>
#include <dpp/dpp.h>

using  commandFunc = std::function<void(dpp::cluster&, const dpp::slashcommand_t&)>;
struct commandDef;

// Build the embed template
inline void EmbedBuild(dpp::embed& embed, uint32_t col, std::string title, std::string fieldTitle, std::string fieldDes, const dpp::user& tgtUser)
{
	embed = dpp::embed()
	.set_color(col)
	.set_title(title)
	.add_field(fieldTitle, fieldDes)
	.set_footer(dpp::embed_footer().set_text(tgtUser.username).set_icon(tgtUser.get_avatar_url()))
	.set_timestamp(time(nullptr));
}

// Build command definition and function
struct commandDef
{
	std::string                      description;
	commandFunc                      function;
	std::vector<dpp::command_option> param = {};
};
