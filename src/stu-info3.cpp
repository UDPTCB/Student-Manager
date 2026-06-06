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
#include "../include/stu-info3.hpp"

// ── Static utility functions ──────────────────────────────────────────────────

std::string StuInfo3::getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto local_time = std::chrono::zoned_time{std::chrono::current_zone(), now};
    return std::format("{:%Y-%m-%d--%H:%M:%S}", local_time);
}

std::string StuInfo3::getTimestampForFilename() {
    auto now = std::chrono::system_clock::now();
    auto local_time = std::chrono::zoned_time{std::chrono::current_zone(), now};
    return std::format("{:%Y%m%d}", local_time);
}

// ── Constructor / Destructor ──────────────────────────────────────────────────

StuInfo3::StuInfo3() : db(nullptr), easter_egg{
    "\t\aIt would be too easy to tell them...\n"
    "\tToo strong for this dream.\n"
    "\tTo tell them how to live is prevent them living.\n"
    "\tI will not tell the player how to live.\n"
} {}
StuInfo3::StuInfo3(const std::filesystem::path& path) : db(nullptr), easter_egg{
    "\t\aIt would be too easy to tell them...\n"
    "\tToo strong for this dream.\n"
    "\tTo tell them how to live is prevent them living.\n"
    "\tI will not tell the player how to live.\n"
} {
    openDatabase(path);
}
StuInfo3::~StuInfo3() {
    closeDatabase();
}

// ── Database operations ───────────────────────────────────────────────────────

