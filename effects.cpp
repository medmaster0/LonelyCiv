//
//  effects.cpp
//  Civ2
//
//  Created by Medz on 4/3/18.
//  Copyright © 2018 Medz. All rights reserved.
//

#include "effects.hpp"
#include "med_algo.hpp"
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

//Constructor
//Initiliaze the effect with it's location
//input: x,y-coords for location of effect
//       type of effect
//EFFECT CODES
// 0 - LUVVY DUVVY
// 1 - SPARKLY
// 2 - SMOKY
Effect::Effect(int xp, int yp, int type){
    
    primTexture = NULL;
    mWidth = 0;
    mHeight = 0;
    x = xp;
    y = yp;
    scrollPos = 0; 
    effect_type = type;
    
    SDL_Color pink;
    SDL_Color gold; 
    switch (effect_type) {
        case 0: //LUVVY DUVVY HEARTS EFFECT
            //loadFromFile("/Users/medz/Documents/0/Cplus/Civ2/Civ2/tiles/heartsPrim.png");
            color1 = generate_pink(); //give effect a random shade of pink
            tile_index = 19;
            //SDL_SetTextureColorMod( primTexture,pink.r, pink.g, pink.b); //generate pinks
            break;
        case 1: //SPARKLES
            //loadFromFile("/Users/medz/Documents/0/Cplus/Civ2/Civ2/tiles/sparklesPrim.png");
            //SDL_SetTextureColorMod( primTexture,255,209,0); //nice shade of gold
            color1 = {255,209,0,255}; //give the effect a nice shade of gold
            tile_index = 20; //specify the correct tile index for our effect
            break;
        case 2: //SHMOKE
            //loadFromFile("/Users/medz/Documents/0/Cplus/Civ2/Civ2/tiles/smokePrim.png");
            //SDL_SetTextureAlphaMod( primTexture, 126); //modulate color, update to match the new one
            color1 = {static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), 126};
            tile_index = 21;
            break;
        case 3: //EXCLAMATION
            color1 = generate_pink();
            tile_index = 27;
            break;
        default:
            break;
    }
    
    animate_timer = SDL_GetTicks();
    
}
//load's the object's image from the specified path
bool Effect::loadFromFile(std::string path){
    primTexture = loadTexture(path);
    SDL_SetTextureColorMod( primTexture, r, g, b); //modulate color, update to match the new one
    mWidth = 16;
    mHeight = 16;
    return primTexture != NULL;
}
//Draws the sprite to screen using private MAP COORDS
void Effect::draw(SDL_Renderer* gRenderer)
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x*16, y*16, mWidth, mHeight };
    SDL_Rect* clip = NULL;
    SDL_RenderCopy( gRenderer, primTexture, clip, &renderQuad );//Render to screen

}
//Draws the sprite effect but also scrolls
void Effect::drawScroll(SDL_Renderer* gRenderer, SDL_Texture** misc_tiles){
    
    //Set up thw two rectangles used for displaying scrolling picture
    SDL_RendererFlip flippy = SDL_FLIP_NONE; //don''t flip the picture
    SDL_Rect sourceQuad_1 = { 0,scrollPos,16,16-scrollPos}; //The first half of the scroll picture (on the texture) -> grab bottom portion
    SDL_Rect destQuad_1 = { x*16,y*16,16,16-scrollPos}; //where on the main map we should draw -> and stick it on top part of map tile
    
    SDL_Rect sourceQuad_2 = { 0,0,16,scrollPos}; //The second half of the scroll picture (on the texture) -> grab top portion
    SDL_Rect destQuad_2 = { x*16,(y*16)+(16-scrollPos),16,scrollPos}; //where on the main map we should draw -> and stick it on bottom part of map tile
    
    //Apply the specific color and alpha (if applicable) to effect.
    SDL_SetTextureColorMod( misc_tiles[tile_index],color1.r, color1.g, color1.b);
    if(effect_type == 2){ //if smoke effect type
        SDL_SetTextureAlphaMod( misc_tiles[tile_index], 126); //modulate color, update to match the new one
    }
    
    SDL_RenderCopyEx(gRenderer, misc_tiles[tile_index], &sourceQuad_1, &destQuad_1, 0, NULL, flippy);
    SDL_RenderCopyEx(gRenderer, misc_tiles[tile_index], &sourceQuad_2, &destQuad_2, 0, NULL, flippy);
    //SDL_RenderCopyEx( gRenderer, primTexture, clip, &renderQuad );//Render to screen
    
    //STNDARD-ISSUE MOVEMENT TIMING VARIABLES
    //spr1->move_timer = SDL_GetTicks(); //start move timer
    int steps_to_pop = 0; //how many steps need to be popped off path
    int carry_over = 0; //how many ticks were rounded off
    int ANIMATE_SPEED = 100; //constant used to throttle animation speed
    
    //Determine how many steps have to be moved while time has passed
    steps_to_pop = ( SDL_GetTicks() - animate_timer ) / ANIMATE_SPEED; //ONe tick every 100 ms
    if(steps_to_pop <= 0){
        return; //no steps need to be taken, continue
    }
    //Now tick that many steps
    //steps_to_pop = steps_to_pop%16; //if more than 16  it just completes a cycle
    scrollPos = (scrollPos + steps_to_pop) % 16; //update scrollPos with new ticks (every 16 cycle is a coplete revolution)
    //Update animate timer
    //Figure out how much time not accounted for
    carry_over = SDL_GetTicks() - animate_timer; //How much time has elapsed since last timing
    carry_over = carry_over - (steps_to_pop * ANIMATE_SPEED); //don't count the time we *have* accounted for
    //Now check new timer but also remember the time we haven't accounted for
    animate_timer = SDL_GetTicks() - carry_over; //Now update the timer, but set it back for unaccounted time
    
}

