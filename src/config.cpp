#include "../include/config.h"
#include "../include/Score_range.h"
#include "../include/stu-info3.hpp"
#include "../include/stu-info.h"
#include <iostream>

int Main::run(int argc, char **argv){
    std::filesystem::path exe_path = std::filesystem::current_path();
    std::filesystem::path student_dir = exe_path / "Student";
    if (!std::filesystem::exists(student_dir)) {
        std::filesystem::create_directory(student_dir);
    }

    std::string choice;

    while(true){
        std::cout << "1- 0.0.1-ALPHA\n2- 0.1.1-ALPHA" << std::endl;
        std::cout << "Choose a version(Keep void to quit): ";
        std::getline(std::cin, choice);
        if (choice == "1" || choice == "1-"){
            StudentInfo si;
            int returnCode = si.run(); 
            std::cout << "Return Code: " << returnCode << std::endl;
            std::cout << std::endl;
        }
        else if (choice == "2" || choice == "2-"){
            int returnCode = StuInfo3::run();
            std::cout << "Return Code: " << returnCode << std::endl;
            std::cout << std::endl;
        }else {
            std::cout << "Googbye." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            break;
        }
    } 

    
    

    return 0;
}