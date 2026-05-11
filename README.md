# Student Manager

A command-line student information management system written in C++.

> **Latest Version:** 0.1.4-ALPHA  
> **License:** [GNU General Public License v3.0 or later](LICENSE)  
> **Author:** Yvhang Cai (Jeffrey Miller) — jeffrey_miller_GD@outlook.com

---

## Versions

| Version | Status | Backend |
|---|---|---|
| 0.0.1-ALPHA | Legacy / Available | JSON files (nlohmann/json) |
| 0.1.4-ALPHA | **Latest** | SQLite3 |

At startup (interactive mode), you will be prompted to choose which version to run. Enter nothing to quit.

---

## Features

- Add, view, edit, and delete student records
- Tracks 9 subjects per student: Chinese, Mathematics, English, Physics, Chemistry, Biology, Geography, History, and Politics
- Input validation for all numeric fields (age and per-subject score limits)
- Records sorted by student ID when viewing all
- Score range distribution statistics across all 9 subjects
- **[New in 0.1.4]** Persistent logging to dated log files
- **[New in 0.1.4]** Full command-line interface (CLI) for non-interactive / scriptable use
- **[New in 0.1.4]** Database import / export between SQLite3 files
- **[New in 0.1.4]** RAII-safe prepared statements and transaction support

