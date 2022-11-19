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

#define ID_SPACING "###"

struct Session
{
    time_t   created_at;
    uint16_t cache_duration;

    std::function<bool(const dpp::button_click_t&)> function;

    Session() : created_at(time(nullptr)), cache_duration(10) {}

    // If expired
    bool isExpired() const
    {
        return difftime(time(nullptr), created_at) > cache_duration * 60;
    }
};

inline std::unordered_map<uint64_t, Session> cachedSessions;
inline std::shared_mutex                     cachedSessionsMutex;
inline uint64_t                              customIdCounter;

inline void ButtonClear() 
{
	std::unique_lock l(cachedSessionsMutex);

	auto it = cachedSessions.begin();

	// Time check button to clear
	while (it != cachedSessions.end()) 
	{
		if (it->second.isExpired()) 
		{
			assert(!cachedSessions.empty());
			it = cachedSessions.erase(it);
		}
		else ++it;
	}
}

inline void ButtonBind(dpp::component& component, const std::function<bool(const dpp::button_click_t&)>& function, uint16_t cache_duration = 10)
{
	ButtonClear();
	
	std::unique_lock l(cachedSessionsMutex);
	bool              customIdAlreadyExists;

	do 
	{
		// Checking initial button create
		if (customIdCounter >= UINT_LEAST64_MAX) 
			customIdCounter = 0;

		customIdCounter++;
		customIdAlreadyExists = cachedSessions.find(customIdCounter) != cachedSessions.end();

		// Checking if existence
		if (!customIdAlreadyExists) 
		{
			component.custom_id    = std::to_string(customIdCounter);

            Session session;

			session.function       = function;
			session.cache_duration = cache_duration;

			component.custom_id    += ID_SPACING + std::to_string(static_cast<long int>(session.created_at));

			cachedSessions[customIdCounter] = session;
			customIdAlreadyExists           = false;
		}
	} 
	while (customIdAlreadyExists);
}

inline void ButtonHandle(const dpp::button_click_t& event)
{
	uint64_t          customId;
	time_t            creationTimestamp;

	try 
	{
		const std::string id       = event.custom_id.substr(0, event.custom_id.find(ID_SPACING));
		const std::string creation = event.custom_id.substr(event.custom_id.find(ID_SPACING) + std::strlen(ID_SPACING), std::string::npos);

		customId          = std::stoul(id);
		creationTimestamp = std::stol(creation);
	}
	catch (std::out_of_range& e) 
	{
		event.reply(
			dpp::message().set_content("Button not found!")
			.set_flags(dpp::m_ephemeral)
		);

		return;
	}
	catch (std::invalid_argument& e) 
	{
		event.reply(
			dpp::message().set_content("Button not found!")
			.set_flags(dpp::m_ephemeral)
		);

		return;
	}

	std::unique_lock             l(cachedSessionsMutex);
	const auto existing = cachedSessions.find(customId);

	if (existing != cachedSessions.end() && existing->second.created_at == creationTimestamp && !existing->second.isExpired()) 
	{
		bool forget = existing->second.function(event);

		// If forget the button
		if (forget) 
		{
			cachedSessions.erase(existing);
		}
	}
}
