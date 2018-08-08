//
//  building.cpp
//  Civ2
//
//  This is a collection of functions for drawing 2D building patterns on the map.
//  Operates on vectors of vectors of Items.
//  First vector: linear vector of the form (y*map_width)+x for indexing
//  Second Vector: list of elements on that x,y tile
//
//  301 is Item code for adobe (bricks)
//  304 is Item code for bricks
//
//  Created by Medz on 4/15/18.
//  Copyright © 2018 Medz. All rights reserved.
//

#include "building.hpp"
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include <vector>

#include "med_algo.hpp"
#include "Item.hpp"
#include "story.hpp"

//Builds an NxN enclosed box on the map out of standrad bricks
//Input the vector of Item vectors to update with new items, as well as the block_map to update
//Input: also need to innput map_width as a sort of key to index the array elements. SORRY
//Input the x,y coordinates of the upper left corner of the building
//Input N, the dimensions of square building
void build_box_NxN(vector<vector<Item>>* map_scenery, bool* block_map, int map_width,int map_height, int x, int y, int z, int n, SDL_Color col1_in, SDL_Color col2_in){
 
    int tempx, tempy; //stores the cuurent positing we are working on building
    int tile = 304; //304 is the Item code for (bricks)
    
    //Generate random colors to use for these buildings
    SDL_Color col1 = col1_in;
    SDL_Color col2 = col2_in;
    
    //create top row
    for(int i = 0; i < n; i++){ //cycle through 5
        
        tempx = x + i;
        tempy = y;
        Item temp_item = Item(tempx, tempy, tile, col1, col2 ); //create the temporary item
        map_scenery->at( (z*map_height*map_width) + (tempy*map_width)+tempx).push_back(temp_item);
        block_map[ (z*map_height*map_width) + (tempy*map_width)+tempx] = true; //update the block_map
            
        
    }
    
    //create left wall
    for(int i = 0 ; i < n-1; i ++){
        
        tempx = x;
        tempy = y + 1 + i;
        Item temp_item = Item(tempx, tempy, tile, col1, col2 ); //create the temporary item
        map_scenery->at( (z*map_height*map_width) + (tempy*map_width)+tempx).push_back(temp_item);
        block_map[ (z*map_height*map_width) + (tempy*map_width)+tempx] = true; //update the block_map
        
    }
    
    //create right wall
    for(int i = 0 ; i < n-1; i ++){
        
        tempx = x + n-1;
        tempy = y + 1 + i;
        Item temp_item = Item(tempx, tempy, tile, col1, col2 ); //create the temporary item
        map_scenery->at( (z*map_height*map_width) + (tempy*map_width)+tempx).push_back(temp_item);
        block_map[ (z*map_height*map_width) + (tempy*map_width)+tempx] = true; //update the block_map
        
    }
    
    //Create bottom row
    for(int i = 0; i < n-2; i++){ //cycle through n minus 2 (since we got them while doing left and right walls)
        
        tempx = x + i + 1;
        tempy = y + n-1;
        Item temp_item = Item(tempx, tempy, tile, col1, col2 ); //create the temporary item
        map_scenery->at( (z*map_height*map_width) + (tempy*map_width)+tempx).push_back(temp_item);
        block_map[ (z*map_height*map_width) + (tempy*map_width)+tempx] = true; //update the block_map

    }
    
}

//Builds an NxN enclosed box on the map out of standrad bricks
//Input the vector of Item vectors to update with new items, as well as the block_map to update
//Input: also need to innput map_width as a sort of key to index the array elements. SORRY
//Input the x,y coordinates of the upper left corner of the building
//Input N, the dimensions of square building
//Deletes one of the bricks along the wall to create a doorway (currently the center brick)
void build_box_NxN_door(vector<vector<Item>>* map_scenery, bool* block_map, int map_width, int map_height, int x, int y, int z, int n, SDL_Color col1_in, SDL_Color col2_in, SDL_Color door_col1_in){
    
    int tempx, tempy; //stores the cuurent positing we are working on building
    int tile = 304; //304 is the Item code for (bricks)
    
    //Generate random colors to use for these buildings
    SDL_Color col1 = col1_in;
    SDL_Color col2 = col2_in;
    
    //Use the exisiting build box routine
    build_box_NxN(map_scenery, block_map, map_width,map_height, x, y, z, n, col1, col2);
    
    //Delete one of the bricks to form a doorway
    int choice = rand()%4; //pick one of the four walls
    //int spot = rand()%(n-2); //random spot along the wall
    switch (choice) {
        case 0: //from top row
            tempx = x + (n/2);
            tempy = y;
            break;
        case 1: //from bottom row
            tempx = x + (n/2);
            tempy = y + n - 1;
            break;
        case 2: //from left wall
            tempx = x;
            tempy = y + (n/2);
            break;
        case 3: //from right wall
            tempx = x + n - 1;
            tempy = y + (n/2);
            break;
        default:
            break;
    }
    
    //now remove that wall and update block map
    block_map[  (z*map_height*map_width) + (tempy*map_width)+tempx] = false; //update the block_map
    map_scenery->at( (z*map_height*map_width) + (tempy*map_width)+tempx).pop_back(); //remove elemement from map array
    
    //Now create a new doorway Item and add to scenery
    SDL_Color col3 = door_col1_in;
    Item temp_door = Item(tempx, tempy, 338, col3, {0,0,0,255}); //create a DOORWAY (Curtains) Item
    map_scenery->at( (z*map_height*map_width) + (tempy*map_width)+tempx).push_back(temp_door);
    
}


//Builds a maze on the map
void build_maze(vector<vector<Item>>* map_scenery, bool* block_map, int map_width, int map_height, int x, int y, int z, SDL_Color p_col_in, SDL_Color s_col_in){

    //Make a brick maze
    vector<vector<short>> gmap = gen_maze_corridor(20,30);
    int con_x = x;//the starting point of construction
    int con_y = y;//the starting point of construction
    //redefine color
    SDL_Color p1 = p_col_in;
    SDL_Color s1 = s_col_in;
    //BASIC RECIPE FOR MAKING A MAZE OUT OF PHYSICAL ITEMS
    for(int j = 0 ; j < gmap.size(); j++){ //the ROWS of
        for(int i = 0; i<gmap[j].size();i++){ //the COLS of

            int tempx = i + con_x;
            if(tempx >= map_width){continue;}
            int tempy = j + con_y;
            if( tempy >= map_height){continue;}
            //Now check if the space is empty or wall
            //(Switch i and j for rotation)
            //if(gmap[i][j] == 1){
            if(gmap[j][i] == 1){
                //then we need a wall here
                Item con_item = Item(tempx, tempy, 335, p1, s1);
                map_scenery->at( (z*map_height*map_width) + (tempy*map_width)+tempx).push_back(con_item);
                block_map[ (z*map_height*map_width) + (tempy*map_width)+tempx] = true;
            }

        }
    }
}

//Builds an NxN box of floor tiles on the map
void build_floor_NxN(vector<vector<Item>>* map_scenery, bool* block_map, int map_width, int map_height, int x, int y, int z, int n, SDL_Color p_col , SDL_Color s_col ){
    
    int FLOOR_TILE_INDEX = 301; //the Item type for floor tiles
    
    //Cycle through floor space
    for(int i = 0; i < n; i++){ //for x dim
        for(int j = 0 ; j < n; j++){ // for y dim
            
            //make sure a tile isn't already there.
            bool has_tile = false;
            for(int p = 0; p < map_scenery->at( (z*map_width*map_height) + ((y+j)*map_width) + (x+i) ).size() ; p++){
                if(map_scenery->at( (z*map_width*map_height) + ((y+j)*map_width) + (x+i) ).at(p).type == FLOOR_TILE_INDEX){
                    has_tile = true; //set flag to prevent creation of new tile
                }
            }
            if(has_tile == true){continue;} //move on to next step
            
            //If made it here, lay a tile
            Item temp_floor = Item(x+i, y+j, FLOOR_TILE_INDEX, p_col, s_col);
            map_scenery->at( (z*map_width*map_height) + (temp_floor.y*map_width) + temp_floor.x ).push_back(temp_floor);
        }
    }
}

//Builds an circular floor of radius radius out of floortile tiles
void build_floor_radius(vector<vector<Item>>* map_scenery, bool* block_map, int map_width, int map_height, int cent_x, int cent_y, int cent_z, int radius, SDL_Color p_col, SDL_Color s_col){
    
//    //NEED TO CHECK IF SPACE IS CLEAR!!! :0
//    if(is_circle_clear(block_map, map_width, map_height, cent_x, cent_y, radius)){
//        return;
//    }
    
    //Cycle through flor space
    for(int y = -radius; y<=radius; y++){ //cycle through all possible points
        for(int x = -radius; x<=radius; x++){ //cycle though all possible points
            if(x*x + y*y <= radius*radius){ //if point within cricle...
                
                //Build Floor
                Item temp_floor = Item(x+cent_x, y+cent_y, 301, p_col, s_col);
                map_scenery->at( (cent_z*map_height*map_width) + (temp_floor.y*map_width) + temp_floor.x ).push_back(temp_floor);
                
            }
        }
    }
    
}

