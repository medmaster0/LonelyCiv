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

using namespace std;

//SDL_Texture* loadTexture( string path, SDL_Renderer* gRenderer );

class Item{
public:
    Item(SDL_Texture** tiles1, SDL_Texture** tiles2);
    Item(int xpos, int ypos, int tile_type, SDL_Color primo, SDL_Color seco);
    Item(int xpos, int ypos, int tile_type);
    SDL_Color primColor; //the pirimary color of item
    SDL_Color secoColor; //the secondary color of the item
    int x,y,z;
    int type; //specifies item type
    void draw(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p,SDL_Texture** item_tiles_s); //draws the item
    
    static SDL_Texture** tilesPrim;
    static SDL_Texture** tilesSeco;
    
};

#endif //item_hpp
