/*
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
 * 
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#ifndef STUINFO_H
#define STUINFO_H
#define VERSION "0.0.1-ALPHA"
#include "./json.hpp"
#include "./exePath.h"
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm>  // std::sort
#include <sodium.h>
class StudentInfo {
private:
    struct StuInfo {
        std::string grade;
        std::string class_value;
        std::string id;
        std::string name;
        int age;
        double Chinese_score;
        double Mathematics_score;
        double English_score;
        double Physics_score;
        double Chemistry_score;
        double Biology_score;
        double Geography_score;
        double History_score;
        double Politics_score;
    };
    std::string easter_egg;
public:
    StudentInfo();
    void to_json(nlohmann::json& j, const StuInfo& s);
    void from_json(const nlohmann::json& j, StuInfo& s);
    void add_student();
    void delete_student();
    void edit_student();
    void view_student();
    void view_all_students();
    int run();
};


#endif