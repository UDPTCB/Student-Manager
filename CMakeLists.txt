cmake_minimum_required(VERSION 3.21)
#set(CMAKE_C_COMPILER /usr/bin/gcc-15)
#set(CMAKE_CXX_COMPILER /usr/bin/g++-15)
project(Manager VERSION 1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(manager
        src/main.cpp
        src/config.cpp
        src/Score_range.cpp
        src/stu-info.cpp
        src/stu-info3.cpp
        src/sqlite3.c

        
)

#find_package(sodium REQUIRED)

