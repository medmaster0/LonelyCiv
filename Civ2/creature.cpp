//
//  creature.cpp
//  Civ2
//
//  Created by Medz on 8/17/17.
//  Copyright © 2017 Medz. All rights reserved.
//

#include "creature.hpp"
#include "dancez.hpp"
#include "Item.hpp"
#include <vector>
#include <string>
#include <iostream>
#include "med_algo.hpp"
#include "story.hpp"

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>

using namespace std;

//////////////////
//SPRITE CLASS
//Constructor
Sprite::Sprite(int xp, int yp)
{
    //Initialize
    name = genName();
    name = giveMonicker(name); //maybe give Monicker
    primTexture = NULL;
    secoTexture = NULL;
    mWidth = 0;
    mHeight = 0;
    hat = nullptr;
    staff = nullptr;
    light = nullptr;
    x = xp;
    y = yp;
    z = 0;
    prev_x = 0;
    prev_y = 0;
    prev_z = 0;
    r = rand() %255;
    g = rand() %255;
    b = rand() %255;
    faveColor = {static_cast<Uint8>(rand() %255), static_cast<Uint8>(rand() %255), static_cast<Uint8>(rand() %255)};
    faveColor2 = {static_cast<Uint8>(rand() %255), static_cast<Uint8>(rand() %255), static_cast<Uint8>(rand() %255)};
    zodiac_sign = rand()%12; //assign a random zodiac sign
    zodiac_element = find_zodiac_element(zodiac_sign); //determine the element, based on the zodiac sign
    inThread = false;
    move_timer = 0; //keeps track of SDL_GetTicks() for last movement
    move_speed = 2.0; //how many tiles to move per 1 second
    isNeededByThread = false; //prevents scheduler from starting another thread
    task_status = "none";
    
}
//Destructor
void Sprite::free(){
    if (secoTexture!=NULL){
        SDL_DestroyTexture( secoTexture );
    }
    if (primTexture!=NULL){
        SDL_DestroyTexture( primTexture );
    }
}
//Load a texture from file for the sprite
bool Sprite::loadFromFile( std::string path1, std::string path2, int w, int h )
{
    primTexture = loadTexture(path1);
    secoTexture = loadTexture(path2);
    SDL_SetTextureColorMod( secoTexture, r, g, b); //modulate color, update to match the new one
    mWidth = w;
    mHeight = h;
    return primTexture != NULL;
    
}
//changes the primary color of the spirte
void Sprite::changePrimColor(SDL_Color new_col){
    SDL_SetTextureColorMod( primTexture, new_col.r, new_col.g, new_col.b); //modulate color, update to match the new one
    return;
}
//changes the secondary color of the spirte
void Sprite::changeSecoColor(SDL_Color new_col){
    SDL_SetTextureColorMod( secoTexture, new_col.r, new_col.g, new_col.b); //modulate color, update to match the new one
    return;
}
//Draws the sprite to the screen at a specific MAP COORDS
//This version of draw places the items left and right for a pose
void Sprite::draw(int in_x, int in_y, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s, SDL_Texture** item_tiles_t){
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { in_x*16, in_y*16, mWidth, mHeight };
    SDL_Rect* clip = NULL;
    SDL_RenderCopy( gRenderer, primTexture, clip, &renderQuad );//Render to screen
    SDL_RenderCopy( gRenderer, secoTexture, clip, &renderQuad );//Render to screen
    
    if(hat != nullptr){
        hat->y = in_y; //move the hat to the right place
        hat->x = in_x;
        hat->draw(gRenderer, item_tiles_p, item_tiles_s);
    }
    
    if(staff != nullptr){
        staff->y = in_y ;
        staff->x = in_x-1;
        staff->draw(gRenderer, item_tiles_p, item_tiles_s);
    }
    
    if(light != nullptr){
        light->y = in_y ;
        light->x = in_x+1;
        light->draw(gRenderer, item_tiles_p, item_tiles_s);
    }
    
}

