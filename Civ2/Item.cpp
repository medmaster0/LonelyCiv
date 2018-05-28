//
//  Item.cpp
//  Civ2
//
//  Created by Medz on 8/29/17.
//  Copyright © 2017 Medz. All rights reserved.
//

#include "Item.hpp"

#include "creature.hpp"

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

using namespace std;

///////////////
//ITEM TILE TYPES
// 0 - 99 - WEEDZ
// 100 - 199 - STONEZ
// 200 - 299 - FRUITZ
// 300 - CAN
// 301 - ADOBE TILE
// 302 - SAX
// 303 - CELLO
// 304 - BRICK
// 305 - HAT
// 306 - BOW
// 307 - STAFF
// 308 - CANDLE
// 309 - FLAG
// 310 - herb
// 311 - fruit
// 312 - stone
// 313 - BAG
// 314 - POTION
// 315 - COIN
// 316 - WINE
// 317 - CURTAINS
// 318 - LADDER
// 319 - RAILING (TOP/BOTTOM)
// 320 - RAILING (LEFT)
// 321 - RAILING (RIGHT)
// 322 - SIGN
// 323 - TOPHAT
// 324 - CROWN
// 325 - MAILBOX
// 326 - MOHAWK
// 327 - WRITING
// 328 - FLOWER
// 329 - PERFUME
// 330 - QUILL
// 331 - DAGGER
// 332 - BOMB
// 333 - CLOUD
// 334 - BLANK (a blank item used for transparent effects)
// 335 - BRICK (OUTSIDE)
// 336 - CROWN (BUNNY)
// 337 - EYEPATCH (BUNNY)
// 338 - DOOR
// 339 - WAND
// 340 - CHALICE
// 341 - BANDANA


//static variable constructor
//This overloaded constructor sets the static class variables
//Item::Item(SDL_Texture** tiles1, SDL_Texture** tiles2){
//    SDL_Texture** Item::tilesPrim = tiles1;
//    SDL_Texture** Item::tilesSeco = tiles2;
//    printf("happpedfsa fuck fuck FFFFFFFF fuck");
//}
////initial constructor
//this overloaded constructor actually constructs an instance of object
Item::Item(int xpos, int ypos, int tile_type, string in_desc){
    
    //static SDL_Texture** tilesPrim = new SDL_Texture * [1]; //contains item tiles, the first version
    //static SDL_Texture** tilesSeco = new SDL_Texture * [1]; //contains item tiles, the secondary version
    //ALso load tiles
    
    x = xpos;
    y = ypos;
    type = tile_type;
    primColor = {static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255)};
    secoColor = {static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255)};
    tertColor = {static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255)};
    description = in_desc;
    
    //tilesPrim = new SDL_Texture * [1];
    
}

////initial constructor
//this overloaded constructor actually constructs an instance of object
Item::Item(int xpos, int ypos, int tile_type, SDL_Color primo, SDL_Color seco, string in_desc){
    
    //static SDL_Texture** tilesPrim = new SDL_Texture * [1]; //contains item tiles, the first version
    //static SDL_Texture** tilesSeco = new SDL_Texture * [1]; //contains item tiles, the secondary version
    //ALso load tiles
    
    x = xpos;
    y = ypos;
    type = tile_type;
    primColor = primo;
    secoColor = seco;
    tertColor = {static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255)}; //give random tert color
    //tertColor = tert;
    description = in_desc;
    
    //tilesPrim = new SDL_Texture * [1];
    
}