//finds and builds a pathway between the two points - won't build over existing floor tiles
void build_floor_path(vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height, int x1, int y1, int x2, int y2, SDL_Color floor_p_col_in, SDL_Color floor_s_col_in){
    
    //Find path between two buildings
    vector<vector<int>> path = A_Star(block_map, map_width, map_height, x1, y1, x2, y2); //a list of steps to get between the two buildings
    
    int FLOOR_TILE_INDEX = 301;
    
    //Now cycle through the Entire Path
    SDL_Color floor_p_col = floor_p_col_in;
    SDL_Color floor_s_col = floor_s_col_in;
    bool has_tile = false; //flag used to indicate the path has a tile
    while(path.size()>0){ //while we still have more steps to go
        
        //get next step from list
        vector<int> temp_step = path.back();
        path.pop_back();
        
        //Check if search failed
        if(temp_step[0] == 9999){ //special error code
            printf("Build Floor Search Failed");
            return;
        }
        
        //make sure a tile isn't already there.
        has_tile = false;
        for(int z = 0; z < map_scenery_bottom->at((temp_step[1]*map_width) + temp_step[0]).size() ; z++){
            if(map_scenery_bottom->at( (temp_step[1]*map_width) + temp_step[0] ).at(z).type == FLOOR_TILE_INDEX){
                has_tile = true; //set flag to prevent creation of new tile
            }
        }
        if(has_tile == true){continue;} //move on to next step
        
        //Create a floortile tile at each step
        Item temp_floor = Item(temp_step[0], temp_step[1], FLOOR_TILE_INDEX, floor_p_col, floor_s_col );
        map_scenery_bottom->at((temp_step[1]*map_width)+temp_step[0]).push_back(temp_floor);
    }
}

//Builds an NxN enclosed box on the map out of standrad bricks, adds a door to one of the walls
void build_tower_NxN(vector<vector<Item>>* map_scenery_top, vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height, int build_x, int build_y, int n, int num_floors, SDL_Color brick_col1 , SDL_Color brick_col2, SDL_Color floor_col1, SDL_Color floor_col2, SDL_Color door_col1,SDL_Color ladder_col1){
    
    int ladder_x, ladder_y; //will keep track of where to put the ladder(s)
    
    //check to make sure we're clear...
    if( ! is_square_clear(block_map, map_width, map_height, build_x, build_y, 0, n)){ //check the first floor
        return; //do nothing since it's blockedtrick.
    }
    
    //Also find a random spot inside building
    ladder_x = build_x + 1 + rand()%(n - 2);
    ladder_y = build_y + 1 + rand()%(n - 2);
    
    //GROUND FLOOR
    //Build the first box walls/doors
    build_box_NxN_door(map_scenery_top, block_map, map_width, map_height, build_x, build_y, 0, n, brick_col1, brick_col2, door_col1);
    //Build the floors within the box
    build_floor_NxN(map_scenery_bottom, block_map, map_width, map_height, build_x+1, build_y+1, 0, n-2, floor_col1, floor_col2);
    //Put the ladder
    Item temp_ladder = Item(ladder_x, ladder_y, 318, ladder_col1, {0,0,0,255});
    map_scenery_top->at( (ladder_y*map_width) + ladder_x ).push_back(temp_ladder);
    
    //ALL OF THE OTHER FLOORS
    for(int i = 0; i < num_floors; i++){
        //Also need to unblock that region (of floor area)
        //set_square_clear(block_map, map_width, map_height, build_x+1, build_y+1, i+1, n-2);
        set_square_clear(block_map, map_width, map_height, build_x-1, build_y-1, i+1, n+2);
        
        //Build the box walls/doors
        build_box_NxN_door(map_scenery_top, block_map, map_width, map_height, build_x, build_y, i+1, n, brick_col1, brick_col2, door_col1);
        
        //Build the floors within the box
        build_floor_NxN(map_scenery_bottom, block_map, map_width, map_height, build_x+1, build_y+1, i+1, n-2, floor_col1, floor_col2);
        //Put the ladder
        Item temp_ladder = Item(ladder_x, ladder_y, 318, ladder_col1, {0,0,0,255});
        map_scenery_top->at( ((i+1)*map_height*map_width) + (ladder_y*map_width) + ladder_x ).push_back(temp_ladder);
        
        //Build a balcony (lay bix box of floor - it will ignore area with floor tile already
        build_floor_NxN(map_scenery_bottom, block_map, map_width, map_height, build_x-1, build_y-1, i+1, n+2, floor_col1, floor_col2);
        //Create a railing
        //Bottom railing
        for(int b = 0; b < n+2; b++){
            int r_x = build_x - 1 + b;
            int r_y = build_y + n;
            Item temp_railing = Item(r_x, r_y, 319, {0,0,0,255}, {0,0,0,255});
            map_scenery_top->at( ((i+1)*map_height*map_width) + (r_y*map_width) + r_x ).push_back(temp_railing);
        }
        
        //Left railing
        for(int b = 0; b < n+2; b++){
            int r_x = build_x - 1;
            int r_y = build_y - 1 + b;
            Item temp_railing = Item(r_x, r_y, 320, {0,0,0,255}, {0,0,0,255});
            map_scenery_top->at( ((i+1)*map_height*map_width) + (r_y*map_width) + r_x ).push_back(temp_railing);
        }
        
        //Right railing
        for(int b = 0; b < n+2; b++){
            int r_x = build_x + n;
            int r_y = build_y - 1 + b;
            Item temp_railing = Item(r_x, r_y, 321, {0,0,0,255}, {0,0,0,255});
            map_scenery_top->at( ((i+1)*map_height*map_width) + (r_y*map_width) + r_x ).push_back(temp_railing);
        }
        
        //Top railing
        for(int b = 0; b < n+2; b++){
            int r_x = build_x - 1 + b;
            int r_y = build_y - 2;
            Item temp_railing = Item(r_x, r_y, 319, {0,0,0,255}, {0,0,0,255});
            map_scenery_top->at( ((i+1)*map_height*map_width) + (r_y*map_width) + r_x ).push_back(temp_railing);
        }
        
    }

    return;
    
}

//TOWER CLASS
//A specific class used to keep track of tower dimensions and building

//initialize and build a tower with the initial input parameters
Tower::Tower(vector<vector<Item>>* map_scenery_top,vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height, int in_x, int in_y, int in_n, int in_num_floors, SDL_Color in_brick_col1, SDL_Color in_brick_col2, SDL_Color in_floor_col1 , SDL_Color in_floor_col2 , SDL_Color in_door_col1, SDL_Color in_ladder_col1, string in_street_name , int in_address_number ){
    
    x = in_x;
    y = in_y;
    n = in_n;
    num_floors = 0; //start at zero (we will build up, later)
    brick_col1 = in_brick_col1;
    brick_col2 = in_brick_col2;
    floor_col1 = in_floor_col1;
    floor_col2 = in_floor_col2;
    door_col1 = in_door_col1;
    ladder_col1 = in_ladder_col1;
    //decide where to put the ladder
    //(find a random spot inside building)
    ladder_x = x + 1 + rand()%(n - 2);
    ladder_y = y + 1 + rand()%(n - 2);
    
    address = to_string(in_address_number) + " " + in_street_name;
    
    last_empty_item_position = 0; //initialize item position
    max_empty_item_position = 0; //initialize max item position.... soon be changing it
    
    //Build ground floor
    build_floor_inner(map_scenery_top, map_scenery_bottom, block_map, map_width, map_height);
    num_floors = num_floors + 1;
    
    //build the rest of the floors
    for(int i = 0; i<in_num_floors; i++ ){
        build_floor(map_scenery_top, map_scenery_bottom, block_map, map_width, map_height);
        build_door(map_scenery_top, block_map, map_width, map_height, num_floors, rand()%4);
        
        num_floors = num_floors + 1;
        
    }
    
}

//build another floor on tower and increment num_floors counter
void Tower::build_floor_inner(vector<vector<Item>>* map_scenery_top,vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height){
    
    //Need to unblock that region (of entire floor area)
    set_square_clear(block_map, map_width, map_height, x-1, y-1, num_floors, n+2);
    
    //Build the box walls/doors
    build_box_NxN(map_scenery_top, block_map, map_width, map_height, x, y, num_floors, n, brick_col1, brick_col2);
    
    //build the floors within the box
    build_floor_NxN(map_scenery_bottom, block_map, map_width, map_height, x+1, y+1, num_floors, n-2, floor_col1, floor_col2);
    
    //Put the ladder
    Item temp_ladder = Item(ladder_x, ladder_y, 318, ladder_col1, {0,0,0,255});
    map_scenery_top->at( (num_floors*map_height*map_width)  + ladder_y*map_width + ladder_x   ).push_back(temp_ladder);
    
}


//builds a new floor at the top level of the tower
void Tower::build_floor(vector<vector<Item>>* map_scenery_top,vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height){
    
//    //Need to unblock that region (of entire floor area)
//    set_square_clear(block_map, map_width, map_height, x-1, y-1, num_floors, n+2);
    
//    //Build the box walls/doors
//    build_box_NxN_door(map_scenery_top, block_map, map_width, map_height, x, y, num_floors, n, brick_col1, brick_col2, door_col1);

//    //build the floors within the box
//    build_floor_NxN(map_scenery_bottom, block_map, map_width, map_height, x+1, y+1, num_floors, n-2, floor_col1, floor_col2);
//
//    //Put the ladder
//    Item temp_ladder = Item(ladder_x, ladder_y, 318, ladder_col1, {0,0,0,255});
//    map_scenery_top->at( (num_floors*map_height*map_width)  + ladder_y*map_width + ladder_x   ).push_back(temp_ladder);
    
    //build the inside part of the floor
    build_floor_inner(map_scenery_top, map_scenery_bottom, block_map, map_width, map_height);
    
    //build a balcony (lay big box of floor - it will ignore area with floor tile already
    build_floor_NxN(map_scenery_bottom, block_map, map_width, map_height, x-1, y-1, num_floors, n+2, floor_col1, floor_col2);
    
    //Create a railing
    //Bottom railing
    for(int b = 0; b < n+2; b++){
        int r_x = x - 1 + b;
        int r_y = y + n;
        Item temp_railing = Item(r_x, r_y, 319, {0,0,0,255}, {0,0,0,255});
        map_scenery_top->at( ((num_floors)*map_height*map_width) + (r_y*map_width) + r_x ).push_back(temp_railing);
    }
    
    //Left railing
    for(int b = 0; b < n+2; b++){
        int r_x = x - 1;
        int r_y = y - 1 + b;
        Item temp_railing = Item(r_x, r_y, 320, {0,0,0,255}, {0,0,0,255});
        map_scenery_top->at( ((num_floors)*map_height*map_width) + (r_y*map_width) + r_x ).push_back(temp_railing);
    }
    
    //Right railing
    for(int b = 0; b < n+2; b++){
        int r_x = x + n;
        int r_y = y - 1 + b;
        Item temp_railing = Item(r_x, r_y, 321, {0,0,0,255}, {0,0,0,255});
        map_scenery_top->at( ((num_floors)*map_height*map_width) + (r_y*map_width) + r_x ).push_back(temp_railing);
    }
        
    //Top railing
    for(int b = 0; b < n+2; b++){
        int r_x = x - 1 + b;
        int r_y = y - 2;
        Item temp_railing = Item(r_x, r_y, 319, {0,0,0,255}, {0,0,0,255});
        map_scenery_top->at( ((num_floors)*map_height*map_width) + (r_y*map_width) + r_x ).push_back(temp_railing);
    }
    
    //also push the required amount of entries in hasItem matrix/vector
    int item_positions_to_add = (int)pow(  (int)(((n-2)/2.0) + 0.5) , 2.0  ); //int conversion always down converts
    for(int i = 0; i<item_positions_to_add; i++){
        hasItem.push_back(false);
    }
    
    //also recalculate max_empty_item_position
    max_empty_item_position = hasItem.size() - 1;
    
    return;
    
}

