#ifndef SCORE_RANGE_H
#define SCORE_RANGE_H
#include <iostream>
#include <vector>
#include <string>

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