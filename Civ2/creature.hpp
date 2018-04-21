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
#include "recipes.hpp"

//#ifndef item_hpp
#include "Item.hpp"
//#endif

#include <vector>
#include <string>
#include <time.h>
using namespace std;

extern vector<vector<int> > map;
extern int map_width;
extern int map_height;

extern SDL_Renderer* gRenderer;//The window renderer

SDL_Texture* loadTexture( std::string path ); //loads a texture specified by string path name... gets implemented in main function since it requires a gRenderer to be declared
vector<vector<int>> findPathToCoord(vector<vector<int>>, int x1, int y1, int x2, int y2);

class Sprite{
public:
    Sprite(int xp, int yp);//Initializes variables
    void free(); //For clearing texture memory
    bool loadFromFile(std::string path1,std::string path2, int w, int h);//Load a texture from file for the sprite
    void draw(); //Draws the sprite to screen using stored MAP COORDS
    void draw(int in_x, int in_y, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s); //Draws the sprite to the screen at a specific MAP COORDS
    void drawInventory(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s); //Draw's the sprite's inventory
    void drawHat(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s); //draaws the creature's hat (just moved up 8 px)
    void drawStaff(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s); //draaws the creatures' staff (just moved  right 16 px)
    void drawLight(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s); //draaws the creature' light (just moved opposite staff 16 px)
    void moveDown();
    void moveUp();
    void moveRight();
    void moveLeft();
    void randomDance();
    string name; //the name of the creature
    int x,y,z; //Position of sprite (MAP COORDS!)
    int prev_x, prev_y, prev_z; //the former pposition of the sprite
    int mWidth;//Image dimensions - pixels
    int mHeight;//Image dimensions - pixels
    vector<Item> inventory; //list of items the sprite is carrying
    SDL_Color faveColor; //the sprite's favorite color
    SDL_Color faveColor2; //the sprite's alternative favorite color
    int zodiac_sign; //code for the creature's zodiac symbol
    int zodiac_element; //code for the creature's zodiac element
    Hat* hat; //a single item containing the sprite's hat
    Staff* staff; //a single item containing the sprite's staff
    Light* light; //a single item containign the sprites light
    void moveTo(int x1, int y1); //moves to a specific (x,y) coord
    //Targeting stuff, below - hopefully GENERAL PURPOSE
    vector<vector<int>> path; //a path to whatever target it has
    int target_index; //the index of the target (index of tile stack OR index of enemy for example
    //For threads
    bool inThread;
    time_t thread_timer; //will store a time stamp - used to tell if current thread should expire (quit and start doing something else)
    //For Movement
    int move_timer; //keeps track of SDL_GetTicks() for last movement
    double move_speed; //how many tiles to move per 1 second
    //For signalling from other threads
    bool isNeededByThread; //indicates sprite is needed in another thread (so not to schedule it another task)
private:
    SDL_Texture* primTexture;//primary texture of the sprite
    SDL_Texture* secoTexture;//secondary texture of the sprite
    int r,g,b; //colors of the sprite
    //Some vars for movement timing
};

class Shroom: public Sprite{
public:
    Shroom(int xp, int yp);//Initializes variables
    vector<Resource> resource_list; //Shrooms keep track of a list of Resources
};


//MISC FUNCTIONS

//Create a display window that prints out the status of a creature
void displayStatus(Sprite spr1, SDL_Renderer* gRenderer, int SCREEN_HEIGHT , SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s, SDL_Texture** misc_tiles);

#endif /* creature_hpp */
