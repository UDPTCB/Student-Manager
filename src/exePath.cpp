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
#include "../include/exePath.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

std::filesystem::path get_executable_path() {

#ifdef _WIN32

    char buffer[MAX_PATH];

    GetModuleFileNameA(NULL, buffer, MAX_PATH);

    return std::filesystem::path(buffer).parent_path();

#else

    char buffer[PATH_MAX];

    ssize_t len =
        readlink("/proc/self/exe",
                 buffer,
                 sizeof(buffer)-1);

    if (len != -1) {
        buffer[len] = '\0';

        return std::filesystem::path(buffer)
            .parent_path();
    }

    return std::filesystem::current_path();

#endif
}