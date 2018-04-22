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

//buildings
void build_box_NxN(vector<vector<Item>>* map_scenery_top, bool* block_map, int map_width, int map_height, int x, int y, int n, SDL_Color p_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color s_col_in = {0,0,0,255}); //Builds an NxN enclosed box on the map out of standrad bricks
void build_box_NxN_door(vector<vector<Item>>* map_scenery_top, bool* block_map, int map_width, int map_height, int x, int y, int n, SDL_Color p_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color s_col_in = {0,0,0,255}); //Builds an NxN enclosed box on the map out of standrad bricks, adds a door to one of the walls
void build_maze(vector<vector<Item>>* map_scenery, bool* block_map, int map_width, int map_height, int x, int y, SDL_Color p_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color s_col_in = {0,0,0,255}); //Builds a maze on the map. (x,y) are upper left coords

//floors
void build_floor_NxN(vector<vector<Item>>* map_scenery, bool* block_map, int map_width, int map_height, int x, int y, int n, SDL_Color p_col = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color s_col = {0,0,0,255}); //Builds an NxN box of floor tiles on the map

//misc
bool is_square_clear(bool* block_map, int map_width, int map_height, int x, int y, int n); //checks the block map to make sure nothing is blocking the current location

//multi-building structures (facilities)
void build_two_house_path(vector<vector<Item>>* map_scenery_top,vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height, SDL_Color floor_p_col = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color floor_s_col = {0,0,0,255}, SDL_Color brick_p_col = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color brick_s_col = {0,0,0,255} ); //builds two houses and puts a path between them




#endif /* building_hpp */
