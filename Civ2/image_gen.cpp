//
//  image_gen.cpp
//  Civ2
//
//  Created by Medz on 12/10/17.
//  Copyright © 2017 Medz. All rights reserved.
//
//   Algorithms for making random images

#include "image_gen.hpp"
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>


//generates a texture of dimensions w x h
//GOD DAM N BROKEN - DOESN"T DRAW TRANSPARENT PIXELS AS  TRANSPARENT BUT AS BLACK!!!! FUCKfuckfuckfuck
SDL_Texture* genTexture(SDL_Renderer* gRenderer, SDL_Window* gWindow){
    
    SDL_Surface* img = IMG_Load("Civ2/Civ2/tiles/blank.png");
    
    SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat( img, SDL_GetWindowPixelFormat( gWindow ), NULL );
    
    //SDL_SetColorKey( img, SDL_TRUE, SDL_MapRGB( img->format, 0, 0xFF, 0xFF ) );//Color key image
    //SDL_Texture* texture = SDL_CreateTextureFromSurface( gRenderer, img ); //Create texture from surface pixels
    
    SDL_Texture* texture = SDL_CreateTexture(gRenderer, SDL_GetWindowPixelFormat( gWindow ), SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h);
    void * pixels;
    SDL_LockTexture(texture, &formattedSurface->clip_rect, &pixels, &formattedSurface->pitch);
    
    
    
    //Allocate format from window
    Uint32 format = SDL_GetWindowPixelFormat( gWindow );
    SDL_PixelFormat* mappingFormat = SDL_AllocFormat( format );
    
//    //Get pixel data
//    Uint32* pixels = (Uint32*)texture->getPixels();
//    int pixelCount = ( texture->getPitch() / 4 ) * texture->getHeight();
    
    //Get proper value for transparent and marked pixels
    Uint32 transparent = SDL_MapRGBA( mappingFormat, 0x00, 0x00, 0x00, 0x00 );
    Uint32 marked = SDL_MapRGBA( mappingFormat, 0xFF, 0xFF, 0xFF, 0xFF );
    
    
    memcpy(pixels, formattedSurface->pixels, formattedSurface->w * formattedSurface->h);
    Uint32 * upixels = (Uint32 *) pixels;
    
    // get or modify pixels
    //THIS USES FROMAT: AGBR!!!!!!!!!!!!
    
    //First, clear out all pixels
    for(int i = 0; i < formattedSurface->w*formattedSurface->h; i++){
        
//        Uint8 * colors = (Uint8 *) upixels[i];
        printf("color %d",upixels[i]);
        
        upixels[i] = transparent;
        
        printf("color %d",upixels[i]);
    }
    
    for(int i = 0; i < 62; i++){
        upixels[i] = 0xFfffffff; //AGBR format
    }
    
    
    //end manipulation
    
    SDL_UnlockTexture(texture);
    
    SDL_FreeSurface( img );
    SDL_FreeSurface( formattedSurface );

    
    return texture;
    
}

//draws a texture at position (x,y)
void drawTexture(SDL_Renderer* gRenderer, SDL_Texture* texture, int x, int y ){
    
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_MOD); //temporarily allow alpha
    
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x*16, y*16, 16,16 };
    SDL_Rect* clip = NULL;
    SDL_RenderCopy( gRenderer, texture, clip, &renderQuad );//Render to screen
    
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE); //turn alpha back off
    
}
