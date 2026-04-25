#include "../include/Score_range.h"

Score_range::Score_range() 
    : SR(11, 0),
      grade(0){}




void Score_range::SRF(){
    SR.assign(11, 0);
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
void Score_range::SRF_120(){
    SR.assign(13, 0);
    while(std::cin >> grade){
        if(grade <= 120){
            ++SR.at(grade/10);
        }
    }
    for (auto &r : SR){ 
        std::cout << r << " ";
    }
    std::cout << std::endl;
}
void Score_range::SRF_150(){
    SR.assign(16, 0);
    while(std::cin >> grade){
        if(grade <= 150){
            ++SR.at(grade/10);
        }
    }
    for (auto &r : SR){ 
        std::cout << r << " ";
    }
    std::cout << std::endl;
}