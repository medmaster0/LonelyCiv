//
//  creature.hpp
//  Civ2
//
//  Created by Medz on 8/17/17.
//  Copyright © 2017 Medz. All rights reserved.
//

#ifndef creature_hpp
#define creature_hpp

#include <stdio.h>
#include <SDL2/SDL.h>

#include "creature.hpp"

//#ifndef item_hpp
#include "Item.hpp"
//#endif

#include <vector>
using namespace std;

extern vector<vector<int> > map;
extern int map_width;
extern int map_height;

extern SDL_Renderer* gRenderer;//The window renderer

SDL_Texture* loadTexture( std::string path );
vector<vector<int>> findPathToCoord(vector<vector<int>>, int x1, int y1, int x2, int y2);

class Sprite{
public:
    Sprite(int xp, int yp);//Initializes variables
    void free(); //For clearing texture memory
    bool loadFromFile(std::string path1,std::string path2, int w, int h);//Load a texture from file for the sprite
    void draw(); //Draws the sprite to screen using MAP COORDS
    void drawInventory(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s); //Draw's the sprite's inventory
    void drawHat(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s); //draaws the items' hat (just moved up 8 px)
    void moveDown();
    void moveUp();
    void moveRight();
    void moveLeft();
    void randomDance();
    vector<vector<int>> path; //a path to whatever target it has
    int x,y,z; //Position of sprite (MAP COORDS!)
    vector<Item> inventory; //list of items the sprite is carrying
    Hat* hat; //a single item containing the sprites hat
    void moveTo(int x1, int y1);
private:
    SDL_Texture* primTexture;//primary texture of the sprite
    SDL_Texture* secoTexture;//secondary texture of the sprite
    int mWidth;//Image dimensions - pixels
    int mHeight;//Image dimensions - pixels
    int r,g,b; //colors of the sprite
    //Some vars for movement timing
    long int timer; //will later store a time stamp
    int moveSpeedPeriod; //how long (ms) it takes to move 1 step
    
};

#endif /* creature_hpp */
