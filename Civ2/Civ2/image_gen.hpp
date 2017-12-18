//
//  image_gen.hpp
//  Civ2
//
//  Created by Medz on 12/10/17.
//  Copyright © 2017 Medz. All rights reserved.
//
// Algorithms for making random images

#ifndef image_gen_hpp
#define image_gen_hpp

#include <stdio.h>
#include <SDL2/SDL.h>

SDL_Texture* genTexture(SDL_Renderer* gRenderer, SDL_Window* gWindow); //generates a texture of dimensions w x h
void drawTexture(SDL_Renderer* gRenderer, SDL_Texture* texture, int x, int y ); //draws a texture at position (x,y)

#endif /* image_gen_hpp */
