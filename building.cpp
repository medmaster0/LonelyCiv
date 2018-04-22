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

//Builds an NxN enclosed box on the map out of standrad bricks
//Input the vector of Item vectors to update with new items, as well as the block_map to update
//Input: also need to innput map_width as a sort of key to index the array elements. SORRY
//Input the x,y coordinates of the upper left corner of the building
//Input N, the dimensions of square building
void build_box_NxN(vector<vector<Item>>* map_scenery, bool* block_map, int map_width,int map_height, int x, int y, int n, SDL_Color col1_in, SDL_Color col2_in){
 
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
        map_scenery->at((tempy*map_width)+tempx).push_back(temp_item);
        block_map[(tempy*map_width)+tempx] = true; //update the block_map
            
        
    }
    
    //create left wall
    for(int i = 0 ; i < n-1; i ++){
        
        tempx = x;
        tempy = y + 1 + i;
        Item temp_item = Item(tempx, tempy, tile, col1, col2 ); //create the temporary item
        map_scenery->at((tempy*map_width)+tempx).push_back(temp_item);
        block_map[(tempy*map_width)+tempx] = true; //update the block_map
        
    }
    
    //create right wall
    for(int i = 0 ; i < n-1; i ++){
        
        tempx = x + n-1;
        tempy = y + 1 + i;
        Item temp_item = Item(tempx, tempy, tile, col1, col2 ); //create the temporary item
        map_scenery->at((tempy*map_width)+tempx).push_back(temp_item);
        block_map[(tempy*map_width)+tempx] = true; //update the block_map
        
    }
    
    //Create bottom row
    for(int i = 0; i < n-2; i++){ //cycle through n minus 2 (since we got them while doing left and right walls)
        
        tempx = x + i + 1;
        tempy = y + n-1;
        Item temp_item = Item(tempx, tempy, tile, col1, col2 ); //create the temporary item
        map_scenery->at((tempy*map_width)+tempx).push_back(temp_item);
        block_map[(tempy*map_width)+tempx] = true; //update the block_map

    }
    
}

//Builds an NxN enclosed box on the map out of standrad bricks
//Input the vector of Item vectors to update with new items, as well as the block_map to update
//Input: also need to innput map_width as a sort of key to index the array elements. SORRY
//Input the x,y coordinates of the upper left corner of the building
//Input N, the dimensions of square building
//Deletes one of the bricks along the wall to create a doorway (currently the center brick)
void build_box_NxN_door(vector<vector<Item>>* map_scenery, bool* block_map, int map_width, int map_height, int x, int y, int n, SDL_Color col1_in, SDL_Color col2_in, SDL_Color door_col1_in){
    
    int tempx, tempy; //stores the cuurent positing we are working on building
    int tile = 304; //304 is the Item code for (bricks)
    
    //Generate random colors to use for these buildings
    SDL_Color col1 = col1_in;
    SDL_Color col2 = col2_in;
    
    //Use the exisiting build box routine
    build_box_NxN(map_scenery, block_map, map_width,map_height, x, y, n, col1, col2);
    
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
    block_map[(tempy*map_width)+tempx] = false; //update the block_map
    map_scenery->at((tempy*map_width)+tempx).pop_back(); //remove elemement from map array
    
    //Now create a new doorway Item and add to scenery
    SDL_Color col3 = door_col1_in;
    Item temp_door = Item(tempx, tempy, 317, col3, {0,0,0,255}); //create a DOORWAY (Curtains) Item
    map_scenery->at((tempy*map_width)+tempx).push_back(temp_door);
    
}


//Builds a maze on the map
void build_maze(vector<vector<Item>>* map_scenery, bool* block_map, int map_width, int map_height, int x, int y){

    //Make a brick maze
    vector<vector<short>> gmap = gen_maze_corridor(20,30);
    int con_x = x;//the starting point of construction
    int con_y = y;//the starting point of construction
    //redefine color
    SDL_Color p1 = {static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255)};
    SDL_Color s1 = {static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255)};
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
                Item con_item = Item(tempx, tempy, 304, p1, s1);
                map_scenery->at((tempy*map_width)+tempx).push_back(con_item);
                block_map[(tempy*map_width)+tempx] = true;
            }

        }
    }
}

