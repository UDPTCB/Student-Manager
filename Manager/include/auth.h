#ifndef AUTH_H
#define AUTH_H
#include <string>

class Auth {
public:
    static std::string hashPassword(const std::string& pwd);
    static bool verifyPassword(const std::string& hash, const std::string& pwd);
};
#endif