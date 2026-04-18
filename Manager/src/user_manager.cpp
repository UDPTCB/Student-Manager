#include "../include/user_manager.h"
#include "../include/auth.h"
#include <fstream>

using json = nlohmann::json;

UserManager::UserManager(const std::string& file) : filename(file) {
    load();
}

void UserManager::load() {
    std::ifstream in(filename);
    if (in) in >> data;
    else data["users"] = json::array();
}

void UserManager::save() {
    std::ofstream out(filename);
    out << data.dump(4);
}

bool UserManager::signup(const std::string& user, const std::string& pass) {
    for (auto& u : data["users"]) {
        if (u["username"] == user)
            return false;
    }

    data["users"].push_back({
        {"username", user},
        {"password_hash", Auth::hashPassword(pass)}
    });

    save();
    return true;
}

bool UserManager::login(const std::string& user, const std::string& pass) {
    for (auto& u : data["users"]) {
        if (u["username"] == user) {
            return Auth::verifyPassword(u["password_hash"], pass);
        }
    }
    return false;
}