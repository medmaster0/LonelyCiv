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
            x1 = x1 + 2 - rand()%6; //-1 to 1
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
            y1 = y1 + 2 - rand()%6; //-3 to 1
            y2 = y2 + rand()%2; //0 to 1
            //another bounds check (see if they cross and become outta whack
            if(y2<y1){
                break; //quit the while loop
            }
            
        }
    
        //upate colors
        p_col_in = warpColor(p_col_in);
        s_col_in = warpColor(s_col_in);
    
    }
    
    
    
}


//clouds

//this function places a cloud, row by row.
//The first (left) row is at x, and the top is at y. (floor is z)
//calculate a "span" that is carefully regulated
//The span can be no shorter than 3, and no longer than 8.
//After making 5 rows, it is eligble for tapering off (to not be too long, but sometimes that will happen)
void cloud_place(vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height, int x, int y, int z, SDL_Color p_col_in , SDL_Color s_col_in , int item_type){
    
    int span; //the size of a row of clouds
    int x1, x2, y1, y2 = 0;
    
    /////////////////////////
    //FIRST ROW
    x1 = x;
    x2 = x;
    y1 = y;
    y2 = y1 + 3;
    for(int i = x1; i <= x2; i++){
        for(int j = y1; j <= y2; j++){
            Item temp_cloud = Item(i,j, item_type, p_col_in, s_col_in); //place a single cloud at x,y,z
            temp_cloud.z = z; //also set the correct floor...
            map_scenery_bottom->at( (temp_cloud.z*map_width*map_height) + (temp_cloud.y*map_width) + temp_cloud.x).push_back(temp_cloud); // add it to global map
        }
    }
    /////////////////////////DONE FIRST ROW
    
    //START DRAWING MIDDLE FLUFFY PART
    //Update for next row
    x1 = x1 + 1; //move on to next row in cloud
    x2 = x2 + 1;
    y1 = y1 - 1; //make y length a little bigger
    y2 = y2 + 1;
    span = y2 - y1;
    while(true){
        
        //STOP CONDITION: Make sure, we don't have too manny rows.
        if(x1 - x > 8){
            break;
        }
        
        //Draw a row
        for(int i = x1; i <= x2; i++){
            for(int j = y1; j <= y2; j++){
                Item temp_cloud = Item(i,j, item_type, p_col_in, s_col_in); //place a single cloud at x,y,z
                temp_cloud.z = z; //also set the correct floor...
                map_scenery_bottom->at( (temp_cloud.z*map_width*map_height) + (temp_cloud.y*map_width) + temp_cloud.x).push_back(temp_cloud); // add it to global map
            }
        }//Done drawing row
        
        //Update for next row
        if(span < 5){ //span too little so need to get bigger
            y1 = y1 - rand()%2; //maybe get a little bigger.
            y2 = y2 + rand()%2;
        }else if(span > 10){ //span to large so need to get smaller
            y1 = y1 + rand()%2;
            y2 = y2 - rand()%2;
        }else{ //otherwise, it doesn't matter -> get bigger or smaller
            y1 = y1 + rand()%2 - rand()%2;
            y2 = y2 + rand()%2 - rand()%2;
        }
        x1 = x1 + 1;
        x2 = x2 + 1;
        //calculate new span
        span = y2 - y1;
        
        
    }
    //DONE FLUFFY PART
    ///////////////////////////
    
    //START DRAWING THE TAPER OFF -> convergence
    while(true){
        
        //STOP CONDITION-> if we've done tapered enough
        if(y2 - y1 <= 1 ){ //small enough to stop
            break;
        }
        
        //Draw a row
        for(int i = x1; i <= x2; i++){
            for(int j = y1; j <= y2; j++){
                Item temp_cloud = Item(i,j, item_type, p_col_in, s_col_in); //place a single cloud at x,y,z
                temp_cloud.z = z; //also set the correct floor...
                map_scenery_bottom->at( (temp_cloud.z*map_width*map_height) + (temp_cloud.y*map_width) + temp_cloud.x).push_back(temp_cloud); // add it to global map
            }
        }//Done drawing row
        
        //update points for next tow
        y1 = y1 + rand()%2;
        y2 = y2 - rand()%2;
        x1 = x1 + 1;
        x2 = x2 + 1;
        
        
    }
    
    
}