//clears the specified coordinate and puts a door there.
//Position indicates where in the tower the door should be
// 0 - top
// 1 - bottom
// 2 - left
// 3 - right
void Tower::build_door(vector<vector<Item>>* map_scenery_top, bool* block_map, int map_width, int map_height, int z, int position){
    
    //Find the correct coords for wall dependant on position code
    int tempx, tempy;
    switch (position) {
        case 0: //from top row
            tempx = x + (n/2);
            tempy = y;
            break;
        case 1: //from bottom row
            tempx = x + (n/2);
            tempy = y + n - 1;
            break;
        case 2: //from left wall
            tempx = x;
            tempy = y + (n/2);
            break;
        case 3: //from right wall
            tempx = x + n - 1;
            tempy = y + (n/2);
            break;
        default:
            break;
    }
    
    //set the field variables
    door_x = tempx;
    door_y = tempy;
    
    //now remove that wall and update block map
    block_map[  (z*map_height*map_width) + (tempy*map_width)+tempx] = false; //update the block_map
    map_scenery_top->at( (z*map_height*map_width) + (tempy*map_width)+tempx).pop_back(); //remove elemement from map array
    
    //Now create a new doorway Item and add to scenery
    Item temp_door = Item(tempx, tempy, 338, door_col1, {0,0,0,255}); //create a DOORWAY Item
    map_scenery_top->at( (z*map_height*map_width) + (tempy*map_width)+tempx).push_back(temp_door);
    
}

//Puts a mailbox near the front door
//Position indicates where in the tower the door IS and where mailbox should be relative
// 0 - top
// 1 - bottom
// 2 - left
// 3 - right
void Tower::build_mailbox(vector<vector<Item>>* map_scenery_top, bool* block_map, int map_width, int map_height, int position, string description){
    
    //Find the correct coords for wall dependant on position code
    int tempx, tempy;
    switch (position) {
        case 0: //from top wall
            tempx = door_x + 2;
            tempy = door_y - 2;
            break;
        case 1: //from bottom wall
            tempx = door_x - 2;
            tempy = door_y + 2;
            break;
        case 2: //from left wall
            tempx = door_x - 2;
            tempy = door_y - 2;
            break;
        case 3: //from right wall
            tempx = door_x + 2;
            tempy = door_y + 2;
            break;
        default:
            break;
    }
    
    //update the object field variables
    mailbox_x = tempx;
    mailbox_y = tempy;
    
    //update the block_map
    block_map[  (0*map_height*map_width) + (tempy*map_width)+tempx] = true;
    Item temp_mailbox = Item(mailbox_x, mailbox_y, 325, description);
    map_scenery_top->at( temp_mailbox.y*map_width + temp_mailbox.x ).push_back(temp_mailbox);
    
}

//calculate where the backyard is on Tower
//Puts a mailbox near the front door
//Position indicates where in the tower the door IS and where mailbox should be relative
// 0 - top
// 1 - bottom
// 2 - left
// 3 - right
void Tower::assign_backyard(int position ){
    
    //Find the correct coords for wall dependant on position code
    switch (position) {
        case 0: //door on top wall
            backyard_x = x + 1;
            backyard_y = y + (n-1);
            backyard_n = n - 2;
            break;
        case 1: //door bottom wall
            backyard_x = x + 1;
            backyard_y = y - (n-1);
            backyard_n = n - 2;
            break;
        case 2: //door left wall
            backyard_x = x + (n-1);
            backyard_y = y + 1;
            backyard_n = n - 2;
            break;
        case 3: //door right wall
            backyard_x = x - (n-1);
            backyard_y = y + 1;
            backyard_n = n - 2;
            break;
        default:
            break;
    }
    
}

//puts an item in the last available item position
void Tower::placeItem(Item item, vector<vector<Item>>* map_scenery_bottom, int map_width, int map_height){
    
    //Calculate coordinates of where to put item (based on last position)
    //First, calculate items_per_row
    int items_per_row = (int)(  ((n-2)/2.0) + 0.5  );
    //Second, calculate floor
    int floor = last_empty_item_position/( pow(items_per_row,2) );
    int single_floor_position = last_empty_item_position%( (int)pow(items_per_row,2) ); //now that we know floor, we can treat row,col as if on first floor to make calculations easier
    
    
    //Third, calculate row and columns of item
    int row = single_floor_position/items_per_row;
    int col = single_floor_position%items_per_row;
    //Third, calculate x,y from row,column
    int place_x = x + 1 + (col*2);
    int place_y = y + 1 + (row*2);
    
    //Place item on map_scenery.
    //Also correct the values
    item.x = place_x;
    item.y = place_y;
    item.z = floor;
    map_scenery_bottom->at( floor*(map_width*map_height) + place_y*map_width + place_x  ).push_back(item);
    
    //update tower data points
    hasItem[last_empty_item_position] = true;
    last_empty_item_position = last_empty_item_position+1;//increment
    
}

////removes the last placed item
//Item Tower::takeItem(vector<vector<Item>>* map_scenery_bottom, int map_width, int map_height){
//    
//}

//returns a coord to a random point on the ground floor that is at least one step away from wall
vector<int> Tower::randomGroundCoord(){
    int rx = x+2 + rand()%(n-4);
    int ry = y+2 + rand()%(n-4);
    vector<int> coords = {rx,ry};
    return coords;
}


//////////////////////END TOWER CLASS
//END TOWER CLASS

//Builds a circular structure of bricks
//Uses Midpoint Circle Drawing Algorithm
//We only calculate one eigth of the circle (0 to 45 deg) - but we can draw the other sectors at the same time
//Start at the right most pixel.
//Keep Going until x > y --->>>> means we've entered next sector > 45 deg
//Each successive pixel is always one higher than previous
//E.x. At point (x,y), next point can be (x,y+1) or (x-1,y+1)
//
//Utilize Error Circle Expression
//   RE(xi,yi) = ( | xi^2 + yi^2 + r^2 | )
//E.X. At point (r,0), RE= | r^2 + 0^2 - r^2 | = 0
//
//Of the two possible points, you can calculate RE. Whichever has less RE, will be the plotted point.
//E.g. IF ( RE(x-1,y+1) < RE(x,y+1) ) THEN plot (x-1,y+1) ELSE plot (x,y+1)
//BUT, You can do a bunch of math on the expression RE(x-1,y+1) < RE(x,y+1)
//And you get the expression 2( RE(x,y) + 1 ) + 1 > 0  (notice sign change)
//So now you only have to calculate RE once