//draws the sprite to the screen at specific map coords, but items "flow" with creature movement (considers prev location)
void Sprite::draw_movement(int at_x, int at_y, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s){
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { at_x*16, at_y*16, mWidth, mHeight };
    SDL_Rect* clip = NULL;
    SDL_RenderCopy( gRenderer, primTexture, clip, &renderQuad );//Render to screen
    SDL_RenderCopy( gRenderer, secoTexture, clip, &renderQuad );//Render to screen
    
    //But we have to put the items in the correct position...
    //Basically does everything that drawHat, drawStaff, drawLight does, but a translated verrsion
    // y's and x's become at_y's and at_x's
    //prev_y becomes at_y + (prev_y - y)
    //prev_x becomes at_x + (prev_x - x)
    
    //DRAW STAFF
    //the staff should flow behind cre so we need to calculate where that's at on the translated image
    if(staff != nullptr){
        staff->y = at_y + (prev_y-y); //Apply the different between y and previous y to the new location
        staff->x = at_x + (prev_x-x); //Apply the different between x and previous x to the new location
        staff->draw(gRenderer, item_tiles_p, item_tiles_s);
    }
    
    //DRAW LIGHT
    if(light != nullptr){
        
        //If has staff, then need to put candle on other side
        if(staff!=nullptr){
            if(prev_y < y && prev_x == x){
                light->y = at_y+1;
                light->x = at_x;
            }
            if(prev_y > y && prev_x == x){
                light->y = at_y-1;
                light->x = at_x;
            }
            if(prev_x < x && prev_y == y){
                light->y = at_y;
                light->x = at_x+1;
            }
            if(prev_x > x && prev_y == y){
                light->y = at_y;
                light->x = at_x-1;
            }
        }else{
            light->y = at_y + (prev_y-y); //Apply the different between y and previous y to the new location
            light->x = at_x + (prev_x-x); //Apply the different between x and previous x to the new location
        }
        
        light->draw(gRenderer, item_tiles_p, item_tiles_s);
    }
    
    //DRAW HAT
    if(hat != nullptr){
        hat->y = at_y; //move the hat to the right place
        hat->x = at_x;
        hat->draw(gRenderer, item_tiles_p, item_tiles_s);
    }
    
}

//draws the sprite's items to the screen at specific map coords, but items "flow" with creature movement (considers prev location)
void Sprite::draw_movement_items(int at_x, int at_y, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s){
//    //Set rendering space and render to screen
//    SDL_Rect renderQuad = { at_x*16, at_y*16, mWidth, mHeight };
//    SDL_Rect* clip = NULL;
//    SDL_RenderCopy( gRenderer, primTexture, clip, &renderQuad );//Render to screen
//    SDL_RenderCopy( gRenderer, secoTexture, clip, &renderQuad );//Render to screen
    
    //But we have to put the items in the correct position...
    //Basically does everything that drawHat, drawStaff, drawLight does, but a translated verrsion
    // y's and x's become at_y's and at_x's
    //prev_y becomes at_y + (prev_y - y)
    //prev_x becomes at_x + (prev_x - x)
    
    //DRAW HAT
    if(hat != nullptr){
        hat->y = at_y; //move the hat to the right place
        hat->x = at_x;
        hat->draw(gRenderer, item_tiles_p, item_tiles_s);
    }
    
    //DRAW STAFF
    //the staff should flow behind cre so we need to calculate where that's at on the translated image
    if(staff != nullptr){
        staff->y = at_y + (prev_y-y); //Apply the different between y and previous y to the new location
        staff->x = at_x + (prev_x-x); //Apply the different between x and previous x to the new location
        staff->draw(gRenderer, item_tiles_p, item_tiles_s);
    }
    
    //DRAW LIGHT
    if(light != nullptr){
        
        //If has staff, then need to put candle on other side
        if(staff!=nullptr){
            if(prev_y < y && prev_x == x){
                light->y = at_y+1;
                light->x = at_x;
            }
            if(prev_y > y && prev_x == x){
                light->y = at_y-1;
                light->x = at_x;
            }
            if(prev_x < x && prev_y == y){
                light->y = at_y;
                light->x = at_x+1;
            }
            if(prev_x > x && prev_y == y){
                light->y = at_y;
                light->x = at_x-1;
            }
        }else{
            light->y = at_y + (prev_y-y); //Apply the different between y and previous y to the new location
            light->x = at_x + (prev_x-x); //Apply the different between x and previous x to the new location
        }
        
        light->draw(gRenderer, item_tiles_p, item_tiles_s);
    }
    
}

