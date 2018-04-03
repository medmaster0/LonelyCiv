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
#include <SDL2/SDL.h>

#endif /* med_algo_hpp */

#include <vector>

using namespace std;

vector<vector<short>> gen_maze_corridor(int rows_in, int cols_in); //generates a random corridor maze
void printMaze(vector<vector<short>> maze); //prints out a maze to console, simple utility

vector<vector<int>> A_Star(bool block_map[],int map_width, int map_height, int x1, int y1, int x2, int y2); //An even better A*, gash damn! not reliant on vector (but will return one for convenience)

//COLOR ALGORITHMS!
int color_diff(SDL_Color col1, SDL_Color col2); //computes the "difference of colors"
vector<int> color_fight(SDL_Color col1, SDL_Color col2); //How to compute which color is superior...
SDL_Color color_blend(SDL_Color col1, SDL_Color col2); //belnd two colors

SDL_Color generate_brown(); //generates a random shade of brown
SDL_Color color_to_pastel(SDL_Color in_col); //given n input color, convert it to a pastel version (increase the lightness)