//
void build_circle_radius(vector<vector<Item>>* map_scenery, bool* block_map, int map_width, int map_height, int radius, int cent_x, int cent_y, int cent_z, SDL_Color p_col_in, SDL_Color s_col_in){
    
    //Initialize, (start at first step_
    int x = radius;
    int y = 0;
    
    while(y<x){ //once x becomes greater, we're done
        
        //CREATE ITEMS
        //Create item in first sector
        Item temp_brick = Item(cent_x+x, cent_y+y, 304, p_col_in, s_col_in);
        map_scenery->at( (cent_z*map_height*map_width) + (temp_brick.y*map_width) + temp_brick.x ).push_back(temp_brick);
        block_map[ (cent_z*map_height*map_width) + (temp_brick.y*map_width)+temp_brick.x] = true;
        //Next sector CCW
        temp_brick = Item(cent_x+y, cent_y+x, 304, p_col_in, s_col_in);
        map_scenery->at( (cent_z*map_height*map_width) + (temp_brick.y*map_width) + temp_brick.x ).push_back(temp_brick);
        block_map[ (cent_z*map_height*map_width) + (temp_brick.y*map_width)+temp_brick.x] = true;
        //Next sector CCW
        temp_brick = Item(cent_x-y, cent_y+x, 304, p_col_in, s_col_in);
        map_scenery->at( (cent_z*map_height*map_width) + (temp_brick.y*map_width) + temp_brick.x ).push_back(temp_brick);
        block_map[ (cent_z*map_height*map_width) + (temp_brick.y*map_width)+temp_brick.x] = true;
        //Next sector CCW
        temp_brick = Item(cent_x-x, cent_y+y, 304, p_col_in, s_col_in);
        map_scenery->at( (cent_z*map_height*map_width) + (temp_brick.y*map_width) + temp_brick.x ).push_back(temp_brick);
        block_map[ (cent_z*map_height*map_width) + (temp_brick.y*map_width)+temp_brick.x] = true;
        //Next sector CCW
        temp_brick = Item(cent_x-x, cent_y-y, 304, p_col_in, s_col_in);
        map_scenery->at( (cent_z*map_height*map_width) + (temp_brick.y*map_width) + temp_brick.x ).push_back(temp_brick);
        block_map[ (cent_z*map_height*map_width) + (temp_brick.y*map_width)+temp_brick.x] = true;
        //Next sector CCW
        temp_brick = Item(cent_x-y, cent_y-x, 304, p_col_in, s_col_in);
        map_scenery->at( (cent_z*map_height*map_width) + (temp_brick.y*map_width) + temp_brick.x ).push_back(temp_brick);
        block_map[ (cent_z*map_height*map_width) + (temp_brick.y*map_width)+temp_brick.x] = true;
        //Next sector CCW
        temp_brick = Item(cent_x+y, cent_y-x, 304, p_col_in, s_col_in);
        map_scenery->at( (cent_z*map_height*map_width) + (temp_brick.y*map_width) + temp_brick.x ).push_back(temp_brick);
        block_map[ (cent_z*map_height*map_width) + (temp_brick.y*map_width)+temp_brick.x] = true;
        //Next sector CCW
        temp_brick = Item(cent_x+x, cent_y-y, 304, p_col_in, s_col_in);
        map_scenery->at( (cent_z*map_height*map_width) + (temp_brick.y*map_width) + temp_brick.x ).push_back(temp_brick);
        block_map[ (cent_z*map_height*map_width) + (temp_brick.y*map_width)+temp_brick.x] = true;
        
        //Determine which pixel to use next.
        //We know we'll use y-1
        //But x-1 or x??
        int err = (x*x) + (y*y) - (radius*radius); //calculate radius error
        if( 2*(err + 1) + 1 > 0 ){ //special equation, equivalent to if RE(x-1) < RE(X,Y)
            //use (x-1, y-1)
            x = x - 1;
            y = y + 1;
        }else{
            //use (x,y+1)
            x = x;
            y = y + 1;
        }
        
    }
    
    //QUICK FIX!!!! AFter algorithm finishes, four extra corner blocks are seen... Need to remove each
    int tempx, tempy;
    tempx = cent_x + radius;
    tempy = cent_y + 0;
    map_scenery->at( (cent_z*map_height*map_width) + (tempy*map_width) + tempx ).pop_back(); //remove elemement from map array
    tempx = cent_x - radius;
    tempy = cent_y + 0;
    map_scenery->at( (cent_z*map_height*map_width) + (tempy*map_width) + tempx ).pop_back(); //remove elemement from map array
    tempx = cent_x + 0;
    tempy = cent_y + radius;
    map_scenery->at( (cent_z*map_height*map_width) + (tempy*map_width) + tempx ).pop_back(); //remove elemement from map array
    tempx = cent_x + 0;
    tempy = cent_y - radius;
    map_scenery->at( (cent_z*map_height*map_width) + (tempy*map_width) + tempx ).pop_back(); //remove elemement from map array
    
}

////Builds a circular structure of bricks
//Uses previous build_circle_radius algorithm, then removes one of the points on the circle
void build_circle_radius_door(vector<vector<Item>>* map_scenery_top, bool* block_map, int map_width, int map_height, int radius_in, int cent_x, int cent_y, int cent_z, SDL_Color p_col_in, SDL_Color s_col_in, SDL_Color door_col1){
    
    int radius = radius_in - 1; //don't count center point
    
    build_circle_radius(map_scenery_top, block_map, map_width, map_height, radius, cent_x, cent_y, cent_z, p_col_in, s_col_in);
    
    //Delete one of the bricks to form a doorway
    int choice = rand()%4; //pick one of the four walls
    int tempx, tempy = 0; //temp vars used to store removed door coords
    switch (choice) {
        case 0: //from right wall
            tempx = cent_x + radius;
            tempy = cent_y + 0;
            break;
        case 1: //from top wall
            tempx = cent_x + 0;
            tempy = cent_y - radius;
            break;
        case 2: //from left wall
            tempx = cent_x - radius;
            tempy = cent_y + 0;
            break;
        case 3: //from bottom wall
            tempx = cent_x + 0;
            tempy = cent_y + radius;
            break;
        default:
            break;
    }
    
    //now remove that wall and update block map
    block_map[ (cent_z*map_height*map_width) + (tempy*map_width)+tempx] = false; //update the block_map
    map_scenery_top->at( (cent_z*map_height*map_width) + (tempy*map_width) + tempx ).pop_back(); //remove elemement from map array
    //map_scenery_top->at( (tempy*map_width) + tempx ).pop_back(); //remove elemement from map array

    //Draw the doors in the correct spots
    //(another switch statement for readability)
    int door_x, door_y = 0; //temp vars for door coords
    switch (choice) {
        case 0: //right wall
            door_x = tempx - 1;
            door_y = tempy;
            break;
        case 1: //top wall
            door_x = tempx;
            door_y = tempy + 1;
            break;
        case 2: //left wall
            door_x = tempx + 1;
            door_y = tempy;
            break;
        case 3: //bottom wall
            door_x = tempx;
            door_y = tempy - 1;
            break;
            
        default:
            break;
    }
    //Now create a new doorway Item and add to scenery
    SDL_Color col3 = door_col1;
    Item temp_door = Item(door_x, door_y, 317, col3, {0,0,0,255}); //create a DOORWAY (Curtains) Item
    map_scenery_top->at( (cent_z*map_height*map_width) + (door_y*map_width) + door_x).push_back(temp_door);
    
//    //Now create a new doorway Item and add to scenery
//    SDL_Color col3 = door_col1;
//    Item temp_door = Item(tempx, tempy, 317, col3, {0,0,0,255}); //create a DOORWAY (Curtains) Item
//    map_scenery_top->at((tempy*map_width)+tempx).push_back(temp_door);
    
}

//checks the block map to make sure nothing is blocking the current location
//Returns a true if clear, false if not
bool is_square_clear(bool* block_map, int map_width, int map_height, int x, int y, int z, int n){
    
    //if out of bounds, return false
    if(x < 0 || y < 0 || x + n > map_width ||  y + n > map_height ){ //last line needs to calculate height
        return false;
    }
    
    for(int i = 0; i < n; i++){ //for cycle x values
        for(int j = 0 ; j < n; j++){ //for cycle y values
            
            if(block_map[ (z*map_height*map_width) + ( (y+j) * map_width ) + (x+i) ] == true){ //if blocked
                return false; //return false
            }
            
        }
    }
    
    //if done cycling, we're all clear
    return true;
    
}

//checks the block map to make sure nothing is blocking the circle
//NOT COMPLETELY COMPATIBLE WITH CIRCLE DRAW MIDPOINT ALGORITHM
//BUT WILL MATCH FLOOD FILE CIRCLE ALGORITHM IN build_floor_radius algorithm
bool is_circle_clear(bool* block_map, int map_width, int map_height, int cent_x, int cent_y, int cent_z, int radius){
    
    //Cycle through flor space
    for(int y = -radius; y<=radius; y++){ //cycle through all possible points
        for(int x = -radius; x<=radius; x++){ //cycle though all possible points
            if(x*x + y*y <= radius*radius){ //if point within cricle...
                
                //Now check that location
                if(block_map[ (cent_z*map_height*map_width) + (y*map_width) + x] == true){ //if point is blocked
                    printf("floor not clear...\n");
                    return false;
                }
                
            }
        }
    }
    
    //If it makes it here, than return all clear signal
    return true;
    
}

//makes the specified square's block_map FALSE
void set_square_clear(bool* block_map, int map_width, int map_height, int x, int y, int z, int n){
    
//    //if out of bounds, return false
//    if(x < 0 || y < 0 || x + n > map_width ||  y + n > map_height ){ //last line needs to calculate height
//        return;
//    }
    
    for(int i = 0; i < n; i++){ //for cycle x values
        for(int j = 0 ; j < n; j++){ //for cycle y values
            
            block_map[ (z*map_width*map_height) + ( (y+j) * map_width ) + (x+i) ] = false;

        }
    }
    
    //if done cycling, we're all clear
    return;
    
}

