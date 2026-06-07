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

cmake_minimum_required(VERSION 3.21)
#set(CMAKE_C_COMPILER /usr/bin/gcc-15)
#set(CMAKE_CXX_COMPILER /usr/bin/g++-15)
project(Manager VERSION 0.1.6 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32)
    enable_language(RC)                    
    set(APP_ICON_RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/resources/myapp.rc")
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)
if(MINGW)
    message(STATUS "Static linking for MSYS2/UCRT64")
    set(CMAKE_CXX_FLAGS "-static-libgcc -static-libstdc++ ${CMAKE_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "-static ${CMAKE_EXE_LINKER_FLAGS}")
endif()


include(FetchContent)

FetchContent_Declare(
    ftxui    
    GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
    GIT_TAG v6.1.9            
)

#set(FTXUI_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
#set(FTXUI_BUILD_DOCS OFF CACHE BOOL "" FORCE)
#set(FTXUI_BUILD_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

add_executable(manager
        
        src/main.cpp
        src/config.cpp
        src/Score_range.cpp
        src/stu-info.cpp
        src/stu-info3.cpp
        src/sqlite3.c
        src/debug.cpp
        src/logger.cpp
        src/exePath.cpp
        src/tui.cpp

        ${APP_ICON_RESOURCE}
)
target_link_libraries(manager PRIVATE
   ftxui::screen

    ftxui::dom

    ftxui::component
)

install(TARGETS manager
    RUNTIME DESTINATION .  # Install to installation root directory, not bin
)

# Create empty Student and logs directories after installation
# Create empty Student and logs directories after installation
install(CODE "
    # Get the installation path
    set(INSTALL_DIR \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}\")
    
    # Create Student directory
    file(MAKE_DIRECTORY \"\${INSTALL_DIR}/Student\")
    
    # Create logs directory
    file(MAKE_DIRECTORY \"\${INSTALL_DIR}/logs\")
    
    # Create a .gitkeep or empty file in logs directory to ensure directory is included in the installation package
    file(WRITE \"\${INSTALL_DIR}/logs/.gitkeep\" \"\")
    
    message(STATUS \"Created directories: Student and logs in \${INSTALL_DIR}\")
" COMPONENT runtime)

# For Windows (NSIS), add script to remove extra directories during uninstallation
if(WIN32)
    set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
        # Delete Student directory and its contents during uninstallation
        RMDir /r \\\"$INSTDIR\\\\Student\\\"
        
        # Delete logs directory and its contents during uninstallation
        RMDir /r \\\"$INSTDIR\\\\logs\\\"
        
        # Attempt to delete installation root directory (if empty)
        RMDir \\\"$INSTDIR\\\"
    ")
endif()

# ... CPack configuration section remains unchanged ...

include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "StudentManager")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Student Information Management System - command line tool")
set(CPACK_PACKAGE_VENDOR "Yvhang Cai(Jeffrey Miller)")
set(CPACK_PACKAGE_CONTACT "jeffrey_miller_GD@outlook.com")

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
endif()
if(WIN32)
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_PACKAGE_NAME "StudentManager")
    set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
    
   
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
    
   
    set(CPACK_NSIS_PACKAGE_NAME "Student Manager")
    set(CPACK_NSIS_DISPLAY_NAME "Student Manager")
    
    
    set(CPACK_NSIS_UNINSTALL_NAME "Uninstall-${PROJECT_VERSION}")
    
    set(CPACK_NSIS_CONTACT "jeffrey_miller_GD@outlook.com")
    set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
    
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources/myapp.ico")
        set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/resources/myapp.ico")
        set(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/resources/myapp.ico")
    endif()
endif()

if(UNIX AND NOT APPLE)
    set(CPACK_GENERATOR "TGZ")
endif()

include(CPack)