# 
# Copyright 2022 harshfeudal and The Harshfeudal Projects contributors
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License. 
# 

cmake_minimum_required(VERSION 3.15)
project(Raiden-Shogun VERSION 0.1.6.0 DESCRIPTION "A new Genshin Impact Discord bot, using Dpp version 10.0.21")

aux_source_directory(src mainsrc)
aux_source_directory(src/information information)
aux_source_directory(src/moderation moderation)
aux_source_directory(src/utility utility)

add_executable(${PROJECT_NAME} ${mainsrc} ${information} ${moderation} ${utility} )

target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/dpp.lib)
target_include_directories(${PROJECT_NAME} PRIVATE "include" "lib")

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)