//draws the sprite to the screen at specific map coords, but items are side by side creature (considers prev location) FOR BALCONY VIEW
void Sprite::draw_forward_pose(int at_x, int at_y, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s){
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { at_x*16, at_y*16, mWidth, mHeight };
    SDL_Rect* clip = NULL;
    SDL_RenderCopy( gRenderer, primTexture, clip, &renderQuad );//Render to screen
    SDL_RenderCopy( gRenderer, secoTexture, clip, &renderQuad );//Render to screen
    
    //But we have to put the items in the correct position...
    //Basically does everything that drawHat, drawStaff, drawLight does, but a translated verrsion
    // y's and x's become at_y's and at_x's
    //prev_y becomes at_y + (prev_y - y)
    //prev_x becomes at_x + (prev_x - x)
    
//    //DRAW STAFF
//    //the staff should flow behind cre so we need to calculate where that's at on the translated image
//    if(staff != nullptr){
//        staff->y = at_y;
//        staff->x = at_x + (prev_x-x); //Apply the different between x and previous x to the new location
//        if(staff->x==at_x){staff->x=staff->x+1;} //make sure it doesn't block the creature though
//        staff->draw(gRenderer, item_tiles_p, item_tiles_s);
//    }
//
//    //DRAW LIGHT
//    if(light != nullptr){
//
//        //If has staff, then need to put candle on other side
//        if(staff!=nullptr){
//
//            //Put it in the opposite position of staff
//            light->y = at_y;
//            light->x = at_x - (prev_x-x);
//            if(light->x==at_x){light->x=light->x-1;} //make sure it doesn't block the creature though
//
//
////            if(prev_x<at_x){
////                light->x = at_x + 1;
////            }else{
////                light->x = at_x - 1;
////            }
//
//        }else{
//            //Put it in the same position staff would be in
//            light->y = at_y;
//            light->x = at_x + (prev_x-x); //Apply the different between x and previous x to the new location
//            if(light->x==at_x){light->x=light->x+1;} //make sure it doesn't block the creature though
//        }
//
//        light->draw(gRenderer, item_tiles_p, item_tiles_s);
//    }
//
//    //DRAW HAT
//    if(hat != nullptr){
//        hat->y = at_y; //move the hat to the right place
//        hat->x = at_x;
//        hat->draw(gRenderer, item_tiles_p, item_tiles_s);
//    }
    
}

//draws the sprite's items to the screen at specific map coords, but items are side by side (considers prev location) FOR BALCONY VIEW
void Sprite::draw_forward_pose_items(int at_x, int at_y, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s, int alpha_mod){
    //    //Set rendering space and render to screen
    //    SDL_Rect renderQuad = { at_x*16, at_y*16, mWidth, mHeight };
    //    SDL_Rect* clip = NULL;
    //    SDL_RenderCopy( gRenderer, primTexture, clip, &renderQuad );//Render to screen
    //    SDL_RenderCopy( gRenderer, secoTexture, clip, &renderQuad );//Render to screen
    
    //But we have to put the items in the correct position...
    //Basically does everything that drawHat, drawStaff, drawLight does, but a translated verrsion
    // y's and x's become at_y's and at_x's
    //prev_y becomes at_y + (prev_y - y)
    //prev_x becomes at_x + (prev_x - x)
    
    //DRAW HAT
    if(hat != nullptr){
        hat->y = at_y; //move the hat to the right place
        hat->x = at_x;
        hat->draw(gRenderer, item_tiles_p, item_tiles_s, alpha_mod);
    }
    
    //DRAW STAFF
    //the staff should flow behind cre so we need to calculate where that's at on the translated image
    if(staff != nullptr){
        staff->y = at_y;
        staff->x = at_x + (prev_x-x); //Apply the different between x and previous x to the new location
        if(staff->x==at_x){staff->x=staff->x+1;} //make sure it doesn't block the creature though
        staff->draw(gRenderer, item_tiles_p, item_tiles_s, alpha_mod);
    }
    
    //DRAW LIGHT
    if(light != nullptr){
        
        //If has staff, then need to put candle on other side
        if(staff!=nullptr){
            
            //Put it in the opposite position of staff
            light->y = at_y;
            light->x = at_x - (prev_x-x);
            if(light->x==at_x){light->x=light->x-1;} //make sure it doesn't block the creature though
            
//            light->y = at_y;
//            if(prev_x<at_x){
//                light->x = at_x + 1;
//            }else{
//                light->x = at_x - 1;
//            }
            
        }else{
            //Put it in the same position staff would be in
            light->y = at_y;
            light->x = at_x + (prev_x-x); //Apply the different between x and previous x to the new location
            if(light->x==at_x){light->x=light->x+1;} //make sure it doesn't block the creature though
        }
        
        light->draw(gRenderer, item_tiles_p, item_tiles_s, alpha_mod);
    }
    
}

