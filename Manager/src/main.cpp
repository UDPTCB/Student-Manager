#include "../include/config.h"
#include <sodium.h>
int main(int argc, char ** argv){
    if (sodium_init() < 0) {     
        return 1;
    }
    return Main::run(argc, argv);
}