//Identical to above, but draws at specific location
void Effect::drawScroll(int at_x, int at_y, SDL_Renderer* gRenderer, SDL_Texture** misc_tiles){
    
    //Set up thw two rectangles used for displaying scrolling picture
    SDL_RendererFlip flippy = SDL_FLIP_NONE; //don''t flip the picture
    SDL_Rect sourceQuad_1 = { 0,scrollPos,16,16-scrollPos}; //The first half of the scroll picture (on the texture) -> grab bottom portion
    SDL_Rect destQuad_1 = { at_x*16,at_y*16,16,16-scrollPos}; //where on the main map we should draw -> and stick it on top part of map tile
    
    SDL_Rect sourceQuad_2 = { 0,0,16,scrollPos}; //The second half of the scroll picture (on the texture) -> grab top portion
    SDL_Rect destQuad_2 = { at_x*16,(at_y*16)+(16-scrollPos),16,scrollPos}; //where on the main map we should draw -> and stick it on bottom part of map tile
    
    //Apply the specific color and alpha (if applicable) to effect.
    SDL_SetTextureColorMod( misc_tiles[tile_index],color1.r, color1.g, color1.b);
    if(effect_type == 2){ //if smoke effect type
        SDL_SetTextureAlphaMod( misc_tiles[tile_index], 126); //modulate color, update to match the new one
    }
    
    SDL_RenderCopyEx(gRenderer, misc_tiles[tile_index], &sourceQuad_1, &destQuad_1, 0, NULL, flippy);
    SDL_RenderCopyEx(gRenderer, misc_tiles[tile_index], &sourceQuad_2, &destQuad_2, 0, NULL, flippy);
    //SDL_RenderCopyEx( gRenderer, primTexture, clip, &renderQuad );//Render to screen
    
    //    //Decide if it's time to tick the scroll
    //    if(SDL_GetTicks() > animate_timer + 100){ //if more than 1 second has passed since last tick
    //        //Update scrollPos
    //        scrollPos = scrollPos + 1;
    //        if(scrollPos == 16){
    //            scrollPos = 0;
    //        }
    //        //Update Timer
    //        animate_timer = SDL_GetTicks();
    //
    //    }
    
    //STNDARD-ISSUE MOVEMENT TIMING VARIABLES
    //spr1->move_timer = SDL_GetTicks(); //start move timer
    int steps_to_pop = 0; //how many steps need to be popped off path
    int carry_over = 0; //how many ticks were rounded off
    int ANIMATE_SPEED = 100; //constant used to throttle animation speed
    
    //Determine how many steps have to be moved while time has passed
    steps_to_pop = ( SDL_GetTicks() - animate_timer ) / ANIMATE_SPEED; //ONe tick every 100 ms
    if(steps_to_pop <= 0){
        return; //no steps need to be taken, continue
    }
    //Now tick that many steps
    //steps_to_pop = steps_to_pop%16; //if more than 16  it just completes a cycle
    scrollPos = (scrollPos + steps_to_pop) % 16; //update scrollPos with new ticks (every 16 cycle is a coplete revolution)
    //Update animate timer
    //Figure out how much time not accounted for
    carry_over = SDL_GetTicks() - animate_timer; //How much time has elapsed since last timing
    carry_over = carry_over - (steps_to_pop * ANIMATE_SPEED); //don't count the time we *have* accounted for
    //Now check new timer but also remember the time we haven't accounted for
    animate_timer = SDL_GetTicks() - carry_over; //Now update the timer, but set it back for unaccounted time
    
}


