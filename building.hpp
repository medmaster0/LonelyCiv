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

void building_5x5(vector<vector<Item>>* map_scenery, bool* block_map, int map_width, int x, int y); //Builds a 5x5 building on the map out of standrad bricks
void building_NxN(vector<vector<Item>>* map_scenery, bool* block_map, int map_width, int x, int y, int n); //Builds an NxN building on the map out of standrad bricks


#endif /* building_hpp */
