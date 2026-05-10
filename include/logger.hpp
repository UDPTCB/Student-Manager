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