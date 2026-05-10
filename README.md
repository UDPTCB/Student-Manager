# Student Manager

A command-line student information management system written in C++.

> **Latest Version:** 0.1.3-ALPHA

---

## Versions

| Version | Status | Backend |
|---|---|---|
| 0.0.1-ALPHA | Available | JSON files (nlohmann/json) |
| 0.1.3-ALPHA | Latest | SQLite3 |

At startup, you will be prompted to choose which version to run. Enter nothing to quit.

---

## Features

- Add, view, edit, and delete student records
- Tracks 9 subjects per student: Chinese, Mathematics, English, Physics, Chemistry, Biology, Geography, History, and Politics
- Input validation for all numeric fields (age, scores), with per-subject score limits (0–100, 0–120, or 0–150)
- Records sorted by student ID when viewing all
- Score distribution statistics across score bands for all 9 subjects
- Import and export records between two databases
- Structured session logging with log rotation
- Command-based CLI with a `help` command

---

## Dependencies

| Dependency | Purpose |
|---|---|
| [nlohmann/json](https://github.com/nlohmann/json) (`json.hpp`) | JSON serialization/deserialization (v0.0.1-ALPHA only) |
| [SQLite3](https://www.sqlite.org/) (`sqlite3.h` / `sqlite3.c`) | Database backend (v0.1.3-ALPHA) |
| C++23 standard library (`std::filesystem`, `std::format`, `std::chrono`) | File/directory management, string formatting, time |

> `json.hpp`, `sqlite3.h`, and `sqlite3.c` are bundled directly in the project — no separate installation needed.

---

## Requirements

- C++23 or later
- CMake 4.3+
- MinGW-w64 / MSYS2 (Windows) or GCC (Linux/macOS)

---

## Build

```sh
# Configure
cmake -S . -B out/build

# Build
cmake --build out/build
```

Clean rebuild:

```sh
cmake --build out/build --clean-first
```

---

## Usage

```sh
./manager        # Linux/macOS
manager.exe      # Windows
```

Version selection menu:

```
1- 0.0.1-Alpha
2- 0.1.3-ALPHA
Choose a version (Keep void to quit):
```

---

## Commands (v0.1.3-ALPHA)

After selecting version 2, you will be dropped into a command prompt:

```
>>># 
```

| Command | Description |
|---|---|
| `add` | Add a new student |
| `delete` | Delete a student by ID |
| `edit` | Edit a student's information by ID |
| `view` | View a student's information by ID |
| `view-all` | View all students (paginated) |
| `score-range` | View score distribution for a subject |
| `import` | Import records from another database |
| `export` | Export records to another database |
| `help` / `h` | Show available commands and version |
| `quit` / `exit` / `q` | Exit the program |

---

## Student Fields

| Field | Type | Score limit |
|---|---|---|
| Grade | String | — |
| Class | String | — |
| ID | String | — |
| Name | String | — |
| Age | Integer | — |
| Chinese Score | Double | 0 – 120 |
| Mathematics Score | Double | 0 – 120 |
| English Score | Double | 0 – 120 |
| Physics Score | Double | 0 – 100 |
| Chemistry Score | Double | 0 – 100 |
| Biology Score | Double | 0 – 100 |
| Geography Score | Double | 0 – 100 |
| History Score | Double | 0 – 100 |
| Politics Score | Double | 0 – 100 |

---

## Editing a Student

Run `edit`, enter the target student's ID, then enter the field to update:

```
Edit (grade/class/id/name/age/scores):
```

Selecting `scores` prompts for a subject name (case-insensitive, e.g. `chinese`, `math`), shows the current value, then asks for a new one.

> **Note:** Changing a student's ID deletes the old record and re-inserts it under the new ID.

---

## Score Range

Run `score-range` and enter a subject name. The output shows how many students fall into each 10-point band:

```
0-9: 0
10-19: 2
20-29: 1
...
110-119: 5
120: 3
```

---

## Import / Export

Run `import` or `export` at the prompt. You will be asked for a direction and two file paths:

```
Import or Export? (i/e): i
From: /path/to/source.db
To: /path/to/target.db
```

All records from the source database are inserted into the target database in a single transaction. If any insert fails, the entire operation is rolled back.

---

## Data Storage

### v0.0.1-ALPHA

Records are saved as individual JSON files under a `Student/` directory, created automatically next to the executable:

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

### v0.1.3-ALPHA

Records are stored in a SQLite3 database (`students.db`) created automatically in a `Student/` directory next to the executable.

```
Student/
  students.db
```

---

## Logging

Each session writes a log file under a `logs/` directory next to the executable. Log filenames use a full datetime timestamp (`YYYYMMDD_HHMMSS.log`) so no two sessions overwrite each other.

Log levels:

| Level | Meaning |
|---|---|
| ERROR | Operation or startup failure |
| WARNING | Invalid input or failed action |
| INFO | Normal operation events |
| DEBUG | Developer-level detail |

Logs are appended to the existing file for the day. Call `rotate()` programmatically to archive the current log to `.bak` and start fresh.

---

## Project Structure

```
Manager/
├── include/
│   ├── config.h
│   ├── debug.h
│   ├── exePath.h
│   ├── json.hpp            # bundled nlohmann/json
│   ├── logger.hpp
│   ├── Score_range.h
│   ├── sqlite3.h           # bundled SQLite3
│   ├── stu-info.h          # v0.0.1-ALPHA
│   └── stu-info3.hpp       # v0.1.3-ALPHA
├── src/
│   ├── main.cpp
│   ├── config.cpp
│   ├── debug.cpp
│   ├── exePath.cpp
│   ├── logger.cpp
│   ├── Score_range.cpp
│   ├── sqlite3.c           # bundled SQLite3
│   ├── stu-info.cpp        # v0.0.1-ALPHA
│   └── stu-info3.cpp       # v0.1.3-ALPHA
├── out/                    # build output directory
└── CMakeLists.txt
```

---

## License

[GPL-3.0](LICENSE)