bool StuInfo3::openDatabase(const std::filesystem::path& dbPath) {
    currentDbPath = dbPath;

    auto parentDir = dbPath.parent_path();
    if (!parentDir.empty() && !std::filesystem::exists(parentDir)) {
        std::error_code ec;
        std::filesystem::create_directories(parentDir, ec); 
        if (ec) {
            std::cerr << "Failed to create directories for: " << parentDir
                      << "\n  Reason: " << ec.message() << std::endl;
            return false;
        }
    }

    if (sqlite3_open(dbPath.string().c_str(), &db) != SQLITE_OK) {
        std::cerr << "Failed to open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    return true;
}

void StuInfo3::closeDatabase() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

// ── CRUD ──────────────────────────────────────────────────────────────────────

bool StuInfo3::createTable() {
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
            Politics_score REAL,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            total_score REAL GENERATED ALWAYS AS (
                COALESCE(Chinese_score, 0) + COALESCE(Mathematics_score, 0) + COALESCE(English_score, 0) +
                COALESCE(Physics_score, 0) + COALESCE(Chemistry_score, 0) + COALESCE(Biology_score, 0) +
                COALESCE(Geography_score, 0) + COALESCE(History_score, 0) + COALESCE(Politics_score, 0)
            ) STORED
        );
    )";
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Error creating table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}
bool StuInfo3::migrateTable() {
    // Step 1: Try adding plain columns (ignore if already exist)
    const char* plainMigrations[] = {
        "ALTER TABLE students ADD COLUMN created_at TEXT DEFAULT '';",
        "ALTER TABLE students ADD COLUMN updated_at TEXT DEFAULT '';"
    };
    for (const char* sql : plainMigrations) {
        char* errMsg = nullptr;
        int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::string err = errMsg ? errMsg : "unknown error";
            sqlite3_free(errMsg);
            if (err.find("duplicate column name") != std::string::npos) continue;
            std::cerr << "Migration error: " << err << std::endl;
            return false;
        }
    }

    // Step 2: Check if total_score exists
    bool hasTotalScore = false;
    sqlite3_stmt* checkStmt = nullptr;
    if (sqlite3_prepare_v2(db, "SELECT total_score FROM students LIMIT 1;",
                           -1, &checkStmt, nullptr) == SQLITE_OK) {
        hasTotalScore = true;
        sqlite3_finalize(checkStmt);
    }

    if (!hasTotalScore) {
        // Step 3: total_score is missing — must recreate the table
        std::cerr << "Migrating: rebuilding table to add total_score...\n";

        const char* rebuildSql = R"(
            BEGIN TRANSACTION;
            CREATE TABLE students_new (
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
                Politics_score REAL,
                created_at TEXT DEFAULT '',
                updated_at TEXT DEFAULT '',
                total_score REAL GENERATED ALWAYS AS (
                    COALESCE(Chinese_score, 0) + COALESCE(Mathematics_score, 0) +
                    COALESCE(English_score, 0) + COALESCE(Physics_score, 0) +
                    COALESCE(Chemistry_score, 0) + COALESCE(Biology_score, 0) +
                    COALESCE(Geography_score, 0) + COALESCE(History_score, 0) +
                    COALESCE(Politics_score, 0)
                ) STORED
            );
            INSERT INTO students_new (
                id, grade, class_value, name, age,
                Chinese_score, Mathematics_score, English_score,
                Physics_score, Chemistry_score, Biology_score,
                Geography_score, History_score, Politics_score,
                created_at, updated_at
            )
            SELECT
                id, grade, class_value, name, age,
                Chinese_score, Mathematics_score, English_score,
                Physics_score, Chemistry_score, Biology_score,
                Geography_score, History_score, Politics_score,
                created_at, updated_at
            FROM students;
            DROP TABLE students;
            ALTER TABLE students_new RENAME TO students;
            COMMIT;
        )";

        char* errMsg = nullptr;
        if (sqlite3_exec(db, rebuildSql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
            std::string err = errMsg ? errMsg : "unknown error";
            sqlite3_free(errMsg);
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            std::cerr << "Failed to rebuild table: " << err << std::endl;
            return false;
        }
        std::cerr << "Migration complete.\n";
    }

    // Always runs — backfills any rows with empty timestamps
    // WHERE clause ensures rows that already have timestamps are untouched
    sqlite3_exec(db,
        "UPDATE students SET created_at = datetime('now','localtime') WHERE created_at = '' OR created_at IS NULL;",
        nullptr, nullptr, nullptr);
    sqlite3_exec(db,
        "UPDATE students SET updated_at = datetime('now','localtime') WHERE updated_at = '' OR updated_at IS NULL;",
        nullptr, nullptr, nullptr);

    return true;
}
bool StuInfo3::createTriggers() {
    sqlite3_exec(db, "DROP TRIGGER IF EXISTS update_students_timestamp;", nullptr, nullptr, nullptr);

    const char* sql = R"(
        CREATE TRIGGER IF NOT EXISTS update_students_timestamp 
        AFTER UPDATE ON students
        BEGIN
            UPDATE students SET updated_at = datetime('now', 'localtime')
            WHERE id = NEW.id;
        END;
    )";
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Error creating trigger: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}
bool StuInfo3::insertStudent(const Student& student) {
    
    const char* sql = R"(
        INSERT OR REPLACE INTO students (
            id, grade, class_value, name, age,
            Chinese_score, Mathematics_score, English_score,
            Physics_score, Chemistry_score, Biology_score,
            Geography_score, History_score, Politics_score,
            created_at, updated_at
        ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?,
                  COALESCE(?, ?),
                  ?);
    )";
    Statement stmt(db, sql);

    std::string now = getCurrentTime(); // ← local time from C++

    sqlite3_bind_text(stmt.get(), 1,  student.id.c_str(),          -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt.get(), 2,  student.grade.c_str(),        -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt.get(), 3,  student.class_value.c_str(),  -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt.get(), 4,  student.name.c_str(),         -1, SQLITE_TRANSIENT);
    sqlite3_bind_int (stmt.get(), 5,  student.age);
    sqlite3_bind_double(stmt.get(), 6,  student.Chinese_score);
    sqlite3_bind_double(stmt.get(), 7,  student.Mathematics_score);
    sqlite3_bind_double(stmt.get(), 8,  student.English_score);
    sqlite3_bind_double(stmt.get(), 9,  student.Physics_score);
    sqlite3_bind_double(stmt.get(), 10, student.Chemistry_score);
    sqlite3_bind_double(stmt.get(), 11, student.Biology_score);
    sqlite3_bind_double(stmt.get(), 12, student.Geography_score);
    sqlite3_bind_double(stmt.get(), 13, student.History_score);
    sqlite3_bind_double(stmt.get(), 14, student.Politics_score);

    // created_at: keep existing if present, otherwise use local now
    if (!student.created_at.empty()) {
        sqlite3_bind_text(stmt.get(), 15, student.created_at.c_str(), -1, SQLITE_TRANSIENT);
    } else {
        sqlite3_bind_null(stmt.get(), 15);
    }
    sqlite3_bind_text(stmt.get(), 16, now.c_str(), -1, SQLITE_TRANSIENT); // fallback for COALESCE
    sqlite3_bind_text(stmt.get(), 17, now.c_str(), -1, SQLITE_TRANSIENT); // updated_at

    if (sqlite3_step(stmt.get()) != SQLITE_DONE) {
        //std::cerr << "Error inserting student: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    //std::cout << "Student inserted successfully!" << std::endl;
    return true;
}

