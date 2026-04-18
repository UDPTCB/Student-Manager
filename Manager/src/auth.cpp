#include "../include/auth.h"
#include <sodium.h>

std::string Auth::hashPassword(const std::string& pwd) {
    char hash[crypto_pwhash_STRBYTES];

    crypto_pwhash_str(
        hash,
        pwd.c_str(),
        pwd.size(),
        crypto_pwhash_OPSLIMIT_INTERACTIVE,
        crypto_pwhash_MEMLIMIT_INTERACTIVE
    );

    return std::string(hash);
}

bool Auth::verifyPassword(const std::string& hash, const std::string& pwd) {
    return crypto_pwhash_str_verify(
        hash.c_str(),
        pwd.c_str(),
        pwd.size()
    ) == 0;
}