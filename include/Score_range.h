/*
 * Copyright (C) 2026 Yvhang Cai(Jeffrey Miller) jeffrey_miller_GD@outlook.com
 * 
 * This file is part of Student Manager.
 * 
 * Student Manager is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Student Manager is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Student Manager.  If not, see <https://www.gnu.org/licenses/>.
 * 
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#ifndef SCORE_RANGE_H
#define SCORE_RANGE_H
#include <iostream>
#include <vector>
#include <string>

namespace SRFM {
/*
    REQUIREMENTS FOR Container::value_type

    Must contain the following members:

        std::string grade;
        std::string class_value;
        std::string id;
        std::string name;

        int age;

        double Chinese_score;
        double Mathematics_score;
        double English_score;
        double Physics_score;
        double Chemistry_score;
        double Biology_score;
        double Geography_score;
        double History_score;
        double Politics_score;

    Example:

        struct Student {
            ...
        };
*/
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

template<typename Container>
bool SRF_M(Container& s, const int m){
    using T = typename Container::value_type;
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