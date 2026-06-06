# Student Manager

> A cross-platform student information management system with CLI and TUI interfaces.

**Student Manager** is a free, open-source tool for managing student records. It stores data in a local SQLite database, supports full CRUD operations, score-range analysis, import/export, structured logging, and a polished terminal UI built with FTXUI.

---

## Features

- **Three interface modes**: Legacy CLI (v0.0.1-ALPHA), modern CLI (v0.1.5-Gamma), and TUI (v0.0.1-Beta)
- **SQLite-backed storage** — persistent, portable, no server required
- **Full CRUD** — add, view, edit, and delete student records
- **9-subject score tracking** — Chinese, Mathematics, English, Physics, Chemistry, Biology, Geography, History, Politics
- **Score-range analysis** — histogram-style distribution per subject
- **Import / Export** — transfer records between databases
- **Structured logging** — timestamped log files with ERROR / WARNING / INFO / DEBUG levels
- **Schema migration** — safely upgrades existing databases to new versions
- **GPL-3.0 licensed** — free software you can study, modify, and redistribute

---

## Requirements

| Requirement | Details |
|---|---|
| CMake | 3.21 or later |
| C++ Standard | C++23 |
| SQLite | Bundled (`sqlite3.c` / `sqlite3.h`) |
| FTXUI | Must be installed and findable by CMake (`find_package(ftxui REQUIRED)`) |
| nlohmann/json | Bundled (`json.hpp`) |
| Platform | Linux, Windows |

---

## Building

```bash
git clone https://github.com/UDPTCB/Student-Manager.git
cd Student-Manager
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

The build system automatically creates `Student/` and `logs/` directories on installation:

```bash
cmake --install build
```

On Windows a NSIS installer package can be generated via CPack; on Linux a `.tar.gz` archive is produced:

```bash
cd build && cpack
```

---

## FTXUI

The TUI module requires the [FTXUI](https://github.com/ArthurSonzogni/FTXUI) library. **Users are responsible for installing and linking FTXUI themselves** — how you obtain it depends on your toolchain and platform.

The author develops on **MSYS2 UCRT64 (GCC)**. The following shows the author's setup for reference.

**`CMakeLists.txt` — finding and linking FTXUI:**

```cmake
find_package(ftxui REQUIRED)

target_link_libraries(manager PRIVATE
    ftxui::screen
    ftxui::dom
    ftxui::component
)
```

**`tui.hpp` — required FTXUI includes:**

```cpp
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
```

On MSYS2 UCRT64, FTXUI can be installed with:

```bash
pacman -S mingw-w64-ucrt-x86_64-ftxui
```

For other platforms and package managers, refer to the [FTXUI documentation](https://github.com/ArthurSonzogni/FTXUI) for installation instructions.

---

## Usage

### Interactive mode (no arguments)

```
./StudentManager
```

You will be prompted to choose:

```
1- 0.0.1-ALPHA       (legacy JSON-based CLI)
2- CLI Version        (SQLite CLI, current)
3- TUI Version        (terminal UI, new)
quit- Exit
```

Type `show w` for warranty details or `show c` for redistribution conditions.

### Command-line mode

```bash
./StudentManager --help
```

| Command | Description |
|---|---|
| `--add` | Add a new student (interactive prompts) |
| `--delete` | Delete a student (prompts for ID) |
| `--delete <id>` | Delete a student without ID prompt |
| `--edit` | Edit a student record (prompts for ID) |
| `--edit <id>` | Edit without ID prompt |
| `--view` | View a student record (prompts for ID) |
| `--view <id>` | View without ID prompt |
| `--view-all` | Page through all student records |
| `--score-range` | Show score distribution for all subjects |
| `--score-range <subject>` | Show score distribution for a specific subject |
| `--import` / `--export` | Import or export student data between databases |
| `--version` | Print current version string |
| `--help` / `-h` | Show this command reference |

### TUI mode

The TUI (Terminal User Interface) is built with [FTXUI](https://github.com/ArthurSonzogni/FTXUI) and provides a keyboard-driven, mouse-optional interface for all student management operations without needing to remember command-line flags.

Launch it from the interactive menu by selecting option `3`, or access it directly from your shell session.

---

## Data & File Layout

```
<executable directory>/
├── Student/
│   └── students.db        # SQLite database
└── logs/
    └── YYYYMMDD.log       # Daily log files
```

---

## Student Record Fields

| Field | Type | Notes |
|---|---|---|
| `id` | string | Unique identifier (required) |
| `name` | string | Student name |
| `grade` | string | Grade / year |
| `class_value` | string | Class identifier |
| `age` | integer | |
| `Chinese_score` | double | 0–150 |
| `Mathematics_score` | double | 0–150 |
| `English_score` | double | 0–150 |
| `Physics_score` | double | 0–150 |
| `Chemistry_score` | double | 0–100 |
| `Biology_score` | double | 0–100 |
| `Geography_score` | double | 0–100 |
| `History_score` | double | 0–100 |
| `Politics_score` | double | 0–100 |
| `total_score` | double | Computed automatically via trigger |
| `created_at` | string | Set on insert |
| `updated_at` | string | Updated on edit |

---

## Version History

| Version | Description |
|---|---|
| `0.0.1-ALPHA` | Initial release — JSON file storage |
| `0.1.5-Gamma` | Current CLI — SQLite backend, migration, triggers, import/export |
| `0.0.1-Beta` | TUI mode — FTXUI-powered terminal interface (new) |

---

## License

Student Manager is free software distributed under the **GNU General Public License v3.0 or later**.

```
Copyright (C) 2026 Yvhang Cai (Jeffrey Miller) <jeffrey_miller_GD@outlook.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
```

See [LICENSE](LICENSE.txt) or <https://www.gnu.org/licenses/> for the full text.
