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

//SDL_Texture* loadTexture( string path, SDL_Renderer* gRenderer );
extern SDL_Renderer* gRenderer;//The window renderer


class Item{
public:
    Item(SDL_Texture** tiles1, SDL_Texture** tiles2);
    Item(int xpos, int ypos, int tile_type, SDL_Color primo, SDL_Color seco, string in_desc = "test description");
    Item(int xpos, int ypos, int tile_type, string in_desc = "test description");
    SDL_Color primColor; //the pirimary color of item
    SDL_Color secoColor; //the secondary color of the item
    int x,y,z; //I think we actually don't keep track of position here....
    int type; //specifies item type
    void draw(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p,SDL_Texture** item_tiles_s); //draws the item
    void draw(int x, int y, SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p,SDL_Texture** item_tiles_s); //draws the item at a specific location
    void draw(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p,SDL_Texture** item_tiles_s,SDL_Texture** item_tiles_t); //overload
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
