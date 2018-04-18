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
//

////Builds a 5x5 building on the map out of standrad bricks
////Input the vector of Item vectors to update with new items, as well as the block_map to update
////Input: also need to innput map_width as a sort of key to index the array elements. SORRY
////Input the x,y coordinates of the upper left corner of the building
//void building_5x5(vector<vector<Item>>* map_scenery, bool* block_map, int map_width, int x, int y){
//
//    int tempx, tempy; //stores the cuurent positing we are working on building
//    int tile = 304; //304 is the Item code for (bricks)
//
//    //Generate random colors to use for these buildings
//    SDL_Color col1 = {static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), 255 };
//    SDL_Color col2 = {static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), 255 };
//
//    //delete(map_scenery);
//    //map_scenery->at(<#size_type __n#>)
//
//    //create top row
//    for(int i = 0; i < 5; i++){ //cycle through 5
//        for(int j = 0; j < 5; j ++){ //cycle through 5
//
//            tempx = x + i;
//            tempy = y;
//            Item temp_item = Item(tempx, tempy, tile, col1, col2 ); //create the temporary item
//            map_scenery->at((tempy*map_width)+tempx).push_back(temp_item);
//            block_map[(tempy*map_width)+tempx] = true; //update the block_map
//
//        }
//    }
//
//    //create left wall
//    for(int i = 0 ; i < 4; i ++){
//
//        tempx = x;
//        tempy = y + 1 + i;
//        Item temp_item = Item(tempx, tempy, tile, col1, col2 ); //create the temporary item
//        map_scenery->at((tempy*map_width)+tempx).push_back(temp_item);
//        block_map[(tempy*map_width)+tempx] = true; //update the block_map
//
//    }
//
//    //create right wall
//    for(int i = 0 ; i < 4; i ++){
//
//        tempx = x + 4;
//        tempy = y + 1 + i;
//        Item temp_item = Item(tempx, tempy, tile, col1, col2 ); //create the temporary item
//        map_scenery->at((tempy*map_width)+tempx).push_back(temp_item);
//        block_map[(tempy*map_width)+tempx] = true; //update the block_map
//
//    }
//
//    //create doorways
//    for(int i = 0; i < (5/2 - 1); i++){
//
//        //left doorway
//        tempx = x+1+i;
//        tempy = y + 4;
//        Item temp_item = Item(tempx, tempy, tile, col1, col2 ); //create the temporary item
//        map_scenery->at((tempy*map_width)+tempx).push_back(temp_item);
//        block_map[(tempy*map_width)+tempx] = true; //update the block_map
//
//        //right doorway
//        tempx = x+4-i-1;
//        tempy = y + 4;
//        temp_item = Item(tempx, tempy, tile, col1, col2 ); //create the temporary item
//        map_scenery->at((tempy*map_width)+tempx).push_back(temp_item);
//        block_map[(tempy*map_width)+tempx] = true; //update the block_map
//
//    }
//
//    return;
//}
//
////Builds an NxN building on the map out of standrad bricks
////Input the vector of Item vectors to update with new items, as well as the block_map to update
////Input: also need to innput map_width as a sort of key to index the array elements. SORRY
////Input the x,y coordinates of the upper left corner of the building
////Input N, the dimensions of square building
//void building_NxN(vector<vector<Item>>* map_scenery, bool* block_map, int map_width, int x, int y, int n){
//
//    int tempx, tempy; //stores the cuurent positing we are working on building
//    int tile = 304; //304 is the Item code for (bricks)
//
//    //Generate random colors to use for these buildings
//    SDL_Color col1 = {static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), 255 };
//    SDL_Color col2 = {static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), 255 };
//
//    //delete(map_scenery);
//    //map_scenery->at(<#size_type __n#>)
//
//    //create top row
//    for(int i = 0; i < n; i++){ //cycle through 5
//        for(int j = 0; j < 5; j ++){ //cycle through 5
//
//            tempx = x + i;
//            tempy = y;
//            Item temp_item = Item(tempx, tempy, tile, col1, col2 ); //create the temporary item
//            map_scenery->at((tempy*map_width)+tempx).push_back(temp_item);
//            block_map[(tempy*map_width)+tempx] = true; //update the block_map
//
//        }
//    }
//
//    //create left wall
//    for(int i = 0 ; i < n-1; i ++){
//
//        tempx = x;
//        tempy = y + 1 + i;
//        Item temp_item = Item(tempx, tempy, tile, col1, col2 ); //create the temporary item
//        map_scenery->at((tempy*map_width)+tempx).push_back(temp_item);
//        block_map[(tempy*map_width)+tempx] = true; //update the block_map
//
//    }
//
//    //create right wall
//    for(int i = 0 ; i < n-1; i ++){
//
//        tempx = x + n-1;
//        tempy = y + 1 + i;
//        Item temp_item = Item(tempx, tempy, tile, col1, col2 ); //create the temporary item
//        map_scenery->at((tempy*map_width)+tempx).push_back(temp_item);
//        block_map[(tempy*map_width)+tempx] = true; //update the block_map
//
//    }
//
//    //create doorways
//    for(int i = 0; i < (n/2 - 1); i++){
//
//        //left doorway
//        tempx = x+1+i;
//        tempy = y + n-1;
//        Item temp_item = Item(tempx, tempy, tile, col1, col2 ); //create the temporary item
//        map_scenery->at((tempy*map_width)+tempx).push_back(temp_item);
//        block_map[(tempy*map_width)+tempx] = true; //update the block_map
//
//        //right doorway
//        tempx = x+n-1-i-1;
//        tempy = y + n-1;
//        temp_item = Item(tempx, tempy, tile, col1, col2 ); //create the temporary item
//        map_scenery->at((tempy*map_width)+tempx).push_back(temp_item);
//        block_map[(tempy*map_width)+tempx] = true; //update the block_map
//
//    }
//
//    return;
//}

