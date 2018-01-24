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
    inThread = false; 
    
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
//Draws the sprite to screen using private MAP COORDS
void Sprite::draw()
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x*16, y*16, mWidth, mHeight };
    SDL_Rect* clip = NULL;
    SDL_RenderCopy( gRenderer, primTexture, clip, &renderQuad );//Render to screen
    SDL_RenderCopy( gRenderer, secoTexture, clip, &renderQuad );//Render to screen
    
}
//Move Down (1 step)
void Sprite::moveDown(){
    prev_y = y;
    prev_x = x;
    y = y +1;
    return;
}
//Move Up (1 step)
void Sprite::moveUp(){
    prev_y = y;
    prev_x = x;
    y = y -1;
    return;
}
//Move Right (1 step)
void Sprite::moveRight(){
    prev_x = x;
    prev_y = y;
    x=x+1;;
    return;
}
//Move Left (1 step)
void Sprite::moveLeft(){
    prev_x = x;
    prev_y = y;
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

//Create a display window that prints out the status of a creature
SDL_Surface* status_display_surface;
SDL_Texture* status_display_texture;
extern TTF_Font* font1; //item font
SDL_Color status_font_clr = {255,255,255}; //color of font
int status_texW = 250;//constants used in displaying fonts
int status_texH = 0;//constants used in displaying fonts
void displayStatus(Sprite spr1, SDL_Renderer* gRenderer, int SCREEN_HEIGHT ){
    string status_text = "Status of " + spr1.name; //Char buffer that is displayed
    //Blackout Box {x,y,w,h}
    SDL_Rect r = {0,0,250,150};
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderFillRect(gRenderer, &r);
    status_display_surface = TTF_RenderText_Blended_Wrapped(font1, status_text.c_str(), status_font_clr, 125);
    status_display_texture = SDL_CreateTextureFromSurface(gRenderer, status_display_surface);
    //create text rect (text box)
    SDL_QueryTexture(status_display_texture, NULL, NULL, &status_texW, &status_texH); //gets the dimenstions of the font/text
    SDL_Rect dstrect = {0, 0, status_texW, status_texH }; //so we can make the proper rect to dispay it
    SDL_RenderCopy(gRenderer, status_display_texture, NULL, &dstrect); //write
    //FREE MEMORY
    SDL_DestroyTexture(status_display_texture);
    SDL_FreeSurface(status_display_surface);
    //FINISH PUtting the Title: Status

    //Show Fave Colors
    SDL_Color status_color1 = spr1.faveColor; //The prim color of the cre
    SDL_Color status_color2 = spr1.faveColor2; //The seco color of the cre
    //FIRST
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND); //temporarily allow alpha
    SDL_Rect colR ={ 0 , 3*15 , 7 , status_texH/2 };
    SDL_SetRenderDrawColor( gRenderer, status_color1.r , status_color1.g, status_color1.b , 255); //color of box
    SDL_RenderFillRect( gRenderer, &colR ); //copy box to renderer
    //SECOND
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND); //temporarily allow alpha
    colR ={ 7 , 3*15 , 7 , status_texH/2 };
    SDL_SetRenderDrawColor( gRenderer, status_color2.r , status_color2.g, status_color2.b , 255); //color of box
    SDL_RenderFillRect( gRenderer, &colR ); //copy box to renderer
    
    
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE); //turn alpha back off
    
}















