#ifndef STUINFO_H
#define STUINFO_H
#define VERSION "0.0.1-ALPHA"
#include "./json.hpp"
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
public:
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