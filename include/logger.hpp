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
#ifndef LOGGER_HPP
#define LOGGER_HPP
#define LOGGER_VERSION "0.0.1"
#include <iostream>
#include <string>
#include <format>
#include <filesystem>
#include <chrono>
#include <fstream>

class logger {
private:
    std::ofstream logFile;
    std::filesystem::path logPath;
    std::string mlevel;
    std::string getCurrentTime() {
        auto now = std::chrono::system_clock::now();
        auto local_time = std::chrono::zoned_time{std::chrono::current_zone(), now};
        return std::format("{:%Y-%m-%d %H:%M:%S}", local_time);
    }

    void ensureLogDirectory() {
        std::filesystem::path logDir = logPath.parent_path();
        if (!logDir.empty() && !std::filesystem::exists(logDir)) {
            std::filesystem::create_directories(logDir);
        }
    }
public:

    static std::string getTimestampForFilename() {
        auto now = std::chrono::system_clock::now();
        auto local_time = std::chrono::zoned_time{std::chrono::current_zone(), now};
        return std::format("{:%Y%m%d}", local_time);
    }

    logger(const std::filesystem::path& filename) : logPath(filename) {
        ensureLogDirectory();
        logFile.open(logPath, std::ios::app);
        if (!logFile.is_open()) {
            throw std::runtime_error("Failed to open log file: " + logPath.string());
        }
    }

    ~logger() {
        if (logFile.is_open()) {
            logFile.close();
        } 
    }

    void log(const std::string& message, int level);

    void setPath(const std::filesystem::path& newPath);

    std::filesystem::path getPath() const;

    bool exists() const;

    std::uintmax_t size() const;

    void rotate();

    void getVersion();
};

#endif