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