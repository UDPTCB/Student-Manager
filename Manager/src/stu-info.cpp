#include "../include/stu-info.h"

template<typename T>
void readNumberStrict(const std::string& prompt, T& value) {
    while (true) {
        std::cout << prompt;

        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return;
        }

        std::cout << "Invalid input. Try again.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void StudentInfo::to_json(nlohmann::json& j, const StuInfo& s){
    j = nlohmann::json{
        {"grade", s.grade},
        {"class_value", s.class_value},
        {"id", s.id},
        {"name", s.name},
        {"age", s.age},
        {"Chinese_score", s.Chinese_score},
        {"Mathematics_score", s.Mathematics_score},
        {"English_score", s.English_score},
        {"Physics_score", s.Physics_score},
        {"Chemistry_score", s.Chemistry_score},
        {"Biology_score", s.Biology_score},
        {"Geography_score", s.Geography_score},
        {"History_score", s.History_score},
        {"Politics_score", s.Politics_score}
    };
}
void StudentInfo::from_json(const nlohmann::json& j, StuInfo& s){
    j.at("grade").get_to(s.grade);
    j.at("class_value").get_to(s.class_value);
    j.at("id").get_to(s.id);
    j.at("name").get_to(s.name);
    j.at("age").get_to(s.age);
    j.at("Chinese_score").get_to(s.Chinese_score);
    j.at("Mathematics_score").get_to(s.Mathematics_score);
    j.at("English_score").get_to(s.English_score);
    j.at("Physics_score").get_to(s.Physics_score);
    j.at("Chemistry_score").get_to(s.Chemistry_score);
    j.at("Biology_score").get_to(s.Biology_score);
    j.at("Geography_score").get_to(s.Geography_score);
    j.at("History_score").get_to(s.History_score);
    j.at("Politics_score").get_to(s.Politics_score);
}
int StudentInfo::run(){
   std::cout << "\033[33m";

    std::cout << R"(

  _____ _             _            _   __  __                                   
 / ____| |           | |          | | |  \/  |                                  
| (___ | |_ _   _  __| | ___ _ __ | |_| \  / | __ _ _ __   __ _  __ _  ___ _ __ 
 \___ \| __| | | |/ _` |/ _ \ '_ \| __| |\/| |/ _` | '_ \ / _` |/ _` |/ _ \ '__|
 ____) | |_| |_| | (_| |  __/ | | | |_| |  | | (_| | | | | (_| | (_| |  __/ |   
|_____/ \__|\__,_|\__,_|\___|_| |_|\__|_|  |_|\__,_|_| |_|\__,_|\__, |\___|_|   
                                                                 __/ |           
                                                                |___/  
   - Alpha Version -

    )" << std::endl;

    
    std::cout << "\033[0m";

    std::cout << "\033[34m";
    std::cout << "Version: " << VERSION << std::endl;
    std::cout << std::endl;
    std::cout << "\033[0m";

    std::cout << "\nWelcome to the Student Information Management System!\n" << std::endl;
    std::cout << "Please select an option:" << std::endl;
    std::cout << "1. Add Student" << std::endl;
    std::cout << "2. Delete Student" << std::endl;
    std::cout << "3. Edit Student" << std::endl;
    std::cout << "4. View Student" << std::endl;
    std::cout << "5. View All Students" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::string choice;
    while (true) {
        std::cout << "Enter your choice: ";
        std::getline(std::cin, choice);
        if (choice == "1") {
            add_student();
        } else if (choice == "2") {
            //std::cout << "Delete student functionality is not implemented yet." << std::endl;
            delete_student();
        } else if (choice == "3") {
            //std::cout << "Edit student functionality is not implemented yet." << std::endl;
            edit_student();
        } else if (choice == "4") {
            view_student();
        } else if (choice == "5") {
            view_all_students();
        } else if (choice == "0") {
            std::cout << "Exiting the system. Goodbye!" << std::endl;
            break;
        } else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
        
        
        std::cout << "1. Add Student" << std::endl;
        std::cout << "2. Delete Student" << std::endl;
        std::cout << "3. Edit Student" << std::endl;
        std::cout << "4. View Student" << std::endl;
        std::cout << "5. View All Students" << std::endl;
        std::cout << "0. Exit" << std::endl;
    }

    return 0;
}
void StudentInfo::add_student(){
    StuInfo new_student;
    //bool success = false;
    std::cout << "\nPlease enter the student's information:" << std::endl;
    std::cout << "Grade: ";
    std::getline(std::cin, new_student.grade);
    std::cout << "Class: ";
    std::getline(std::cin, new_student.class_value);
    std::cout << "ID: ";
    std::getline(std::cin, new_student.id);
    std::cout << "Name: ";
    std::getline(std::cin, new_student.name);
    readNumberStrict("Age: ", new_student.age);
    readNumberStrict("Chinese Score: ", new_student.Chinese_score);
    readNumberStrict("Mathematics Score: ", new_student.Mathematics_score);
    readNumberStrict("English Score: ", new_student.English_score);
    readNumberStrict("Physics Score: ", new_student.Physics_score);
    readNumberStrict("Chemistry Score: ", new_student.Chemistry_score);
    readNumberStrict("Biology Score: ", new_student.Biology_score);
    readNumberStrict("Geography Score: ", new_student.Geography_score);
    readNumberStrict("History Score: ", new_student.History_score);
    readNumberStrict("Politics Score: ", new_student.Politics_score);
    
    nlohmann::json j;
    to_json(j, new_student);

    std::filesystem::path exe_path = std::filesystem::current_path();
    std::filesystem::path student_dir = exe_path / "Student";
    if (!std::filesystem::exists(student_dir)) {
        std::filesystem::create_directory(student_dir);
    }

    std::filesystem::path file_path = student_dir / (new_student.id + ".json");
    std::ofstream file(file_path, std::ios::out);
    if (!file.is_open()){
        std::cerr << "Failed to open file for writing." << std::endl;
        return;
    }
    file << std::setw(4) << j << std::endl;
    file.close();
    std::cout << "Student added successfully!" << std::endl;
    
}

void StudentInfo::view_student(){
    std::string student_id;
    std::cout << "Enter the student ID to view: ";
    std::getline(std::cin, student_id);
    std::filesystem::path exe_path = std::filesystem::current_path();
    std::filesystem::path student_dir = exe_path / "Student";
    std::filesystem::path file_path = student_dir / (student_id + ".json");
    if (!std::filesystem::exists(file_path)) {
        std::cout << "Student with ID " << student_id << " not found." << std::endl;
        return;
    }
    std::ifstream file(file_path, std::ios::in);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading." << std::endl;
        return;
    }
    nlohmann::json j;
    file >> j;
    file.close();

    StuInfo stu;
    from_json(j, stu);

    std::cout << "\nStudent Information:\n" << std::endl;
    std::cout << "Grade: " << stu.grade << std::endl;
    std::cout << "Class: " << stu.class_value << std::endl;
    std::cout << "ID: " << stu.id << std::endl;
    std::cout << "Name: " << stu.name << std::endl;
    std::cout << "Age: " << stu.age << std::endl;
    std::cout << "Chinese Score: " << stu.Chinese_score << std::endl
                << "Mathematics Score: " << stu.Mathematics_score << std::endl
                << "English Score: " << stu.English_score << std::endl
                << "Physics Score: " << stu.Physics_score << std::endl
                << "Chemistry Score: " << stu.Chemistry_score << std::endl
                << "Biology Score: " << stu.Biology_score << std::endl
                << "Geography Score: " << stu.Geography_score << std::endl
                << "History Score: " << stu.History_score << std::endl
                << "Politics Score: " << stu.Politics_score << std::endl;
    std::cout << "\nStudent information displayed successfully!" << std::endl;
}
void StudentInfo::view_all_students(){
    std::filesystem::path exe_path = std::filesystem::current_path();
    std::filesystem::path student_dir = exe_path / "Student";
    if (!std::filesystem::exists(student_dir)) {
        std::cout << "No students found." << std::endl;
        return;
    }
    std::vector<StuInfo> students;
    for (const auto& entry : std::filesystem::directory_iterator(student_dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".json") {
            std::ifstream file(entry.path(), std::ios::in);
            if (!file.is_open()) {
                std::cerr << "Failed to open file: " << entry.path() << std::endl;
                continue;
            }
            nlohmann::json j;
            file >> j;
            file.close();

            StuInfo stu;
            from_json(j, stu);
            students.push_back(stu);
        }
    }
    std::stable_sort(students.begin(), students.end(), 
    [](const StuInfo& a, const StuInfo& b) {
        return a.id < b.id;  
    });
    std::cout << "\nAll Students:\n" << std::endl;
    int count = students.size();
    if (count == 0) {
            std::cout << "No more students to display." << std::endl;
            return;
    }
    for (const auto& student : students) {
        
        std::cout << "Grade: " << student.grade << std::endl;
        std::cout << "Class: " << student.class_value << std::endl;
        std::cout << "ID: " << student.id << std::endl;
        std::cout << "Name: " << student.name << std::endl;
        std::cout << "Age: " << student.age << std::endl;
        std::cout << "Chinese Score: " << student.Chinese_score << std::endl
                    << "Mathematics Score: " << student.Mathematics_score << std::endl
                    << "English Score: " << student.English_score << std::endl
                    << "Physics Score: " << student.Physics_score << std::endl
                    << "Chemistry Score: " << student.Chemistry_score << std::endl
                    << "Biology Score: " << student.Biology_score << std::endl
                    << "Geography Score: " << student.Geography_score << std::endl
                    << "History Score: " << student.History_score << std::endl
                    << "Politics Score: " << student.Politics_score << std::endl;
        std::cout << "---" << std::endl;
        if (count == 1) {
            std::cout << "All students displayed." << std::endl;
            break;
        }
        std::cout << "Press Enter to view the next student..." << std::endl;
        std::cin.get();
        count--;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void StudentInfo::delete_student(){
    std::cout << "ID of the student to delete: ";
    std::string student_id;
    std::getline(std::cin, student_id);
    std::filesystem::path exe_path = std::filesystem::current_path();
    std::filesystem::path student_dir = exe_path / "Student";
    std::filesystem::path file_path = student_dir / (student_id + ".json");
    if (!std::filesystem::exists(file_path)) {
        std::cout << "Student with ID " << student_id << " not found." << std::endl;
        return;
    }
    std::filesystem::remove(file_path);
    std::cout << "Student with ID " << student_id << " deleted successfully." << std::endl;
}
void StudentInfo::edit_student(){
    std::cout << "ID of the student to edit: ";
    std::string student_id;
    std::getline(std::cin, student_id);
    std::filesystem::path exe_path = std::filesystem::current_path();
    std::filesystem::path student_dir = exe_path / "Student";
    std::filesystem::path file_path = student_dir / (student_id + ".json");
    if (!std::filesystem::exists(file_path)) {
        std::cout << "Student with ID " << student_id << " not found." << std::endl;
        return;
    }
    std::ifstream in(file_path, std::ios::in);
    nlohmann::json j;
    in >> j;
    in.close();

    StuInfo s;
    from_json(j, s);

    std::cout << "Edit (grade/class/id/name/age/scores): ";
    std::string choice;
    std::getline(std::cin, choice);

    std::string input;

    if (choice == "grade") {
        std::cout << "New grade: ";
        std::getline(std::cin, input);
        if (!input.empty()) s.grade = input;
    }
    else if (choice == "class") {
        std::cout << "New class: ";
        std::getline(std::cin, input);
        if (!input.empty()) s.class_value = input;
    }
    else if (choice == "id") {
        std::cout << "New ID: ";
        std::getline(std::cin, input);
        if (!input.empty()) {
            std::filesystem::path new_path = student_dir / (input + ".json");
            std::filesystem::rename(file_path, new_path);
            file_path = new_path;
            s.id = input;
        }
    }
    else if (choice == "name") {
        std::cout << "New name: ";
        std::getline(std::cin, input);
        if (!input.empty()) s.name = input;
    }
    else if (choice == "age") {
        readNumberStrict("New age: ", s.age);
    }
    else if (choice == "scores") {

        auto editScore = [&](const std::string& name, double& score) {
            std::cout << name << " (current: " << score << ")\n";
            readNumberStrict("New value: ", score);
        };

        editScore("Chinese", s.Chinese_score);
        editScore("Math", s.Mathematics_score);
        editScore("English", s.English_score);
        editScore("Physics", s.Physics_score);
        editScore("Chemistry", s.Chemistry_score);
        editScore("Biology", s.Biology_score);
        editScore("Geography", s.Geography_score);
        editScore("History", s.History_score);
        editScore("Politics", s.Politics_score);
    }
    else {
        std::cout << "Invalid choice\n";
        return;
    }

    
    nlohmann::json out_j;
    to_json(out_j, s);

    std::ofstream out(file_path, std::ios::out);
    out << std::setw(4) << out_j << std::endl;
    out.close();

    std::cout << "Update success!\n";

}
