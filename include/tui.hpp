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
#ifndef TUI_HPP
#define TUI_HPP
#define TUI_VERSION "0.0.1-Beta"
#include <string>
#include <vector>
#include <filesystem>
#include "./exePath.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include "./stu-info3.hpp"
constexpr int page_size = 5; 
class tui{
private:
   
     struct Field {
        std::string label;
        std::string value;
    };
    std::vector<Field> fields{
        {"Name", ""},
        {"ID", ""},
        {"Grade", ""},
        {"Class", ""},
        {"Age", ""},
        {"Chinese", ""},
        {"Mathematics", ""},
        {"English", ""},
        {"Physics", ""},
        {"Chemistry", ""},
        {"Biology", ""},
        {"Geography", ""},
        {"History", ""},
        {"Politics", ""}
    };
    bool to_int(const std::string& str, int& value){
        try {
            size_t pos;
            value = std::stoi(str, &pos);

            return pos == str.size();
        }
        catch (...) {
            return false;
        }
    }
    bool to_double(const std::string& str, double& value){
        try {
            size_t pos;
            value = std::stod(str, &pos);

            return pos == str.size();
        }
        catch (...) {
            return false;
        }
    }
    bool comfirmInput(StuInfo3::Student& stu, std::string& message){
        for (const auto& field : fields){
            if (field.label == "Name"){
                if (!field.value.empty()){
                    stu.name = field.value;
                }
                else {
                    message = "Name can not be empty";
                    return false;
                }
                
            }
            else if (field.label == "ID"){
                if (!field.value.empty()){
                    stu.id = field.value;
                }
                else {
                    message = "ID is required";
                    return false;
                }
            }
            else if (field.label == "Class"){
                if (!field.value.empty()){
                    stu.class_value = field.value;
                }
                else {
                    message = "Class is required";
                    return false;
                }
            }
            else if (field.label == "Grade"){
                if (!field.value.empty()){
                    stu.grade = field.value;
                }
                else {
                    message = "Grade is required";
                    return false;
                }
            }
            else if (field.label == "Age" && !to_int(field.value, stu.age)){
                message = "Invalid age";
                return false;
            }
            else {
                if (field.label == "Chinese" && !to_double(field.value, stu.Chinese_score)){
                    message = "Invalid Chinese score";
                    return false;
                }
                else if (field.label == "Mathematics" && !to_double(field.value, stu.Mathematics_score)){
                    message = "Invalid Mathematics score";
                    return false;
                }
                else if (field.label == "English" && !to_double(field.value, stu.English_score)){
                    message = "Invalid English score";
                    return false;
                }
                else if (field.label == "Physics" && !to_double(field.value, stu.Physics_score)){
                    message = "Invalid Physics score";
                    return false;
                }
                else if (field.label == "Chemistry" && !to_double(field.value, stu.Chemistry_score)){
                    message = "Invalid Chemistry score";
                    return false;
                }
                else if (field.label == "Biology" && !to_double(field.value, stu.Biology_score)){
                    message = "Invalid Biology score";
                    return false;
                }
                else if (field.label == "Geography" && !to_double(field.value, stu.Geography_score)){
                    message = "Invalid Geography score";
                    return false;
                }
                else if (field.label == "History" && !to_double(field.value, stu.History_score)){
                    message = "Invalid History score";
                    return false;
                }
                else if (field.label == "Politics" && !to_double(field.value, stu.Politics_score)){
                    message = "Invalid Politics score";
                    return false;
                }
            }
        }
        return true;
    }
public:
    static std::string getVersion() {
        return TUI_VERSION;
    }
    void run();
};

#endif // TUI_HPP