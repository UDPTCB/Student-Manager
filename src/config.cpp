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
#include "../include/tui.hpp"
#include "../include/logger.hpp"
#include <iostream>


int Main::run(int argc, char **argv){
    // print GPL notice
    std::filesystem::path exe_path   = get_executable_path();
    std::filesystem::path log_path= exe_path / "logs";
    if (!std::filesystem::exists(log_path)){
        if (!std::filesystem::create_directory(log_path)){
            std::cerr << "[ERROR] : Failed to create log path" << std::endl;
            return 1;
        }
    }
    std::string time = logger::getTimestampForFilename();
    std::filesystem::path logs = log_path / (time + ".log");
    logger logr(logs);
    std::filesystem::path studir = exe_path / "Student";
    if (!std::filesystem::exists(studir)) {
        if (!std::filesystem::create_directory(studir)){
            std::cerr << "[ERROR] : Failed to create student data path" << std::endl;
            logr.log("Failed to create student data path", 0);
            return 1;
        }
    }

    logr.log("Programme running", 2);
    
    if (argc >= 2){
        printGPLNotice();
        std::cout << "----------------------------------------------------------------" << std::endl;
        std::cout << "To view warranty details (show w) or redistribution conditions (show c)," << std::endl;
        std::cout << "please run the program WITHOUT any command line arguments.\n\n\a" << std::endl;
        std::filesystem::path data_file  = studir / "students.db";
        StuInfo3 manager(data_file);
        std::string_view command = argv[1];
        if (!manager.createTable()) {
            std::cerr << "Failed to create table in database" << std::endl;
            logr.log("Failed to create table in database: " + data_file.string(), 0);
            return 10;
        }
        if (!manager.migrateTable()) {          
            std::cerr << "Failed to migrate table" << std::endl;
            logr.log("Migration failed: " + data_file.string(), 0);
            return 11;
        }
        if (!manager.createTriggers()) {
            std::cerr << "Warning: Failed to create triggers" << std::endl;
            logr.log("Failed to create triggers in database: " + data_file.string(), 1);
        }
        if (command == "--version"){
            std::cout << "Student Manager version: " << StuInfo3::getVersion() << std::endl;
            logr.log("User checked version: " + StuInfo3::getVersion(), 2);
            return 0;
        } else if (command == "--add"){
            logr.log("User chose to add a student.", 2);
            StuInfo3::Student newStu;

            std::string id{};
            std::cout << "ID: ";
            std::getline(std::cin, id);
            newStu.id = id;
            if (newStu.id.empty()) {
                std::cout << "Student ID cannot be empty. Student not added." << std::endl;
                logr.log("Failed to add student: empty ID", 0);
                return 1;
            }
            
            if (manager.existsStudentByID(newStu.id)){
                std::cout << "A student with ID " << newStu.id << " already exists.\n";
                std::cout << "Student not added." << std::endl;
                logr.log("Add student cancelled by user: " + newStu.id, 1);
                return 1;
            }


            

            manager.enterStudent(newStu);
            
           

            if (manager.insertStudent(newStu)){
                std::cout << "Student added successfully." << std::endl;
                logr.log("Student added: " + newStu.name + " (ID: " + newStu.id + ")", 2);
            } else {
                std::cerr << "Failed to add student." << std::endl;
                logr.log("Failed to add student: " + newStu.name, 0);
            }
            return 0;
        } else if (command == "--delete"){
            logr.log("User chose to delete a student.", 2);
            if (argc == 3) {
                std::string id = argv[2];
                if (!manager.deleteStudentByID(id)) {
                    std::cerr << "Not found" << std::endl;
                    logr.log("Failed to delete student with ID: " + id, 0);
                    return 1;
                }
                logr.log("Successfully deleted student with ID: " + id, 2);
                return 0;
            } else {
                std::string id{};
                std::cout << "ID: ";
                std::getline(std::cin, id);
                if (!manager.deleteStudentByID(id)) {
                    std::cerr << "Not found" << std::endl;
                    logr.log("Failed to delete student with ID: " + id, 0);
                    return 1;
                }
                logr.log("Successfully deleted student with ID: " + id, 2);
                return 0;
            }
        } else if (command == "--edit"){
            logr.log("User chose to edit a student.", 2);
            if (argc == 3) {
                std::string id = argv[2];
                if (!manager.editStudentByID(id)) {
                    std::cerr << "Failed" << std::endl;
                    logr.log("Failed to edit student with ID: " + id, 0);
                    return 1;
                }
                logr.log("Successfully edited student with ID: " + id, 2);
                return 0;
            } else {
                std::string id{};
                std::cout << "ID: ";
                std::getline(std::cin, id);
                if (!manager.editStudentByID(id)) {
                    std::cerr << "Failed" << std::endl;
                    logr.log("Failed to edit student with ID: " + id, 0);
                    return 1;
                }
                logr.log("Successfully edited student with ID: " + id, 2);
                return 0;
            }
        } else if (command == "--view"){
            // Implementation for viewing a student
            logr.log("User chose to view a student.", 2);
            if (argc == 3) {
                std::string id = argv[2];
                StuInfo3::Student out;
                if (manager.selectStudentByID(id, out)) {
                    manager.printStudent(out);
                    logr.log("Successfully viewed student with ID: " + id, 2);
                    return 0;
                } else {
                    logr.log("Student not found with ID: " + id, 0);
                    return 1;
                }
            } else {
                std::string id{};
                std::cout << "ID: ";
                std::getline(std::cin, id);
                StuInfo3::Student out;
                if (manager.selectStudentByID(id, out)) {
                    manager.printStudent(out);
                    logr.log("Successfully viewed student with ID: " + id, 2);
                    return 0;
                } else {
                    logr.log("Student not found with ID: " + id, 0);
                    return 1;
                }
            }
        } else if (command == "--view-all"){
            logr.log("User chose to view all students.", 2);
            std::vector<StuInfo3::Student> s = manager.getAllStudent();
            int count = s.size();
            if (count == 0) {
                std::cout << "No more students to display." << std::endl;
                logr.log("No students to display.", 2);
                return -1;
            }
            for (const auto& student : s) {
                manager.printStudent(student);
                if (count == 1) {
                    std::cout << "All students displayed." << std::endl;
                    logr.log("All students displayed.", 2);
                    break;
                }
                std::cout << "Press Enter to view the next student..." << std::endl;
                std::cin.get();
                count--;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return 0;
        } else if (command == "--score-range") {
            logr.log("User chose to view score range.", 2);
            if (argc == 3) {
                std::string subject = argv[2];
                manager.scoreRangeShow(subject);
                return 0;
            }
            manager.scoreRangeShow();
            return 0;
        } else if (command == "--import" || command == "--export") {
            logr.log("User chose to import/export student data.", 2);
            impOrExp io;
            if (!io.runapp()) {
                std::cerr << "Import/Export operation failed." << std::endl;
                logr.log("Import/Export operation failed.", 1);
                return 1;
            }
            return 0;
        } else if (command == "--help" || command == "-h") {
            logr.log("User chose to view help information.", 2);
            std::cout << StuInfo3::getVersion() <<"\n"
                      << "Available commands:\n"
                      << "  --add         - Add a new student\n"
                      << "  --delete      - Delete a student by ID\n"
                      << "  --delete <id> - Delete a student by ID without prompt\n"
                      << "  --edit        - Edit a student's information by ID\n"
                      << "  --edit <id>   - Edit a student's information by ID without input id prompt\n"
                      << "  --view        - View a student's information by ID\n"
                      << "  --view <id>   - View a student's information by ID without input id prompt\n"
                      << "  --view-all    - View all students' information\n"
                      << "  --score-range  - View score range for a subject\n"
                      << "  --score-range <subject> - View score range for a specific subject\n"
                      << "  --help         - Show this help message\n"
                      << "  --import/--export - Import or export student data\n";
            return 0;
        } else {
            logr.log("User entered an unknown command: " + std::string(command), 1);
            std::cerr << "Unknown command: " << command << "\n";
            std::cerr << "Use --help to see available commands.\n";
            return 1;
        }
    } else {
        printGPLNotice();
        std::string choice;

        while(true){
            std::cout << "Please choose the version of the program you want to run: " << std::endl;
            std::cout << "1- 0.0.1-ALPHA\n2- CLI Version--" << StuInfo3::getVersion() << "\n3- TUI Version--" << tui::getVersion() << "\nquit- Exit the program" << std::endl;
            std::cout << ">>># ";
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
            } else if (choice == "debug"){
                logr.log("User run debug command, it will be make something strange", 1);
                debug dg;
                dg.run();

                std::cout << std::endl;
            }else if (choice == "3" || choice == "3-" || choice == tui::getVersion()){
                tui ui;
                ui.run();
                std::cerr << "QUIT\a\r\n" << std::endl;
            }  else if (choice == "show w"){
                Main::printNoWarranty();
            }   else if (choice == "show c"){
                Main::printRedistributionConditions();
            }   else if (choice == "quit" || choice == "exit"){
                std::cout << "Goodbye." << std::endl;
                logr.log("User chose to exit the program.", 2);
                std::this_thread::sleep_for(std::chrono::seconds(1));
                break;
            }   else {
                logr.log("Unknow choice", 1);
                std::cout << "Unknown choice, please try again." << std::endl;
                continue;
            }
        } 

        logr.log("Programme stop", 2);
    
    }
    return 0;
}