//draw method
//void Item::draw(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p,SDL_Texture** item_tiles_s,SDL_Texture** item_tiles_t){
//    //Set rendering space and render to screen
//    SDL_Rect renderQuad = { x*16, y*16, 16, 16 };
//    SDL_Rect* clip = NULL;
//    //Drawing tile_p
//    //SDL_SetTextureAlphaMod( item_tiles_p[type], alpha_mod); //change the tile's alpha transparency'
//    SDL_SetTextureColorMod( item_tiles_p[type], primColor.r, primColor.g, primColor.b); //modulate color, update to match the new one
//    SDL_RenderCopy( gRenderer, item_tiles_p[type], clip, &renderQuad );//Render to screen
//    //SDL_SetTextureAlphaMod( item_tiles_p[type], 255); //change the tile's alpha transparency' back to normal
//    //drawing tile_s
//    if(item_tiles_s[type] != (SDL_Texture*) 0x9999 ){
//        //SDL_SetTextureAlphaMod( item_tiles_s[type], alpha_mod); //change the tile's alpha transparency'
//        SDL_SetTextureColorMod( item_tiles_s[type], secoColor.r, secoColor.g, secoColor.b); //modulate color, update to match the new one
//        SDL_RenderCopy( gRenderer, item_tiles_s[type], clip, &renderQuad );//Render to screen
//        //SDL_SetTextureAlphaMod( item_tiles_s[type], 255); //change the tile's alpha transparency' back to normal
//    }
//    if(item_tiles_t[type] != (SDL_Texture*) 0x9999 ){
//        //SDL_SetTextureAlphaMod( item_tiles_t[type], alpha_mod); //change the tile's alpha transparency'
//        SDL_SetTextureColorMod( item_tiles_t[type], tertColor.r, tertColor.g, tertColor.b); //modulate color, update to match the new one
//        SDL_RenderCopy( gRenderer, item_tiles_t[type], clip, &renderQuad );//Render to screen
//        //SDL_SetTextureAlphaMod( item_tiles_t[type], 255); //change the tile's alpha transparency' back to normal
//    }
//    
//}

//draw method at specific location
void Item::draw(int in_x, int in_y, SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p,SDL_Texture** item_tiles_s,SDL_Texture** item_tiles_t){
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { in_x*16, in_y*16, 16, 16 };
    SDL_Rect* clip = NULL;
    //drawing tile_p
    SDL_SetTextureColorMod( item_tiles_p[type], primColor.r, primColor.g, primColor.b); //modulate color, update to match the new one
    SDL_RenderCopy( gRenderer, item_tiles_p[type], clip, &renderQuad );//Render to screen
    //drawing tile_ss
    if(item_tiles_s[type] != (SDL_Texture*) 0x9999 ){
        SDL_SetTextureColorMod( item_tiles_s[type], secoColor.r, secoColor.g, secoColor.b); //modulate color, update to match the new one
        SDL_RenderCopy( gRenderer, item_tiles_s[type], clip, &renderQuad );//Render to screen
    }
    
    //Draw the tert part (if applicable)
    if(item_tiles_t[type] != (SDL_Texture*) 0x9999 ){
        SDL_SetTextureColorMod( item_tiles_t[type], tertColor.r, tertColor.g, tertColor.b); //no need to modulate this one
        SDL_RenderCopy( gRenderer, item_tiles_t[type], clip, &renderQuad );//Render to screen
    }
    
}


