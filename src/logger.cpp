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