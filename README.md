# Student Manager

A command-line student information management system written in C++.

> **Version:** 0.0.1-ALPHA

---

## Features

- Add, view, edit, and delete student records
- Each student record stored as an individual JSON file (powered by [nlohmann/json](https://github.com/nlohmann/json))
- Tracks 9 subjects per student: Chinese, Mathematics, English, Physics, Chemistry, Biology, Geography, History, and Politics
- Input validation for all numeric fields (age, scores)
- Records sorted by student ID when viewing all

---

## Planned Features

- **Score range statistics** (`Score_range`) — distribution analysis across score bands (0–10, 11–20, … 91–100), coming in a future version

---

## Dependencies

| Dependency | Purpose |
|---|---|
| [nlohmann/json](https://github.com/nlohmann/json) (`json.hpp`) | JSON serialization/deserialization for student records |
| C++17 standard library (`std::filesystem`) | File and directory management |

> `json.hpp` is bundled directly in `include/` — no separate installation needed.

---

## Requirements

- C++17 or later
- CMake 3.15+
- MinGW-w64 / MSYS2 (Windows) or GCC (Linux/macOS)

---

## Build

```bash
# Configure
cmake -S . -B out/build/x64-release

# Build
cmake --build out/build/x64-release
```

Clean rebuild:

```bash
cmake --build out/build/x64-release --clean-first
```

---

## Usage

```bash
./manager        # Linux/macOS
manager.exe      # Windows
```

Main menu:

```
1. Add Student
2. Delete Student
3. Edit Student
4. View Student
5. View All Students
0. Exit
```

### Student Fields

| Field | Type |
|---|---|
| Grade | String |
| Class | String |
| ID | String (used as filename) |
| Name | String |
| Age | Integer |
| Chinese Score | Double |
| Mathematics Score | Double |
| English Score | Double |
| Physics Score | Double |
| Chemistry Score | Double |
| Biology Score | Double |
| Geography Score | Double |
| History Score | Double |
| Politics Score | Double |

### Editing a Student

Enter the field to update when prompted:

```
grade / class / id / name / age / scores
```

Selecting `scores` walks through all 9 subjects, showing the current value before each prompt.

> **Note:** Changing a student's ID also renames their JSON file on disk.

---

## Data Storage

Records are saved as JSON files under a `Student/` directory, created automatically in the working directory:

```
Student/
  <student_id>.json
  ...
```

Example record:

```json
{
    "grade": "10",
    "class_value": "3",
    "id": "20240001",
    "name": "Zhang Wei",
    "age": 16,
    "Chinese_score": 92.5,
    "Mathematics_score": 88.0,
    "English_score": 79.5,
    "Physics_score": 85.0,
    "Chemistry_score": 91.0,
    "Biology_score": 87.5,
    "Geography_score": 83.0,
    "History_score": 90.0,
    "Politics_score": 88.5
}
```

---

## Project Structure

```
.
├── include/
│   ├── config.h
│   ├── json.hpp          # bundled nlohmann/json
│   ├── Score_range.h     # future: score distribution feature
│   └── stu-info.h
├── src/
│   ├── main.cpp
│   ├── config.cpp
│   ├── Score_range.cpp   # future: score distribution feature
│   └── stu-info.cpp
└── CMakeLists.txt
```

---

## License

This project is currently in alpha. License TBD.
