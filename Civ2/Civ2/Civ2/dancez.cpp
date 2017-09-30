//
//  dancez.cpp
//  Civ2
//
//  Created by Medz on 8/16/17.
//  Copyright © 2017 Medz. All rights reserved.
//


// FunAndGen.cpp : Defines the entry point for the console application.
//

#include "dancez.hpp"


//#include "stdafx.h"
#include <vector>
#include <iostream>

using namespace std;

std::vector<std::vector<int>> generateChaCha() {
    
    std::vector<int> step;
    std::vector<std::vector<int>> steps;
    
    for (int j = 0; j < 16; j++) {
        switch (j) {
            case 0:
                step = {-1,0};
                steps.push_back(step);
                break;
            case 1:
                step = {0,0};
                steps.push_back(step);
                break;
            case 2:
                step = {0,1};
                steps.push_back(step);
                break;
            case 3:
                step = {0,0};
                steps.push_back(step);
                break;
            case 4:
                step = {0,-1};
                steps.push_back(step);
                break;
            case 5:
                step = {0,0};
                steps.push_back(step);
                break;
            case 6:
                step = {+1,0};
                steps.push_back(step);
                break;
            case 7:
                step = {+1,0};
                steps.push_back(step);
                break;
                
                //Next Measure / 8 count
            case 8:
                step = {+1,0};
                steps.push_back(step);
                break;
            case 9:
                step = {0,0};
                steps.push_back(step);
                break;
            case 10:
                step = {0,-1};
                steps.push_back(step);
                break;
            case 11:
                step = {0,0};
                steps.push_back(step);
                break;
            case 12:
                step = {0,+1};
                steps.push_back(step);
                break;
            case 13:
                step = {0,0};
                steps.push_back(step);
                break;
            case 14:
                step = {-1,0};
                steps.push_back(step);
                break;
            case 15:
                step = {-1,0};
                steps.push_back(step);
                break;
                
                
                
        }
    }
    
    return steps;
}

//int main()
//{
//    printf("test");
//    
//    std::vector<std::vector<int>> routine;
//    
//    routine = generateChaCha();
//    
//    printf("%d %d %d",routine[0][0],routine[2][1],routine[4][1]);
//    
//    while (1) {
//        1 + 1; //gotta manually exit
//    }
//    return 0;
//}

