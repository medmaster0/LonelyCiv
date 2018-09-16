//
//  building.hpp
//  Civ2
//
//  Created by Medz on 4/15/18.
//  Copyright © 2018 Medz. All rights reserved.
//

#ifndef building_hpp
#define building_hpp

#include "building.hpp"

#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <vector>
#include <string>
using namespace std;

#include "Item.hpp"
#include "story.hpp"
#include "creature.hpp"

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

//Streetlights...
void build_streetlight(vector<vector<Item>>* map_scenery,vector<vector<Item>>* map_clouds, bool* block_map, int map_width, int map_height,int x, int y, int z = 0,SDL_Color p_col = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255) } ); //builds a streetlight

//Soup Cauldrons...
void build_cauldron(vector<vector<Item>>* map_scenery, bool* block_map,int map_width, int map_height,int x, int y, int z = 0);
void fill_cauldron(vector<vector<Item>>* map_scenery, int map_width, int map_height,int x, int y, int z = 0, SDL_Color p_col = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color s_col = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color t_col = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255});

//Towers...
void build_tower_NxN(vector<vector<Item>>* map_scenery_top,vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height, int x, int y, int n, int num_floors, SDL_Color brick_col1 = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color brick_col2 = {0,0,0,255}, SDL_Color floor_col1 = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color floor_col2 = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color door_col1 = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255},SDL_Color ladder_col1 = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}); //Builds an NxN enclosed box on the map out of standrad bricks, adds a door to one of the walls

//TOWER CLASS
//A specific class used to keep track of tower dimensions and building
class Tower{
public:
    Tower(vector<vector<Item>>* map_scenery_top,vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height, int x, int y, int n, int num_floors, SDL_Color brick_col1 = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color brick_col2 = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color floor_col1 = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color floor_col2 = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color door_col1 = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255},SDL_Color ladder_col1 = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, string street_name = genStreetName(), int address_number = rand()%900); //initialize and build a tower with the initial input parameters
    int x, y, n; //the location and dimensions of the tower
                //n, outer wall thickness, (doesn't include balcony walkway path
                //x,y upper left coords
    int ladder_x, ladder_y; //the location of the main ladder within the tower
    int door_x, door_y; //the location of the front door on the tower
    int mailbox_x, mailbox_y; //the location of the tower's mailbox
    int backyard_x, backyard_y, backyard_n; //the location and dimensions of the back yard of the tower (where rituals happen)
    int num_floors; //how tall the tower is
    SDL_Color brick_col1;
    SDL_Color brick_col2;
    SDL_Color floor_col1;
    SDL_Color floor_col2;
    SDL_Color door_col1;
    SDL_Color ladder_col1;
    string address; //the name of the address
    vector<bool> hasItem; //list of items inside the tower
    int last_empty_item_position; //the index of the position of last empty item space (see item position explanation below class definition...)
    int max_empty_item_position; //the maximum position item can be at (top floor, bottom right)
    bool isOwned = false; 
    void build_floor(vector<vector<Item>>* map_scenery_top,vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height); //build another floor on tower, and increment num_floors counter
    void build_floor_inner(vector<vector<Item>>* map_scenery_top,vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height); //build another floor on tower and increment num_floors counter
    void build_door(vector<vector<Item>>* map_scenery_top, bool* block_map, int map_width, int map_height, int floor, int position = 0); //clears the specified coordinate and puts a door there.
    void build_mailbox(vector<vector<Item>>* map_scenery_top, bool* block_map, int map_width, int map_height, int position = 0, string description = "test"); //Puts a mailbox near the front door
    void assign_backyard(int position); //calculate where the backyard is on Tower
    /////TOWER INVENTORY
    //DRAW TO map_scenery_bottom, but keep track of positions internally
    //consult ITEM POSITIONS below
    void placeItem(Item item, vector<vector<Item>>* map_scenery_bottom, int map_width, int map_height); //puts an item in the last available item position
    //Item takeItem(vector<vector<Item>>* map_scenery_bottom, int map_width, int map_height); //removes the last placed item
    vector<int> randomGroundCoord(); //returns a coord to a random point on the ground floor that is at least one step away from wall
    
    //TOWER DIRECTIONALITY:
    //NEED TO KNOW IF TOWER DOOR FACES north, east, south, west (1,2,3,4) never eat shredded wheat
    int cardinality; 
    
};

//ITEM POSITIONS
//7x7
//xxxxxxx
//x0 1 2x
//x     x
//x3 4 5x
//x     x
//x6 7 8x
//xxxxxxx
//
//6x6
//xxxxxx
//x0 1 x
//x    x
//x2 3 x
//x    x
//xxxxxx
//
//xxxxxx
//x4 5 x
//x    x
//x6 7 x
//x    x
//xxxxxx
//second floor
//
//EXAMPLES OF POSITION LOCATIONS

//misc
bool is_square_clear(bool* block_map, int map_width, int map_height, int x, int y, int z, int n); //checks the block map to make sure nothing is blocking the current location
bool is_circle_clear(bool* block_map, int map_width, int map_height, int x, int y, int z, int r); //checks the block map to make sure nothing is blocking the circle
void set_square_clear(bool* block_map, int map_width, int map_height, int x, int y, int z, int n); //makes the specified square's block_map FALSE

//multi-building structures (facilities)
//void build_two_house_path(vector<vector<Item>>* map_scenery_top,vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height, SDL_Color floor_p_col = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color floor_s_col = {0,0,0,255}, SDL_Color brick_p_col = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color brick_s_col = {0,0,0,255},
//    SDL_Color door_col1 = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}); //builds two houses and puts a path between them

vector<Tower> build_neighborhood(vector<vector<Item>>* map_scenery_top,vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height, SDL_Color brick_p_col = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color brick_s_col = {0,0,0,255}, SDL_Color floor_p_col = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color floor_s_col = {0,0,0,255},SDL_Color door_col1 = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color ladder_col_p = {0,0,0,255} ); //builds a few rows of houses

void build_neighbor_grid(vector<vector<Item>>* map_scenery_top,vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height, SDL_Color brick_p_col = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color brick_s_col = {0,0,0,255}, SDL_Color floor_p_col = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color floor_s_col = {0,0,0,255},SDL_Color door_col1 = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color ladder_col_p = {0,0,0,255} ); //builds a giant grid of houses


#endif /* building_hpp */