bool StuInfo3::existsStudentByID(const std::string& id){
    const char* sql = "SELECT id FROM students WHERE id = ? LIMIT 1;";
    Statement stmt(db, sql);
    sqlite3_bind_text(stmt.get(), 1, id.c_str(), -1, SQLITE_TRANSIENT);
    return sqlite3_step(stmt.get()) == SQLITE_ROW;
}
bool StuInfo3::selectStudentByID(const std::string& id, Student& outStudent) {
    const char* sql = "SELECT grade, class_value, id, name, age, "
                      "Chinese_score, Mathematics_score, English_score, "
                      "Physics_score, Chemistry_score, Biology_score, "
                      "Geography_score, History_score, Politics_score, "
                      "created_at, updated_at, total_score "
                      "FROM students WHERE id = ?;";
    Statement stmt(db, sql);
    

    sqlite3_bind_text(stmt.get(), 1, id.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt.get()) == SQLITE_ROW) {
        outStudent.grade       = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 0));
        outStudent.class_value = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 1));
        outStudent.id          = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 2));
        outStudent.name        = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 3));
        outStudent.age                = sqlite3_column_int(stmt.get(), 4);
        outStudent.Chinese_score      = sqlite3_column_double(stmt.get(), 5);
        outStudent.Mathematics_score  = sqlite3_column_double(stmt.get(), 6);
        outStudent.English_score      = sqlite3_column_double(stmt.get(), 7);
        outStudent.Physics_score      = sqlite3_column_double(stmt.get(), 8);
        outStudent.Chemistry_score    = sqlite3_column_double(stmt.get(), 9);
        outStudent.Biology_score      = sqlite3_column_double(stmt.get(), 10);
        outStudent.Geography_score    = sqlite3_column_double(stmt.get(), 11);
        outStudent.History_score      = sqlite3_column_double(stmt.get(), 12);
        outStudent.Politics_score     = sqlite3_column_double(stmt.get(), 13);
        outStudent.created_at = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 14));
        outStudent.updated_at = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 15));
        outStudent.total_score = sqlite3_column_double(stmt.get(), 16);
        
        return true;
    } else {
        return false;
    }
}

