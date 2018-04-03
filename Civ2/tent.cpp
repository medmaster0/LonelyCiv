//
//  tent.cpp
//  Civ2
//
//  Created by Medz on 9/15/17.
//  Copyright © 2017 Medz. All rights reserved.
//

#include <stdio.h>
#include <SDL2/SDL.h>

#include "tent.hpp"

Tent::Tent(int xpos, int ypos, int tent_type, SDL_Color primo, SDL_Color seco){
    
    x = xpos;
    y = ypos;
    type = tent_type;
    primColor = primo;
    secoColor = seco;
    
    //Some type specific color schemes.... (GOOD IDEA OR NO?)
    if(type == 1 ){
        secoColor = {200, 0, 0 };
        primColor = {255, 255, 255};
    }
    
}

Tent::Tent(int xpos, int ypos, int tent_type){

    x = xpos;
    y = ypos;
    type = tent_type;
    primColor = {static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255)};
    secoColor = {static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255)};
    
    
}

//draw method
void Tent::draw(SDL_Renderer* gRenderer, SDL_Texture** tent_tiles_p,SDL_Texture** tent_tiles_s){
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { (x-1)*16, (y-2)*16, 48, 48 };
    //DEBUG: GETTING EXC_BAD_ACCESS above
    SDL_Rect* clip = NULL;
    SDL_SetTextureColorMod( tent_tiles_p[type], primColor.r, primColor.g, primColor.b); //modulate color, update to match the new one
    SDL_RenderCopy( gRenderer, tent_tiles_p[type], clip, &renderQuad );//Render to screen
    if(tent_tiles_s[type] != (SDL_Texture*) 0x9999 ){
        SDL_SetTextureColorMod( tent_tiles_s[type], secoColor.r, secoColor.g, secoColor.b); //modulate color, update to match the new one
        SDL_RenderCopy( gRenderer, tent_tiles_s[type], clip, &renderQuad );//Render to screen
    }
    
}


//WORKSHOP STUFF
//Workshops are 32x32 structure

Workshop::Workshop(int xpos, int ypos, int workshop_type, SDL_Color primo, SDL_Color seco){
    
    x = xpos;
    y = ypos;
    type = workshop_type;
    primColor = primo;
    secoColor = seco;
    
    //Some type specific color schemes.... (GOOD IDEA OR NO?)
    if(type == 1 ){
        secoColor = {200, 0, 0 };
        primColor = {255, 255, 255};
    }
    
}

Workshop::Workshop(int xpos, int ypos, int workshop_type){
    
    x = xpos;
    y = ypos;
    type = workshop_type;
    primColor = {static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255)};
    secoColor = {static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255)};
    
    
}

//draw method
void Workshop::draw(SDL_Renderer* gRenderer, SDL_Texture** workshop_tiles_p,SDL_Texture** workshop_tiles_s){
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { (x)*16, (y-1)*16, 32, 32 };
    //DEBUG: GETTING EXC_BAD_ACCESS above
    SDL_Rect* clip = NULL;
    SDL_SetTextureColorMod( workshop_tiles_p[type], primColor.r, primColor.g, primColor.b); //modulate color, update to match the new one
    SDL_RenderCopy( gRenderer, workshop_tiles_p[type], clip, &renderQuad );//Render to screen
    if(workshop_tiles_s[type] != (SDL_Texture*) 0x9999 ){
        SDL_SetTextureColorMod( workshop_tiles_s[type], secoColor.r, secoColor.g, secoColor.b); //modulate color, update to match the new one
        SDL_RenderCopy( gRenderer, workshop_tiles_s[type], clip, &renderQuad );//Render to screen
    }
    
}




