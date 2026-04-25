#include "../include/stu-info3.hpp"

void StuInfo3::enterStudent(StuInfo& newStu){
    std::cout << "ID: ";
    std::getline(std::cin, newStu.id);
    std::cout << "Grade: ";
    std::getline(std::cin, newStu.grade);
    std::cout << "Class: ";
    std::getline(std::cin, newStu.class_value);
    std::cout << "Name: ";
    std::getline(std::cin, newStu.name);
    enterNum("Age: ", newStu.age);
    enterNum("Chinese score: ", newStu.Chinese_score);
    enterNum("Mathematics score: ", newStu.Mathematics_score);
    enterNum("English score: ", newStu.English_score);
    enterNum("Physics score: ", newStu.Physics_score);
    enterNum("Chemistry: ", newStu.Chemistry_score);
    enterNum("Biology score: ", newStu.Biology_score);
    enterNum("Geography score: ", newStu.Geography_score);
    enterNum("History: ", newStu.History_score);
    enterNum("Politics: ", newStu.Politics_score);
}


bool StuInfo3::createTable(sqlite3* db){
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS students (
            id TEXT PRIMARY KEY,
            grade TEXT,
            class_value TEXT,
            name TEXT,
            age INTEGER,
            Chinese_score REAL,
            Mathematics_score REAL,
            English_score REAL,
            Physics_score REAL,
            Chemistry_score REAL,
            Biology_score REAL,
            Geography_score REAL,
            History_score REAL,
            Politics_score REAL
        );
    )";
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK){
        std::cerr << "Error creating table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool StuInfo3::insertStudent(sqlite3* db, const StuInfo& student){
    const char* sql = R"(
        INSERT OR REPLACE INTO students (
            id, grade, class_value, name, age,
            Chinese_score, Mathematics_score, English_score,
            Physics_score, Chemistry_score, Biology_score,
            Geography_score, History_score, Politics_score
            ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
        
    )";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK){
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    sqlite3_bind_text(stmt, 1, student.id.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, student.grade.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, student.class_value.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, student.name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, student.age);
    sqlite3_bind_double(stmt, 6, student.Chinese_score);
    sqlite3_bind_double(stmt, 7, student.Mathematics_score);
    sqlite3_bind_double(stmt, 8, student.English_score);
    sqlite3_bind_double(stmt, 9, student.Physics_score);
    sqlite3_bind_double(stmt, 10, student.Chemistry_score);
    sqlite3_bind_double(stmt, 11, student.Biology_score);
    sqlite3_bind_double(stmt, 12, student.Geography_score);
    sqlite3_bind_double(stmt, 13, student.History_score);
    sqlite3_bind_double(stmt, 14, student.Politics_score);

    if (sqlite3_step(stmt) != SQLITE_DONE){
        std::cerr << "Error inserting student: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    std::cout << "Student inserted successfully!" << std::endl;
    return true;
}

bool StuInfo3::SelectStudentByID(sqlite3* db, const std::string& id, StuInfo& outStudent){
    const char* sql = "SELECT grade, class_value, id, name, age, "
                      "Chinese_score, Mathematics_score, English_score, "
                      "Physics_score, Chemistry_score, Biology_score, "
                      "Geography_score, History_score, Politics_score "
                      "FROM students WHERE id = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK){
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW){
        outStudent.grade = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        outStudent.class_value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        outStudent.id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        outStudent.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        outStudent.age = sqlite3_column_int(stmt, 4);
        outStudent.Chinese_score = sqlite3_column_double(stmt, 5);
        outStudent.Mathematics_score = sqlite3_column_double(stmt, 6);
        outStudent.English_score = sqlite3_column_double(stmt, 7);
        outStudent.Physics_score = sqlite3_column_double(stmt, 8);
        outStudent.Chemistry_score = sqlite3_column_double(stmt, 9);
        outStudent.Biology_score = sqlite3_column_double(stmt, 10);
        outStudent.Geography_score = sqlite3_column_double(stmt, 11);
        outStudent.History_score = sqlite3_column_double(stmt, 12);
        outStudent.Politics_score = sqlite3_column_double(stmt, 13);

        sqlite3_finalize(stmt);
        return true;
    } else {
        sqlite3_finalize(stmt);
        return false;
    }
}