//Move Down (1 step)
void Sprite::moveDown(){
    prev_y = y;
    prev_x = x;
    prev_z = z;
    y = y +1;
    return;
}
//Move Up (1 step)
void Sprite::moveUp(){
    prev_y = y;
    prev_x = x;
    prev_z = z;
    y = y -1;
    return;
}
//Move Right (1 step)
void Sprite::moveRight(){
    prev_x = x;
    prev_y = y;
    prev_z = z;
    x=x+1;;
    return;
}
//Move Left (1 step)
void Sprite::moveLeft(){
    prev_x = x;
    prev_y = y;
    prev_z = z;
    x=x-1;;
    return;
}
//Move TO
void Sprite::moveTo(int x1, int y1){
    prev_x = x;
    prev_y = y;
    prev_z = z;
    x = x1;
    y = y1;
    
//    //Also update position of sprite's items
//    if(hat != nullptr){
//        hat->y = y; //move the hat to the right place
//        hat->x = x;
//    }
//    if(staff != nullptr){
//        staff->y = prev_y;
//        staff->x = prev_x;
//    }
//    if(light != nullptr){
//        //If has staff, then need to put candle on other side
//        if(staff!=nullptr){
//            if(prev_y < y && prev_x == x){
//                light->y = y+1;
//                light->x = x;
//            }
//            if(prev_y > y && prev_x == x){
//                light->y = y-1;
//                light->x = x;
//            }
//            if(prev_x < x && prev_y == y){
//                light->y = y;
//                light->x = x+1;
//            }
//            if(prev_x > x && prev_y == y){
//                light->y = y;
//                light->x = x-1;
//            }
//        }else{
//            light->y = prev_y;
//            light->x = prev_x;
//        }
//    }
//    //End updating sprite's items
    
}

//Sprite's random path will be dance pattern
void Sprite::randomDance(){
    vector<int> step; //a temporary step for the path
    int curx = x; //used as a reference for finding next stepp
    int cury = y; //y'know
    
    //get a new dance if need be
    if(path.empty()){
        vector<vector<int>> routine = generateChaCha();
        for(int i = 0; i < routine.size(); i++){
            curx = curx+routine[i][0];
            cury = cury+routine[i][1];
            step = {curx,cury};  //the routine merely says where we go *relative* to where we are
            path.push_back(step);
            
        }
    }
    
    //Now move to next step in the path (if move Period has been reached
    vector<int> next = path.front();
    x = next[0];
    y = next[1];
    path.erase(path.begin());
    SDL_Delay(100);
    
}

//Draw the sprite's inventory
void Sprite::drawInventory(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s){
    for(int i = 0; i < inventory.size(); i++){
        inventory[i].x = x;
        inventory[i].y = y;
        inventory[i].draw(gRenderer, item_tiles_p, item_tiles_s);
    }
}

//Draw the sprite's hat
void Sprite::drawHat(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s ){
    
    if(hat != nullptr){
        hat->y = y; //move the hat to the right place
        hat->x = x;
        hat->draw(gRenderer, item_tiles_p, item_tiles_s);
    }
    
}

//Draw the sprite's staff
void Sprite::drawStaff(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s ){
    
    if(staff != nullptr){
        staff->y = prev_y;
        staff->x = prev_x;
        staff->draw(gRenderer, item_tiles_p, item_tiles_s);
    }
    
}

