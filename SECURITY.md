# Security Policy

## Supported Versions

Student Manager is currently in early alpha. Only the latest release receives security fixes.

| Version | Supported |
| --- | --- |
| 0.1.6 | ✅ Yes (latest) |
| 0.0.1-ALPHA | ❌ No |

Please upgrade to the latest version before reporting a vulnerability, as it may already be resolved.

---

## Reporting a Vulnerability

If you discover a security vulnerability in Student Manager, **please do not open a public GitHub issue.** Instead, report it privately so it can be addressed before public disclosure.

**Contact:** [jeffrey_miller_GD@outlook.com](mailto:jeffrey_miller_GD@outlook.com)

Please include the following in your report:

- A clear description of the vulnerability
- Steps to reproduce the issue
- The version of Student Manager affected
- Your platform and build environment (OS, compiler, CMake version)
- Any relevant files, logs, or proof-of-concept code

---

## What to Expect

- **Acknowledgement:** You will receive a response within **7 days** of your report.
- **Assessment:** The report will be reviewed and triaged. You will be notified whether the vulnerability is accepted or declined, and why.
- **Fix & Disclosure:** If accepted, a fix will be developed and released. You will be credited in the release notes unless you prefer to remain anonymous.

---

## Scope

The following are considered in scope for security reports:

- **SQL injection** via student record fields (ID, name, grade, class, scores) into the SQLite3 database
- **Path traversal or file system issues** affecting the `Student/` data directory or `logs/` directory, e.g. malformed student IDs or log filenames influencing file paths
- **Buffer overflows or memory safety issues** in C/C++ code, particularly in input parsing routines across the CLI and TUI interfaces
- **Unsafe handling of bundled dependencies** (`sqlite3.c`, `json.hpp`)
- **Privilege escalation or unintended file access** during normal operation
- **TUI input validation bypasses** — cases where the FTXUI-based interface accepts and persists malformed or malicious data that the CLI interface would reject

The following are **out of scope:**

- Vulnerabilities in bundled third-party libraries (`nlohmann/json`, `SQLite3`, `FTXUI`) that are upstream issues — please report those to their respective maintainers
- Issues in unsupported versions (`0.0.1-ALPHA`)
- Build environment or compiler-specific edge cases unrelated to the application logic

---

## Security Considerations for Users

Since Student Manager stores student records locally as a SQLite3 database, users are responsible for:

- **Restricting file system access** to the `Student/` data directory to authorized users only
- **Backing up** the `students.db` database regularly
- **Not running** the application with elevated privileges (e.g. root/Administrator) unless necessary
- **Sanitizing inputs** if integrating this tool into a larger pipeline or script
- **Protecting log files** in the `logs/` directory, as they may contain student IDs and operation history

---

## Disclosure Policy

This project follows a **coordinated disclosure** model. Please allow reasonable time for a fix to be prepared before publicly disclosing any vulnerability. We appreciate responsible security research and will acknowledge your contribution.
