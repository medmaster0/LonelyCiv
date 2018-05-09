//
//  dispersal.cpp
//  Civ2
//
//  Created by Medz on 5/7/18.
//  Copyright © 2018 Medz. All rights reserved.
//
//
//This class is for algorithms that disperse items across the map.
//Try different techniques for spreading items (clustering) around the map.
//Can also play around with different color dispersal patterns!!!!


#include "dispersal.hpp"

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include "Item.hpp"
#include "med_algo.hpp"

//Disperses items out across the map in a cone shape (with algo vriance input inside). Cone starts at (x,y) then keeps branching out until end of map (or collapses to 0)
//I wanted to still be able to apply some of the WEEDZ patterns even though we aren't using them anymore
//basically generate a spray, layer by layer, row by row:
// *
// **
// ***
// ****
//Just like that but the lengths can be randomly changing
//isVert allows control of vertical or horizontal cone
//isPositiveDir allows for control of direction, pos: down/right, neg: up/left
void flower_spray(vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height, int x, int y, int z, bool isVert, bool isPositiveDir, SDL_Color p_col_in , SDL_Color s_col_in, int item_type){
    
    //initial variables used throughout
    int x1,y1; //the left/upmost point in a span
    int x2,y2; //the right/bottommost point in a span
    
    //Create the starting point
    x1 = x;
    x2 = x+1;
    y1 = y;
    y2 = y+1;
    
    //Keep going until we reach end of map
    while(true){
        
        //Create a single row of spray
        for(int i = x1; i <= x2; i++){ //cycle all the x's we need to
            printf("some mo spray\n");
            for(int j = y1; j <= y2; j++){ //cycle all the y's we need to
                
//                //Randomly skip a flower to create holes...
//                if(rand()%9 > 6){
//                    continue; //don't do it
//                }
                
                //create an item there.
                Item temp_flower = Item(i,j,item_type,p_col_in,s_col_in);
                map_scenery_bottom->at( (z*map_height*map_width) + (j*map_width)+ i ).push_back(temp_flower);
                
                
            }
        }
        
        //Update the row span
        if(isVert == true){
            //going vertical
            
            //Take a step in the cone's direction
            if(isPositiveDir == true){
                y1 = y1 + 1;
                y2 = y2 + 1;
            }else{
                y1 = y1 - 1;
                y2 = y2 - 1;
            }
            //Bounds check
            if(y1<0 || y1 > map_height){
                break;//quite the wile loop
            }
            
            //generate a (random) span
            x1 = x1 - 2 + rand()%6; //-1 to 1
            x2 = x2 + rand()%2; //0 to 1
            //another bounds check (see if they cross and become outta whack
            if(x2<x1){
                break; //quit the while loop
            }
            
        }else{
            //going horizontal
            
            //Take a step in the cone's directions
            if(isPositiveDir == true){
                x1 = x1 + 1;
                x2 = x2 + 1;
            }else{
                x1 = x1 - 1;
                x2 = x2 - 1;
            }
            //Bounds check
            if(x1<0 || x1 > map_width){
                break;//quite the wile loop
            }
            
            //generate a (random) span
            y1 = y1 - 2 + rand()%6; //-3 to 1
            y2 = y2 + rand()%2; //0 to 1
            //another bounds check (see if they cross and become outta whack
            if(y2<y1){
                break; //quit the while loop
            }
            
        }
    
        //upate colors
        p_col_in = warpColor(p_col_in);
        s_col_in = warpColor(s_col_in);
        
        printf("(%d,%d,%d)",p_col_in.r,p_col_in.g,p_col_in.b); 
    
    }
    
    
    
}