//Draw the sprite's light
void Sprite::drawLight(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s ){
    
    if(light != nullptr){
        
        //If has staff, then need to put candle on other side
        if(staff!=nullptr){
            if(prev_y < y && prev_x == x){
                light->y = y+1;
                light->x = x;
            }
            if(prev_y > y && prev_x == x){
                light->y = y-1;
                light->x = x;
            }
            if(prev_x < x && prev_y == y){
                light->y = y;
                light->x = x+1;
            }
            if(prev_x > x && prev_y == y){
                light->y = y;
                light->x = x-1;
            }
        }else{
            light->y = prev_y;
            light->x = prev_x;
        }
        
        light->draw(gRenderer, item_tiles_p, item_tiles_s);
    }
    
}

//END OF SPRITE CLASS
///////////////////////////

//SHROOM CLASS
Shroom::Shroom(int tx, int ty) : Sprite(tx, ty){
    
    //Also, assign Shroom some random recipe resources
    resource_list.push_back(Resource(0));
    resource_list.push_back(Resource(1));
    resource_list.push_back(Resource(2));
    resource_list.push_back(Resource(3));
    resource_list.push_back(Resource(4));
    
    //Also, assign Shroom some random recipes (made out of resources??)
    recipe_list.push_back(Recipe(0, resource_list[2], resource_list[0]));  //create a recipe, with the proper resources
    recipe_list.push_back(Recipe(1, resource_list[1], resource_list[3]));  //create a recipe, with the proper resources
    recipe_list.push_back(Recipe(2, resource_list[4], resource_list[3]));  //create a recipe, with the proper resources
    recipe_list.push_back(Recipe(3, resource_list[3], resource_list[4]));  //create a recipe, with the proper resources
    
}


