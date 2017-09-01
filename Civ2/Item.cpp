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


//initial constructor
Item::Item(int xpos, int ypos){
    
    x = xpos;
    y = ypos;
    //tilesPrim = new SDL_Texture * [1];
    
}
//draw method
void Item::draw(){
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x*16, y*16, 16, 16 };
    SDL_Rect* clip = NULL;
    SDL_RenderCopy( gRenderer, *Item::tilesPrim, clip, &renderQuad );//Render to screen
    SDL_RenderCopy( gRenderer, *Item::tilesSeco, clip, &renderQuad );//Render to screen
}