### Version 0.0.1-ALPHA
- Each student record stored as an individual JSON file (powered by [nlohmann/json](https://github.com/nlohmann/json))

### Version 0.1.1-ALPHA
- All student records stored in a single SQLite3 database

### Version 0.1.2-ALPHA
- Score range statistics — distribution analysis across score bands (0–9, 10–19, … up to 150) for all 9 subjects
- Enhanced per-subject score limit validation (0–100, 0–120, or 0–150)

### Version 0.1.4-ALPHA
- **Logging system** — application events, errors, and user actions written to `logs/<date>.log`
- **CLI mode** — run operations directly via command-line arguments without entering the interactive menu
- **Import / Export** — copy student records between SQLite3 database files via the `impOrExp` class
- **Transaction safety** — bulk inserts wrapped in SQLite3 transactions for performance and data integrity
- **RAII wrappers** — `Statement` and `Transaction` classes prevent resource leaks from prepared statements

---

## Dependencies

| Dependency | Purpose |
|---|---|
| [nlohmann/json](https://github.com/nlohmann/json) (`json.hpp`) | JSON serialization (v0.0.1-ALPHA only) |
| [SQLite3](https://www.sqlite.org/) (`sqlite3.h` / `sqlite3.c`) | Database backend (v0.1.x-ALPHA) |
| C++23 standard library | `std::filesystem`, `std::format`, `std::chrono::zoned_time`, etc. |

> `json.hpp`, `sqlite3.h`, and `sqlite3.c` are bundled directly in the project — no separate installation needed.

---

## Requirements

- C++23 or later
- CMake 4.3+
- MinGW-w64 / MSYS2 (Windows) or GCC 13+ (Linux)

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

### Package (optional)

```sh
# Windows → NSIS installer
# Linux   → .tar.gz archive
cmake --build out/build --target package
```

---

## Usage

```sh
./manager        # Linux
manager.exe      # Windows
```

### Interactive Mode (no arguments)

Version selection menu:

```
1- 0.0.1-ALPHA
2- 0.1.4-ALPHA
>>># (Keep void to quit):
```

Main menu (0.1.4-ALPHA):

```
1. Add Student
2. Delete Student
3. Edit Student
4. View Student
5. View All Students
6. View all students score range
0. Exit
```

### CLI Mode (with arguments)

All CLI operations target the SQLite3 backend (0.1.4-ALPHA).

```sh
manager --help                  # Show help
manager --version               # Print version and exit
manager --add                   # Interactively add a new student
manager --delete                # Prompt for ID then delete
manager --delete <ID>           # Delete student with given ID
manager --edit                  # Prompt for ID then edit
manager --edit <ID>             # Edit student with given ID
manager --view                  # Prompt for ID then view
manager --view <ID>             # View student with given ID
manager --view-all              # Page through all students
manager --score-range           # View score distribution for a subject
manager --import                # Launch import/export utility
manager --export                # Launch import/export utility
```

---

## Student Fields

| Field | Type | Notes |
|---|---|---|
| Grade | String | e.g. "10" |
| Class | String | e.g. "3" |
| ID | String | Unique identifier |
| Name | String | |
| Age | Integer | |
| Chinese Score | Double | 0 – 120 |
| Mathematics Score | Double | 0 – 150 |
| English Score | Double | 0 – 120 |
| Physics Score | Double | 0 – 150 |
| Chemistry Score | Double | 0 – 150 |
| Biology Score | Double | 0 – 100 |
| Geography Score | Double | 0 – 100 |
| History Score | Double | 0 – 100 |
| Politics Score | Double | 0 – 100 |

### Editing a Student

Enter the field name to update when prompted:

```
grade / class / id / name / age / scores
```

Selecting `scores` walks through all 9 subjects, showing the current value before each prompt.

> **Note (v0.0.1-ALPHA):** Changing a student's ID also renames their JSON file on disk.

---

## Data Storage

### Version 0.0.1-ALPHA

Records are saved as individual JSON files under a `Student/` directory created automatically next to the executable:

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

### Version 0.1.x-ALPHA

Records are stored in a SQLite3 database (`Student/students.db`) created automatically next to the executable.

---

## Logging

Starting from 0.1.4-ALPHA, the application writes a log file to:

```
logs/<YYYYMMDD>.log
```

Log entries use the format:

```
[LEVEL] YYYY-MM-DD HH:MM:SS: message
```

Levels: `INFO`, `WARNING`, `ERROR`, `DEBUG`.

The `logs/` directory is created automatically on first run. Log files can be rotated via `logger::rotate()`, which backs up the current file with a `.bak` extension and opens a fresh log.

---

## Project Structure

```
Manager/
├── include/
│   ├── config.h            # Main class declaration
│   ├── debug.h             # Debug utilities (disabled)
│   ├── exePath.h           # Executable path helper
│   ├── json.hpp            # Bundled nlohmann/json
│   ├── logger.hpp          # Logging system (new in 0.1.4)
│   ├── Score_range.h       # Score distribution templates
│   ├── sqlite3.h           # Bundled SQLite3
│   ├── stu-info.h          # v0.0.1-ALPHA (JSON backend)
│   └── stu-info3.hpp       # v0.1.x-ALPHA (SQLite3 backend)
├── src/
│   ├── main.cpp
│   ├── config.cpp          # Application entry logic & CLI
│   ├── debug.cpp           # Debug stub (disabled)
│   ├── exePath.cpp
│   ├── logger.cpp          # Logging implementation (new in 0.1.4)
│   ├── Score_range.cpp
│   ├── sqlite3.c           # Bundled SQLite3
│   ├── stu-info.cpp        # v0.0.1-ALPHA implementation
│   └── stu-info3.cpp       # v0.1.x-ALPHA implementation
├── resources/
│   └── myapp.rc            # Windows icon resource
├── out/                    # Build output (generated)
├── CMakeLists.txt
└── LICENSE
```

---

## Known Issues / Limitations

- Version 0.0.1-ALPHA (JSON backend) is considered legacy and may be removed in a future release.
- The `debug` module is compiled out and contains no functionality.
- macOS support is not verified.

---

## Changelog

| Version | Highlights |
|---|---|
| **0.1.4-ALPHA** | Logging system, CLI mode, import/export, transaction safety, RAII wrappers |
| 0.1.2-ALPHA | Score range statistics, per-subject score limit validation |
| 0.1.1-ALPHA | Migrated from JSON files to SQLite3 |
| 0.0.1-ALPHA | Initial release with JSON file backend |

---

## License

Student Manager is free software: you can redistribute it and/or modify it under the terms of the [GNU General Public License](LICENSE) as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

```
Student Manager  Copyright (C) 2026  Yvhang Cai (Jeffrey Miller)
This program comes with ABSOLUTELY NO WARRANTY.
This is free software, and you are welcome to redistribute it
under certain conditions; see LICENSE for details.
```