//CONSTRUCTOR FOR ANIMATION
//Right now, tile_list_in must be a 4-element array of ints, speicfying seuqnece on animation
Animation::Animation(int xp, int yp, int tile_list_in[]){
    mWidth = 16;
    mHeight = 16;
    x = xp;
    y = yp;
    z = 0;
    animate_index = 0;
    tile_list[0] = tile_list_in[0];
    tile_list[1] = tile_list_in[1];
    tile_list[2] = tile_list_in[2];
    tile_list[3] = tile_list_in[3];
    
    animate_timer = SDL_GetTicks(); //start timer bitch
    
}

void Animation::draw(SDL_Renderer* gRenderer, SDL_Texture** misc_tiles){
    
    int temp_index = 0; //user to store and check the animation index before actually writing it
    
    //STNDARD-ISSUE MOVEMENT TIMING VARIABLES
    //spr1->move_timer = SDL_GetTicks(); //start move timer
    int steps_to_pop = 0; //how many steps need to be popped off path
    int carry_over = 0; //how many ticks were rounded off
    int ANIMATE_SPEED = 100; //constant used to throttle animation speed
    
    //determine if need to change tile index, animate_index
    if(SDL_GetTicks() > animate_timer + 100){ //if more than 1 second has passed since last tick
    
        //Determine how many steps have to be moved while time has passed
        steps_to_pop = ( SDL_GetTicks() - animate_timer ) / ANIMATE_SPEED; //ONe tick every 100 ms
        
        //Update Animation tile index (calculate how many ticks have gone by
        temp_index = (animate_index + steps_to_pop)%4; //if above 4, wrap around... built in bounds check, should never be above 4
        animate_index = temp_index;//temp_index should be safe to write to animate_index, now
        
        //Update animate timer
        //Figure out how much time not accounted for
        carry_over = SDL_GetTicks() - animate_timer; //How much time has elapsed since last timing
        carry_over = carry_over - (steps_to_pop * ANIMATE_SPEED); //don't count the time we *have* accounted for
        //Now check new timer but also remember the time we haven't accounted for
        animate_timer = SDL_GetTicks() - carry_over; //Now update the timer, but set it back for unaccounted time
        
    }//otherwise, we won't have to do any updating
    
    //Now animate_index should be all figured out and ready to draw
    //determine the required tile
    int tile = tile_list[animate_index];
    
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x*16, y*16, mWidth, mHeight };
    SDL_Rect* clip = NULL;
    SDL_RenderCopy( gRenderer, misc_tiles[tile], clip, &renderQuad );//Render to screen
    
    
}

