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
#include "Item.hpp"

#endif /* med_algo_hpp */

#include <vector>

using namespace std;

vector<vector<short>> gen_maze_corridor(int rows_in, int cols_in); //generates a random corridor maze
void printMaze(vector<vector<short>> maze); //prints out a maze to console, simple utility


void printPath(vector<vector<int>> path);//print path //print the values of the given path


vector<vector<int>> A_Star(bool block_map[],int map_width, int map_height, int x1, int y1, int x2, int y2); //An even better A*, gash damn! not reliant on vector (but will return one for convenience)

vector<vector<int>> A_Star_Z(bool block_map[], vector<vector<Item>>* map_scenery_top, int map_width, int map_height, int x1, int y1, int z1, int x2, int y2, int z2); //An even better A*, gash damn! not reliant on vector (but will return one for convenience)
vector<vector<int>> A_Star_Free_Fall(bool block_map[], int map_width, int map_height, int x1, int y1, int z1, int x2, int y2, int z2); //An even A* algorithm that will path over any unblocked area (without needing a ladder)

//COLOR ALGORITHMS!
int color_diff(SDL_Color col1, SDL_Color col2); //computes the "difference of colors"
vector<int> color_fight(SDL_Color col1, SDL_Color col2); //How to compute which color is superior...
SDL_Color color_blend(SDL_Color col1, SDL_Color col2); //belnd two colors

SDL_Color generate_brown(); //generates a random shade of brown
SDL_Color generate_pink(); //generates a random shade of pink
SDL_Color generate_green(); //generates a random shade of green
SDL_Color generate_golden(); //generates a random shade of gold
//SDL_Color color_to_pastel(SDL_Color in_col); //given n input color, convert it to a pastel version (increase the lightness)
SDL_Color color_to_pastel(SDL_Color in_col, SDL_Color low_gray = {180,180,180,255});  //given n input color, convert it to a pastel (increase lightness). Use low_gray as a baseline for "black". E.G. INputting black will out put "low_gray", increasing in_col will hit all colors between low_gray and {255,255,255}

SDL_Color redNoise(SDL_Color redHue); //takes a color and gradually changes it along the redish/orange spectrum
SDL_Color warpColor(SDL_Color inputHue, int variance = 10); //take a color and gradually (slightly) change the RGB values