//same as cloud_place, but it also creates an "shadow" under each item places (a series of blank/alpha transparent tiles )
void cloud_place_shadow(vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height, int x, int y, int z, SDL_Color p_col_in , SDL_Color s_col_in , int item_type){
        
        int span; //the size of a row of clouds
        int x1, x2, y1, y2 = 0;
        
        /////////////////////////
        //FIRST ROW
        x1 = x;
        x2 = x;
        y1 = y;
        y2 = y1 + 3;
        for(int i = x1; i <= x2; i++){
            for(int j = y1; j <= y2; j++){
                Item temp_cloud = Item(i,j, item_type, p_col_in, s_col_in); //place a single cloud at x,y,z
                temp_cloud.z = z; //also set the correct floor...
                map_scenery_bottom->at( (temp_cloud.z*map_width*map_height) + (temp_cloud.y*map_width) + temp_cloud.x).push_back(temp_cloud); // add it to global map
                //SHADOW: Also create a bunch of blank/transparent items underneath
                int cur_z = z - 1;
                while(cur_z >= 0){ //As long as we have more cloud/shadow to draw
                    //Create item 334's (blank item)
                    int alpha_value = 150 - ((z - cur_z) * 5 ); //calculate the alpha value (more transparent as you move away/go down from cloud)
                    if(alpha_value<0)alpha_value = 0; //bounds check
                    SDL_Color shadow_col_p = {p_col_in.r, p_col_in.g, p_col_in.b, 255 };
                    shadow_col_p.a = alpha_value;
                    SDL_Color shadow_col_s = {0,0,0,0};
                    Item temp_shadow = Item(i, j, 334, shadow_col_p, shadow_col_s); //place a single cloud at x,y
                    temp_shadow.z = cur_z;
                    map_scenery_bottom->at( (temp_shadow.z*map_width*map_height) + (temp_shadow.y*map_width) + temp_shadow.x).push_back(temp_shadow); // add it to global map
                    
                    cur_z = cur_z - 1;
                }//End shadow
            }
        }
        /////////////////////////DONE FIRST ROW
        
        //START DRAWING MIDDLE FLUFFY PART
        //Update for next row
        x1 = x1 + 1; //move on to next row in cloud
        x2 = x2 + 1;
        y1 = y1 - 1; //make y length a little bigger
        y2 = y2 + 1;
        span = y2 - y1;
        while(true){
            
            //STOP CONDITION: Make sure, we don't have too manny rows.
            if(x1 - x > 8){
                break;
            }
            
            //Draw a row
            for(int i = x1; i <= x2; i++){
                for(int j = y1; j <= y2; j++){
                    Item temp_cloud = Item(i,j, item_type, p_col_in, s_col_in); //place a single cloud at x,y,z
                    temp_cloud.z = z; //also set the correct floor...
                    map_scenery_bottom->at( (temp_cloud.z*map_width*map_height) + (temp_cloud.y*map_width) + temp_cloud.x).push_back(temp_cloud); // add it to global map
                    //SHADOW: Also create a bunch of blank/transparent items underneath
                    int cur_z = z - 1;
                    while(cur_z >= 0){ //As long as we have more cloud/shadow to draw
                        //Create item 334's (blank item)
                        int alpha_value = 150 - ((z - cur_z) * 5 ); //calculate the alpha value (more transparent as you move away/go down from cloud)
                        if(alpha_value<0)alpha_value = 0; //bounds check
                        SDL_Color shadow_col_p = {p_col_in.r, p_col_in.g, p_col_in.b, 255 };
                        shadow_col_p.a = alpha_value;
                        SDL_Color shadow_col_s = {0,0,0,0};
                        Item temp_shadow = Item(i, j, 334, shadow_col_p, shadow_col_s); //place a single cloud at x,y
                        temp_shadow.z = cur_z;
                        map_scenery_bottom->at( (temp_shadow.z*map_width*map_height) + (temp_shadow.y*map_width) + temp_shadow.x).push_back(temp_shadow); // add it to global map
                        
                        cur_z = cur_z - 1;
                    }//End shadow
                    
                    
                }
            }//Done drawing row
            
            //Update for next row
            if(span < 5){ //span too little so need to get bigger
                y1 = y1 - rand()%2; //maybe get a little bigger.
                y2 = y2 + rand()%2;
            }else if(span > 10){ //span to large so need to get smaller
                y1 = y1 + rand()%2;
                y2 = y2 - rand()%2;
            }else{ //otherwise, it doesn't matter -> get bigger or smaller
                y1 = y1 + rand()%2 - rand()%2;
                y2 = y2 + rand()%2 - rand()%2;
            }
            x1 = x1 + 1;
            x2 = x2 + 1;
            //calculate new span
            span = y2 - y1;
            
            
        }
        //DONE FLUFFY PART
        ///////////////////////////
        
        //START DRAWING THE TAPER OFF -> convergence
        while(true){
            
            //STOP CONDITION-> if we've done tapered enough
            if(y2 - y1 <= 1 ){ //small enough to stop
                break;
            }
            
            //Draw a row
            for(int i = x1; i <= x2; i++){
                for(int j = y1; j <= y2; j++){
                    Item temp_cloud = Item(i,j, item_type, p_col_in, s_col_in); //place a single cloud at x,y,z
                    temp_cloud.z = z; //also set the correct floor...
                    map_scenery_bottom->at( (temp_cloud.z*map_width*map_height) + (temp_cloud.y*map_width) + temp_cloud.x).push_back(temp_cloud); // add it to global map
                    //SHADOW: Also create a bunch of blank/transparent items underneath
                    int cur_z = z - 1;
                    while(cur_z >= 0){ //As long as we have more cloud/shadow to draw
                        //Create item 334's (blank item)
                        int alpha_value = 150 - ((z - cur_z) * 5 ); //calculate the alpha value (more transparent as you move away/go down from cloud)
                        if(alpha_value<0)alpha_value = 0; //bounds check
                        SDL_Color shadow_col_p = {p_col_in.r, p_col_in.g, p_col_in.b, 255 };
                        shadow_col_p.a = alpha_value;
                        SDL_Color shadow_col_s = {0,0,0,0};
                        Item temp_shadow = Item(i, j, 334, shadow_col_p, shadow_col_s); //place a single cloud at x,y
                        temp_shadow.z = cur_z;
                        map_scenery_bottom->at( (temp_shadow.z*map_width*map_height) + (temp_shadow.y*map_width) + temp_shadow.x).push_back(temp_shadow); // add it to global map
                        
                        cur_z = cur_z - 1;
                    }//End shadow
                }
            }//Done drawing row
            
            //update points for next row
            y1 = y1 + rand()%2;
            y2 = y2 - rand()%2;
            x1 = x1 + 1;
            x2 = x2 + 1;
            
        }
}