//Identical to the above, but draws it at a specified x,y coord)
void Animation::draw(int at_x, int at_y, SDL_Renderer* gRenderer, SDL_Texture** misc_tiles){
    
    int temp_index = 0; //user to store and check the animation index before actually writing it
    
    //STNDARD-ISSUE MOVEMENT TIMING VARIABLES
    //spr1->move_timer = SDL_GetTicks(); //start move timer
    int steps_to_pop = 0; //how many steps need to be popped off path
    int carry_over = 0; //how many ticks were rounded off
    int ANIMATE_SPEED = 100; //constant used to throttle animation speed
    
    //determine if need to change tile index, animate_index
    if(SDL_GetTicks() > animate_timer + 100){ //if more than 1 second has passed since last tick
        
        //Determine how many steps have to be moved while time has passed
        steps_to_pop = ( SDL_GetTicks() - animate_timer ) / ANIMATE_SPEED; //ONe tick every 100 ms
        
        //Update Animation tile index (calculate how many ticks have gone by
        temp_index = (animate_index + steps_to_pop)%4; //if above 4, wrap around... built in bounds check, should never be above 4
        animate_index = temp_index;//temp_index should be safe to write to animate_index, now
        
        //Update animate timer
        //Figure out how much time not accounted for
        carry_over = SDL_GetTicks() - animate_timer; //How much time has elapsed since last timing
        carry_over = carry_over - (steps_to_pop * ANIMATE_SPEED); //don't count the time we *have* accounted for
        //Now check new timer but also remember the time we haven't accounted for
        animate_timer = SDL_GetTicks() - carry_over; //Now update the timer, but set it back for unaccounted time
        
    }//otherwise, we won't have to do any updating
    
    //Now animate_index should be all figured out and ready to draw
    //determine the required tile
    int tile = tile_list[animate_index];
    
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { at_x*16, at_y*16, mWidth, mHeight };
    SDL_Rect* clip = NULL;
    SDL_RenderCopy( gRenderer, misc_tiles[tile], clip, &renderQuad );//Render to screen
    
    
}



//CONSTRUCTOR FOR ANIMATION_BIG
//Right now, tile_list_in must be a 4-element array of ints, speicfying seuqnece on animation
Animation_Big::Animation_Big(int xp, int yp, int tile_list_in[]){
    
    mWidth = 32;
    mHeight = 32;
    x = xp;
    y = yp;
    z = 0;
    animate_index = 0;
    tile_list[0] = tile_list_in[0];
    tile_list[1] = tile_list_in[1];
    tile_list[2] = tile_list_in[2];
    tile_list[3] = tile_list_in[3];
    
    animate_timer = SDL_GetTicks(); //start timer bitch
    
}



//Identical to the above, but draws it at a specified x,y coord)
void Animation_Big::draw(int at_x, int at_y, SDL_Renderer* gRenderer, SDL_Texture** misc_tiles){
    
    int temp_index = 0; //user to store and check the animation index before actually writing it
    
    //STNDARD-ISSUE MOVEMENT TIMING VARIABLES
    //spr1->move_timer = SDL_GetTicks(); //start move timer
    int steps_to_pop = 0; //how many steps need to be popped off path
    int carry_over = 0; //how many ticks were rounded off
    int ANIMATE_SPEED = 100; //constant used to throttle animation speed
    
    //determine if need to change tile index, animate_index
    if(SDL_GetTicks() > animate_timer + 100){ //if more than 1 second has passed since last tick
        
        //Determine how many steps have to be moved while time has passed
        steps_to_pop = ( SDL_GetTicks() - animate_timer ) / ANIMATE_SPEED; //ONe tick every 100 ms
        
        //Update Animation tile index (calculate how many ticks have gone by
        temp_index = (animate_index + steps_to_pop)%4; //if above 4, wrap around... built in bounds check, should never be above 4
        animate_index = temp_index;//temp_index should be safe to write to animate_index, now
        
        //Update animate timer
        //Figure out how much time not accounted for
        carry_over = SDL_GetTicks() - animate_timer; //How much time has elapsed since last timing
        carry_over = carry_over - (steps_to_pop * ANIMATE_SPEED); //don't count the time we *have* accounted for
        //Now check new timer but also remember the time we haven't accounted for
        animate_timer = SDL_GetTicks() - carry_over; //Now update the timer, but set it back for unaccounted time
        
    }//otherwise, we won't have to do any updating
    
    //Now animate_index should be all figured out and ready to draw
    //determine the required tile
    int tile = tile_list[animate_index];
    
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { at_x*16-8, at_y*16-8, mWidth, mHeight };
    SDL_Rect* clip = NULL;
    SDL_RenderCopy( gRenderer, misc_tiles[tile], clip, &renderQuad );//Render to screen
    
    
}

