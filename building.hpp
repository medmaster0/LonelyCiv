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
#include "story.hpp"

//buildings
void build_box_NxN(vector<vector<Item>>* map_scenery_top, bool* block_map, int map_width, int map_height, int x, int y, int z, int n, SDL_Color p_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color s_col_in = {0,0,0,255}); //Builds an NxN enclosed box on the map out of standrad bricks
void build_box_NxN_door(vector<vector<Item>>* map_scenery_top, bool* block_map, int map_width, int map_height, int x, int y, int z, int n, SDL_Color p_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color s_col_in = {0,0,0,255}, SDL_Color door_col1 = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}); //Builds an NxN enclosed box on the map out of standrad bricks, adds a door to one of the walls
void build_maze(vector<vector<Item>>* map_scenery, bool* block_map, int map_width, int map_height, int x, int y, int z, SDL_Color p_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color s_col_in = {0,0,0,255}); //Builds a maze on the map. (x,y) are upper left coords

void build_circle_radius(vector<vector<Item>>* map_scenery_top, bool* block_map, int map_width, int map_height, int radius, int cent_x, int cent_y, int cent_z, SDL_Color p_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color s_col_in = {0,0,0,255}); //Builds a circular structure of bricks
void build_circle_radius_door(vector<vector<Item>>* map_scenery_top, bool* block_map, int map_width, int map_height, int radius, int cent_x, int cent_y, int cent_z, SDL_Color p_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color s_col_in = {0,0,0,255}, SDL_Color door_col1 = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}); //Builds a circular structure of bricks


//floors
void build_floor_NxN(vector<vector<Item>>* map_scenery, bool* block_map, int map_width, int map_height, int x, int y, int z, int n, SDL_Color p_col = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color s_col = {0,0,0,255}); //Builds an NxN box of floor tiles on the map
//void lay_floor_NxN(vector<vector<Item>>* map_scenery, bool* block_map, int map_width, int map_height, int x, int y, int z, int n, SDL_Color p_col = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color s_col = {0,0,0,255}); //Lays an NxN box on floor. differs from above function in that it will simply skip a blocked or "floortiled" tile 
void build_floor_radius(vector<vector<Item>>* map_scenery, bool* block_map, int map_width, int map_height, int x, int y, int z, int r, SDL_Color p_col = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color s_col = {0,0,0,255}); //Builds an circular floor of radius radius out of floortile tiles
void build_floor_path(vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height, int x1, int y1, int x2, int y2, SDL_Color p_col = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color s_col = {0,0,0,255}); //finds and builds a pathway between the two points - won't build over existing floor tiles

//Towers...
void build_tower_NxN(vector<vector<Item>>* map_scenery_top,vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height, int x, int y, int n, int num_floors, SDL_Color brick_col1 = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color brick_col2 = {0,0,0,255}, SDL_Color floor_col1 = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color floor_col2 = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color door_col1 = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255},SDL_Color ladder_col1 = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}); //Builds an NxN enclosed box on the map out of standrad bricks, adds a door to one of the walls

//misc
bool is_square_clear(bool* block_map, int map_width, int map_height, int x, int y, int z, int n); //checks the block map to make sure nothing is blocking the current location
bool is_circle_clear(bool* block_map, int map_width, int map_height, int x, int y, int z, int r); //checks the block map to make sure nothing is blocking the circle
void set_square_clear(bool* block_map, int map_width, int map_height, int x, int y, int z, int n); //makes the specified square's block_map FALSE

//multi-building structures (facilities)
void build_two_house_path(vector<vector<Item>>* map_scenery_top,vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height, SDL_Color floor_p_col = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color floor_s_col = {0,0,0,255}, SDL_Color brick_p_col = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color brick_s_col = {0,0,0,255},
    SDL_Color door_col1 = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}); //builds two houses and puts a path between them

void build_neighborhood(vector<vector<Item>>* map_scenery_top,vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height, SDL_Color brick_p_col = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color brick_s_col = {0,0,0,255}, SDL_Color floor_p_col = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color floor_s_col = {0,0,0,255},SDL_Color door_col1 = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color ladder_col_p = {0,0,0,255} ); //builds a few rows of houses

void build_neighbor_grid(vector<vector<Item>>* map_scenery_top,vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height, SDL_Color brick_p_col = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color brick_s_col = {0,0,0,255}, SDL_Color floor_p_col = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color floor_s_col = {0,0,0,255},SDL_Color door_col1 = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color ladder_col_p = {0,0,0,255} ); //builds a giant grid of houses


#endif /* building_hpp */
