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

//This is a 48x48 structure
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


///////////////
//WORKSHOP TYPES
//
// 0 - SPIN WHEL

//This is a 32x32 structure
class Workshop{
public:
    Workshop(int xpos, int ypos, int workshop_type, SDL_Color primo, SDL_Color seco);
    Workshop(int xpos, int ypos, int workshop_type);
    SDL_Color primColor; //the pirimary color of item
    SDL_Color secoColor; //the secondary color of the item
    int x,y,z;
    int type; //specifies item type
    void draw(SDL_Renderer* gRenderer, SDL_Texture** workshop_tiles_p,SDL_Texture** workshop_tiles_s); //draws the item
    void draw(SDL_Renderer* gRenderer, SDL_Texture** workshop_tiles_p,SDL_Texture** workshop_tiles_s,SDL_Texture** workshop_tiles_t); //overload
    
};



#endif /* tent_h */
