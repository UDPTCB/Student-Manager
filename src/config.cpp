/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2026 Yvhang Cai(Jeffrey Miller) jeffrey_miller_GD@outlook.com
 * 
 * This file is part of Student Manager.
 * 
 * Student Manager is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Student Manager is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Student Manager.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "../include/config.h"
#include "../include/Score_range.h"
#include "../include/stu-info3.hpp"
#include "../include/stu-info.h"
//#include "../include/debug.h"
#include "../include/logger.hpp"
#include <iostream>
void Main::printGPLNotice(){
    std::cout << "Student Manager Copyright (C) 2026 Yvhang Cai(Jeffrey Miller)" << std::endl;
    std::cout << "This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'." << std::endl;
    std::cout << "This is free software, and you are welcome to redistribute it" << std::endl;
    std::cout << "it under certain conditions; type `show c' for details." << std::endl;
    std::cout << std::endl;
}
int Main::run(int argc, char **argv){
    std::filesystem::path exe_path   = get_executable_path();
    std::filesystem::path log_path= exe_path / "logs";
    if (!std::filesystem::exists(log_path)){
        if (!std::filesystem::create_directory(log_path)){
            std::cerr << "[ERROR] : Failed to create log path" << std::endl;
        }
    }
    std::string time = logger::getTimestampForFilename();
    std::filesystem::path logs = log_path / (time + ".log");
    logger logr(logs);
    std::filesystem::path studir = exe_path / "Student";
    if (!std::filesystem::exists(studir)) {
        if (!std::filesystem::create_directory(studir)){

        }
    }

    logr.log("Programme running", 2);
    std::string choice;
//  print GPL notice
    printGPLNotice();
    while(true){
        std::cout << "Please choose the version of the program you want to run: " << std::endl;
        std::cout << "1- 0.0.1-ALPHA\n2- " << StuInfo3::getVersion() << std::endl;
        std::cout << ">>># (Keep void to quit): ";
        std::getline(std::cin, choice);
        if (choice == "1" || choice == "1-" || choice == "0.0.1-ALPHA"){
            logr.log("Unsafe, user chose old version.", 1);
            StudentInfo si;
            int returnCode = si.run(); 
            std::cout << "Return Code: " << returnCode << std::endl;
            std::cout << std::endl;
        }
        else if (choice == "2" || choice == "2-" || choice == StuInfo3::getVersion()){
            logr.log("Safe, user chose latest version", 2);
            int returnCode = stu::runStuInfo3(logr);
            std::cout << "Return Code: " << returnCode << std::endl;
            std::cout << std::endl;
        } /*else if (choice == "debug"){
            logr.log("User run debug command, it will be make something strange", 1);
            debug::debug();
            std::cout << std::endl;
        } */else if (choice == "show w"){
            std::cout << "\nNO WARRANTY" << std::endl;
            std::cout << "This program is distributed in the hope that it will be useful," << std::endl;
            std::cout << "but WITHOUT ANY WARRANTY; without even the implied warranty of" << std::endl;
            std::cout << "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << std::endl;
            std::cout << "See the GNU General Public License for more details." << std::endl;
            std::cout << std::endl;
        }   else if (choice == "show c"){
            std::cout << "\nREDISTRIBUTION CONDITIONS" << std::endl;
            std::cout << "This program is free software: you can redistribute it and/or modify" << std::endl;
            std::cout << "it under the terms of the GNU General Public License as published by" << std::endl;
            std::cout << "the Free Software Foundation, either version 3 of the License, or" << std::endl;
            std::cout << "(at your option) any later version." << std::endl;
            std::cout << std::endl;
        }   else {
            logr.log("User chose to exit the program.", 2);
            std::cout << "Goodbye." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            break;
        }
    } 

    logr.log("Programme stop", 2);
    

    return 0;
}