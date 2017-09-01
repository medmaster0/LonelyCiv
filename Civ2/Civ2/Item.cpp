//
//  Item.cpp
//  Civ2
//
//  Created by Medz on 8/29/17.
//  Copyright © 2017 Medz. All rights reserved.
//

#include "Item.hpp"

#include "creature.hpp"

#include <SDL2/SDL.h>

using namespace std;

///////////////
//ITEM TILE TYPES
// 300 - CAN
// 301 - ADOBE

//static variable constructor
//This overloaded constructor sets the static class variables
//Item::Item(SDL_Texture** tiles1, SDL_Texture** tiles2){
//    SDL_Texture** Item::tilesPrim = tiles1;
//    SDL_Texture** Item::tilesSeco = tiles2;
//    printf("happpedfsa fuck fuck FFFFFFFF fuck");
//}
////initial constructor
//this overloaded constructor actually constructs an instance of object
Item::Item(int xpos, int ypos, int tile_type){
    
    //static SDL_Texture** tilesPrim = new SDL_Texture * [1]; //contains item tiles, the first version
    //static SDL_Texture** tilesSeco = new SDL_Texture * [1]; //contains item tiles, the secondary version
    //ALso load tiles
    
    x = xpos;
    y = ypos;
    type = tile_type;
    primColor = {static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255)};
    secoColor = {static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255)};
    
    //tilesPrim = new SDL_Texture * [1];
    
}

////initial constructor
//this overloaded constructor actually constructs an instance of object
Item::Item(int xpos, int ypos, int tile_type, SDL_Color primo, SDL_Color seco){
    
    //static SDL_Texture** tilesPrim = new SDL_Texture * [1]; //contains item tiles, the first version
    //static SDL_Texture** tilesSeco = new SDL_Texture * [1]; //contains item tiles, the secondary version
    //ALso load tiles
    
    x = xpos;
    y = ypos;
    type = tile_type;
    primColor = primo;
    secoColor = seco;
    
    //tilesPrim = new SDL_Texture * [1];
    
}

//draw method
void Item::draw(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p,SDL_Texture** item_tiles_s){
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x*16, y*16, 16, 16 };
    SDL_Rect* clip = NULL;
    SDL_SetTextureColorMod( item_tiles_p[type], primColor.r, primColor.g, primColor.b); //modulate color, update to match the new one
    SDL_RenderCopy( gRenderer, item_tiles_p[type], clip, &renderQuad );//Render to screen
    if(item_tiles_s[type] != (SDL_Texture*) 0x9999 ){
        SDL_SetTextureColorMod( item_tiles_s[type], secoColor.r, secoColor.g, secoColor.b); //modulate color, update to match the new one
        SDL_RenderCopy( gRenderer, item_tiles_s[type], clip, &renderQuad );//Render to screen
    }
}
