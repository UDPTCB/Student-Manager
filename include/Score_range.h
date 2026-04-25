#ifndef SCORE_RANGE_H
#define SCORE_RANGE_H
#include <iostream>
#include <vector>
#include <string>

namespace SRFM {

template<typename Container, typename Func>
void print_score_range(const Container& se, Func get_score) {
    std::vector<int> SR(16, 0);

    for (const auto& stu : se) {
        int score = get_score(stu);   

        if (score >= 0 && score <= 150) {
            ++SR.at(score / 10);
        }
    }

    for (size_t i = 0; i < SR.size(); ++i) {
        if (i < 15) {
            std::cout << i * 10 << "-" << i * 10 + 9;
        } else {
            std::cout << "150";
        }
        std::cout << ": " << SR[i] << std::endl;
    }
}

template<typename Container, typename T>
bool SRF_M(Container& s, const int m, T& temp){
    
    if (m == 0){
        print_score_range(s, [](const T& see) -> double {
            return see.Chinese_score;
        });
        return true;
    } else if (m == 1){
        print_score_range(s, [](const T& see) -> double {
            return see.Mathematics_score;
        });
        return true;
    } else if (m == 2){
        print_score_range(s, [](const T& see) -> double {
            return see.English_score;
        });
        return true;
    } else if (m == 3){
        print_score_range(s, [](const T& see) -> double {
            return see.Physics_score;
        });
        return true;
    } else if (m == 4){
        print_score_range(s, [](const T& see) -> double {
            return see.Chemistry_score;
        });
        return true;
    } else if (m == 5){
        print_score_range(s, [](const T& see) -> double {
            return see.Biology_score;
        });
        return true;
    } else if (m == 6){
        print_score_range(s, [](const T& see) -> double {
            return see.Geography_score;
        });
        return true;
    } else if (m == 7){
        print_score_range(s, [](const T& see) -> double {
            return see.History_score;
        });
        return true;
    } else if (m == 8){
        print_score_range(s, [](const T& see) -> double {
            return see.Politics_score;
        });
        return true;
    } else {
        return false;
    }
    
}
};


class Score_range {
private:
    std::vector<int> SR;
    unsigned int grade;
public:
    Score_range();
    void SRF();
    void SRF_120();
    void SRF_150();
   
    
};
#endif // SCORE_RANGE_H