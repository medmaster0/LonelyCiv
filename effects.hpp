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

class Effect{
public:
    Effect(int xp, int yp, int effect_type); //Constructor: input xy-pos and effect_type
    bool loadFromFile(std::string path); //load's the object's image from the specified path
    void draw(SDL_Renderer* gRenderer); //Draws the sprite to screen using stored MAP COORDS
    void drawScroll(SDL_Renderer* gRenderer); //Draws the sprite effect but also scrolls
    int x,y,z; //the location of effect
private:
    SDL_Texture* primTexture;//primary texture of the sprite
    int mWidth;//Image dimensions - pixels
    int mHeight;//Image dimensions - pixels
    int r,g,b; //colors of the sprite
    int effect_type; //type of effect (code)
    int scrollPos; //the position (in tile) where we draw the first line of the sprite in the scroll
    unsigned int animate_timer; //a timer used in animation
};

#endif /* effects_hpp */