////builds two houses and puts a path between them
//void build_two_house_path(vector<vector<Item>>* map_scenery_top,vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height, SDL_Color floor_p_col_in, SDL_Color floor_s_col_in, SDL_Color brick_p_col_in, SDL_Color brick_s_col_in, SDL_Color door_col1 ){
//
//    int x1,y1,x2,y2; //stores the coords of two points in the buildings for path finding
//
//    int build_x, build_y, build_dim; //building parameters
//
//    while(true){
//        //BUILD FIRST BUILDING
//        build_x = rand()%map_width;
//        build_y = rand()%map_height;
//        build_dim = 5+rand()%3;
//        if(is_square_clear(block_map, map_width, map_height, build_x-1, build_y-1, 0, build_dim+2)){ //If we are all clear, start building... keep a 1 tile wide perimeter
//
//            build_box_NxN_door(map_scenery_top, block_map, map_width, map_height, build_x, build_y, 0, build_dim, brick_p_col_in, brick_s_col_in, door_col1); //call building routine
//
//            //Build a floor in building
//            build_floor_NxN(map_scenery_bottom, block_map, map_width, map_height, build_x+1, build_y+1, 0, build_dim - 2, floor_p_col_in, floor_s_col_in);
//
//            //Also find a spot inside building
//            x1 = build_x + 1 + rand()%(build_dim - 2);
//            y1 = build_y + 1 + rand()%(build_dim - 2);
//            break;
//        }
//    }
//
//    while(true){
//        //BUILD SECOND BUILDING
//        build_x = rand()%map_width;
//        build_y = rand()%map_height;
//        build_dim = 5+rand()%3;
//        if(is_square_clear(block_map, map_width, map_height, build_x-1, build_y-1, 0, build_dim+2)){ //If we are all clear, start building... keep a 1 tile wide perimeter
//
//            build_box_NxN_door(map_scenery_top, block_map, map_width, map_height, build_x, build_y, 0, build_dim, brick_p_col_in, brick_s_col_in, door_col1); //call building routine
//
//            //Build a floor in building
//            build_floor_NxN(map_scenery_bottom, block_map, map_width, map_height, build_x+1, build_y+1, 0, build_dim - 2, floor_p_col_in, floor_s_col_in);
//
//            //Also find a spot inside building
//            x2 = build_x + 1 + rand()%(build_dim - 2);
//            y2 = build_y + 1 + rand()%(build_dim - 2);
//            break;
//        }
//    }
//
//    //Find path between two buildings
//    vector<vector<int>> path = A_Star(block_map, map_width, map_height, x1, y1, x2, y2); //a list of steps to get between the two buildings
//
//    int FLOOR_TILE_INDEX = 301;
//
//    //Now cycle through the Entire Path
//    SDL_Color floor_p_col = floor_p_col_in;
//    SDL_Color floor_s_col = floor_s_col_in;
//    bool has_tile = false; //flag used to indicate the path has a tile
//    while(path.size()>0){
//
//        //get next step from list
//        vector<int> temp_step = path.back();
//        path.pop_back();
//
//        //make sure a tile isn't already there.
//        has_tile = false;
//        for(int z = 0; z < map_scenery_bottom->at((temp_step[1]*map_width) + temp_step[0]).size() ; z++){
//            if(map_scenery_bottom->at( (temp_step[1]*map_width) + temp_step[0] ).at(z).type == FLOOR_TILE_INDEX){
//                has_tile = true; //set flag to prevent creation of new tile
//            }
//        }
//        if(has_tile == true){continue;} //move on to next step
//
//        //Create a floortile tile at each step
//        Item temp_floor = Item(temp_step[0], temp_step[1], FLOOR_TILE_INDEX, floor_p_col, floor_s_col );
//        map_scenery_bottom->at((temp_step[1]*map_width)+temp_step[0]).push_back(temp_floor);
//    }
//
//}


////Builds a a few rows of towers and connects them by paths
//void build_neighborhood(vector<vector<Item>>* map_scenery_top,vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height, SDL_Color brick_p_col_in, SDL_Color brick_s_col_in, SDL_Color floor_p_col_in, SDL_Color floor_s_col_in,  SDL_Color door_col1, SDL_Color ladder_col_p ){
//
//    //Variables Declare
//    int x1,y1,x2,y2; //stores the coords of two points in the buildings for path finding
//    int build_x, build_y, build_dim, build_floors; //building parameters, temporary
//    int row, col; //stores the x/y coord we'll be building the grid along
//
//    //Set (random) neighboorhood grid
//    //First, pick a random starting point, All paths (that make up a single street) will lead here...
//    //x1 = rand()%map_width;
//    //y1 = rand()%map_height;
//    x1 = 15 - rand()%5;
//    y1 = 70;
//    //Build a sign post here
//    Item temp_sign = Item(x1, y1-1, 322, ladder_col_p, {0,0,0,255}, genStreetName());
//    map_scenery_top->at( temp_sign.y*map_width + temp_sign.x ).push_back(temp_sign);
//    block_map[temp_sign.y*map_width + temp_sign.x ] = true;
//
//    //Now that we have a reference point, start going through a placing towers (slightly skewed) along the FIRST ROW
//    for(int i = 0 ; i < 8; i++){ //This is how many towers/row we'll be making
//
//        build_x = 15 + i*15 + rand()%4; //incremental, skewed
//        build_y = y1 + rand()%3 - rand()%3; //along the same line, skewed
//        build_dim = rand()%4 + 5;
//        build_floors = 3 + rand()%4;
//
//        //check if square is blocked
//        if( is_square_clear(block_map, map_width, map_height, build_x-1, build_y-1, 0, build_dim+2) == false){
//            continue; //just skip if square is blocked
//        }
//
//        //now build a tower there
//        build_tower_NxN(map_scenery_top, map_scenery_bottom, block_map, map_width, map_height, build_x, build_y, build_dim, build_floors, brick_p_col_in, brick_s_col_in, floor_p_col_in, floor_s_col_in, door_col1, ladder_col_p);
//
//        //now connect tower to road by path
//        //find random point in new tower
//        x2 = build_x + 1 + rand()%(build_dim-2); //random point inside tower
//        y2 = build_y + 1 + rand()%(build_dim-2); //random point inside tower
//        //Find path between two buildings
//        vector<vector<int>> path = A_Star(block_map, map_width, map_height, x1, y1, x2, y2); //a list of steps to get between the two buildings
//        build_floor_path(map_scenery_bottom, block_map, map_width, map_height, x1, y1, x2, y2, floor_p_col_in, floor_s_col_in);
//
//        //Build a mailbox at new house
//        //Item temp_mailbox = Item(build_x+rand()%build_dim, build_y-2, 325);
//        Item temp_mailbox = Item(build_x-1, build_y-1, 325);
//        map_scenery_top->at( temp_mailbox.y*map_width + temp_mailbox.x ).push_back(temp_mailbox);
//        block_map[temp_mailbox.y*map_width + temp_mailbox.x ] = true;
//
//        //now update the reference points
//        x1 = x2;
//        y1 = y2;
//
//    }
//
//    //What if we changed the design scheme colors for the next row?
//    brick_p_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255};
//    brick_s_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255};
//    floor_p_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255};
//    floor_s_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255};
//    door_col1 = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255};
//    //ladder_p_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255};
//
//    //NOW DO the SECOND ROW
//    x1 = 15 - rand()%5;
//    y1 = 40;
//    //Build a sign post here
//    temp_sign = Item(x1, y1-1, 322, ladder_col_p, {0,0,0,255}, genStreetName());
//    map_scenery_top->at( temp_sign.y*map_width + temp_sign.x ).push_back(temp_sign);
//    block_map[temp_sign.y*map_width + temp_sign.x ] = true;
//
//    //Now that we have a reference point, start going through a placing towers (slightly skewed) along the FIRST ROW
//    for(int i = 0 ; i < 8; i++){ //This is how many towers/row we'll be making
//
//        build_x = 15 + i*17 + rand()%4; //incremental, skewed
//        build_y = y1 + rand()%3 - rand()%3; //along the same line, skewed
//        build_dim = rand()%4 + 5;
//        build_floors = 3 + rand()%4;
//
//        //check if square is blocked
//        if( is_square_clear(block_map, map_width, map_height, build_x-1, build_y-1, 0, build_dim+2) == false){
//            continue; //just skip if square is blocked
//        }
//
//        //now build a tower there
//        build_tower_NxN(map_scenery_top, map_scenery_bottom, block_map, map_width, map_height, build_x, build_y, build_dim, build_floors, brick_p_col_in, brick_s_col_in, floor_p_col_in, floor_s_col_in, door_col1, ladder_col_p);
//
//        //now connect tower to road by path
//        //find random point in new tower
//        x2 = build_x + 1 + rand()%(build_dim-2); //random point inside tower
//        y2 = build_y + 1 + rand()%(build_dim-2); //random point inside tower
//        //Find path between two buildings
//        vector<vector<int>> path = A_Star(block_map, map_width, map_height, x1, y1, x2, y2); //a list of steps to get between the two buildings
//        build_floor_path(map_scenery_bottom, block_map, map_width, map_height, x1, y1, x2, y2, floor_p_col_in, floor_s_col_in);
//
//        //Build a mailbox at new house
//        //Item temp_mailbox = Item(build_x+rand()%build_dim, build_y-2, 325);
//        Item temp_mailbox = Item(build_x-1, build_y-1, 325);
//        map_scenery_top->at( temp_mailbox.y*map_width + temp_mailbox.x ).push_back(temp_mailbox);
//        block_map[temp_mailbox.y*map_width + temp_mailbox.x ] = true;
//
//        //now update the reference points
//        x1 = x2;
//        y1 = y2;
//
//    }
//
//    //What if we changed the design scheme colors for the next row?
//    brick_p_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255};
//    brick_s_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255};
//    floor_p_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255};
//    floor_s_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255};
//    door_col1 = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255};
//    //ladder_p_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255};
//
//    //NOW DO the THHIRD ROW
//    x1 = 15 - rand()%5;
//    y1 = 110;
//    //Build a sign post here
//    temp_sign = Item(x1, y1-1, 322, ladder_col_p, {0,0,0,255}, genStreetName());
//    map_scenery_top->at( temp_sign.y*map_width + temp_sign.x ).push_back(temp_sign);
//    block_map[temp_sign.y*map_width + temp_sign.x ] = true;
//
//    //Now that we have a reference point, start going through a placing towers (slightly skewed) along the FIRST ROW
//    for(int i = 0 ; i < 8; i++){ //This is how many towers/row we'll be making
//
//        build_x = 15 + i*17 + rand()%4; //incremental, skewed
//        build_y = y1 + rand()%3 - rand()%3; //along the same line, skewed
//        build_dim = rand()%4 + 5;
//        build_floors = 3 + rand()%4;
//
//        //check if square is blocked
//        if( is_square_clear(block_map, map_width, map_height, build_x-1, build_y-1, 0, build_dim+2) == false){
//            continue; //just skip if square is blocked
//        }
//
//        //now build a tower there
//        build_tower_NxN(map_scenery_top, map_scenery_bottom, block_map, map_width, map_height, build_x, build_y, build_dim, build_floors, brick_p_col_in, brick_s_col_in, floor_p_col_in, floor_s_col_in, door_col1, ladder_col_p);
//
//        //now connect tower to road by path
//        //find random point in new tower
//        x2 = build_x + 1 + rand()%(build_dim-2); //random point inside tower
//        y2 = build_y + 1 + rand()%(build_dim-2); //random point inside tower
//        //Find path between two buildings
//        vector<vector<int>> path = A_Star(block_map, map_width, map_height, x1, y1, x2, y2); //a list of steps to get between the two buildings
//        build_floor_path(map_scenery_bottom, block_map, map_width, map_height, x1, y1, x2, y2, floor_p_col_in, floor_s_col_in);
//
//        //Build a mailbox at new house
//        //Item temp_mailbox = Item(build_x+rand()%build_dim, build_y-2, 325);
//        Item temp_mailbox = Item(build_x-1, build_y-1, 325);
//        map_scenery_top->at( temp_mailbox.y*map_width + temp_mailbox.x ).push_back(temp_mailbox);
//        block_map[temp_mailbox.y*map_width + temp_mailbox.x ] = true;
//
//        //now update the reference points
//        x1 = x2;
//        y1 = y2;
//
//    }
//
//    //What if we changed the design scheme colors for the next row?
//    brick_p_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255};
//    brick_s_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255};
//    floor_p_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255};
//    floor_s_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255};
//    door_col1 = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255};
//    //ladder_p_col_in = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255};
//
//    //NOW DO the FOURTH ROW
//    x1 = 15 - rand()%5;
//    y1 = 20;
//    //Build a sign post here
//    temp_sign = Item(x1, y1-1, 322, ladder_col_p, {0,0,0,255}, genStreetName());
//    map_scenery_top->at( temp_sign.y*map_width + temp_sign.x ).push_back(temp_sign);
//    block_map[temp_sign.y*map_width + temp_sign.x ] = true;
//
//    //Now that we have a reference point, start going through a placing towers (slightly skewed) along the FIRST ROW
//    for(int i = 0 ; i < 8; i++){ //This is how many towers/row we'll be making
//
//        build_x = 15 + i*17 + rand()%4; //incremental, skewed
//        build_y = y1 + rand()%2 - rand()%2; //along the same line, skewed
//        build_dim = rand()%4 + 5;
//        build_floors = 3 + rand()%4;
//
//        //check if square is blocked
//        if( is_square_clear(block_map, map_width, map_height, build_x-1, build_y-1, 0, build_dim+2) == false){
//            continue; //just skip if square is blocked
//        }
//
//        //now build a tower there
//        build_tower_NxN(map_scenery_top, map_scenery_bottom, block_map, map_width, map_height, build_x, build_y, build_dim, build_floors, brick_p_col_in, brick_s_col_in, floor_p_col_in, floor_s_col_in, door_col1, ladder_col_p);
//
//        //now connect tower to road by path
//        //find random point in new tower
//        x2 = build_x + 1 + rand()%(build_dim-2); //random point inside tower
//        y2 = build_y + 1 + rand()%(build_dim-2); //random point inside tower
//        //Find path between two buildings
//        vector<vector<int>> path = A_Star(block_map, map_width, map_height, x1, y1, x2, y2); //a list of steps to get between the two buildings
//        build_floor_path(map_scenery_bottom, block_map, map_width, map_height, x1, y1, x2, y2, floor_p_col_in, floor_s_col_in);
//
//        //Build a mailbox at new house
//        //Item temp_mailbox = Item(build_x+rand()%build_dim, build_y-2, 325);
//        Item temp_mailbox = Item(build_x-1, build_y-1, 325);
//        map_scenery_top->at( temp_mailbox.y*map_width + temp_mailbox.x ).push_back(temp_mailbox);
//        block_map[temp_mailbox.y*map_width + temp_mailbox.x ] = true;
//
//        //now update the reference points
//        x1 = x2;
//        y1 = y2;
//
//    }
//
//}