//Create a display window that prints out the status of a creature
SDL_Surface* status_display_surface;
SDL_Texture* status_display_texture;
extern TTF_Font* font1; //item font
SDL_Color status_font_clr = {255,255,255}; //color of font
int status_texW = 650;//constants used in displaying fonts
int status_texH = 0;//constants used in displaying fonts
void displayStatus(Sprite spr1, SDL_Renderer* gRenderer, int SCREEN_HEIGHT , SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s, SDL_Texture** misc_tiles){
    string status_text = "                  Status of " + spr1.name; //Char buffer that is displayed
    //Blackout Box {x,y,w,h}
    SDL_Rect r = {0,0,160,112};
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderFillRect(gRenderer, &r);
    status_display_surface = TTF_RenderText_Blended_Wrapped(font1, status_text.c_str(), status_font_clr, 155);
    status_display_texture = SDL_CreateTextureFromSurface(gRenderer, status_display_surface);
    //create text rect (text box)
    SDL_QueryTexture(status_display_texture, NULL, NULL, &status_texW, &status_texH); //gets the dimenstions of the font/text
    SDL_Rect dstrect = {0, 0, status_texW, status_texH }; //so we can make the proper rect to dispay it
    SDL_RenderCopy(gRenderer, status_display_texture, NULL, &dstrect); //write
    //FREE MEMORY
    SDL_DestroyTexture(status_display_texture);
    SDL_FreeSurface(status_display_surface);
    //FINISH PUtting the Title
    
    //Write out what the CURRENT TASK is
    status_display_surface = TTF_RenderText_Blended_Wrapped(font1, ("Task: " + spr1.task_status).c_str(), status_font_clr, 155);
    status_display_texture = SDL_CreateTextureFromSurface(gRenderer, status_display_surface);
    //create text rect (text box)
    SDL_QueryTexture(status_display_texture, NULL, NULL, &status_texW, &status_texH); //gets the dimenstions of the font/text
    dstrect = {0, 5*16, status_texW, status_texH }; //so we can make the proper rect to dispay it
    SDL_RenderCopy(gRenderer, status_display_texture, NULL, &dstrect); //write
    //FREE MEMORY
    SDL_DestroyTexture(status_display_texture);
    SDL_FreeSurface(status_display_surface);
    //FINISH Writing task stauts

    //Show Fave Colors
    SDL_Color status_color1 = spr1.faveColor; //The prim color of the cre
    SDL_Color status_color2 = spr1.faveColor2; //The seco color of the cre
    //FIRST
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND); //temporarily allow alpha
    SDL_Rect colR ={ 0 , 3*16 , 7 , status_texH/2 };
    SDL_SetRenderDrawColor( gRenderer, status_color1.r , status_color1.g, status_color1.b , 255); //color of box
    SDL_RenderFillRect( gRenderer, &colR ); //copy box to renderer
    //SECOND
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND); //temporarily allow alpha
    colR ={ 7 , 3*16 , 7 , status_texH/2 };
    SDL_SetRenderDrawColor( gRenderer, status_color2.r , status_color2.g, status_color2.b , 255); //color of box
    SDL_RenderFillRect( gRenderer, &colR ); //copy box to renderer
    
    //Draw creature there as well
    spr1.draw(2, 3, item_tiles_p, item_tiles_s);
    
    //Draw inventory underneath
    for(int i = 0; i<spr1.inventory.size(); i++){
        spr1.inventory[i].x = 1+i;
        spr1.inventory[i].y = 4;
        spr1.inventory[i].draw(gRenderer, item_tiles_p, item_tiles_s);
    }
    
    //Draw zodiac symbol
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { 4*16, 3*16, 16, 16 };
    SDL_Rect* clip = NULL;
    SDL_RenderCopy( gRenderer, misc_tiles[spr1.zodiac_sign], clip, &renderQuad );//Render to screen
    //draw balance status symbol
    renderQuad = {7*16, 2*16, 16,16};
    SDL_RenderCopy( gRenderer, misc_tiles[12], clip, &renderQuad );//Render to screen
    //draw health status symbol
    renderQuad = {7*16, 3*16, 16,16};
    SDL_RenderCopy( gRenderer, misc_tiles[13], clip, &renderQuad );//Render to screen
    //draw happiness status symbol
    renderQuad = {7*16, 4*16, 16,16};
    SDL_RenderCopy( gRenderer, misc_tiles[14], clip, &renderQuad );//Render to screen

    
    //Draw zodiac element
    renderQuad = {5*16, 3*16, 16,16};
    switch(spr1.zodiac_element){
        case 1:
            SDL_RenderCopy( gRenderer, misc_tiles[15], clip, &renderQuad );//Swords/Air signs
            break;
        case 2:
            SDL_RenderCopy( gRenderer, misc_tiles[16], clip, &renderQuad );//Wands/Fire
            break;
        case 3:
            SDL_RenderCopy( gRenderer, misc_tiles[17], clip, &renderQuad );//Cups/Water
            break;
        case 4:
            SDL_RenderCopy( gRenderer, misc_tiles[18], clip, &renderQuad );//Pentacles/Earth
            break;
            
    }
    
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE); //turn alpha back off
    
}

//draw's the sprites inventory in piles on the ground
//Takes the sprite's inventory, and displays it on the ground
// Along a recangular grid (of size NxN) centered on sprite's positon
//Mainly useful for a stationary creature with many items (shroom)
// Draws the inventory backwards -> last is the first one to be drawn....
void drawHorde(Sprite* spr1, SDL_Renderer* gRenderer, int draw_map_x, int draw_map_y, int display_n, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s, SDL_Texture** item_tiles_t){
    
    int x1,y1; //the left corner of the display grid
    
    //Determine grid bounds (center on Sprite's position
    if(display_n%2 == 0){ //if even n
        x1 = spr1->x - ((display_n/2) - 1);
        y1 = spr1->y - ((display_n/2) - 1);
    }else{
        x1 = spr1->x - (display_n/2);
        y1 = spr1->y - (display_n/2);
    }
    
    //Also apply the draw_map positioning
    x1 = x1 - draw_map_x;
    y1 = y1 - draw_map_y; 
    
    //Now cycle throughout BOTH the grid and the items in inventory...
    int inventory_left = spr1->inventory.size(); //the amount of items in inventory left to draw
    for(int i = 0; i < display_n; i++){
        for(int j = 0; j < display_n; j++){
            
            if(inventory_left>0){ //if there are items left to draw...
                spr1->inventory[inventory_left-1].draw(x1+i, y1+j, gRenderer, item_tiles_p, item_tiles_s, item_tiles_t); //then draw them, dummy
                inventory_left = inventory_left - 1; //Also descrease counter
            }else{
                break;
            }
        }
    }
    
}












