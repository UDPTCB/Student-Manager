# Copyright (C) 2026 Yvhang Cai (Jeffrey Miller) <jeffrey_miller_GD@outlook.com>
# 
# This file is part of Student Manager.
# 
# Student Manager is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# Student Manager is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with Student Manager.  If not, see <https://www.gnu.org/licenses/>.
# 
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 4.3)
#set(CMAKE_C_COMPILER /usr/bin/gcc-15)
#set(CMAKE_CXX_COMPILER /usr/bin/g++-15)
project(Manager VERSION 0.1.3 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32)
    enable_language(RC)                    
    set(APP_ICON_RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/resources/myapp.rc")
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)
#add_subdirectory(FTXUI-6.1.9)
add_executable(manager
        
        src/main.cpp
        src/config.cpp
        src/Score_range.cpp
        src/stu-info.cpp
        src/stu-info3.cpp
        src/sqlite3.c
        #src/debug.cpp
        src/logger.cpp
        src/exePath.cpp

        ${APP_ICON_RESOURCE}
)
#target_link_libraries(manager
#    PRIVATE
#    ftxui::screen
#    ftxui::dom
#    ftxui::component
#)
#target_link_libraries(manager PRIVATE 
#    stdc++exp  
#    kernel32 user32 gdi32 winspool shell32 ole32 oleaut32 uuid comdlg32 advapi32
#)
