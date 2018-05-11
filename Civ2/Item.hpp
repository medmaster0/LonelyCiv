//
//  item.hpp
//  Civ2
//
//  Created by Medz on 8/17/17.
//  Copyright © 2017 Medz. All rights reserved.
//

#ifndef item_hpp
#define item_hpp

#include <SDL2/SDL.h>
#include <vector>
#include <string>

#include "Item.hpp"

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
// 322 - SING
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

//SDL_Texture* loadTexture( string path, SDL_Renderer* gRenderer );
extern SDL_Renderer* gRenderer;//The window renderer


class Item{
public:
    Item(SDL_Texture** tiles1, SDL_Texture** tiles2);
    Item(int xpos, int ypos, int tile_type, SDL_Color primo, SDL_Color seco, string in_desc = "test description");
    Item(int xpos, int ypos, int tile_type, string in_desc = "test description");
    SDL_Color primColor; //the pirimary color of item
    SDL_Color secoColor; //the secondary color of the item
    SDL_Color tertColor; //the tertiary color of the item (usually optional)
    int x,y,z; //I think we actually don't keep track of position here....
    int type; //specifies item type
    void draw(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p,SDL_Texture** item_tiles_s); //draws the item
    void draw(int x, int y, SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p,SDL_Texture** item_tiles_s, SDL_Texture** item_tiles_t); //draws the item at a specific location
    string description; //A Description of the item
    
};

class Hat : public Item{
public:
    virtual void draw(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p,SDL_Texture** item_tiles_s); //draws the item
    Hat(int xpos, int ypos, int tile_type);
};

class Staff : public Item{
public:
    virtual void draw(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p,SDL_Texture** item_tiles_s); //draws the item
    Staff(int xpos, int ypos, int tile_type);
};

class Light : public Item{
public:
    virtual void draw(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p,SDL_Texture** item_tiles_s); //draws the item
    Light(int xpos, int ypos, int tile_type);
};

//display list of items
void displayItemList(vector<Item> item_list, SDL_Renderer* gRenderer, int SCREEN_HEIGHT);

//MISC ITEM functions
bool isItemInList(vector<Item> item_list, int type); //determines if item of input type is in the list

#endif //item_hpp
