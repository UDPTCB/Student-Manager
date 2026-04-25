#ifndef STUDENT3_HPP
#define STUDENT3_HPP
#define STUINFO_VERSION "0.1.2-ALPHA"
#include "./sqlite3.h"
#include "./Score_range.h"
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
    bool editStudentByID(sqlite3* db, const std::string& id);
    void printStudent(const StuInfo& s);
    void scoreRangeShow(sqlite3* db);
    void enterStudent(StuInfo& newStu);
    
    int run();

    template<typename S>
    void enterNum(const std::string& p, S& v, const int m, const int n){
        while(true){
            std::cout << p;
            if (m == 0){
                if (std::cin >> v){
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }
                else{
                    std::cout << "Invalid enter, try again. \n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }
            else {
                if (n == 0){
                    if (std::cin >> v && v <= 100 && v >= 0){
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        break;
                    }
                    else{
                        std::cout << "Invalid enter, try again ";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                }
                else if (n == 1){
                    if (std::cin >> v && v <= 120 && v >= 0){
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        break;
                    }
                    else{
                        std::cout << "Invalid enter, try again ";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                }
                else if (n == 2){
                    if (std::cin >> v && v <= 150 && v >= 0){
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        break;
                    }
                    else{
                        std::cout << "Invalid enter, try again ";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                }
            }
        }
    }
}


#endif