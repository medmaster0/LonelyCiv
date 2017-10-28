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

#include "Item.hpp"

using namespace std;

//SDL_Texture* loadTexture( string path, SDL_Renderer* gRenderer );
extern SDL_Renderer* gRenderer;//The window renderer


class Item{
public:
    Item(SDL_Texture** tiles1, SDL_Texture** tiles2);
    Item(int xpos, int ypos, int tile_type, SDL_Color primo, SDL_Color seco);
    Item(int xpos, int ypos, int tile_type);
    SDL_Color primColor; //the pirimary color of item
    SDL_Color secoColor; //the secondary color of the item
    int x,y,z;
    int type; //specifies item type
    void draw(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p,SDL_Texture** item_tiles_s); //draws the item
    void draw(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p,SDL_Texture** item_tiles_s,SDL_Texture** item_tiles_t); //overload
    
    
};

class Hat : public Item{
    
public:
    virtual void draw(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p,SDL_Texture** item_tiles_s); //draws the item
    Hat(int xpos, int ypos, int tile_type);
};

//display list of items
void displayItemList(vector<Item> item_list, SDL_Renderer* gRenderer, int SCREEN_HEIGHT);

#endif //item_hpp
