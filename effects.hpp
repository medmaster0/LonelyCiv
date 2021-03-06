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
    void drawScroll(SDL_Renderer* gRenderer, SDL_Texture** misc_tiles); //Draws the sprite effect but also scrolls
    void drawScroll(int at_x, int at_y, SDL_Renderer* gRenderer, SDL_Texture** misc_tiles); //Draws the sprite effect but also scrolls. Draws at specified location
    int x,y,z; //the location of effect
    SDL_Color color1; //the color of the effect
    //FOr Animation
//    int animate_code; //determines how the sprite gets animated: 1: scroll, 2: sprite sheet sequence
//    int animate_tiles[4]; //list of tiles used in the animation
private:
    SDL_Texture* primTexture;//primary texture of the sprite
    int mWidth;//Image dimensions - pixels
    int mHeight;//Image dimensions - pixels
    int r,g,b; //colors of the sprite
    int effect_type; //type of effect (code)
    int tile_index; //the inddex that corresponds to the tile in misc_tiles that corresponds to our effect
    int scrollPos; //the position (in tile) where we draw the first line of the sprite in the scroll
    unsigned int animate_timer; //a timer used in animation
};

//ANIMATION CLASS
class Animation{
public:
    Animation(int xp, int yp, int tile_list_in[]); //Constructor: input xy-pos and a list of tiles to follow
    void draw(SDL_Renderer* gRenderer, SDL_Texture** misc_tiles); //Draws the sprite to the screen using stored MAP COORDS, changes the tile periodically
    void draw(int at_x, int at_y, SDL_Renderer* gRenderer, SDL_Texture** misc_tiles); //Draws the sprite to the screen at specified map coords, changes the tile periodically
    int x,y,z; //the location of the animation
    //SDL_Color color1; //the color of the effect
private:
    int mWidth;//Image dimensions - pixels
    int mHeight;//Image dimensions - pixels
    int r,g,b; //colors of the sprite
    int tile_list[4]; //a list of tiles to sequence through in animation (indices for "misc_tiles")
    int animate_index; //the current index in the tile_list that we are displaying
    unsigned int animate_timer; //a timer used in animation
    
};

//A BIGGER ANIMATION CLASS
class Animation_Big{
public:
    Animation_Big(int xp, int yp, int tile_list_in[]); //constructor; input xy-pos and a list of tiles to follow
    void draw(int at_x, int at_y, SDL_Renderer* gRenderer, SDL_Texture** misc_tiles); //Draws the sprite to the screen at specified map
    int x,y,z; //the location of the animation
private:
    int mWidth; //Image dimenstions - pixels
    int mHeight;//Image dimensions - pixels
    int r,g,b; //colors of the sprite
    int tile_list[4]; //a list of tiles to sequence through in animation (indices for "misc_tiles")
    int animate_index; //the current index in the tile_list that we are displaying
    unsigned int animate_timer; //a timer used in animation
};

//AN COLORFUL ANIMATION CLASS
//that has the potential for different colors....
class Color_Animation{
public:
    Color_Animation(int xp, int yp, int zp, int tile_list_in[], SDL_Color primCol = {0,0,0}, SDL_Color secoCol = {0,0,0}, SDL_Color tertCol = {0,0,0}); //construcor: input xy-pos and list of tiles to follow, and also color
    void draw(int at_x, int at_y, SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s, SDL_Texture** item_tiles_t, int alpha_mod = 255.0); //draws the sprite tot hte screen at a specified map
    int x,y,z;
    SDL_Color prim_col, seco_col, tert_col; // colors of the sprite
private:
    int mWidth; //Image dimenstions - pixels
    int mHeight;//Image dimensions - pixels
    int tile_list[4]; //a list of tiles to sequence through in animation (indices for "misc_tiles")
    int animate_index; //the current index in the tile_list that we are displaying
    unsigned int animate_timer; //a timer used in animation
};













#endif /* effects_hpp */