//Create a display window that prints out a list of items...
//globals, will later be tweaked by main game loop... dunno if that's a good idea but fuck
extern int item_display_index; //keeps track of inventory segment to draw.
SDL_Surface * item_display_surface; //used in drawing
SDL_Texture * item_display_texture; //
//TTF_Font * item_font; //item font
extern TTF_Font * font1; //item font
SDL_Color item_font_clr = {255,255,255}; //color of font
int item_texW = 0;//constants used in displaying fonts
int item_texH = 0;//constants used in displaying fonts
//interface
extern int counterInventory;
void displayItemList(vector<Item> item_list, SDL_Renderer* gRenderer, int SCREEN_HEIGHT){
    
    string item_text = "inv: "; //char buffer that is displayed
    item_text = item_text + std::to_string(item_list.size()) + " items\n";
    //Black out Box {x, y, w, h}
    SDL_Rect r={0,SCREEN_HEIGHT,150,-150};
    SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 255 ); //color of box
    SDL_RenderFillRect( gRenderer, &r ); //copy box to renderer
    item_display_surface = TTF_RenderText_Blended_Wrapped(font1, item_text.c_str(), item_font_clr, 125);
    item_display_texture = SDL_CreateTextureFromSurface(gRenderer, item_display_surface);
    //Create text rect (text box)
    SDL_QueryTexture(item_display_texture, NULL, NULL, &item_texW, &item_texH); //This get's the dimensions of the font/text
    SDL_Rect dstrect = { 0, SCREEN_HEIGHT+r.h, item_texW, item_texH }; //so we can make the proper rect to display it
    SDL_RenderCopy(gRenderer, item_display_texture, NULL, &dstrect); //write
    //Free Memory!!!
    SDL_DestroyTexture(item_display_texture);
    SDL_FreeSurface(item_display_surface);
    ////////////FINISH PUTTING THE TITLE: inventory
    
    //If list is empty, just return;
    if(item_list.size()<=0){
        //std::cout << "ya bastard..";
        return;
    }
    
    //START DRAWING OUT ITEM TEXT
    std::string item_name;
    int item_index; //keeps track of what item in inventory we are actually printing - depends on two factors, down below
    SDL_Color item_color1; //The prim color of the item
    SDL_Color item_color2; //The seco color of the item
    //Bounds checking of cursor
    if(item_display_index<0){item_display_index = item_list.size()-1;}
    if(item_display_index>item_list.size()-1){ item_display_index = 0;}
    //We'll also reuse some of the previously used objects
    for(int u = 0; u<7; u++){ //cycle through however many elements you wanna fit on screen
        item_index = u+item_display_index; //Allow the user to control the scroll by referencing global counterInventory
        if(item_index >= item_list.size()) {//check if sprite's inventory index is out of bounds
            break;
        }
        
                //Need to:
                //Determine item type
                //  Make string for item_type
                //Determine color of item
                //  Make color of item
                //make surface with string, color, and pre-created font
                //make texture from surface
                //Query texture for dimensions
                //create rectangle at correct position and with determined dimensions
                //copy onto renderer
        
        //Determine item type
        if(item_list[item_index].type<100){ //Then it's a weed
            item_name = "    weed";
        }else if(item_list[item_index].type<200){ //Then it's a stone
            item_name = "    stone";
        }else if(item_list[item_index].type<300){ //Then it's a fruit
            item_name = "    fruit";
        }else if(item_list[item_index].type==300){ //Then it's a stone
            item_name = "    can";
        }else if(item_list[item_index].type==301){ //Then it's a stone
            item_name = "    adobe";
        }else if(item_list[item_index].type==302){ //Then it's a stone
            item_name = "    sax";
        }else if(item_list[item_index].type==303){ //Then it's a stone
            item_name = "    cello";
        }else if(item_list[item_index].type==304){ //Then it's a stone
            item_name = "    brick";
        }else if(item_list[item_index].type==305){ //Then it's a stone
            item_name = "    hat";
        }else if(item_list[item_index].type==306){ //Then it's a stone
            item_name = "    bow";
        }else if(item_list[item_index].type==307){ //Then it's a stone
            item_name = "    staff";
        }else if(item_list[item_index].type==310){ //Then it's a stone
            item_name = "    weed";
        }else if(item_list[item_index].type==311){ //Then it's a stone
            item_name = "    fruit";
        }else if(item_list[item_index].type==312){ //Then it's a stone
            item_name = "    stone";
        }else if(item_list[item_index].type==313){ //Then it's a stone
            item_name = "    powder";
        }else if(item_list[item_index].type==314){ //Then it's a stone
            item_name = "    potion";
        }else if(item_list[item_index].type==315){ //Then it's a stone
            item_name = "    coin";
        }else if(item_list[item_index].type==316){ //Then it's a stone
            item_name = "    wine";
//        }else if(item_list[item_index].type==317){ //Then it's a stone
//            item_name = "    curtains";
//        }else if(item_list[item_index].type==318){ //Then it's a stone
//            item_name = "    ladder";
        }else if(item_list[item_index].type==327){ //Then it's a stone
            item_name = "    writing";
        }else if(item_list[item_index].type==328){ //Then it's a stone
            item_name = "    flower";
        }else if(item_list[item_index].type==329){ //Then it's a stone
            item_name = "    perfume";
        }else if(item_list[item_index].type==330){ //Then it's a stone
            item_name = "    quill";
        }else if(item_list[item_index].type==331){ //Then it's a stone
            item_name = "    dagger";
        }else if(item_list[item_index].type==332){ //Then it's a stone
            item_name = "    bomb";
        }else if(item_list[item_index].type==339){ //Then it's a stone
            item_name = "    staff";
        }else if(item_list[item_index].type==340){ //Then it's a stone
            item_name = "    chalice";
        }
    
        //Determine Color
        item_color1 = item_list[item_index].primColor;
        item_color2 = item_list[item_index].secoColor;
        //Make surface and texture
        item_display_surface = TTF_RenderText_Blended_Wrapped(font1, item_name.c_str(), {255,255,255}, 125);
        item_display_texture = SDL_CreateTextureFromSurface(gRenderer, item_display_surface);
        //Make the destination rectangle
        SDL_QueryTexture(item_display_texture, NULL, NULL, &item_texW, &item_texH); //This get's the dimensions of the font/text
        dstrect = { 0, SCREEN_HEIGHT+r.h + 15+(u*15), item_texW, item_texH }; //so we can make the proper rect to display it
        //Render to screen
        SDL_RenderCopy(gRenderer, item_display_texture, NULL, &dstrect); //write
        
        //Free Memory!!!
        SDL_DestroyTexture(item_display_texture);
        SDL_FreeSurface(item_display_surface);
        
        //std::cout << "tdf" << item_name;
        
        //Also draw rects for the colors
        SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND); //temporarily allow alpha
        SDL_Rect colR ={0,SCREEN_HEIGHT+r.h+15+(u*15),7, item_texH};
        SDL_SetRenderDrawColor( gRenderer, item_color1.r , item_color1.g, item_color1.b , item_color1.a); //color of box
        SDL_RenderFillRect( gRenderer, &colR ); //copy box to renderer
        //AAAnd second color
        colR ={7,SCREEN_HEIGHT+r.h+15+(u*15),7, item_texH};
        SDL_SetRenderDrawColor( gRenderer, item_color2.r , item_color2.g, item_color2.b , item_color2.a); //color of box
        SDL_RenderFillRect( gRenderer, &colR ); //copy box to renderer
        SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE); //turn alpha back off
    }
}

