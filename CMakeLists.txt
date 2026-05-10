# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2026 Yvhang Cai(Jeffrey Miller) jeffrey_miller_GD@outlook.com
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


install(TARGETS manager
    RUNTIME DESTINATION bin
)


include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "StudentManager")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Student Information Management System - command line tool")
set(CPACK_PACKAGE_VENDOR "Yvhang Cai")
set(CPACK_PACKAGE_CONTACT "jeffrey_miller_GD@outlook.com")

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
endif()

if(WIN32)
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_NSIS_PACKAGE_NAME "Student Manager")
    set(CPACK_NSIS_DISPLAY_NAME "Student Manager")
    set(CPACK_NSIS_CONTACT "jeffrey_miller_GD@outlook.com")
    set(CPACK_NSIS_MODIFY_PATH ON)  
    
   
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources/myapp.ico")
        set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/resources/myapp.ico")
        set(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/resources/myapp.ico")
    endif()
endif()

if(UNIX AND NOT APPLE)
    set(CPACK_GENERATOR "TGZ")  
endif()

include(CPack)