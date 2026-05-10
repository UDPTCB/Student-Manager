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