//CLOUD CLASZ
//Cloud class contains a recangle specifying where a cloud is
//Also responsible for moving cloud
Cloud::Cloud(int in_xmin, int in_xmax, int in_ymin, int in_ymax, int in_z){
    xmin = in_xmin;
    xmax = in_xmax;
    ymin = in_ymin;
    ymax = in_ymax;
    z = in_z;
    hasCreature = false;
}

//Generates a Cloud and records it's extreme points (for a rectangle perimeter)
void Cloud::cloud_place_shadow(vector<vector<Item>>* map_scenery_bottom, bool* block_map, int map_width, int map_height, int in_x, int in_y, int in_z, SDL_Color p_col_in , SDL_Color s_col_in , int item_type){
    
//    int temp_xmin, temp_xmax, temp_ymin, temp_ymax; //keep track of the extreme points
//    temp_xmin = in_x;
//    temp_xmax = in_x;
//    temp_ymin = in_y;
//    temp_ymax = in_y;
    xmin = in_x;
    xmax = in_x;
    ymin = in_y;
    ymax = in_y;
    z = in_z;
    
    int span; //the size of a row of clouds
    int x1, x2, y1, y2 = 0;
    
    /////////////////////////
    //FIRST ROW
    x1 = in_x;
    x2 = in_x;
    y1 = in_y;
    y2 = y1 + 3;
    for(int i = x1; i <= x2; i++){
        for(int j = y1; j <= y2; j++){
            Item temp_cloud = Item(i,j, item_type, p_col_in, s_col_in); //place a single cloud at x,y,z
            temp_cloud.z = in_z; //also set the correct floor...
            map_scenery_bottom->at( (temp_cloud.z*map_width*map_height) + (temp_cloud.y*map_width) + temp_cloud.x).push_back(temp_cloud); // add it to global map
            //SHADOW: Also create a bunch of blank/transparent items underneath
            int cur_z = in_z - 1;
            while(cur_z >= 0){ //As long as we have more cloud/shadow to draw
                //Create item 334's (blank item)
                int alpha_value = 150 - ((in_z - cur_z) * 5 ); //calculate the alpha value (more transparent as you move away/go down from cloud)
                if(alpha_value<0)alpha_value = 0; //bounds check
                SDL_Color shadow_col_p = {p_col_in.r, p_col_in.g, p_col_in.b, 255 };
                shadow_col_p.a = alpha_value;
                SDL_Color shadow_col_s = {0,0,0,0};
                Item temp_shadow = Item(i, j, 334, shadow_col_p, shadow_col_s); //place a single cloud at x,y
                temp_shadow.z = cur_z;
                map_scenery_bottom->at( (temp_shadow.z*map_width*map_height) + (temp_shadow.y*map_width) + temp_shadow.x).push_back(temp_shadow); // add it to global map
                
                cur_z = cur_z - 1;
            }//End shadow
        }
    }
    /////////////////////////DONE FIRST ROW
    
    //START DRAWING MIDDLE FLUFFY PART
    //Update for next row
    x1 = x1 + 1; //move on to next row in cloud
    x2 = x2 + 1;
    y1 = y1 - 1; //make y length a little bigger
    y2 = y2 + 1;
    span = y2 - y1;
    while(true){
        
        //STOP CONDITION: Make sure, we don't have too manny rows.
        if(x1 - in_x > 8){
            break;
        }
        
        //RECTANGLE MEASURE
        if( ymax <= y2 ){ ymax = y2; }
        if( ymin >= y1 ){ ymin = y1; }
        
        //Draw a row
        for(int i = x1; i <= x2; i++){
            for(int j = y1; j <= y2; j++){
                Item temp_cloud = Item(i,j, item_type, p_col_in, s_col_in); //place a single cloud at x,y,z
                temp_cloud.z = z; //also set the correct floor...
                map_scenery_bottom->at( (temp_cloud.z*map_width*map_height) + (temp_cloud.y*map_width) + temp_cloud.x).push_back(temp_cloud); // add it to global map
                //SHADOW: Also create a bunch of blank/transparent items underneath
                int cur_z = z - 1;
                while(cur_z >= 0){ //As long as we have more cloud/shadow to draw
                    //Create item 334's (blank item)
                    int alpha_value = 150 - ((z - cur_z) * 5 ); //calculate the alpha value (more transparent as you move away/go down from cloud)
                    if(alpha_value<0)alpha_value = 0; //bounds check
                    SDL_Color shadow_col_p = {p_col_in.r, p_col_in.g, p_col_in.b, 255 };
                    shadow_col_p.a = alpha_value;
                    SDL_Color shadow_col_s = {0,0,0,0};
                    Item temp_shadow = Item(i, j, 334, shadow_col_p, shadow_col_s); //place a single cloud at x,y
                    temp_shadow.z = cur_z;
                    map_scenery_bottom->at( (temp_shadow.z*map_width*map_height) + (temp_shadow.y*map_width) + temp_shadow.x).push_back(temp_shadow); // add it to global map
                    
                    cur_z = cur_z - 1;
                }//End shadow
                
                
            }
        }//Done drawing row
        
        //Update for next row
        if(span < 5){ //span too little so need to get bigger
            y1 = y1 - rand()%2; //maybe get a little bigger.
            y2 = y2 + rand()%2;
        }else if(span > 10){ //span to large so need to get smaller
            y1 = y1 + rand()%2;
            y2 = y2 - rand()%2;
        }else{ //otherwise, it doesn't matter -> get bigger or smaller
            y1 = y1 + rand()%2 - rand()%2;
            y2 = y2 + rand()%2 - rand()%2;
        }
        x1 = x1 + 1;
        x2 = x2 + 1;
        //calculate new span
        span = y2 - y1;
        
        
    }
    //DONE FLUFFY PART
    ///////////////////////////
    
    //START DRAWING THE TAPER OFF -> convergence
    while(true){
        
        //STOP CONDITION-> if we've done tapered enough
        if(y2 - y1 <= 1 ){ //small enough to stop
            break;
        }
        
        //RECTANGLE MEASURE
        xmax = x2;
        
        //Draw a row
        for(int i = x1; i <= x2; i++){
            for(int j = y1; j <= y2; j++){
                Item temp_cloud = Item(i,j, item_type, p_col_in, s_col_in); //place a single cloud at x,y,z
                temp_cloud.z = z; //also set the correct floor...
                map_scenery_bottom->at( (temp_cloud.z*map_width*map_height) + (temp_cloud.y*map_width) + temp_cloud.x).push_back(temp_cloud); // add it to global map
                //SHADOW: Also create a bunch of blank/transparent items underneath
                int cur_z = z - 1;
                while(cur_z >= 0){ //As long as we have more cloud/shadow to draw
                    //Create item 334's (blank item)
                    int alpha_value = 150 - ((z - cur_z) * 5 ); //calculate the alpha value (more transparent as you move away/go down from cloud)
                    if(alpha_value<0)alpha_value = 0; //bounds check
                    SDL_Color shadow_col_p = {p_col_in.r, p_col_in.g, p_col_in.b, 255 };
                    shadow_col_p.a = alpha_value;
                    SDL_Color shadow_col_s = {0,0,0,0};
                    Item temp_shadow = Item(i, j, 334, shadow_col_p, shadow_col_s); //place a single cloud at x,y
                    temp_shadow.z = cur_z;
                    map_scenery_bottom->at( (temp_shadow.z*map_width*map_height) + (temp_shadow.y*map_width) + temp_shadow.x).push_back(temp_shadow); // add it to global map
                    
                    cur_z = cur_z - 1;
                }//End shadow
            }
        }//Done drawing row
        
        //update points for next row
        y1 = y1 + rand()%2;
        y2 = y2 - rand()%2;
        x1 = x1 + 1;
        x2 = x2 + 1;
        
    }//End TAPER off
    
    printf("CLOUD:%d,%d,%d,%d,%d\n",xmin, xmax, ymin, ymax, z);
    
}






