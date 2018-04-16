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

//Builds a 5x5 building on the map out of standrad bricks
//Input the vector of Item vectors to update with new items, as well as the block_map to update
//Input: also need to innput map_width as a sort of key to index the array elements. SORRY
//Input the x,y coordinates of the upper left corner of the building
void building_5x5(vector<vector<Item>>* map_scenery, bool* block_map, int map_width, int x, int y){
    
    int tempx, tempy; //stores the cuurent positing we are working on building
    int tile = 304; //304 is the Item code for (bricks)
    
    //Generate random colors to use for these buildings
    SDL_Color col1 = {static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), 255 };
    SDL_Color col2 = {static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), 255 };
    
    //delete(map_scenery);
    //map_scenery->at(<#size_type __n#>)
    
    //create top row
    for(int i = 0; i < 5; i++){ //cycle through 5
        for(int j = 0; j < 5; j ++){ //cycle through 5
         
            tempx = x + i;
            tempy = y;
            Item temp_item = Item(tempx, tempy, tile, col1, col2 ); //create the temporary item
            map_scenery->at((tempy*map_width)+tempx).push_back(temp_item);
            block_map[(tempy*map_width)+tempx] = true; //update the block_map
            
        }
    }
    
    //create left wall
    for(int i = 0 ; i < 4; i ++){
        
        tempx = x;
        tempy = y + 1 + i;
        Item temp_item = Item(tempx, tempy, tile, col1, col2 ); //create the temporary item
        map_scenery->at((tempy*map_width)+tempx).push_back(temp_item);
        block_map[(tempy*map_width)+tempx] = true; //update the block_map
        
    }
    
    //create right wall
    for(int i = 0 ; i < 4; i ++){
        
        tempx = x + 4;
        tempy = y + 1 + i;
        Item temp_item = Item(tempx, tempy, tile, col1, col2 ); //create the temporary item
        map_scenery->at((tempy*map_width)+tempx).push_back(temp_item);
        block_map[(tempy*map_width)+tempx] = true; //update the block_map
        
    }
    
    //create doorways
    for(int i = 0; i < (5/2 - 1); i++){
        
        //left doorway
        tempx = x+1+i;
        tempy = y + 4;
        Item temp_item = Item(tempx, tempy, tile, col1, col2 ); //create the temporary item
        map_scenery->at((tempy*map_width)+tempx).push_back(temp_item);
        block_map[(tempy*map_width)+tempx] = true; //update the block_map
        
        //right doorway
        tempx = x+4-i-1;
        tempy = y + 4;
        temp_item = Item(tempx, tempy, tile, col1, col2 ); //create the temporary item
        map_scenery->at((tempy*map_width)+tempx).push_back(temp_item);
        block_map[(tempy*map_width)+tempx] = true; //update the block_map
        
    }
    
    return;
}

//Builds an NxN building on the map out of standrad bricks
//Input the vector of Item vectors to update with new items, as well as the block_map to update
//Input: also need to innput map_width as a sort of key to index the array elements. SORRY
//Input the x,y coordinates of the upper left corner of the building
//Input N, the dimensions of square building
void building_NxN(vector<vector<Item>>* map_scenery, bool* block_map, int map_width, int x, int y, int n){
    
    int tempx, tempy; //stores the cuurent positing we are working on building
    int tile = 304; //304 is the Item code for (bricks)
    
    //Generate random colors to use for these buildings
    SDL_Color col1 = {static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), 255 };
    SDL_Color col2 = {static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), 255 };
    
    //delete(map_scenery);
    //map_scenery->at(<#size_type __n#>)
    
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
    
    //create doorways
    for(int i = 0; i < (n/2 - 1); i++){
        
        //left doorway
        tempx = x+1+i;
        tempy = y + n-1;
        Item temp_item = Item(tempx, tempy, tile, col1, col2 ); //create the temporary item
        map_scenery->at((tempy*map_width)+tempx).push_back(temp_item);
        block_map[(tempy*map_width)+tempx] = true; //update the block_map
        
        //right doorway
        tempx = x+n-1-i-1;
        tempy = y + n-1;
        temp_item = Item(tempx, tempy, tile, col1, col2 ); //create the temporary item
        map_scenery->at((tempy*map_width)+tempx).push_back(temp_item);
        block_map[(tempy*map_width)+tempx] = true; //update the block_map
        
    }
    
    return;
}

