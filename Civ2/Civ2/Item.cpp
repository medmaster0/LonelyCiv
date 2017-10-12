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
// 300 - CAN
// 301 - ADOBE
// 302 - SAX
// 303 - CELLO
// 304 - BRICK
// 305 - HAT
// 306 - BOW

//static variable constructor
//This overloaded constructor sets the static class variables
//Item::Item(SDL_Texture** tiles1, SDL_Texture** tiles2){
//    SDL_Texture** Item::tilesPrim = tiles1;
//    SDL_Texture** Item::tilesSeco = tiles2;
//    printf("happpedfsa fuck fuck FFFFFFFF fuck");
//}
////initial constructor
//this overloaded constructor actually constructs an instance of object
Item::Item(int xpos, int ypos, int tile_type){
    
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

////initial constructor
//this overloaded constructor actually constructs an instance of object
Item::Item(int xpos, int ypos, int tile_type, SDL_Color primo, SDL_Color seco){
    
    //static SDL_Texture** tilesPrim = new SDL_Texture * [1]; //contains item tiles, the first version
    //static SDL_Texture** tilesSeco = new SDL_Texture * [1]; //contains item tiles, the secondary version
    //ALso load tiles
    
    x = xpos;
    y = ypos;
    type = tile_type;
    primColor = primo;
    secoColor = seco;
    
    //tilesPrim = new SDL_Texture * [1];
    
}

//draw method
void Item::draw(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p,SDL_Texture** item_tiles_s){
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x*16, y*16, 16, 16 };
    SDL_Rect* clip = NULL;
    SDL_SetTextureColorMod( item_tiles_p[type], primColor.r, primColor.g, primColor.b); //modulate color, update to match the new one
    SDL_RenderCopy( gRenderer, item_tiles_p[type], clip, &renderQuad );//Render to screen
    if(item_tiles_s[type] != (SDL_Texture*) 0x9999 ){
        SDL_SetTextureColorMod( item_tiles_s[type], secoColor.r, secoColor.g, secoColor.b); //modulate color, update to match the new one
        SDL_RenderCopy( gRenderer, item_tiles_s[type], clip, &renderQuad );//Render to screen
    }
    
}


//Overloaded draw method
void Item::draw(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p,SDL_Texture** item_tiles_s,SDL_Texture** item_tiles_t){
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x*16, y*16, 16, 16 };
    SDL_Rect* clip = nullptr;
    SDL_SetTextureColorMod( item_tiles_p[type], primColor.r, primColor.g, primColor.b); //modulate color, update to match the new one
    SDL_RenderCopy( gRenderer, item_tiles_p[type], clip, &renderQuad );//Render to screen
    if(item_tiles_s[type] != (SDL_Texture*) 0x9999 ){
        SDL_SetTextureColorMod( item_tiles_s[type], secoColor.r, secoColor.g, secoColor.b); //modulate color, update to match the new one
        SDL_RenderCopy( gRenderer, item_tiles_s[type], clip, &renderQuad );//Render to screen
    }
    //Draw the tert pic... shouldnt change color
    if(item_tiles_t[type] != (SDL_Texture*) 0x9999 ){
        SDL_RenderCopy( gRenderer, item_tiles_t[type], clip, &renderQuad );//Render to screen
        //DEBUGGING ABOVE LINE!!!:
        //item_tiles_t[type] is NOT the EXC_BAD_ACCESS...
        //gRenderer is NOT the mutha fuckin EXC_BAD_ACCESS
        //renderQuad ain't the EXC_BAD_ACCESS...
        //Must be something Else
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



