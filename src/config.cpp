#include "../include/config.h"
#include "../include/Score_range.h"
#include "../include/stu-info.h"

#include <iostream>
int Main::run(int argc, char **argv){
    std::filesystem::path exe_path = std::filesystem::current_path();
    std::filesystem::path student_dir = exe_path / "Student";
    if (!std::filesystem::exists(student_dir)) {
        std::filesystem::create_directory(student_dir);
    }

    StudentInfo si;
    si.run();
    

    return 0;
}