//MISC ITEM functions

//determines if item of input type is in the list
bool isItemInList(vector<Item> item_list, int in_type){
    
    for(int i = 0; i < item_list.size(); i++){
        if(item_list[i].type == in_type ){
            return true; //return true
        }
    }
    
    return false; //if still havene't found it
    
}





//////////////////
//Hat CLASS

Hat::Hat(int xpos, int ypos, int tile_type) : Item(xpos, ypos, tile_type){
    
    //static SDL_Texture** tilesPrim = new SDL_Texture * [1]; //contains item tiles, the first version
    //static SDL_Texture** tilesSeco = new SDL_Texture * [1]; //contains item tiles, the secondary version
    //ALso load tiles
    
    x = xpos;
    y = ypos;
    type = tile_type;
    primColor = {static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255)};
    secoColor = {static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255)};
    
    //tilesPrim = new SDL_Texture * [1];
    
}

//draw the hat ( slightly off grid )
void Hat::draw(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s, SDL_Texture** item_tiles_t, int alpha_mod ){
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x*16, y*16-8, 16, 16 };
    SDL_Rect* clip = NULL;
    SDL_SetTextureAlphaMod( item_tiles_p[type], alpha_mod); //change the tile's alpha transparency'
    SDL_SetTextureColorMod( item_tiles_p[type], primColor.r, primColor.g, primColor.b); //modulate color, update to match the new one
    SDL_RenderCopy( gRenderer, item_tiles_p[type], clip, &renderQuad );//Render to screen
    SDL_SetTextureAlphaMod( item_tiles_p[type], 255); //change the tile's alpha transparency' back to normal
    if(item_tiles_s[type] != (SDL_Texture*) 0x9999 ){
        SDL_SetTextureAlphaMod( item_tiles_s[type], alpha_mod); //change the tile's alpha transparency'
        SDL_SetTextureColorMod( item_tiles_s[type], secoColor.r, secoColor.g, secoColor.b); //modulate color, update to match the new one
        SDL_RenderCopy( gRenderer, item_tiles_s[type], clip, &renderQuad );//Render to screen
        SDL_SetTextureAlphaMod( item_tiles_s[type], 255); //change the tile's alpha transparency' back to normal
    }
    if(item_tiles_t[type] != (SDL_Texture*) 0x9999 ){
        SDL_SetTextureAlphaMod( item_tiles_t[type], alpha_mod); //change the tile's alpha transparency'
        SDL_SetTextureColorMod( item_tiles_t[type], tertColor.r, tertColor.g, tertColor.b); //modulate color, update to match the new one
        SDL_RenderCopy( gRenderer, item_tiles_t[type], clip, &renderQuad );//Render to screen
        SDL_SetTextureAlphaMod( item_tiles_t[type], 255); //change the tile's alpha transparency' back to normal
    }
    
}

//////////////////////
//Staff CLASS

