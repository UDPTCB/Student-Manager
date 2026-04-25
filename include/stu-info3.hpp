#ifndef STUDENT3_HPP
#define STUDENT3_HPP
#define STUINFO_VERSION "0.1.1-ALPHA"
#include "./sqlite3.h"
#include <string>
#include <iostream>
#include <filesystem>
#include <format>
#include <vector>
#include <algorithm>
namespace StuInfo3 {
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

    bool createTable(sqlite3* db);
    bool insertStudent(sqlite3* db, const StuInfo& student);
    bool SelectStudentByID(sqlite3* db, const std::string& id, StuInfo& outStudent);
    std::vector<StuInfo> getAllStudent(sqlite3* db);
    bool deleteStudentByID(sqlite3* db, const std::string& id);
    // stu-info3.hpp
    bool editStudentByID(sqlite3* db, const std::string& id);
    void printStudent(const StuInfo& s);
    void enterStudent(StuInfo& newStu);
    int run();

    template<typename S>
    void enterNum(const std::string& p, S& v){
        while(true){
            std::cout << p;
            if (std::cin >> v){
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
            else{
                std::cout << "Invalid enter, try agin.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }
}


#endif