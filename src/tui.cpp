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
#include "../include/tui.hpp"
void tui::run(){

    StuInfo3 manager;
    std::filesystem::path exe_path   = get_executable_path();
    std::filesystem::path log_path = exe_path / "logs";
    
    if (!std::filesystem::exists(log_path)){
        if (!std::filesystem::create_directory(log_path)){
            return;
        }
    }

    std::string time = logger::getTimestampForFilename();
    std::filesystem::path logs = log_path / (time + ".log");
    logger logr(logs);


    std::filesystem::path studir = exe_path / "Student";
    if (!std::filesystem::exists(studir)) {
        if (!std::filesystem::create_directory(studir)){
            logr.log("Failed to create student data path", 0);
             return;
        }
    }

    std::filesystem::path data_file  = studir / "students.db";

    if (!manager.openDatabase(data_file)) {
        
        logr.log("Failed to open database: " + data_file.string(), 0);
        return;
    }

    if (!manager.createTable()) {
        
        logr.log("Failed to create table in database: " + data_file.string(), 0);
        return;
    }

    if (!manager.migrateTable()) {         
        
        logr.log("Migration failed: " + data_file.string(), 0);
        return;
    }
    if (!manager.createTriggers()) {
       
        logr.log("Failed to create triggers in database: " + data_file.string(), 1);
    }


    auto screen = ftxui::ScreenInteractive::FitComponent();

    
    
    std::string message{};
    std::vector<std::string> options = {"Add Student", "View Student","View All Students","Edit Student","Delete Student",  "Exit"};
    int selected = 0;

    auto menu = ftxui::Menu(&options, &selected);

    auto show_submenu = [&]() {
        if(options[selected] == "Add Student"){
            StuInfo3::Student stu{};
            std::vector<ftxui::Component> inputs;

            for(auto& field : fields)
            {
                inputs.push_back(
                    ftxui::Input(&field.value, field.label)
                );
            }

            auto confirm1 = ftxui::Button("Comfirm", [&]()->void{
                
                if (!comfirmInput(stu, message)){
                    return;
                }
                if (manager.existsStudentByID(stu.id)){
                    message = "Student with ID " + stu.id + " already exists";
                    return;
                }
                if (manager.insertStudent(stu)){
                    logr.log("Student added: " + stu.name + " (ID: " + stu.id + ")", 2);
                     message = "Add student successfully";
                     logr.log("Student added: " + stu.name + " (ID: " + stu.id + ")", 2);
                } else {
                    logr.log("Failed to add student: " + stu.name, 0);
                    message = "Failed to add student";
                    logr.log("Failed to add student: " + stu.name, 0);
                }
               
            });
            auto back = ftxui::Button("Back", [&](){
                for(auto& field : fields)
                {
                    field.value.clear();
                }
                screen.ExitLoopClosure()();
            });
            inputs.push_back(confirm1);
            inputs.push_back(back);
            auto container = ftxui::Container::Vertical(inputs);
            auto renderer = ftxui::Renderer(container, [&](){
                if (!message.empty()) {
                    ftxui::Elements elements;
                    elements.push_back(ftxui::text("Add Student") | ftxui::bold);
                    elements.push_back(ftxui::separator());
                    for(auto& input : inputs)
                    {
                        elements.push_back(input->Render());
                    }
                    elements.push_back(ftxui::separator());
                    
                    elements.push_back(ftxui::text(message) | ftxui::color(ftxui::Color::Green));
                    return ftxui::vbox(
                        elements
                    ) | ftxui::border;
                }
                else {
                    ftxui::Elements elements;
                    elements.push_back(ftxui::text("Add Student") | ftxui::bold);
                    elements.push_back(ftxui::separator());
                    for(auto& input : inputs)
                    {
                        elements.push_back(input->Render());
                    }
                    elements.push_back(ftxui::separator());
                    
                    return ftxui::vbox(
                       elements
                    ) | ftxui::border;
                }
            });
            screen.Loop(renderer);
        }
        else if (options[selected] == "Exit"){
            logr.log("User chose to exit the program.", 2);
            screen.Exit();
        }
        else if (options[selected] == "View Student"){
            //message = "Feature not implemented yet";
            StuInfo3::Student stu{};
            std::string student_id;
            bool confirmed = false;
            auto input = ftxui::Input(&student_id, "Student ID");
            auto back = ftxui::Button("Back", [&](){
                        for(auto& field : fields)
                        {
                            field.value.clear();
                        }
                        screen.ExitLoopClosure()();
                    });
            auto confirm2 = ftxui::Button("Comfirm", [&]() {
                 if(student_id.empty()){
                    message = "ID can not be empty";
                    return;
                }
                auto back = ftxui::Button("Back", [&](){
                        screen.ExitLoopClosure()();
                    });
                message = "Viewing student with ID: " + student_id;
                if (!manager.selectStudentByID(student_id, stu)) {
                    message = "Student with ID " + student_id + " not found";
                }
                confirmed = true;
                logr.log("Viewed student with ID: " + student_id, 2);
            });
            auto container = ftxui::Container::Vertical({input, confirm2, back});
            auto renderer = ftxui::Renderer(container, [&](){
                
                    if (confirmed){
                        ftxui::Elements elements;
                        elements.push_back(ftxui::text("View Student") | ftxui::bold);
                        elements.push_back(ftxui::separator());
                        elements.push_back(input->Render());
                        elements.push_back(confirm2->Render());
                        elements.push_back(back->Render());
                        elements.push_back(ftxui::separator());
                        elements.push_back(ftxui::text("ID: " + stu.id));
                        elements.push_back(ftxui::text("Name: " + stu.name));
                        elements.push_back(ftxui::text("Grade: " + stu.grade));
                        elements.push_back(ftxui::text("Class: " + stu.class_value));
                        elements.push_back(ftxui::text("Age: " + std::to_string(stu.age)));
                        elements.push_back(ftxui::text("Chinese Score: " + std::to_string(stu.Chinese_score)));
                        elements.push_back(ftxui::text("Mathematics Score: " + std::to_string(stu.Mathematics_score)));
                        elements.push_back(ftxui::text("English Score: " + std::to_string(stu.English_score)));
                        elements.push_back(ftxui::text("Physics Score: " + std::to_string(stu.Physics_score)));
                        elements.push_back(ftxui::text("Chemistry Score: " + std::to_string(stu.Chemistry_score)));
                        elements.push_back(ftxui::text("Biology Score: " + std::to_string(stu.Biology_score)));
                        elements.push_back(ftxui::text("Geography Score: " + std::to_string(stu.Geography_score)));
                        elements.push_back(ftxui::text("History Score: " + std::to_string(stu.History_score)));
                        elements.push_back(ftxui::text("Politics Score: " + std::to_string(stu.Politics_score)));
                        elements.push_back(ftxui::separator());
                        elements.push_back(ftxui::text(message) | ftxui::color(ftxui::Color::Red));
                        return ftxui::vbox(
                            elements
                        ) | ftxui::border;
                    }
                    else 
                    {
                        ftxui::Elements elements;
                        elements.push_back(ftxui::text("View Student") | ftxui::bold);
                        elements.push_back(ftxui::separator());
                        elements.push_back(input->Render());
                        elements.push_back(confirm2->Render());
                        elements.push_back(back->Render());
                        elements.push_back(ftxui::separator());
                        elements.push_back(ftxui::text(message) | ftxui::color(ftxui::Color::Red));
                        return ftxui::vbox(
                            elements
                        ) | ftxui::border;
                    }
                    
                });
            screen.Loop(renderer);
        }
        else if (options[selected] == "View All Students"){
            //message = "Feature not implemented yet";
            logr.log("User chose to view all students.", 2);
            std::vector<StuInfo3::Student> students = manager.getAllStudent();
            std::vector<std::string> names;
            for (const auto& student : students) {
                names.push_back(student.name + " (ID: " + student.id + ")");
            }
            int selected_student = 0;
            auto menu_students = ftxui::Menu(&names, &selected_student);
            auto back = ftxui::Button("Back", [&](){
                        for(auto& field : fields)
                        {
                            field.value.clear();
                        }
                        screen.ExitLoopClosure()();
                    });
            auto container =
            ftxui::Container::Vertical({
                menu_students,
                back
            });
            auto renderer = ftxui::Renderer(container, [&](){
               
                if (students.empty()){
                    message = "No students found";
                    return ftxui::text("No students");
                    
                }
                const auto& student = students[selected_student];
                auto left = menu_students->Render() | ftxui::border;
                auto right = ftxui::vbox({
                    ftxui::text("All Students") | ftxui::bold,
                    ftxui::separator(),
                    ftxui::text("ID: " + student.id),
                    ftxui::text("Name: " + student.name),
                    ftxui::text("Grade: " + student.grade),
                    ftxui::text("Class: " + student.class_value),
                    ftxui::text("Age: " + std::to_string(student.age)),
                    ftxui::text("Chinese Score: " + std::to_string(student.Chinese_score)),
                    ftxui::text("Mathematics Score: " + std::to_string(student.Mathematics_score)),
                    ftxui::text("English Score: " + std::to_string(student.English_score)),
                    ftxui::text("Physics Score: " + std::to_string(student.Physics_score)),
                    ftxui::text("Chemistry Score: " + std::to_string(student.Chemistry_score)),
                    ftxui::text("Biology Score: " + std::to_string(student.Biology_score)),
                    ftxui::text("Geography Score: " + std::to_string(student.Geography_score)),
                    ftxui::text("History Score: " + std::to_string(student.History_score)),
                    ftxui::text("Politics Score: " + std::to_string(student.Politics_score)),
                    ftxui::separator(),
                }) | ftxui::border;
                return ftxui::hbox({
                    left | ftxui::flex,
                    right | ftxui::flex,
                    ftxui::separator(),
                    back->Render()
                });
            });

            screen.Loop(renderer);
        }
        else if (options[selected] == "Edit Student"){
            //message = "Feature not implemented yet";
            logr.log("User chose to edit a student.", 2);
            std::vector<StuInfo3::Student> students = manager.getAllStudent();
            std::vector<std::string> names;
            StuInfo3::Student stu{};
            for(const auto& student : students){
                names.push_back(student.name + " (ID: " + student.id + ")");
            }
            int selected_student = 0;
            auto menu_students = ftxui::Menu(&names, &selected_student);
            auto back = ftxui::Button("Back", [&](){
                for(auto& field : fields)
                {
                    field.value.clear();
                }
                screen.ExitLoopClosure()();
            });
            std::vector<ftxui::Component> inputs;
            inputs.push_back(menu_students);
            inputs.push_back(back);
            for(auto& field : fields)
            {
                inputs.push_back(
                    ftxui::Input(&field.value, field.label)
                );
            }
            auto confirm5 = ftxui::Button("Confirm", [&](){

                if (!comfirmInput(stu, message)){
                    return;
                }
                if (!manager.existsStudentByID(stu.id)){
                    message = "Student with ID " + stu.id + " already not exists";
                    logr.log("Failed to edit student: " + stu.name, 0);
                    return;
                }
                if (manager.insertStudent(stu)){
                    logr.log("Student edited: " + stu.name + " (ID: " + stu.id + ")", 2);
                    message = "Edit student successfully";
                    logr.log("Student edited: " + stu.name + " (ID: " + stu.id + ")", 2);
                } else {
                    logr.log("Failed to Edit student: " + stu.name, 0);
                    message = "Failed to edit student";
                    logr.log("Failed to Edit student: " + stu.name, 0);
                }
                students = manager.getAllStudent();
                names.clear();
                for(const auto& student : students){
                    names.push_back(student.name + " (ID: " + student.id + ")");
                }
            });
            inputs.push_back(confirm5);
            auto container =
            ftxui::Container::Vertical(
                inputs
            );
            auto renderer = ftxui::Renderer(container, [&](){
                if (students.empty()){
                    message = "No students found";
                    return ftxui::text("No students");
                }
                const auto& student = students[selected_student];
                auto left = inputs[0]->Render() | ftxui::border;
                auto middle = ftxui::vbox({
                    ftxui::text("Edit Student") | ftxui::bold,
                    ftxui::separator(),
                    ftxui::text("ID: " + student.id),
                    ftxui::text("Name: " + student.name),
                    ftxui::text("Grade: " + student.grade),
                    ftxui::text("Class: " + student.class_value),
                    ftxui::text("Age: " + std::to_string(student.age)),
                    ftxui::text("Chinese Score: " + std::to_string(student.Chinese_score)),
                    ftxui::text("Mathematics Score: " + std::to_string(student.Mathematics_score)),
                    ftxui::text("English Score: " + std::to_string(student.English_score)),
                    ftxui::text("Physics Score: " + std::to_string(student.Physics_score)),
                    ftxui::text("Chemistry Score: " + std::to_string(student.Chemistry_score)),
                    ftxui::text("Biology Score: " + std::to_string(student.Biology_score)),
                    ftxui::text("Geography Score: " + std::to_string(student.Geography_score)),
                    ftxui::text("History Score: " + std::to_string(student.History_score)),
                    ftxui::text("Politics Score: " + std::to_string(student.Politics_score)),
                    ftxui::separator(),
                    ftxui::text(message) | ftxui::color(ftxui::Color::Red)
                }) | ftxui::border;
                auto right = ftxui::vbox({
                    ftxui::text("Edit") | ftxui::bold,
                    ftxui::separator(),
                    inputs[2]->Render(),
                    inputs[3]->Render(),
                    inputs[4]->Render(),
                    inputs[5]->Render(),
                    inputs[6]->Render(),
                    inputs[7]->Render(),
                    inputs[8]->Render(),
                    inputs[9]->Render(),
                    inputs[10]->Render(),
                    inputs[11]->Render(),
                    inputs[12]->Render(),
                    inputs[13]->Render(),
                    inputs[14]->Render(),
                    inputs[15]->Render(),
                    confirm5->Render(),
                    ftxui::separator(),
                    inputs[1]->Render(),
                }) | ftxui::border;
                return ftxui::hbox({
                    left | ftxui::flex,
                    middle | ftxui::flex,
                    right | ftxui::flex
                });
            });
            screen.Loop(renderer);
        }
        else if (options[selected] == "Delete Student") {
            // Implementation for deleting a student
            std::vector<StuInfo3::Student> students = manager.getAllStudent();
            std::vector<std::string> names;
            for(const auto& student : students){
                names.push_back(student.name + " (ID: " + student.id + ")");
            }
            int selected_student = 0;
            auto menu_students = ftxui::Menu(&names, &selected_student);
            auto back = ftxui::Button("Back", [&](){
                for(auto& field : fields)
                {
                    field.value.clear();
                }
                screen.ExitLoopClosure()();
            });
            auto confirm6 = ftxui::Button("Confirm", [&](){
                const auto& student = students[selected_student];
                if (!manager.deleteStudentByID(student.id)){
                    message = "Failed to delete student with ID " + student.id;
                    logr.log("Failed to delete student: " + student.name + " (ID: " + student.id + ")", 0);
                } else {
                    message = "Deleted student with ID " + student.id;
                    logr.log("Deleted student: " + student.name + " (ID: " + student.id + ")", 2);
                    students = manager.getAllStudent();
                    names.clear();
                    for(const auto& s : students){
                        names.push_back(s.name + " (ID: " + s.id + ")");
                    }
                }
            });
            
            auto container =
            ftxui::Container::Vertical({
                menu_students,
                confirm6,
                back
            });

            auto renderer = ftxui::Renderer(container, [&](){
                if (students.empty()){
                    message = "No students found";
                    return ftxui::text("No students");
                }
                const auto& student = students[selected_student];
                auto left = menu_students->Render() | ftxui::border;
                auto right = ftxui::vbox({
                    ftxui::text("Delete Student") | ftxui::bold,
                    ftxui::separator(),
                    ftxui::text("ID: " + student.id),
                    ftxui::text("Name: " + student.name),
                    ftxui::text("Grade: " + student.grade),
                    ftxui::text("Class: " + student.class_value),
                    ftxui::text("Age: " + std::to_string(student.age)),
                    ftxui::text("Chinese Score: " + std::to_string(student.Chinese_score)),
                    ftxui::text("Mathematics Score: " + std::to_string(student.Mathematics_score)),
                    ftxui::text("English Score: " + std::to_string(student.English_score)),
                    ftxui::text("Physics Score: " + std::to_string(student.Physics_score)),
                    ftxui::text("Chemistry Score: " + std::to_string(student.Chemistry_score)),
                    ftxui::text("Biology Score: " + std::to_string(student.Biology_score)),
                    ftxui::text("Geography Score: " + std::to_string(student.Geography_score)),
                    ftxui::text("History Score: " + std::to_string(student.History_score)),
                    ftxui::text("Politics Score: " + std::to_string(student.Politics_score)),
                    ftxui::separator(),
                    ftxui::text(message) | ftxui::color(ftxui::Color::Red),
                    ftxui::separator(),
                    confirm6->Render(),
                    back->Render()
                }) | ftxui::border;
                return ftxui::hbox({
                    left | ftxui::flex,
                    right | ftxui::flex,
                });
            });
            screen.Loop(renderer);
        }
    };
    auto confirm = ftxui::Button("Confirm", show_submenu);

    auto main_container = ftxui::Container::Vertical({menu, confirm});
    auto main_loop_renderer = ftxui::Renderer(main_container, [&] {
        return ftxui::vbox({
            ftxui::text("Student Manager") | ftxui::bold | ftxui::center,
            ftxui::text("Use arrow keys to navigate and Enter to select") | ftxui::center,
            ftxui::text("Database Version: " + std::string(STUINFO_VERSION)) | ftxui::color(ftxui::Color::Blue) | ftxui::center,
            ftxui::text("TUI Version: " + std::string(TUI_VERSION)) | ftxui::color(ftxui::Color::Blue) | ftxui::center,
            ftxui::separator() | ftxui::color(ftxui::Color::Yellow),
            ftxui::separator(),
            menu->Render() | ftxui::border,
            ftxui::separator(),
            confirm->Render()
        }) | ftxui::border;
    });

    screen.Loop(main_loop_renderer);

}