//Builds a row off towers and assigns them addresses, etc.
//Returns a list of all the created towers
vector<Tower> build_neighborhood(vector<vector<Item>>* map_scenery_top,vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height, SDL_Color brick_p_col_in, SDL_Color brick_s_col_in, SDL_Color floor_p_col_in, SDL_Color floor_s_col_in,  SDL_Color door_col1, SDL_Color ladder_col_p ){
    
    //Create the return variable (a list of Towers)
    vector<Tower> map_towers; //stores the Towers we create
    
    //Variables Declare
    int x1,y1,x2,y2; //stores the coords of two points in the buildings for path finding
    int build_x, build_y, build_dim, build_floors; //building parameters, temporary
    int address_numer; //a variable used to keep track of address numbering
    
    
    //Build FIRST street (Top-Running Row)
    //========================================
    x1 = 0;
    y1 = 35 + rand()%5 - rand()%5;
    x2 = map_width-1;
    y2 = y1;
    
    //Create the "street" walkway itself
    build_floor_path(map_scenery_bottom, block_map, map_width, map_height, x1, y1, x2, y2, floor_p_col_in, floor_s_col_in);
 
    //Build a sign post at each end
    string street_name = genStreetName(); //string holding name of street we're making
    Item temp_sign = Item(5+rand()%5, y1-1, 322, ladder_col_p, {0,0,0,255}, street_name);
    map_scenery_top->at( temp_sign.y*map_width + temp_sign.x ).push_back(temp_sign);
    block_map[temp_sign.y*map_width + temp_sign.x ] = true;
    temp_sign = Item(map_width-5-rand()%5, y1-1, 322, ladder_col_p, {0,0,0,255}, street_name);
    map_scenery_top->at( temp_sign.y*map_width + temp_sign.x ).push_back(temp_sign);
    block_map[temp_sign.y*map_width + temp_sign.x ] = true;
    
    //Initialize address stuff
    address_numer = rand()%20;
    
    //Place towers all along the path...
    x1 = 35;//move the x cursor on the other side of the vertical column path
    while(x1<map_width-35-25){
        
        //Initialize the build dimensions along the pathh....
        build_dim = rand()%4 + 5;
        build_x = x1 + 15 + rand()%5;
        build_y = y1 - 5 - rand()%5 - build_dim;
        build_floors = 3 + rand()%7;
        
        //Now update reference points
        x1 = build_x;
        y1 = y1;
        
        //check if square is blocked
        if( is_square_clear(block_map, map_width, map_height, build_x-1, build_y-1, 0, build_dim+2) == false){
            continue; //just skip if square is blocked
        }
        
        //Now build a tower there;
        Tower temp_tower = Tower(map_scenery_top, map_scenery_bottom, block_map, map_width, map_height, build_x, build_y, build_dim, build_floors);
        
        //Need to create a door in the correct place...
        temp_tower.build_door(map_scenery_top, block_map, map_width, map_height, 0, 1); //build door in position 1 (bottom of square)
    
        //build a mailbox.
        temp_tower.build_mailbox(map_scenery_top, block_map, map_width, map_height, 1, to_string(address_numer) + " " + street_name ); //position 1
        
        //put the backyard
        temp_tower.assign_backyard(1);
        
        //now connect tower to road by path
        //first point is the step in front of door
        x2 = temp_tower.door_x;
        y2 = temp_tower.door_y + 1;
        //Find path between two buildings
        vector<vector<int>> path = A_Star(block_map, map_width, map_height, x1, y1, x2, y2); //a list of steps between the point and the street ref point
        build_floor_path(map_scenery_bottom, block_map, map_width, map_height, x1, y1, x2, y2, floor_p_col_in, floor_s_col_in);
        
        //update address for next tower
        address_numer = address_numer + rand()%9 + 1;
        
        //add tower to running list
        map_towers.push_back(temp_tower);
        
    }
    
    //Build SECOND street (Bottom-Running Row)
    //========================================
    x1 = 0;
    y1 = map_height - 35 + rand()%5 - rand()%5;
    x2 = map_width-1;
    y2 = y1;
    
    //Create the "street" walkway itself
    build_floor_path(map_scenery_bottom, block_map, map_width, map_height, x1, y1, x2, y2, floor_p_col_in, floor_s_col_in);
    
    //Build a sign post at each end
    street_name = genStreetName(); //string holding name of street we're making
    temp_sign = Item(5+rand()%5, y1+1, 322, ladder_col_p, {0,0,0,255}, street_name);
    map_scenery_top->at( temp_sign.y*map_width + temp_sign.x ).push_back(temp_sign);
    block_map[temp_sign.y*map_width + temp_sign.x ] = true;
    temp_sign = Item(map_width-5-rand()%5, y1+1, 322, ladder_col_p, {0,0,0,255}, street_name);
    map_scenery_top->at( temp_sign.y*map_width + temp_sign.x ).push_back(temp_sign);
    block_map[temp_sign.y*map_width + temp_sign.x ] = true;
    
    //Initialize address stuff
    address_numer = rand()%20;
    
    //Place towers all along the path...
    x1 = 35;//move the x cursor on the other side of the vertical column path
    while(x1<map_width-35-25){
        
        //Initialize the build dimensions along the pathh....
        build_dim = rand()%4 + 5;
        build_x = x1 + 15 + rand()%5;
        build_y = y1 + 5 + rand()%5 + build_dim;
        build_floors = 3 + rand()%4;
        
        //Now update reference points
        x1 = build_x;
        y1 = y1;
        
        //check if square is blocked
        if( is_square_clear(block_map, map_width, map_height, build_x-1, build_y-1, 0, build_dim+2) == false){
            continue; //just skip if square is blocked
        }
        
        //Now build a tower there;
        Tower temp_tower = Tower(map_scenery_top, map_scenery_bottom, block_map, map_width, map_height, build_x, build_y, build_dim, build_floors);
        
        //Need to create a door in the correct place...
        temp_tower.build_door(map_scenery_top, block_map, map_width, map_height, 0, 0); //build door in position 1 (top of square)
        
        //build a mailbox.
        temp_tower.build_mailbox(map_scenery_top, block_map, map_width, map_height, 0, to_string(address_numer) + " " + street_name); //position 0
        
        //put the backyard
        temp_tower.assign_backyard(0);
        
        //now connect tower to road by path
        //first point is the step in front of door
        x2 = temp_tower.door_x;
        y2 = temp_tower.door_y - 1;
        //Find path between two buildings
        vector<vector<int>> path = A_Star(block_map, map_width, map_height, x1, y1, x2, y2); //a list of steps between the point and the street ref point
        build_floor_path(map_scenery_bottom, block_map, map_width, map_height, x1, y1, x2, y2, floor_p_col_in, floor_s_col_in);
        
        //update address for next tower
        address_numer = address_numer + rand()%9 + 1;
        
        //add tower to running list
        map_towers.push_back(temp_tower);
        
    }
    
    //Build THIRD street (RIGHT-Running Row)
    //========================================
    x1 = map_width - 35 + rand()%5 - rand()%5;
    y1 = 0;
    x2 = x1;
    y2 = map_height-1;
    
    //Create the "street" walkway itself
    build_floor_path(map_scenery_bottom, block_map, map_width, map_height, x1, y1, x2, y2, floor_p_col_in, floor_s_col_in);
    
    //Build a sign post at each end
    street_name = genStreetName(); //string holding name of street we're making
    temp_sign = Item(x1+1, 5+rand()%5, 322, ladder_col_p, {0,0,0,255}, street_name);
    map_scenery_top->at( temp_sign.y*map_width + temp_sign.x ).push_back(temp_sign);
    block_map[temp_sign.y*map_width + temp_sign.x ] = true;
    temp_sign = Item(x1+1, map_height-5-rand()%5, 322, ladder_col_p, {0,0,0,255}, street_name);
    map_scenery_top->at( temp_sign.y*map_width + temp_sign.x ).push_back(temp_sign);
    block_map[temp_sign.y*map_width + temp_sign.x ] = true;
    
    //Initialize address stuff
    address_numer = rand()%20;
    
    //Place towers all along the path...
    y1 = 35;//move the x cursor on the other side of the vertical column path
    while(y1<map_height-35-15){
        
        //Initialize the build dimensions along the pathh....
        build_dim = rand()%4 + 5;
        build_x = x1 + 5 + rand()%5 + build_dim;
        build_y = y1 + 15 + rand()%5;
        build_floors = 3 + rand()%4;
        
        //Now update reference points
        x1 = x1;
        y1 = build_y-1;
        
        //check if square is blocked
        if( is_square_clear(block_map, map_width, map_height, build_x-1, build_y-1, 0, build_dim+2) == false){
            continue; //just skip if square is blocked
        }
        
        //Now build a tower there;
        Tower temp_tower = Tower(map_scenery_top, map_scenery_bottom, block_map, map_width, map_height, build_x, build_y, build_dim, build_floors);
        
        //Need to create a door in the correct place...
        temp_tower.build_door(map_scenery_top, block_map, map_width, map_height, 0, 2); //build door in position 2 (left of square)
        
        //build a mailbox.
        temp_tower.build_mailbox(map_scenery_top, block_map, map_width, map_height, 2, to_string(address_numer) + " " + street_name); //position 2
        
        //put the backyard
        temp_tower.assign_backyard(2);
        
        //now connect tower to road by path
        //first point is the step in front of door
        x2 = temp_tower.door_x-1;
        y2 = temp_tower.door_y;
        //Find path between two buildings
        vector<vector<int>> path = A_Star(block_map, map_width, map_height, x1, y1, x2, y2); //a list of steps between the point and the street ref point
        build_floor_path(map_scenery_bottom, block_map, map_width, map_height, x1, y1, x2, y2, floor_p_col_in, floor_s_col_in);
        
        //update address for next tower
        address_numer = address_numer + rand()%9 + 1;
        
        //add tower to running list
        map_towers.push_back(temp_tower);
        
    }
    
    
    //Build FOURTH street (LEFT-Running Row)
    //========================================
    x1 = 35 + rand()%5 - rand()%5;
    y1 = 0;
    x2 = x1;
    y2 = map_height-1;
    
    //Create the "street" walkway itself
    build_floor_path(map_scenery_bottom, block_map, map_width, map_height, x1, y1, x2, y2, floor_p_col_in, floor_s_col_in);
    
    //Build a sign post at each end
    street_name = genStreetName(); //string holding name of street we're making
    temp_sign = Item(x1-1, 5+rand()%5, 322, ladder_col_p, {0,0,0,255}, street_name);
    map_scenery_top->at( temp_sign.y*map_width + temp_sign.x ).push_back(temp_sign);
    block_map[temp_sign.y*map_width + temp_sign.x ] = true;
    temp_sign = Item(x1-1, map_height-5-rand()%5, 322, ladder_col_p, {0,0,0,255}, street_name);
    map_scenery_top->at( temp_sign.y*map_width + temp_sign.x ).push_back(temp_sign);
    block_map[temp_sign.y*map_width + temp_sign.x ] = true;
    
    //Initialize address stuff
    address_numer = rand()%20;
    
    //Place towers all along the path...
    y1 = 35;//move the x cursor on the other side of the vertical column path
    while(y1<map_height-35-15){
        
        //Initialize the build dimensions along the pathh....
        build_dim = rand()%4 + 5;
        build_x = x1 - 5 - rand()%5 - build_dim;
        build_y = y1 + 15 + rand()%5;
        build_floors = 3 + rand()%4;
        
        //Now update reference points
        x1 = x1;
        y1 = build_y-1;
        
        //check if square is blocked
        if( is_square_clear(block_map, map_width, map_height, build_x-1, build_y-1, 0, build_dim+2) == false){
            continue; //just skip if square is blocked
        }
        
        //Now build a tower there;
        Tower temp_tower = Tower(map_scenery_top, map_scenery_bottom, block_map, map_width, map_height, build_x, build_y, build_dim, build_floors);
        
        //Need to create a door in the correct place...
        temp_tower.build_door(map_scenery_top, block_map, map_width, map_height, 0, 3); //build door in position 3 (right of square)
        
        //build a mailbox.
        temp_tower.build_mailbox(map_scenery_top, block_map, map_width, map_height, 3, to_string(address_numer) + " " + street_name); //position 3
        
        //put the backyard
        temp_tower.assign_backyard(3);
        
        //now connect tower to road by path
        //first point is the step in front of door
        x2 = temp_tower.door_x+1;
        y2 = temp_tower.door_y;
        //Find path between two buildings
        vector<vector<int>> path = A_Star(block_map, map_width, map_height, x1, y1, x2, y2); //a list of steps between the point and the street ref point
        build_floor_path(map_scenery_bottom, block_map, map_width, map_height, x1, y1, x2, y2, floor_p_col_in, floor_s_col_in);
        
        //update address for next tower
        address_numer = address_numer + rand()%9 + 1;
        
        //add tower to running list
        map_towers.push_back(temp_tower);
        
    }
    
    
    
    return map_towers;
    
}