void StuInfo3::printStudent(const StuInfo& s){
    std::cout << "===========================================================" << std::endl;
    std::cout << std::format("ID: {}\nName: {}\nGrade: {}\nAge: {}\n\nChinese score: {}\nMathematics score: {}\nEnglish: {}\nPhysics score: {}\nChemistry score: {}\nBiology score: {}\nGeography score: {}\nHistory score: {}\nPolitics: {}\n"
                                , s.id, s.name, s.grade, s.age, s.Chinese_score, s.Mathematics_score, s.English_score, s.Physics_score, s.Chemistry_score, s.Biology_score, s.Geography_score, s.History_score, s.Politics_score) 
    << std::endl;
}

std::vector<StuInfo3::StuInfo> StuInfo3::getAllStudent(sqlite3* db){
    std::vector<StuInfo> students;
    const char* sql = "SELECT grade, class_value, id, name, age, "
                      "Chinese_score, Mathematics_score, English_score, "
                      "Physics_score, Chemistry_score, Biology_score, "
                      "Geography_score, History_score, Politics_score "
                      "FROM students;";
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK){
        std::cerr << "Error preparing statement: " << std::endl;
        return students;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW){
        StuInfo s;
        s.grade = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        s.class_value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        s.id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        s.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        s.age = sqlite3_column_int(stmt, 4);
        s.Chinese_score = sqlite3_column_double(stmt, 5);
        s.Mathematics_score = sqlite3_column_double(stmt, 6);
        s.English_score = sqlite3_column_double(stmt, 7);
        s.Physics_score = sqlite3_column_double(stmt, 8);
        s.Chemistry_score = sqlite3_column_double(stmt, 9);
        s.Biology_score = sqlite3_column_double(stmt, 10);
        s.Geography_score = sqlite3_column_double(stmt, 11);
        s.History_score = sqlite3_column_double(stmt, 12);
        s.Politics_score = sqlite3_column_double(stmt, 13);
        students.push_back(s);
    }
    sqlite3_finalize(stmt);
    return students;
}

bool StuInfo3::deleteStudentByID(sqlite3* db, const std::string& id){
    const char* sql = "DELETE FROM students WHERE id = ?;";
    sqlite3_stmt* stmt;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK){
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE){
        std::cerr << "Failed to delete: " << id << std::endl;
        return false;
    }

    sqlite3_finalize(stmt);
    std::cout << "Deleted student successfully: " << id << std::endl;
    return true; 
}

bool StuInfo3::editStudentByID(sqlite3* db, const std::string& id){
    StuInfo s;
    if (!SelectStudentByID(db, id, s)) {
        std::cout << "Student with ID " << id << " not found.\n";
        return false;
    }

    while(true){
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
                // Delete old record, update ID, re-insert
                deleteStudentByID(db, s.id);
                s.id = input;
            }
        }
        else if (choice == "name") {
            std::cout << "New name: ";
            std::getline(std::cin, input);
            if (!input.empty()) s.name = input;
        }
        else if (choice == "age") {
            enterNum("New age: ", s.age);
        }
        else if (choice == "scores") {
            auto editScore = [&](const std::string& name, double& score) {
                std::cout << name << " (current: " << score << ")\n";
                enterNum("New value: ", score);
            };

            std::cout << "Chinese; Mathematics; English; Physics; Chemistry;"
                        "\nBiology; Geography; History; Politics."
            << std::endl;
            std::cout << "Choose subject: ";
            std::string choosesss;
            std::getline(std::cin, choosesss);

            // normalize input: lowercase and strip trailing semicolon
            std::transform(choosesss.begin(), choosesss.end(), choosesss.begin(), ::tolower);
            if (!choosesss.empty() && choosesss.back() == ';') choosesss.pop_back();

            if      (choosesss == "chinese")     editScore("Chinese",     s.Chinese_score);
            else if (choosesss == "mathematics" || choosesss == "math")
                                                editScore("Mathematics", s.Mathematics_score);
            else if (choosesss == "english")     editScore("English",     s.English_score);
            else if (choosesss == "physics")     editScore("Physics",     s.Physics_score);
            else if (choosesss == "chemistry")   editScore("Chemistry",   s.Chemistry_score);
            else if (choosesss == "biology")     editScore("Biology",     s.Biology_score);
            else if (choosesss == "geography")   editScore("Geography",   s.Geography_score);
            else if (choosesss == "history")     editScore("History",     s.History_score);
            else if (choosesss == "politics")    editScore("Politics",    s.Politics_score);
            else    std::cout << "Invalid subject.\n";
        }
        else {
            std::cout << "Invalid choice\n";
            continue;
        }
        if (insertStudent(db, s)) {
            std::cout << "Update success!\n";
            return true;
        } else {
            std::cout << "Update failed.\n";
            return false;
        }
    }

    
}

