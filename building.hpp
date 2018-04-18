//
//  building.hpp
//  Civ2
//
//  Created by Medz on 4/15/18.
//  Copyright © 2018 Medz. All rights reserved.
//

#ifndef building_hpp
#define building_hpp

#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <vector>

#include "Item.hpp"

//void building_5x5(vector<vector<Item>>* map_scenery, bool* block_map, int map_width, int x, int y); //Builds a 5x5 building on the map out of standrad bricks
//void building_NxN(vector<vector<Item>>* map_scenery, bool* block_map, int map_width, int x, int y, int n); //Builds an NxN building on the map out of standrad bricks

void build_box_NxN(vector<vector<Item>>* map_scenery, bool* block_map, int map_width, int x, int y, int n); //Builds an NxN enclosed box on the map out of standrad bricks
void build_box_NxN_door(vector<vector<Item>>* map_scenery, bool* block_map, int map_width, int x, int y, int n); //Builds an NxN enclosed box on the map out of standrad bricks, adds a door to one of the walls

bool is_square_clear(bool* block_map, int map_width, int x, int y, int n); //checks the block map to make sure nothing is blocking the current location

#endif /* building_hpp */
