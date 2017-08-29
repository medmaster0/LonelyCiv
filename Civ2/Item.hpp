//
//  item.hpp
//  Civ2
//
//  Created by Medz on 8/17/17.
//  Copyright © 2017 Medz. All rights reserved.
//

#ifndef item_hpp
#define item_hpp

#include <SDL2/SDL.h>

//#include "Item.hpp"
#include "creature.hpp"


class Item{
public:
    Item(int xpos, int ypos);
    static SDL_Texture** tilesPrim; //contains item tiles, the first version
    static SDL_Texture** tilesSeco; //contains item tiles, the secondary version
    SDL_Color primColor; //the pirimary color of item
    SDL_Color secoColor; //the secondary color of the item
    int x,y,z;
    int type; //specifies item type
    void draw(); //draws the item
    
    
    
};

#endif //item_hpp
