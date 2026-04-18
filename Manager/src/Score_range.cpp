#include "../include/Score_range.h"

Score_range::Score_range() 
    : SR(11, 0),
      grade(0){}


void Score_range::SRF(){
    while(std::cin >> grade){
        if(grade <= 100){
            ++SR.at(grade/10);
        }
    }
    for (auto &r : SR){ 
        std::cout << r << " ";
    }
    std::cout << std::endl;
}