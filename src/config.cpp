#include "../include/config.h"
#include "../include/Score_range.h"
#include "../include/stu-info3.hpp"
#include "../include/stu-info.h"
//#include "../include/debug.h"
#include "../include/logger.hpp"
#include <iostream>

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
//
    while(true){
        std::cout << "1- 0.0.1-Alpha\n2- " << StuInfo3::getVersion() << std::endl;
        std::cout << "Choose a version(Keep void to quit): ";
        std::getline(std::cin, choice);
        if (choice == "1" || choice == "1-"){
            logr.log("Unsafe, user chose old version.", 1);
            StudentInfo si;
            int returnCode = si.run(); 
            std::cout << "Return Code: " << returnCode << std::endl;
            std::cout << std::endl;
        }
        else if (choice == "2" || choice == "2-"){
            logr.log("Safe, user chose latest version", 2);
            int returnCode = stu::runStuInfo3(logr);
            std::cout << "Return Code: " << returnCode << std::endl;
            std::cout << std::endl;
        } /*else if (choice == "debug"){
            logr.log("User run debug command, it will be make something strange", 1);
            debug::debug();
            std::cout << std::endl;
        } */else {
            logr.log("User chose to exit the program.", 2);
            std::cout << "Goodbye." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            break;
        }
    } 

    logr.log("Programme stop", 2);
    

    return 0;
}