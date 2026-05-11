/*
 * SPDX-License-Identifier: GPL-3.0-or-later
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
 */
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