//Builds an NxN enclosed box on the map out of standrad bricks
//Input the vector of Item vectors to update with new items, as well as the block_map to update
//Input: also need to innput map_width as a sort of key to index the array elements. SORRY
//Input the x,y coordinates of the upper left corner of the building
//Input N, the dimensions of square building
void build_box_NxN(vector<vector<Item>>* map_scenery, bool* block_map, int map_width, int x, int y, int n){
 
    int tempx, tempy; //stores the cuurent positing we are working on building
    int tile = 304; //304 is the Item code for (bricks)
    
    //Generate random colors to use for these buildings
    SDL_Color col1 = {static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), 255 };
    SDL_Color col2 = {static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), 255 };
    
    //create top row
    for(int i = 0; i < n; i++){ //cycle through 5
        for(int j = 0; j < 5; j ++){ //cycle through 5
            
            tempx = x + i;
            tempy = y;
            Item temp_item = Item(tempx, tempy, tile, col1, col2 ); //create the temporary item
            map_scenery->at((tempy*map_width)+tempx).push_back(temp_item);
            block_map[(tempy*map_width)+tempx] = true; //update the block_map
            
        }
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
void build_box_NxN_door(vector<vector<Item>>* map_scenery, bool* block_map, int map_width, int x, int y, int n){
    
    int tempx, tempy; //stores the cuurent positing we are working on building
    int tile = 304; //304 is the Item code for (bricks)
    
    //Generate random colors to use for these buildings
    SDL_Color col1 = {static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), 255 };
    SDL_Color col2 = {static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), 255 };
    
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
    printf("size%d\n", map_scenery->at((tempy*map_width)+tempx).size());
    map_scenery->at((tempy*map_width)+tempx).pop_back(); //remove elemement from map array
    printf("size%d\n", map_scenery->at((tempy*map_width)+tempx).size());
    
    //Now create a new doorway Item and add to scenery
    SDL_Color col3 = {static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), 255 };
    Item temp_door = Item(tempx, tempy, 317, col3, {0,0,0,255}); //create a DOORWAY (Curtains) Item
    map_scenery->at((tempy*map_width)+tempx).push_back(temp_door);
    printf("size after door%d\n\n\n", map_scenery->at((tempy*map_width)+tempx).size());
    
}



//checks the block map to make sure nothing is blocking the current location
//Returns a true if clear, false if not
bool is_square_clear(bool* block_map, int map_width, int x, int y, int n){
    
    //if out of bounds, return false
    if(x < 0 || y < 0){
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