std::vector<StuInfo3::Student> StuInfo3::getAllStudent() {
    std::vector<Student> students;
    const char* sql = "SELECT grade, class_value, id, name, age, "
                      "Chinese_score, Mathematics_score, English_score, "
                      "Physics_score, Chemistry_score, Biology_score, "
                      "Geography_score, History_score, Politics_score, "
                      "created_at, updated_at, total_score "
                      "FROM students;";
    Statement stmt(db, sql);

    while (sqlite3_step(stmt.get()) == SQLITE_ROW) {
        Student s;
        s.grade       = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 0));
        s.class_value = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 1));
        s.id          = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 2));
        s.name        = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 3));
        s.age                = sqlite3_column_int(stmt.get(), 4);
        s.Chinese_score      = sqlite3_column_double(stmt.get(), 5);
        s.Mathematics_score  = sqlite3_column_double(stmt.get(), 6);
        s.English_score      = sqlite3_column_double(stmt.get(), 7);
        s.Physics_score      = sqlite3_column_double(stmt.get(), 8);
        s.Chemistry_score    = sqlite3_column_double(stmt.get(), 9);
        s.Biology_score      = sqlite3_column_double(stmt.get(), 10);
        s.Geography_score    = sqlite3_column_double(stmt.get(), 11);
        s.History_score      = sqlite3_column_double(stmt.get(), 12);
        s.Politics_score     = sqlite3_column_double(stmt.get(), 13);
        s.created_at         = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 14));
        s.updated_at         = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 15));
        s.total_score        = sqlite3_column_double(stmt.get(), 16);
        students.push_back(s);
    }
   

    std::stable_sort(students.begin(), students.end(),
        [](const Student& a, const Student& b) {
            return a.id < b.id;
        });
    return students;
}

bool StuInfo3::deleteStudentByID(const std::string& id) {
    const char* sql = "DELETE FROM students WHERE id = ?;";
    Statement stmt(db, sql);

    sqlite3_bind_text(stmt.get(), 1, id.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt.get()) != SQLITE_DONE) {
        //std::cerr << "Failed to delete: " << id << std::endl;
        return false;
    }
    
    //std::cout << "Deleted student successfully: " << id << std::endl;
    return true;
}

bool StuInfo3::editStudentByID(const std::string& id) {
    Student s;

    if (!selectStudentByID(id, s)) {
        std::cout << "Student with ID " << id << " not found.\n";
        return false;
    }

    while (true) {
        std::cout << "Edit (grade/class/id/name/age/scores) or enter 'done' to exit: ";
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
                deleteStudentByID(s.id);
                s.id = input;
            }
        }
        else if (choice == "name") {
            std::cout << "New name: ";
            std::getline(std::cin, input);
            if (!input.empty()) s.name = input;
        }
        else if (choice == "age") {
            enterNum("New age: ", s.age, 0, 0);
        }
        else if (choice == "scores") {
            auto editScore = [&](const std::string& name, double& score, const int& m) {
                std::cout << name << " (current: " << score << ")\n";
                enterNum("New value: ", score, 1, m);
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

            if      (choosesss == "chinese")                          editScore("Chinese",     s.Chinese_score, 1);
            else if (choosesss == "mathematics" || choosesss == "math") editScore("Mathematics", s.Mathematics_score, 1);
            else if (choosesss == "english")                          editScore("English",     s.English_score, 1);
            else if (choosesss == "physics")                          editScore("Physics",     s.Physics_score, 0);
            else if (choosesss == "chemistry")                        editScore("Chemistry",   s.Chemistry_score, 0);
            else if (choosesss == "biology")                          editScore("Biology",     s.Biology_score, 0);
            else if (choosesss == "geography")                        editScore("Geography",   s.Geography_score, 0);
            else if (choosesss == "history")                          editScore("History",     s.History_score, 0);
            else if (choosesss == "politics")                         editScore("Politics",    s.Politics_score, 0);
            else    std::cout << "Invalid subject.\n";
        }
        else if (choice == "done") {
            break;
        }
        else {
            std::cout << "Invalid choice\n";
            continue;
        }
        if (insertStudent(s)) {
            std::cout << "Update success!\n";
            return true;
        } else {
            std::cout << "Update failed.\n";
            return false;
        }
    }
    
    return false;
}

