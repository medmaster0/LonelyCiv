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

//generates a list of steps to take for a dance
//returns a list of coords of steps to take
vector<vector <int>> dancePathGen(int refx, int refy, int refz, int type){
    
    vector<vector <int>> steps; //list of steps to return
    vector<int> step; //a single step (list of coords)
    
    //NOTE TO DANCE WRITERS:
    //when developing these dances, you need to push items in from the top up.
    //so the order of actually moved steps will be from the bottom up
    //(i.e. last step listed is the first one to be moves)
    switch (type) {
        case 0:
            printf("you chose, circle dance times 3\n");
            
            //do it three times
            for(int i = 0; i<3; i++){
                
                //add the required steps, one by one
                steps.push_back({ refx, refy, refz });
                steps.push_back({ refx, refy + 1, refz });
                steps.push_back({ refx - 1, refy + 1, refz });
                steps.push_back({ refx - 2, refy + 1, refz });
                steps.push_back({ refx - 2, refy, refz });
                steps.push_back({ refx - 2, refy - 1, refz });
                steps.push_back({ refx - 1, refy - 1, refz });
                steps.push_back({ refx, refy - 1, refz });
                
            }
            break;
        case 1:
            printf("you chose cha cha\n");
            
            //do it three times
            for(int i = 0; i<3; i++){
                //add the required steps, one by one
                steps.push_back({ refx, refy, refz });
                steps.push_back({ refx + 1, refy, refz });
                steps.push_back({ refx + 1, refy - 1, refz });
                steps.push_back({ refx + 1, refy, refz });
                steps.push_back({ refx, refy, refz });
                steps.push_back({ refx - 1 , refy, refz });
                steps.push_back({ refx - 1 , refy + 1, refz });
                steps.push_back({ refx - 1 , refy, refz });
                
            }
            break;
            
        default:
            break;
    }
    
    return steps;
    
}

