//Builds a couple of rows and a couple of columns of houses, making a square grid
void build_neighbor_grid(vector<vector<Item>>* map_scenery_top,vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height, SDL_Color brick_p_col_in, SDL_Color brick_s_col_in, SDL_Color floor_p_col_in, SDL_Color floor_s_col_in,  SDL_Color door_col1, SDL_Color ladder_col_p ){
    
    //Variables Declare
    int x1,y1,x2,y2; //stores the coords of two points in the buildings for path finding
    int build_x, build_y, build_dim, build_floors; //building parameters, temporary
    
    //Build FIRST street (Top-Running Row)
    //------------------------------------------------------------
    x1 = 0;
    y1 = 35 + rand()%5 - rand()%5;
    x2 = map_width-1;
    y2 = y1;
    
    //Create the path
    build_floor_path(map_scenery_bottom, block_map, map_width, map_height, x1, y1, x2, y2, floor_p_col_in, floor_s_col_in);
    
    //Build a sign posts at each end
    string street_name = genStreetName(); //string holding name of street we're making
    Item temp_sign = Item(5+rand()%5, y1-1, 322, ladder_col_p, {0,0,0,255}, street_name);
    map_scenery_top->at( temp_sign.y*map_width + temp_sign.x ).push_back(temp_sign);
    block_map[temp_sign.y*map_width + temp_sign.x ] = true;
    temp_sign = Item(map_width-5-rand()%5, y1-1, 322, ladder_col_p, {0,0,0,255}, street_name);
    map_scenery_top->at( temp_sign.y*map_width + temp_sign.x ).push_back(temp_sign);
    block_map[temp_sign.y*map_width + temp_sign.x ] = true;
    
    //Place towers all along the path...
    x1 = 35;//move the x cursor on the other side of the vertical column path
    while(x1<map_width-35-15){
        
        //Initialize the build dimensions along the pathh....
        build_dim = rand()%4 + 5;
        build_x = x1 + 15 + rand()%5;
        build_y = y1 - 5 - rand()%5 - build_dim;
        build_floors = 3 + rand()%4;
        
        //Now update reference points
        x1 = build_x;
        y1 = y1;
        
        //check if square is blocked
        if( is_square_clear(block_map, map_width, map_height, build_x-1, build_y-1, 0, build_dim+2) == false){
            continue; //just skip if square is blocked
        }
        
        //now build a tower there
        build_tower_NxN(map_scenery_top, map_scenery_bottom, block_map, map_width, map_height, build_x, build_y, build_dim, build_floors, brick_p_col_in, brick_s_col_in, floor_p_col_in, floor_s_col_in, door_col1, ladder_col_p);
        
        //Build a mailbox at new house
        //Item temp_mailbox = Item(build_x+rand()%build_dim, build_y-2, 325);
        Item temp_mailbox = Item(build_x-1, build_y+1+build_dim, 325);
        map_scenery_top->at( temp_mailbox.y*map_width + temp_mailbox.x ).push_back(temp_mailbox);
        block_map[temp_mailbox.y*map_width + temp_mailbox.x ] = true;
        
        //now connect tower to road by path
        //find random point in new tower
        x2 = build_x + 1 + rand()%(build_dim-2); //random point inside tower
        y2 = build_y + 1 + rand()%(build_dim-2); //random point inside tower
        //Find path between two buildings
        vector<vector<int>> path = A_Star(block_map, map_width, map_height, x1, y1, x2, y2); //a list of steps between the point and the street ref point
        build_floor_path(map_scenery_bottom, block_map, map_width, map_height, x1, y1, x2, y2, floor_p_col_in, floor_s_col_in);
    
    }
    
    //Build SECOND street (Bottom-Running Row)
    //------------------------------------------------------------
    x1 = 0;
    y1 = map_height - 35 + rand()%5 - rand()%5;
    x2 = map_width-1;
    y2 = y1;
    
    //Create the path
    build_floor_path(map_scenery_bottom, block_map, map_width, map_height, x1, y1, x2, y2, floor_p_col_in, floor_s_col_in);
    
    //Build a sign posts at each end
    street_name = genStreetName(); //string holding name of street we're making
    temp_sign = Item(5+rand()%5, y1+1, 322, ladder_col_p, {0,0,0,255}, street_name);
    map_scenery_top->at( temp_sign.y*map_width + temp_sign.x ).push_back(temp_sign);
    block_map[temp_sign.y*map_width + temp_sign.x ] = true;
    temp_sign = Item(map_width-5-rand()%5, y1+1, 322, ladder_col_p, {0,0,0,255}, street_name);
    map_scenery_top->at( temp_sign.y*map_width + temp_sign.x ).push_back(temp_sign);
    block_map[temp_sign.y*map_width + temp_sign.x ] = true;

    //Place towers all along the path...
    x1 = 35;//move the x cursor on the other side of the vertical column path
    while(x1<map_width-35-15){

        //Initialize the build dimensions along the pathh....
        build_dim = rand()%4 + 5;
        build_x = x1 + 15 + rand()%5;
        build_y = y1 + 5 + rand()%5 + build_dim;
        build_floors = 3 + rand()%4;

        //Now update reference points
        x1 = build_x;
        y1 = y1;

        //check if square is blocked
        if( is_square_clear(block_map, map_width, map_height, build_x-1, build_y-1, 0, build_dim+2) == false){
            continue; //just skip if square is blocked
        }

        //now build a tower there
        build_tower_NxN(map_scenery_top, map_scenery_bottom, block_map, map_width, map_height, build_x, build_y, build_dim, build_floors, brick_p_col_in, brick_s_col_in, floor_p_col_in, floor_s_col_in, door_col1, ladder_col_p);

        //Build a mailbox at new house
        //Item temp_mailbox = Item(build_x+rand()%build_dim, build_y-2, 325);
        Item temp_mailbox = Item(build_x-1, build_y-2, 325);
        map_scenery_top->at( temp_mailbox.y*map_width + temp_mailbox.x ).push_back(temp_mailbox);
        block_map[temp_mailbox.y*map_width + temp_mailbox.x ] = true;
        
        //now connect tower to road by path
        //find random point in new tower
        x2 = build_x + 1 + rand()%(build_dim-2); //random point inside tower
        y2 = build_y + 1 + rand()%(build_dim-2); //random point inside tower
        //Find path between two buildings
        vector<vector<int>> path = A_Star(block_map, map_width, map_height, x1, y1, x2, y2); //a list of steps between the point and the street ref point
        build_floor_path(map_scenery_bottom, block_map, map_width, map_height, x1, y1, x2, y2, floor_p_col_in, floor_s_col_in);

    }
    
    //Build THIRD street (RIGHT-Running Row)
    //------------------------------------------------------------
    x1 = map_width - 35 + rand()%5 - rand()%5;
    y1 = 0;
    x2 = x1;
    y2 = map_height-1;
    
    //Create the path
    build_floor_path(map_scenery_bottom, block_map, map_width, map_height, x1, y1, x2, y2, floor_p_col_in, floor_s_col_in);
    
    //Build a sign posts at each end
    street_name = genStreetName(); //string holding name of street we're making
    temp_sign = Item(x1+1, 5+rand()%5, 322, ladder_col_p, {0,0,0,255}, street_name);
    map_scenery_top->at( temp_sign.y*map_width + temp_sign.x ).push_back(temp_sign);
    block_map[temp_sign.y*map_width + temp_sign.x ] = true;
    temp_sign = Item(x1+1, map_height-5-rand()%5, 322, ladder_col_p, {0,0,0,255}, street_name);
    map_scenery_top->at( temp_sign.y*map_width + temp_sign.x ).push_back(temp_sign);
    block_map[temp_sign.y*map_width + temp_sign.x ] = true;
    
    //Place towers all along the path...
    y1 = 35;//move the x cursor on the other side of the vertical column path
    while(y1<map_height-35-15){

        //Initialize the build dimensions along the pathh....
        build_dim = rand()%4 + 5;
        build_x = x1 + 5 + rand()%5 + build_dim;
        build_y = y1 + 15 + rand()%5;
        build_floors = 3 + rand()%4;

        //Now update reference points
        x1 = x1;
        y1 = build_y-1;

        //check if square is blocked
        if( is_square_clear(block_map, map_width, map_height, build_x-1, build_y-1, 0, build_dim+2) == false){
            continue; //just skip if square is blocked
        }

        //now build a tower there
        build_tower_NxN(map_scenery_top, map_scenery_bottom, block_map, map_width, map_height, build_x, build_y, build_dim, build_floors, brick_p_col_in, brick_s_col_in, floor_p_col_in, floor_s_col_in, door_col1, ladder_col_p);

        //Build a mailbox at new house
        //Item temp_mailbox = Item(build_x+rand()%build_dim, build_y-2, 325);
        Item temp_mailbox = Item(build_x-2, build_y-1+build_dim, 325);
        map_scenery_top->at( temp_mailbox.y*map_width + temp_mailbox.x ).push_back(temp_mailbox);
        block_map[temp_mailbox.y*map_width + temp_mailbox.x ] = true;
        
        //now connect tower to road by path
        //find random point in new tower
        x2 = build_x + 1 + rand()%(build_dim-2); //random point inside tower
        y2 = build_y + 1 + rand()%(build_dim-2); //random point inside tower
        //Find path between two buildings
        vector<vector<int>> path = A_Star(block_map, map_width, map_height, x1, y1, x2, y2); //a list of steps between the point and the street ref point
        build_floor_path(map_scenery_bottom, block_map, map_width, map_height, x1, y1, x2, y2, floor_p_col_in, floor_s_col_in);


    }
    
    //Build FOURTH street (LEFT-Running Row)
    //------------------------------------------------------------
    x1 = 35 + rand()%5 - rand()%5;
    y1 = 0;
    x2 = x1;
    y2 = map_height-1;
    
    //Create the path
    build_floor_path(map_scenery_bottom, block_map, map_width, map_height, x1, y1, x2, y2, floor_p_col_in, floor_s_col_in);
    
    //Build a sign posts at each end
    street_name = genStreetName(); //string holding name of street we're making
    temp_sign = Item(x1-1, 5+rand()%5, 322, ladder_col_p, {0,0,0,255}, street_name);
    map_scenery_top->at( temp_sign.y*map_width + temp_sign.x ).push_back(temp_sign);
    block_map[temp_sign.y*map_width + temp_sign.x ] = true;
    temp_sign = Item(x1-1, map_height-5-rand()%5, 322, ladder_col_p, {0,0,0,255}, street_name);
    map_scenery_top->at( temp_sign.y*map_width + temp_sign.x ).push_back(temp_sign);
    block_map[temp_sign.y*map_width + temp_sign.x ] = true;
    
    //Place towers all along the path...
    y1 = 35;//move the x cursor on the other side of the vertical column path
    while(y1<map_height-35-15){

        //Initialize the build dimensions along the pathh....
        build_dim = rand()%4 + 5;
        build_x = x1 - 5 - rand()%5 - build_dim;
        build_y = y1 + 15 + rand()%5;
        build_floors = 3 + rand()%4;

        //Now update reference points
        x1 = x1;
        y1 = build_y-1;

        //check if square is blocked
        if( is_square_clear(block_map, map_width, map_height, build_x-1, build_y-1, 0, build_dim+2) == false){
            continue; //just skip if square is blocked
        }

        //now build a tower there
        build_tower_NxN(map_scenery_top, map_scenery_bottom, block_map, map_width, map_height, build_x, build_y, build_dim, build_floors, brick_p_col_in, brick_s_col_in, floor_p_col_in, floor_s_col_in, door_col1, ladder_col_p);

        //Build a mailbox at new house
        //Item temp_mailbox = Item(build_x+rand()%build_dim, build_y-2, 325);
        Item temp_mailbox = Item(build_x+1 + build_dim, build_y-1+build_dim, 325);
        map_scenery_top->at( temp_mailbox.y*map_width + temp_mailbox.x ).push_back(temp_mailbox);
        block_map[temp_mailbox.y*map_width + temp_mailbox.x ] = true;
        
        //now connect tower to road by path
        //find random point in new tower
        x2 = build_x + 1 + rand()%(build_dim-2); //random point inside tower
        y2 = build_y + 1 + rand()%(build_dim-2); //random point inside tower
        //Find path between two buildings
        vector<vector<int>> path = A_Star(block_map, map_width, map_height, x1, y1, x2, y2); //a list of steps between the point and the street ref point
        build_floor_path(map_scenery_bottom, block_map, map_width, map_height, x1, y1, x2, y2, floor_p_col_in, floor_s_col_in);


    }
    
}