int StuInfo3::run(){
    sqlite3* db = nullptr;
    std::filesystem::path exe_path = std::filesystem::current_path();
    std::filesystem::path student_dir = exe_path / "Student";
    if (!std::filesystem::exists(student_dir)) {
        std::filesystem::create_directory(student_dir);
    }
    std::filesystem::path data_file = student_dir / "students.db";
    if (sqlite3_open(data_file.string().c_str(), &db) != SQLITE_OK){
        std::cerr << "Failed to open database file" << sqlite3_errmsg(db) << std::endl;
        return 1;
    }
    std::cout << "Successfully connected to the database." << std::endl;

    if(!createTable(db)){
        sqlite3_close(db);
        return 10;
    }

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
    std::cout << "Version: " << STUINFO_VERSION << std::endl;
    std::cout << std::endl;
    std::cout << "\033[0m";
    std::cout << "\nWorkplace: " << std::filesystem::current_path() << std::endl;
    std::cout << "\nWelcome to the Student Information Management System!\n" << std::endl;
    std::cout << "Please select an option:" << std::endl;
    std::cout << "1. Add Student" << std::endl;
    std::cout << "2. Delete Student" << std::endl;
    std::cout << "3. Edit Student" << std::endl;
    std::cout << "4. View Student" << std::endl;
    std::cout << "5. View All Students" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::string choice;
    while(true){
        std::cout << ">>># ";
        std::getline(std::cin, choice);
        if (choice == "1"){
            StuInfo newStu;
            enterStudent(newStu);

            insertStudent(db, newStu);
        }
        else if (choice == "2"){
            //std::cout << "Delete student functionality is not implemented yet." << std::endl;
            std::string id;
            StuInfo out;
            std::cout << "ID: ";
            std::getline(std::cin, id);
            if (!deleteStudentByID(db, id)){
                std::cerr << "Not found" << std::endl;
            }
        }
        else if (choice == "3"){
            //std::cout << "Edit student functionality is not implemented yet." << std::endl;
            std::string id;
            StuInfo out;
            std::cout << "ID: ";
            std::getline(std::cin, id);
            if (!editStudentByID(db, id)){
                std::cerr << "Failed" << std::endl;
            }
        }
        else if (choice == "5"){
            //std::cout << "View all student functionality is not implemented yet." << std::endl;
            std::vector<StuInfo> s = getAllStudent(db);
            int count = s.size();
            if (count == 0) {
                std::cout << "No more students to display." << std::endl;
            }

            for (const auto& student : s){
                printStudent(student);

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
        else if (choice == "4"){
            std::string id;
            StuInfo out;
            std::cout << "ID: ";
            std::getline(std::cin, id);
            if (SelectStudentByID(db, id, out)){
                printStudent(out);
            }
            else {
                std::cout << "Student not found" << std::endl;
            }
        }
        else if (choice == "0"){
            break;
        }
        else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }

        std::cout << "1. Add Student" << std::endl;
        std::cout << "2. Delete Student" << std::endl;
        std::cout << "3. Edit Student" << std::endl;
        std::cout << "4. View Student" << std::endl;
        std::cout << "5. View All Students" << std::endl;
        std::cout << "0. Exit" << std::endl;
    }

    sqlite3_close(db);
    
    return 0;  
}