//
//  effects.hpp
//  Civ2
//
//  Created by Medz on 4/3/18.
//  Copyright © 2018 Medz. All rights reserved.
//

#ifndef effects_hpp
#define effects_hpp

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include <stdio.h>
#include <string>
using namespace std;

SDL_Texture* loadTexture( std::string path ); //loads a texture specified by string path name... gets implemented in main function since it requires a gRenderer to be declared

//EFFECT CLASS
//Basically a scrolling ribbon that can go above a creature's head
class Effect{
public:
    Effect(int xp, int yp, int effect_type); //Constructor: input xy-pos and effect_type
    bool loadFromFile(std::string path); //load's the object's image from the specified path
    void draw(SDL_Renderer* gRenderer); //Draws the sprite to screen using stored MAP COORDS
    void drawScroll(SDL_Renderer* gRenderer); //Draws the sprite effect but also scrolls
    int x,y,z; //the location of effect
    //FOr Animation
//    int animate_code; //determines how the sprite gets animated: 1: scroll, 2: sprite sheet sequence
//    int animate_tiles[4]; //list of tiles used in the animation
private:
    SDL_Texture* primTexture;//primary texture of the sprite
    int mWidth;//Image dimensions - pixels
    int mHeight;//Image dimensions - pixels
    int r,g,b; //colors of the sprite
    int effect_type; //type of effect (code)
    int scrollPos; //the position (in tile) where we draw the first line of the sprite in the scroll
    unsigned int animate_timer; //a timer used in animation
};

//ANIMATION CLASS
class Animation{
public:
    Animation(int xp, int yp, int tile_list_in[]); //Constructor: input xy-pos and a list of tiles to follow
    void draw(SDL_Renderer* gRenderer, SDL_Texture** misc_tiles); //Draws the sprite to the screen using stored MAP COORDS, changes the tile periodically
    int x,y,z; //the location of the animation
    
private:
    int mWidth;//Image dimensions - pixels
    int mHeight;//Image dimensions - pixels
    int r,g,b; //colors of the sprite
    int tile_list[4]; //a list of tiles to sequence through in animation (indices for "misc_tiles")
    int animate_index; //the current index in the tile_list that we are displaying
    unsigned int animate_timer; //a timer used in animation
    
};


#endif /* effects_hpp */
