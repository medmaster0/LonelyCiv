//
//  dispersal.hpp
//  Civ2
//
//  Created by Medz on 5/7/18.
//  Copyright © 2018 Medz. All rights reserved.
//
//This class is for algorithms that disperse items across the map.
//Try different techniques for spreading items (clustering) around the map.
//Can also play around

#ifndef dispersal_hpp
#define dispersal_hpp

#include <stdio.h>

#include "Item.hpp"


//flowers
void flower_spray(vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height, int x, int y, int z, bool isVert, bool isPositive, SDL_Color p_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color s_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, int item_type = 328); //Disperses items out across the map in a cone shape (with algo vriance input inside). Cone starts at (x,y) then keeps branching out until end of map (or collapses to 0)

//clouds
void cloud_place(vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height, int x, int y, int z, SDL_Color p_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color s_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, int item_type = 333);
void cloud_place_shadow(vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height, int x, int y, int z, SDL_Color p_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color s_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, int item_type = 333); //same as cloud_place, but it also creates an "shadow" under each item places (a series of blank/alpha transparent tiles )

//CLOUD CLASS
//Cloud Class will be used to keep track of where clouds are
//Will also be responsible for moving clouds around later
class Cloud{
public:
    Cloud(int xmin = 0, int xmax = 0, int ymin = 0, int ymax= 0, int z = 0); //construct new cloud
    int xmin, xmax, ymin, ymax, z; //Form a rectangle around CLoud
    bool hasCreature; //A flag indicating a creature is on cloud
    //generate a cloud and record it's extreme points for cloud rectangle
    void cloud_place_shadow(vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height, int x, int y, int z, SDL_Color p_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, SDL_Color s_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255}, int item_type = 333);
    //void move_cloud(); //moves the cloud by shifting everything in the cloud's perimeter, one item at a time... (very long process)
    
    
};






#endif /* dispersal_hpp */
