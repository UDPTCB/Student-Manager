#pragma once
#include <string>
#include "../include/json.hpp"

class UserManager {
public:
    UserManager(const std::string& file);

    bool signup(const std::string& user, const std::string& pass);
    bool login(const std::string& user, const std::string& pass);

private:
    std::string filename;
    nlohmann::json data;

    void load();
    void save();
};