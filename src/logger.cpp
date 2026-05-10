/*
 * Copyright (C) 2026 Yvhang Cai (Jeffrey Miller) <jeffrey_miller_GD@outlook.com>
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
 * 
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "../include/logger.hpp"


void logger::log(const std::string& message, int level) {
    if (level == 0) { mlevel = "ERROR"; }
    else if (level == 1) { mlevel = "WARNING"; }
    else if (level == 2) { mlevel = "INFO"; }
    else if (level == 3) { mlevel = "DEBUG"; }
    else { mlevel = "UNKNOWN"; }
    std::string output = std::format("[{}] {}: {}", mlevel, getCurrentTime(), message);
    if (logFile.is_open()) {
        logFile << output << std::endl;
    }
}

void logger::setPath(const std::filesystem::path& newPath) {
    if (logFile.is_open()){
        logFile.close();
    }

    logPath = newPath;
    ensureLogDirectory();
    logFile.open(logPath, std::ios::app);
}

std::filesystem::path logger::getPath() const {
    return logPath;
}

bool logger::exists() const {
    return std::filesystem::exists(logPath);
}

std::uintmax_t logger::size() const {
    return std::filesystem::exists(logPath) ? std::filesystem::file_size(logPath) : 0;
}

void logger::rotate() {
    if (std::filesystem::exists(logPath)){
        std::filesystem::path backup = logPath;
        backup += ".bak";
        std::filesystem::rename(logPath, backup);
        logFile.close();
        logFile.open(logPath, std::ios::out | std::ios::app);
    }
}

void logger::getVersion(){
    std::cout << LOGGER_VERSION << std::endl;
}