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
#ifndef STUDENT3_HPP
#define STUDENT3_HPP
#define STUINFO_VERSION "0.1.5-ALPHA"
#define CLI_VERSION "0.1.5-Gamma"
#include "./sqlite3.h"
#include "./Score_range.h"
#include "./logger.hpp"
#include "./exePath.h"
#include <string>
#include <iostream>
#include <filesystem>
#include <format>
#include <vector>
#include <algorithm>
#include <limits>
#include <stdexcept>



class Statement {
private:
    sqlite3_stmt* stmt = nullptr;
public:
    Statement(sqlite3* db, const char* sql){
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            stmt = nullptr;
            throw std::runtime_error("Error preparing statement: " + std::string(sqlite3_errmsg(db)));
            
        }
    }

    ~Statement() {
        if (stmt) {
            sqlite3_finalize(stmt);
        }
    }
    Statement(const Statement&) = delete;
    Statement& operator=(const Statement&) = delete;
    sqlite3_stmt* get() const { return stmt; }
};




class Transaction {
private:
    sqlite3* db;
    bool committed = false;

public:
    Transaction(sqlite3* database) : db(database) {
        sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
    }

    void commit() {
        sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
        committed = true;
    }

    ~Transaction() {
        if (!committed) {
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        }
    }
};
class StuInfo3 {
public:
    struct Student {
        std::string grade{};
        std::string class_value{};
        std::string id{};
        std::string name{};
        int age{};
        double Chinese_score{};
        double Mathematics_score{};
        double English_score{};
        double Physics_score{};
        double Chemistry_score{};
        double Biology_score{};
        double Geography_score{};
        double History_score{};
        double Politics_score{};
        std::string created_at{};
        std::string updated_at{};
        double total_score{};
    };
    
    // Constructor / Destructor
    StuInfo3();
    StuInfo3(const std::filesystem::path& path);
    ~StuInfo3();

    // Database operations
    bool openDatabase(const std::filesystem::path& dbPath);
    void closeDatabase();
    bool isOpen() const { return db != nullptr; }

    // CRUD operations
    bool createTable();
    bool insertStudent(const Student& student);
    bool insertStudents(const std::vector<Student>& students);
    bool selectStudentByID(const std::string& id, Student& outStudent);
    bool existsStudentByID(const std::string& id);
    std::vector<Student> getAllStudent();
    bool deleteStudentByID(const std::string& id);
    bool editStudentByID(const std::string& id);
    bool createTriggers();
    bool migrateTable();
    // Display operations
    void printStudent(const Student& s) const;
    void scoreRangeShow(std::string subject = "");

    // Input operations
    void enterStudent(Student& newStu, int mode = 0);

    // Static utility functions (e.g. for log file naming)
    static std::string getVersion() { return STUINFO_VERSION; }
    static std::string getCurrentTime();
    static std::string getTimestampForFilename();

    std::string easter_egg;
private:
    sqlite3* db = nullptr;
    std::filesystem::path currentDbPath;

    // Internal helper function
    template<typename T>
    void enterNum(const std::string& prompt, T& value, int mode, int limit = 0) {
        while (true) {
            std::cout << prompt;
            if (mode == 0) {
                if (std::cin >> value) {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                } else {
                    std::cout << "Invalid enter, try again. \n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            } else {
                if (limit == 0) {
                    if (std::cin >> value && value <= 100 && value >= 0) {
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        break;
                    } else {
                        std::cout << "Invalid enter, try again ";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                } else if (limit == 1) {
                    if (std::cin >> value && value <= 120 && value >= 0) {
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        break;
                    } else {
                        std::cout << "Invalid enter, try again ";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                } else if (limit == 2) {
                    if (std::cin >> value && value <= 150 && value >= 0) {
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        break;
                    } else {
                        std::cout << "Invalid enter, try again ";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                }
            }
        }
    }
};

class impOrExp {
private:
    StuInfo3 from;
    StuInfo3 to;
public:
    impOrExp(const StuInfo3& from, const StuInfo3& to) : from(from), to(to) {}
    impOrExp() = default;
    ~impOrExp() = default;
    bool importFromDB(const std::filesystem::path& fmPath, const std::filesystem::path& toPath);
    bool exportToDB(const std::filesystem::path& fmPath, const std::filesystem::path& toPath);
    bool runapp();
};


// Standalone function (not a member of StuInfo3)
namespace stu{

    int runStuInfo3(logger& log);
};


#endif