// ── Display operations ────────────────────────────────────────────────────────
void StuInfo3::printStudent(const Student& s) const {
    std::cout << "===========================================================" << std::endl;
    std::cout << std::format(
        "ID: {}\nName: {}\nGrade: {}\nAge: {}\n\n"
        "Chinese score: {}\nMathematics score: {}\n"
        "English: {}\nPhysics score: {}\nChemistry score: {}\n"
        "Biology score: {}\nGeography score: {}\nHistory score: {}\nPolitics: {}\n\n"
        "Total score: {}\nCreated at: {}\nUpdated at: {}\n",
        s.id, s.name, s.grade, s.age,
        s.Chinese_score, s.Mathematics_score, s.English_score,
        s.Physics_score, s.Chemistry_score, s.Biology_score,
        s.Geography_score, s.History_score, s.Politics_score,
        s.total_score, s.created_at, s.updated_at)
    << std::endl;
}
void StuInfo3::scoreRangeShow(std::string subject) {
    std::vector<Student> sse = getAllStudent();
    if (sse.empty()){
        std::cout << "No students found." << std::endl;
        return;
    }
    std::string choo = subject;
    int mode = 0;
    if (subject.empty()) {
        std::cout << "Chinese; Mathematics; English; Physics; Chemistry;"
                            "\nBiology; Geography; History; Politics."
                << std::endl;
        std::cout << "Choose a subject: ";
        std::string chooa{};
        
        std::getline(std::cin, chooa);
        choo = chooa;
    }
    std::transform(choo.begin(), choo.end(), choo.begin(), ::tolower);
    if (!choo.empty() && choo.back() == ';') {
        choo.pop_back();
    }
    if (choo == "chinese") mode = 0;
    else if (choo == "mathematics") mode = 1;
    else if (choo == "english") mode = 2;
    else if (choo == "physics") mode = 3;
    else if (choo == "chemistry") mode = 4;
    else if (choo == "biology") mode = 5;
    else if (choo == "geography") mode = 6;
    else if (choo == "history") mode = 7;
    else if (choo == "politics") mode = 8;
    else std::cout << "Invalid subject" << std::endl;;
    if (SRFM::SRF_M(sse, mode)) {
        std::cout << "Displayed successfully" << std::endl;
    } else {
        std::cout << "Failed to displayed" << std::endl;
    }
    
}

// ── Input operations ──────────────────────────────────────────────────────────

void StuInfo3::enterStudent(Student& newStu, int mode) {

    if (mode == 0){
        std::cout << "ID: ";
        std::getline(std::cin, newStu.id);
        if (newStu.id.empty()) {
            std::cout << "ID cannot be empty. Please enter a valid ID." << std::endl;
            return;
        }
    }
    std::cout << "Grade: ";
    std::getline(std::cin, newStu.grade);
    if (newStu.grade.empty()) {
        std::cout << "empty grade, set to default value 'N/A'." << std::endl;
        newStu.grade = "N/A";
    }
    std::cout << "Class: ";
    std::getline(std::cin, newStu.class_value);
    if (newStu.class_value.empty()) {
        std::cout << "empty class, set to default value 'N/A'." << std::endl;
        newStu.class_value = "N/A";
    }
    std::cout << "Name: ";
    std::getline(std::cin, newStu.name);
    if (newStu.name.empty()) {
        std::cout << "empty name, set to default value 'N/A'." << std::endl;
        newStu.name = "N/A";
    }
    enterNum("Age: ", newStu.age, 0, 0);
    enterNum("Chinese score: ", newStu.Chinese_score, 1, 1);
    enterNum("Mathematics score: ", newStu.Mathematics_score, 1, 1);
    enterNum("English score: ", newStu.English_score, 1, 1);
    enterNum("Physics score: ", newStu.Physics_score, 1, 0);
    enterNum("Chemistry: ", newStu.Chemistry_score, 1, 0);
    enterNum("Biology score: ", newStu.Biology_score, 1, 0);
    enterNum("Geography score: ", newStu.Geography_score, 1, 0);
    enterNum("History: ", newStu.History_score, 1, 0);
    enterNum("Politics: ", newStu.Politics_score, 1, 0);
}