void Main::printGPLNotice(){
    std::cout << "Student Manager Copyright (C) 2026 Yvhang Cai(Jeffrey Miller)" << std::endl;
    std::cout << "This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'." << std::endl;
    std::cout << "This is free software, and you are welcome to redistribute it" << std::endl;
    std::cout << "it under certain conditions; type `show c' for details.\n\n" << std::endl;
    std::cout << std::endl;
}

void Main::printNoWarranty(){
    std::cout << "\nNO WARRANTY" << std::endl;
    std::cout << "This program is distributed in the hope that it will be useful," << std::endl;
    std::cout << "but WITHOUT ANY WARRANTY; without even the implied warranty of" << std::endl;
    std::cout << "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << std::endl;
    std::cout << "See the GNU General Public License for more details.\n\n\a" << std::endl;
    std::cout << std::endl;
}

void Main::printRedistributionConditions(){
    std::cout << "\nREDISTRIBUTION CONDITIONS" << std::endl;
    std::cout << "This program is free software: you can redistribute it and/or modify" << std::endl;
    std::cout << "it under the terms of the GNU General Public License as published by" << std::endl;
    std::cout << "the Free Software Foundation, either version 4 of the License, or" << std::endl;
    std::cout << "(at your option) any later version.\n\n\a" << std::endl;
    std::cout << std::endl;
}