//Builds an NxN box of floor tiles on the map
void build_floor_NxN(vector<vector<Item>>* map_scenery, bool* block_map, int map_width, int map_height, int x, int y, int n, SDL_Color p_col , SDL_Color s_col ){
    
    if( !is_square_clear(block_map, map_width, map_height, x, y, n)){
        printf("floor space not clear... not building\n"); 
        return; //do nothing
    }
    
    //Cycle through floor space
    for(int i = 0; i < n; i++){
        for(int j = 0 ; j < n; j++){
            Item temp_floor = Item(x+i, y+j, 301, p_col, s_col);
            map_scenery->at( (temp_floor.y*map_width) + temp_floor.x ).push_back(temp_floor);
        }
    }
    
    
}


//Builds a circular structure of bricks
//Uses Midpoint Circl Drawing Algorithm
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
void build_circle_radius(vector<vector<Item>>* map_scenery, bool* block_map, int map_width, int radius, int cent_x, int cent_y, SDL_Color p_col_in, SDL_Color s_col_in){
    
    //Initialize, (start at first step_
    int x = radius;
    int y = 0;
    
    while(y<x){ //once x becomes greater, we're done
        
        //CREATE ITEMS
        //Create item in first sector
        Item temp_brick = Item(cent_x+x, cent_y+y, 304, p_col_in, s_col_in);
        map_scenery->at( (temp_brick.y*map_width) + temp_brick.x ).push_back(temp_brick);
        block_map[(temp_brick.y*map_width)+temp_brick.x] = true;
        //Next sector CCW
        temp_brick = Item(cent_x+y, cent_y+x, 304, p_col_in, s_col_in);
        map_scenery->at( (temp_brick.y*map_width) + temp_brick.x ).push_back(temp_brick);
        block_map[(temp_brick.y*map_width)+temp_brick.x] = true;
        //Next sector CCW
        temp_brick = Item(cent_x-y, cent_y+x, 304, p_col_in, s_col_in);
        map_scenery->at( (temp_brick.y*map_width) + temp_brick.x ).push_back(temp_brick);
        block_map[(temp_brick.y*map_width)+temp_brick.x] = true;
        //Next sector CCW
        temp_brick = Item(cent_x-x, cent_y+y, 304, p_col_in, s_col_in);
        map_scenery->at( (temp_brick.y*map_width) + temp_brick.x ).push_back(temp_brick);
        block_map[(temp_brick.y*map_width)+temp_brick.x] = true;
        //Next sector CCW
        temp_brick = Item(cent_x-x, cent_y-y, 304, p_col_in, s_col_in);
        map_scenery->at( (temp_brick.y*map_width) + temp_brick.x ).push_back(temp_brick);
        block_map[(temp_brick.y*map_width)+temp_brick.x] = true;
        //Next sector CCW
        temp_brick = Item(cent_x-y, cent_y-x, 304, p_col_in, s_col_in);
        map_scenery->at( (temp_brick.y*map_width) + temp_brick.x ).push_back(temp_brick);
        block_map[(temp_brick.y*map_width)+temp_brick.x] = true;
        //Next sector CCW
        temp_brick = Item(cent_x+y, cent_y-x, 304, p_col_in, s_col_in);
        map_scenery->at( (temp_brick.y*map_width) + temp_brick.x ).push_back(temp_brick);
        block_map[(temp_brick.y*map_width)+temp_brick.x] = true;
        //Next sector CCW
        temp_brick = Item(cent_x+x, cent_y-y, 304, p_col_in, s_col_in);
        map_scenery->at( (temp_brick.y*map_width) + temp_brick.x ).push_back(temp_brick);
        block_map[(temp_brick.y*map_width)+temp_brick.x] = true;
        
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
    
}




//checks the block map to make sure nothing is blocking the current location
//Returns a true if clear, false if not
bool is_square_clear(bool* block_map, int map_width, int map_height, int x, int y, int n){
    
    //if out of bounds, return false
    if(x < 0 || y < 0 || x + n > map_width ||  y + n > map_height ){ //last line needs to calculate height
        return false;
    }
    
    for(int i = 0; i < n; i++){ //for cycle x values
        for(int j = 0 ; j < n; j++){ //for cycle y values
            
            if(block_map[ ( (y+j) * map_width ) + (x+i) ] == true){ //if blocked
                return false; //return false
            }
            
        }
    }
    
    //if done cycling, we're all clear
    return true;
    
}

//builds two houses and puts a path between them
void build_two_house_path(vector<vector<Item>>* map_scenery_top,vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height, SDL_Color floor_p_col_in, SDL_Color floor_s_col_in, SDL_Color brick_p_col_in, SDL_Color brick_s_col_in, SDL_Color door_col1 ){
    
    int x1,y1,x2,y2; //stores the coords of two points in the buildings for path finding
    
    int build_x, build_y, build_dim; //building parameters
    
    while(true){
        //BUILD FIRST BUILDING
        build_x = rand()%map_width;
        build_y = rand()%map_height;
        build_dim = 5+rand()%3;
        if(is_square_clear(block_map, map_width, map_height, build_x-1, build_y-1, build_dim+2)){ //If we are all clear, start building... keep a 1 tile wide perimeter
            
            build_box_NxN_door(map_scenery_top, block_map, map_width, map_height, build_x, build_y, build_dim, brick_p_col_in, brick_s_col_in, door_col1); //call building routine
        
            //Build a floor in building
            build_floor_NxN(map_scenery_bottom, block_map, map_width, map_height, build_x+1, build_y+1, build_dim - 2, floor_p_col_in, floor_s_col_in);
            
            //Also find a spot inside building
            x1 = build_x + 1 + rand()%(build_dim - 2);
            y1 = build_y + 1 + rand()%(build_dim - 2);
            break;
        }
    }
    
    while(true){
        //BUILD SECOND BUILDING
        build_x = rand()%map_width;
        build_y = rand()%map_height;
        build_dim = 5+rand()%3;
        if(is_square_clear(block_map, map_width, map_height, build_x-1, build_y-1, build_dim+2)){ //If we are all clear, start building... keep a 1 tile wide perimeter
            
            build_box_NxN_door(map_scenery_top, block_map, map_width, map_height, build_x, build_y, build_dim, brick_p_col_in, brick_s_col_in, door_col1); //call building routine
            
            //Build a floor in building
            build_floor_NxN(map_scenery_bottom, block_map, map_width, map_height, build_x+1, build_y+1, build_dim - 2, floor_p_col_in, floor_s_col_in);
            
            //Also find a spot inside building
            x2 = build_x + 1 + rand()%(build_dim - 2);
            y2 = build_y + 1 + rand()%(build_dim - 2);
            break;
        }
    }
        
    //Find path between two buildings
    vector<vector<int>> path = A_Star(block_map, map_width, map_height, x1, y1, x2, y2); //a list of steps to get between the two buildings
    
    int FLOOR_TILE_INDEX = 301;
    
    //Now cycle through the Entire Path
    SDL_Color floor_p_col = floor_p_col_in;
    SDL_Color floor_s_col = floor_s_col_in;
    bool has_tile = false; //flag used to indicate the path has a tile
    while(path.size()>0){
        
        //get next step from list
        vector<int> temp_step = path.back();
        path.pop_back();
        
        //make sure a tile isn't already there.
        has_tile = false;
        for(int z = 0; z < map_scenery_bottom->at((temp_step[1]*map_width) + temp_step[0]).size() ; z++){
            if(map_scenery_bottom->at( (temp_step[1]*map_width) + temp_step[0] ).at(z).type == FLOOR_TILE_INDEX){
                printf("skip tile\n");
                has_tile = true; //set flag to prevent creation of new tile
            }
        }
        if(has_tile == true){continue;} //move on to next step
        
        //Create a floortile tile at each step
        Item temp_floor = Item(temp_step[0], temp_step[1], FLOOR_TILE_INDEX, floor_p_col, floor_s_col );
        map_scenery_bottom->at((temp_step[1]*map_width)+temp_step[0]).push_back(temp_floor);
    }
    
}

