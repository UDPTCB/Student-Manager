#include "../include/config.h"
#include "../include/Score_range.h"
#include "../include/stu-info.h"
#include "../include/auth.h"
#include "../include/user_manager.h"
#include <iostream>
int Main::run(int argc, char **argv){
    std::filesystem::path exe_path = std::filesystem::current_path();
    std::filesystem::path student_dir = exe_path / "Student";
    if (!std::filesystem::exists(student_dir)) {
        std::filesystem::create_directory(student_dir);
    }


    


    UserManager um("users.json");

    std::string choice;
    std::string user, pass;

    while (true) {
        std::cout << "\n1. Signup\n2. Login\n3. Exit\n> ";
        std::getline(std::cin, choice);

        if (choice == "1") {
            std::cout << "Username: ";
            std::getline(std::cin, user);

            std::cout << "Password: ";
            std::getline(std::cin, pass);

            if (um.signup(user, pass))
                std::cout << "Signup success\n";
            else
                std::cout << "User exists\n";
        }
        else if (choice == "2") {
            std::cout << "Username: ";
            std::getline(std::cin, user);

            std::cout << "Password: ";
            std::getline(std::cin, pass);

            if (um.login(user, pass)) {
                std::cout << "Login success\n";
                StudentInfo si;
                if (si.run() == 0) {
                    break;
                }
            } else {
                std::cout << "Login failed\n";
            }
        }
        else if (choice == "3") {
            break;
        }
        else {
            std::cout << "Invalid choice\n";
        }
    }

    

    return 0;
}