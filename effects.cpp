//
//  effects.cpp
//  Civ2
//
//  Created by Medz on 4/3/18.
//  Copyright © 2018 Medz. All rights reserved.
//

#include "effects.hpp"
#include "med_algo.hpp"
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

//Constructor
//Initiliaze the effect with it's location
//input: x,y-coords for location of effect
//       type of effect
//EFFECT CODES
// 0 - LUVVY DUVVY
// 1 - SPARKLY
// 2 - SMOKY
Effect::Effect(int xp, int yp, int type){
    
    primTexture = NULL;
    mWidth = 0;
    mHeight = 0;
    x = xp;
    y = yp;
    scrollPos = 0; 
    effect_type = type;
    
    SDL_Color pink;
    SDL_Color gold; 
    switch (effect_type) {
        case 0: //LUVVY DUVVY HEARTS EFFECT
            //loadFromFile("/Users/medz/Documents/0/Cplus/Civ2/Civ2/tiles/heartsPrim.png");
            color1 = generate_pink(); //give effect a random shade of pink
            tile_index = 19;
            //SDL_SetTextureColorMod( primTexture,pink.r, pink.g, pink.b); //generate pinks
            break;
        case 1: //SPARKLES
            //loadFromFile("/Users/medz/Documents/0/Cplus/Civ2/Civ2/tiles/sparklesPrim.png");
            //SDL_SetTextureColorMod( primTexture,255,209,0); //nice shade of gold
            color1 = {255,209,0,255}; //give the effect a nice shade of gold
            tile_index = 20; //specify the correct tile index for our effect
            break;
        case 2: //SHMOKE
            //loadFromFile("/Users/medz/Documents/0/Cplus/Civ2/Civ2/tiles/smokePrim.png");
            //SDL_SetTextureAlphaMod( primTexture, 126); //modulate color, update to match the new one
            color1 = {static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), 126};
            tile_index = 21;
            break;
        default:
            break;
    }
    
    animate_timer = SDL_GetTicks();
    
}
//load's the object's image from the specified path
bool Effect::loadFromFile(std::string path){
    primTexture = loadTexture(path);
    SDL_SetTextureColorMod( primTexture, r, g, b); //modulate color, update to match the new one
    mWidth = 16;
    mHeight = 16;
    return primTexture != NULL;
}
//Draws the sprite to screen using private MAP COORDS
void Effect::draw(SDL_Renderer* gRenderer)
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x*16, y*16, mWidth, mHeight };
    SDL_Rect* clip = NULL;
    SDL_RenderCopy( gRenderer, primTexture, clip, &renderQuad );//Render to screen

}
//Draws the sprite effect but also scrolls
void Effect::drawScroll(SDL_Renderer* gRenderer, SDL_Texture** misc_tiles){
    
    //Set up thw two rectangles used for displaying scrolling picture
    SDL_RendererFlip flippy = SDL_FLIP_NONE; //don''t flip the picture
    SDL_Rect sourceQuad_1 = { 0,scrollPos,16,16-scrollPos}; //The first half of the scroll picture (on the texture) -> grab bottom portion
    SDL_Rect destQuad_1 = { x*16,y*16,16,16-scrollPos}; //where on the main map we should draw -> and stick it on top part of map tile
    
    SDL_Rect sourceQuad_2 = { 0,0,16,scrollPos}; //The second half of the scroll picture (on the texture) -> grab top portion
    SDL_Rect destQuad_2 = { x*16,(y*16)+(16-scrollPos),16,scrollPos}; //where on the main map we should draw -> and stick it on bottom part of map tile
    
    //Apply the specific color and alpha (if applicable) to effect.
    SDL_SetTextureColorMod( misc_tiles[tile_index],color1.r, color1.g, color1.b);
    if(effect_type == 2){ //if smoke effect type
        SDL_SetTextureAlphaMod( misc_tiles[tile_index], 126); //modulate color, update to match the new one
    }
    
    SDL_RenderCopyEx(gRenderer, misc_tiles[tile_index], &sourceQuad_1, &destQuad_1, 0, NULL, flippy);
    SDL_RenderCopyEx(gRenderer, misc_tiles[tile_index], &sourceQuad_2, &destQuad_2, 0, NULL, flippy);
    //SDL_RenderCopyEx( gRenderer, primTexture, clip, &renderQuad );//Render to screen
    
    //Decide if it's time to tick the scroll
    if(SDL_GetTicks() > animate_timer + 100){ //if more than 1 second has passed since last tick
        //Update scrollPos
        scrollPos = scrollPos + 1;
        if(scrollPos == 16){
            scrollPos = 0;
        }
        //Update Timer
        animate_timer = SDL_GetTicks();
        
    }
    
}

//CONSTRUCTOR FOR ANIMATION
//Right now, tile_list_in must be a 4-element array of ints, speicfying seuqnece on animation
Animation::Animation(int xp, int yp, int tile_list_in[]){
    mWidth = 16;
    mHeight = 16;
    x = xp;
    y = yp;
    animate_index = 0;
    tile_list[0] = tile_list_in[0];
    tile_list[1] = tile_list_in[1];
    tile_list[2] = tile_list_in[2];
    tile_list[3] = tile_list_in[3];
    
    animate_timer = SDL_GetTicks(); //start timer bitch
    
}

void Animation::draw(SDL_Renderer* gRenderer, SDL_Texture** misc_tiles){
    
    //determine if need to change tile index, animate_index
    if(SDL_GetTicks() > animate_timer + 100){ //if more than 1 second has passed since last tick
    
        //Update Animation tile index
        animate_index = animate_index + 1;
        if(animate_index >= 4){
            animate_index = 0; 
        }
        
        //Update Timer
        animate_timer = SDL_GetTicks();
        
    }
    
    //determine the required tile
    int tile = tile_list[animate_index];
    
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x*16, y*16, mWidth, mHeight };
    SDL_Rect* clip = NULL;
    SDL_RenderCopy( gRenderer, misc_tiles[tile], clip, &renderQuad );//Render to screen
}