// ── Insert Students from vector ────────────────────────────────────────────────────────
bool StuInfo3::insertStudents(const std::vector<Student>& students) {
    Transaction tn(db); // RAII transaction management
    
    for (const auto& student : students) {
        if (!insertStudent(student)) {
            try {
                throw std::runtime_error("Failed to insert student with ID: " + student.id);
            }  catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
                return false;
            }
        }
    }

    tn.commit();
    return true;
}

// ── Import / Export ─────────────────────────────────────────────────────────
bool impOrExp::importFromDB(
    const std::filesystem::path& fmPath,
    const std::filesystem::path& toPath)
{
    if (!from.openDatabase(fmPath)) {
        std::cerr << "Failed to open source database: "
                  << fmPath.string() << std::endl;

        return false;
    }

    if (!from.createTable()) {
        std::cerr << "Failed to create table in source database: "
                  << fmPath.string() << std::endl;

        return false;
    }

    if (!from.migrateTable()) {
        std::cerr << "Failed to migrate table in source database: "
                  << fmPath.string() << std::endl;

        return false;
    }

    if (!from.createTriggers()) {
        std::cerr << "Warning: Failed to create triggers in source database: "
                  << fmPath.string() << std::endl;
    }

    

    std::vector<StuInfo3::Student> students = from.getAllStudent();

    if (!to.openDatabase(toPath)) {
        std::cerr << "Failed to open target database: "
                  << toPath.string() << std::endl;

        return false;
    }

    if (!to.createTable()) {
        std::cerr << "Failed to create table in target database: "
                  << toPath.string() << std::endl;

        return false;
    }

    if (!to.migrateTable()) {
        std::cerr << "Failed to migrate table in target database: "
                  << toPath.string() << std::endl;

        return false;
    }

    if (!to.createTriggers()) {
        std::cerr << "Warning: Failed to create triggers in target database: "
                  << toPath.string() << std::endl;
    }

    if (!to.insertStudents(students)) {
        std::cerr << "Failed to insert students into target database: "
                  << toPath.string() << std::endl;

        return false;
    }

    std::cout << "Import successful from "
              << fmPath.string()
              << " to "
              << toPath.string()
              << std::endl;

    return true;
}
bool impOrExp::exportToDB(
    const std::filesystem::path& fmPath,
    const std::filesystem::path& toPath)
{
    return importFromDB(fmPath, toPath);
}

// ── Import export edit ───────────────────────────────────────────────────────────

bool impOrExp::runapp() {
    std::cout << "Import or Export? (i/e): ";
    std::string choice;
    std::string input;
    std::getline(std::cin, choice);

    auto cleanString = [](std::string& s) {
        
        if (!s.empty() && s.back() == '\r') s.pop_back();
       
        if (!s.empty() && s.front() == '"') s.erase(0, 1);
        if (!s.empty() && s.back() == '"') s.pop_back();
    };
    cleanString(choice);

    if (choice == "i") {
        std::filesystem::path fromPath, toPath;
        std::cout << "From: ";
        std::getline(std::cin, input); cleanString(input); fromPath = input;
        std::cout << "To: ";
        std::getline(std::cin, input); cleanString(input); toPath = input;
        return importFromDB(fromPath, toPath);
    } else if (choice == "e") {
        std::filesystem::path fromPath, toPath;
        std::cout << "From: ";
        std::getline(std::cin, input); cleanString(input); fromPath = input;
        std::cout << "To: ";
        std::getline(std::cin, input); cleanString(input); toPath = input;
        return exportToDB(fromPath, toPath);
    } else {
        std::cout << "Invalid choice." << std::endl;
        return false;
    }
}



// ── Standalone function ───────────────────────────────────────────────────────


