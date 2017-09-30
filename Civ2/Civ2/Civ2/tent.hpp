//
//  tent.hpp
//  Civ2
//
//  Created by Medz on 9/15/17.
//  Copyright © 2017 Medz. All rights reserved.
//

#ifndef tent_h
#define tent_h
///////////
//TENT TYPES
//
// 0 - MANCY

class Tent{
public:
    Tent(int xpos, int ypos, int tent_type, SDL_Color primo, SDL_Color seco);
    Tent(int xpos, int ypos, int tent_type);
    SDL_Color primColor; //the pirimary color of item
    SDL_Color secoColor; //the secondary color of the item
    int x,y,z;
    int type; //specifies item type
    void draw(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p,SDL_Texture** item_tiles_s); //draws the item
    void draw(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p,SDL_Texture** item_tiles_s,SDL_Texture** item_tiles_t); //overload
    
};


#endif /* tent_h */