//CONSTRUCTOR FOR COLOR_ANIMATION
////Right now, tile_list_in must be a 4-element array of ints, speicfying seuqnece on animation
//construcor: input xy-pos and list of tiles to follow, and also color
Color_Animation::Color_Animation(int xp, int yp, int zp, int tile_list_in[], SDL_Color primCol, SDL_Color secoCol, SDL_Color tertCol){
    
    mWidth = 16;
    mHeight = 16;
    x = xp;
    y = yp;
    z = zp;
    animate_index = 0;
    tile_list[0] = tile_list_in[0];
    tile_list[1] = tile_list_in[1];
    tile_list[2] = tile_list_in[2];
    tile_list[3] = tile_list_in[3];
    prim_col = primCol;
    seco_col = secoCol;
    tert_col = tertCol;
    
    animate_timer = SDL_GetTicks(); //start timer bitch
    
}

//Keeps track of drawing and updating frames
void Color_Animation::draw(int at_x, int at_y , SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s, SDL_Texture** item_tiles_t, int alpha_mod){
    
    int temp_index = 0; //user to store and check the animation index before actually writing it
    
    //STNDARD-ISSUE MOVEMENT TIMING VARIABLES
    //spr1->move_timer = SDL_GetTicks(); //start move timer
    int steps_to_pop = 0; //how many steps need to be popped off path
    int carry_over = 0; //how many ticks were rounded off
    int ANIMATE_SPEED = 100; //constant used to throttle animation speed
    
    //determine if need to change tile index, animate_index
    if(SDL_GetTicks() > animate_timer + 100){ //if more than 1 second has passed since last tick
        
        //Determine how many steps have to be moved while time has passed
        steps_to_pop = ( SDL_GetTicks() - animate_timer ) / ANIMATE_SPEED; //ONe tick every 100 ms
        
        //Update Animation tile index (calculate how many ticks have gone by
        temp_index = (animate_index + steps_to_pop)%4; //if above 4, wrap around... built in bounds check, should never be above 4
        animate_index = temp_index;//temp_index should be safe to write to animate_index, now
        
        //Update animate timer
        //Figure out how much time not accounted for
        carry_over = SDL_GetTicks() - animate_timer; //How much time has elapsed since last timing
        carry_over = carry_over - (steps_to_pop * ANIMATE_SPEED); //don't count the time we *have* accounted for
        //Now check new timer but also remember the time we haven't accounted for
        animate_timer = SDL_GetTicks() - carry_over; //Now update the timer, but set it back for unaccounted time
        
    }//otherwise, we won't have to do any updating
    
    //Now animate_index should be all figured out and ready to draw
    //determine the required tile
    int tile = tile_list[animate_index];
    
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { at_x*16, at_y*16, mWidth, mHeight };
    SDL_Rect* clip = NULL;
    
    //drawing tile_p
    SDL_SetTextureColorMod( item_tiles_p[tile], prim_col.r, prim_col.g, prim_col.b); //modulate color, update to match the new one
    SDL_RenderCopy( gRenderer, item_tiles_p[tile], clip, &renderQuad );//Render to screen
    //drawing tile_ss
    if(item_tiles_s[tile] != (SDL_Texture*) 0x9999 ){
        SDL_SetTextureColorMod( item_tiles_s[tile], seco_col.r, seco_col.g, seco_col.b); //modulate color, update to match the new one
        SDL_RenderCopy( gRenderer, item_tiles_s[tile], clip, &renderQuad );//Render to screen
    }
    //Draw the tert part (if applicable)
    if(item_tiles_t[tile] != (SDL_Texture*) 0x9999 ){
        SDL_SetTextureColorMod( item_tiles_t[tile], tert_col.r, tert_col.g, tert_col.b); //no need to modulate this one
        SDL_RenderCopy( gRenderer, item_tiles_t[tile], clip, &renderQuad );//Render to screen
    }

    
}