int stu::runStuInfo3(logger& log) {
    StuInfo3 manager;
    std::filesystem::path exe_path   = get_executable_path();
    
  
    std::filesystem::path studir = exe_path / "Student";
    if (!std::filesystem::exists(studir)) {
        if (!std::filesystem::create_directory(studir)){

        }
    }

    std::filesystem::path data_file  = studir / "students.db";

    

    std::cout << "\033[33m";
    std::cout << R"(

_____  _             _            _   __  __                                   
/ ____| |           | |          | | |  \/  |                                  
| (___| |_ _   _  __| | ___ _ __ | |_| \  / | __ _ _ __   __ _  __ _  ___ _ __ 
\___ \| __| | | |/ _` |/ _ \ '_ \| __| |\/| |/ _` | '_ \ / _` |/ _` |/ _ \ '__|
____) | |_| |_| | (_| |  __/ | | | |_| |  | | (_| | | | | (_| | (_| |  __/ |   
|_____/\__|\__,_|\__,_|\___|_| |_|\__|_|  |_|\__,_|_| |_|\__,_|\__, |\___|_|   
                                                                __/ |           
                                                                |___/  
- Alpha Version -
    
    )" << std::endl;
    /*
     *\033[30m : black
     *\033[31m : red
     *\033[32m : green
     *\033[33m : yellow
     *\033[34m : blue
     *\033[35m : magenta
     *\033[36m : cyan
     *\033[37m : white
     *\033[0m  : reset
     */
    std::cout << "\033[0m";
    std::cout << "\033[34m";
    std::cout << "Database-Version: " << StuInfo3::getVersion() << std::endl;
    std::cout << "CLI-Version: " << CLI_VERSION << std::endl;
    std::cout << std::endl;
    std::cout << "\033[0m";

    if (!manager.openDatabase(data_file)) {
        std::cerr << "Failed to open database" << std::endl;
        log.log("Failed to open database: " + data_file.string(), 0);
        log.log(StuInfo3::getVersion() + " Program exited.", 2);
        return 1;
    }
    std::cout << "Successfully connected to the database." << std::endl;
    log.log("Successfully connected to the database: " + data_file.string(), 2);
    if (!manager.createTable()) {
        std::cerr << "Failed to create table in database" << std::endl;
        log.log("Failed to create table in database: " + data_file.string(), 0);
        return 10;
    }
    if (!manager.migrateTable()) {         
        std::cerr << "Failed to migrate table" << std::endl;
        log.log("Migration failed: " + data_file.string(), 0);
        return 11;
    }
    if (!manager.createTriggers()) {
        std::cerr << "Warning: Failed to create triggers" << std::endl;
        log.log("Failed to create triggers in database: " + data_file.string(), 1);
    }
    std::cout << "\nWorkplace: " << get_executable_path() << std::endl;
    log.log("Workplace: " + get_executable_path().string(), 2);
    
    std::string choice;
    while (true) {
        std::cout << ">>># ";
        std::getline(std::cin, choice);
        if (choice == "add") {
            StuInfo3::Student newStu;
            std::string id{};
            std::cout << "ID: ";
            std::getline(std::cin, id);
            newStu.id = id;
            
            if (newStu.id.empty()) {
                std::cout << "Student ID cannot be empty. Student not added." << std::endl;
                log.log("Failed to add student: empty ID", 0);
                continue;
            }

            
            if (manager.existsStudentByID(newStu.id)){
                std::cout << "A student with ID " << newStu.id << " already exists. Do you want to edit it? (y/n): ";
                std::string overwriteChoice;
                std::getline(std::cin, overwriteChoice);
                if (overwriteChoice != "y" && overwriteChoice != "Y") {
                    std::cout << "Student not added." << std::endl;
                    log.log("Add student cancelled by user: " + newStu.id, 1);
                    continue;
                }
                else if (overwriteChoice == "y" || overwriteChoice == "Y") {
                    std::cout << "Editing existing student with ID " << newStu.id << "." << std::endl;
                    log.log("Editing existing student: " + newStu.id, 1);
                    manager.editStudentByID(newStu.id);
                    continue;
                }
            }

            manager.enterStudent(newStu, 1);
            
            

            if (manager.insertStudent(newStu)){
                std::cout << "Student added successfully." << std::endl;
                log.log("Student added: " + newStu.name + " (ID: " + newStu.id + ")", 2);
            } else {
                std::cerr << "Failed to add student." << std::endl;
                log.log("Failed to add student: " + newStu.name, 0);
            }
        }
        else if (choice == "delete") {
            std::string id;
            std::cout << "ID: ";
            std::getline(std::cin, id);
            if (!manager.deleteStudentByID(id)) {
                std::cerr << "Not found" << std::endl;
                log.log("Failed to delete student with ID: " + id, 0);
            } else {
                log.log("Student deleted: ID " + id, 2);
            }
        }
        else if (choice == "edit") {
            std::string id;
            std::cout << "ID: ";
            std::getline(std::cin, id);
            if (!manager.editStudentByID(id)) {
                std::cerr << "Failed" << std::endl;
                log.log("Failed to edit student with ID: " + id, 0);
            } else {
                log.log("Student edited: ID " + id, 2);
            }
        }
        else if (choice == "view") {
            std::string id;
            StuInfo3::Student out;
            std::cout << "ID: ";
            std::getline(std::cin, id);
            if (manager.selectStudentByID(id, out)) {
                manager.printStudent(out);
            } else {
                std::cout << "Student not found" << std::endl;
                log.log("Student not found with ID: " + id, 0);
            }
            log.log("Student viewed: ID " + id, 2);
        }
        else if (choice == "view-all") {
            std::vector<StuInfo3::Student> s = manager.getAllStudent();
            log.log("Viewed all students. Total count: " + std::to_string(s.size()), 2);
            int count = s.size();
            if (count == 0) {
                std::cout << "No more students to display." << std::endl;
            }
            for (const auto& student : s) {
                manager.printStudent(student);
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
        else if (choice == "score-range") {
            log.log("Viewing score range for a subject.", 2);
            manager.scoreRangeShow();
        }
        else if (choice == "quit" || choice == "exit" || choice == "q") {
            log.log("Exiting the program.", 2);
            break;
        }

        else if (choice == "help" || choice == "h") {
            std::cout << StuInfo3::getVersion() <<"\n"
                      << "Available commands:\n"
                      << "  add         - Add a new student\n"
                      << "  delete      - Delete a student by ID\n"
                      << "  delete <id> - Delete a student by ID without prompt\n"
                      << "  edit        - Edit a student's information by ID\n"
                      << "  edit <id>   - Edit a student's information by ID without input id prompt\n"
                      << "  view        - View a student's information by ID\n"
                      << "  view <id>   - View a student's information by ID without input id prompt\n"
                      << "  view-all    - View all students' information\n"
                      << "  score-range  - View score range for a subject\n"
                      << "  score-range <subject> - View score range for a specific subject\n"
                      << "  help         - Show this help message\n"
                      << "  quit/exit/q   - Exit the program\n"
                      << "  import/export - Import or export student data\n";
            log.log("Help information displayed.", 2);
        }
        else if (choice == "import" || choice == "export") {
            log.log("Running import/export operation.", 2);
            impOrExp io;
            if (!io.runapp()) {
                std::cerr << "Import/Export operation failed." << std::endl;
                log.log("Import/Export operation failed.", 1);
            }
        }
        else if (choice == "minecraft" || choice == "MINECRAFT" || choice == "Minecraft") {
            std::cout << manager.easter_egg << std::endl;
            log.log("Easter egg revealed.", 2);
        }
        else {
            std::cout << "Invalid choice. Please try again." << std::endl;
            log.log("Invalid command entered: " + choice, 1);
        }

        
    }
    log.log(StuInfo3::getVersion() + " Program exited.", 2);
    return 0;
}

