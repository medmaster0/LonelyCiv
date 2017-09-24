//
//  med_algo.hpp
//  Civ2
//
//  Created by Medz on 9/1/17.
//  Copyright © 2017 Medz. All rights reserved.
//

#ifndef med_algo_hpp
#define med_algo_hpp

#include <stdio.h>

#endif /* med_algo_hpp */

#include <vector>

using namespace std;

vector<vector<short>> gen_maze_corridor(); //generates a random corridor maze
void printMaze(vector<vector<short>> maze); //prints out a maze to console, simple utility

vector<vector<int>> findPathToCoord_A_Star(bool block_map[], int x1, int y1, int x2, int y2); //The A* path finding algorithm, gash damn!
vector<vector<int>> A_star(bool block_map[], int x1, int y1, int x2, int y2); //An even better A*, gash damn! not reliant on vector (but will return one for convenience) 