Staff::Staff(int xpos, int ypos, int tile_type) : Item(xpos, ypos, tile_type){
    
    x = xpos;
    y = ypos;
    type = tile_type;
    primColor = {static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255)};
    secoColor = {static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255)};
    
}

//draw the staff ( slightly off grid )
void Staff::draw(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s, SDL_Texture** item_tiles_t, int alpha_mod ){
    
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x*16, y*16, 16, 16 };
    SDL_Rect* clip = NULL;
    SDL_SetTextureAlphaMod( item_tiles_p[type], alpha_mod); //change the tile's alpha transparency'
    SDL_SetTextureColorMod( item_tiles_p[type], primColor.r, primColor.g, primColor.b); //modulate color, update to match the new one
    SDL_RenderCopy( gRenderer, item_tiles_p[type], clip, &renderQuad );//Render to screen
    SDL_SetTextureAlphaMod( item_tiles_p[type], 255); //change the tile's alpha transparency' back to normal
    if(item_tiles_s[type] != (SDL_Texture*) 0x9999 ){
        SDL_SetTextureAlphaMod( item_tiles_s[type], alpha_mod); //change the tile's alpha transparency'
        SDL_SetTextureColorMod( item_tiles_s[type], secoColor.r, secoColor.g, secoColor.b); //modulate color, update to match the new one
        SDL_RenderCopy( gRenderer, item_tiles_s[type], clip, &renderQuad );//Render to screen
        SDL_SetTextureAlphaMod( item_tiles_s[type], 255); //change the tile's alpha transparency' back to normal
    }
    if(item_tiles_t[type] != (SDL_Texture*) 0x9999 ){
        SDL_SetTextureAlphaMod( item_tiles_t[type], alpha_mod); //change the tile's alpha transparency'
        SDL_SetTextureColorMod( item_tiles_t[type], tertColor.r, tertColor.g, tertColor.b); //modulate color, update to match the new one
        SDL_RenderCopy( gRenderer, item_tiles_t[type], clip, &renderQuad );//Render to screen
        SDL_SetTextureAlphaMod( item_tiles_t[type], 255); //change the tile's alpha transparency' back to normal
    }
}

//////////////////////
//Light CLASS

Light::Light(int xpos, int ypos, int tile_type) : Item(xpos, ypos, tile_type){
    
    x = xpos;
    y = ypos;
    type = tile_type;
    primColor = {static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255)};
    secoColor = {static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255)};
    
}

//draw the staff ( slightly off grid )
void Light::draw(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s ,SDL_Texture** item_tiles_t, int alpha_mod){
    
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x*16, y*16, 16, 16 };
    SDL_Rect* clip = NULL;
    SDL_SetTextureAlphaMod( item_tiles_p[type], alpha_mod); //change the tile's alpha transparency'
    SDL_SetTextureColorMod( item_tiles_p[type], primColor.r, primColor.g, primColor.b); //modulate color, update to match the new one
    SDL_RenderCopy( gRenderer, item_tiles_p[type], clip, &renderQuad );//Render to screen
    SDL_SetTextureAlphaMod( item_tiles_p[type], 255); //change the tile's alpha transparency' back to normal
    if(item_tiles_s[type] != (SDL_Texture*) 0x9999 ){
        SDL_SetTextureAlphaMod( item_tiles_s[type], alpha_mod); //change the tile's alpha transparency'
        SDL_SetTextureColorMod( item_tiles_s[type], secoColor.r, secoColor.g, secoColor.b); //modulate color, update to match the new one
        SDL_RenderCopy( gRenderer, item_tiles_s[type], clip, &renderQuad );//Render to screen
        SDL_SetTextureAlphaMod( item_tiles_s[type], 255); //change the tile's alpha transparency' back to normal
    }
    if(item_tiles_t[type] != (SDL_Texture*) 0x9999 ){
        SDL_SetTextureAlphaMod( item_tiles_t[type], alpha_mod); //change the tile's alpha transparency'
        SDL_SetTextureColorMod( item_tiles_t[type], tertColor.r, tertColor.g, tertColor.b); //modulate color, update to match the new one
        SDL_RenderCopy( gRenderer, item_tiles_t[type], clip, &renderQuad );//Render to screen
        SDL_SetTextureAlphaMod( item_tiles_t[type], 255); //change the tile's alpha transparency' back to normal
    }
}





