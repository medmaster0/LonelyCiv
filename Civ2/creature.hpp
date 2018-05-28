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

//#ifndef building_hpp
//#include "building.hpp"
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

class Tower; //defined else where
class Sprite{
public:
    Sprite(int xp, int yp);//Initializes variables
    void free(); //For clearing texture memory
    bool loadFromFile(std::string path1,std::string path2, int w, int h);//Load a texture from file for the sprite
    void changePrimColor(SDL_Color new_col);//changes the primary color of the spirte
    void changeSecoColor(SDL_Color new_col);//changes the secondary color of the spirte
    void draw(int in_x, int in_y, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s, SDL_Texture** item_tiles_t); //Draws the sprite to the screen at a specific MAP COORDS. Also draws items in "pose form"
    void draw_movement(int at_x, int at_y, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s, SDL_Texture** item_tiles_t); //draws the sprite to the screen at specific map coords, but items "flow" with creature movement (considers prev location)
    void draw_movement_items(int at_x, int at_y, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s, SDL_Texture** item_tiles_t); //draws the sprite's items to the screen at specific map coords, but items "flow" with creature movement (considers prev location)
    //////////////////////////////////
    //SPECIAL BALCONY DRAW FUNCTIONS
    void draw_forward_pose(int at_x, int at_y, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s, SDL_Texture** item_tiles_t); //draws the sprite to the screen at specific map coords, but items are side by side creature (considers prev location) FOR BALCONY VIEW
    void draw_forward_pose_items(int at_x, int at_y, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s, SDL_Texture** item_tiles_t, int alpha_mod = 255); //draws the sprite's items to the screen at specific map coords, but items are side by side (considers prev location) FOR BALCONY VIEW
    //END BALCONY DRAW FUNCTIONS
    //////////////////////////////////
    void drawInventory(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s, SDL_Texture** item_tiles_t); //Draw's the sprite's inventory
    //These functions position the Creature's Items after movement
    void drawHat(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s, SDL_Texture** item_tiles_t); //draaws the creature's hat (just moved up 8 px)
    void drawStaff(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s, SDL_Texture** item_tiles_t); //draaws the creatures' staff (just moved  right 16 px)
    void drawLight(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s, SDL_Texture** item_tiles_t); //draaws the creature' light (just moved opposite staff 16 px)
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
    SDL_Texture* primTexture;//primary texture of the sprite
    SDL_Texture* secoTexture;//secondary texture of the sprite
    vector<Item> inventory; //list of items the sprite is carrying
    Tower* owned_tower; //the creature's tower
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
    string task_status; //a string describing what creaure is doing...
    int target_index; //the index of the target (index of tile stack OR index of enemy for example
    //For threads
    bool inThread;
    time_t thread_timer; //will store a time stamp - used to tell if current thread should expire (quit and start doing something else)
    //For Movement
    int move_timer; //keeps track of SDL_GetTicks() for last movement
    double move_speed; //how many tiles to move per 1 second
    //For signalling from other threads
    bool isNeededByThread; //indicates sprite is needed in another thread (so not to schedule it another task)
//private:
    int r,g,b; //colors of the sprite
    //Some vars for movement timing
};

class Shroom: public Sprite{
public:
    Shroom(int xp, int yp);//Initializes variables
    vector<Resource> resource_list; //Shrooms keep track of a list of Resources
    vector<Recipe> recipe_list; //Shrooms keep track of a list of Recipes
};

//An Oversized Sprite 2x2
class Sprite2x2: public Sprite{
public:
    Sprite2x2(int xp, int yp); //Initializes variables
    SDL_Texture* tertTexture = nullptr; //These will also have a third texture
    bool loadFromFile(std::string path1,std::string path2,std::string path3, int w, int h);//Load a texture from file for the sprite
    void draw(int x, int y); //Draws the Big Creature at the coordinates, taking into consideration spatial issues
//private:
    int r2,g2,b2; //seco colors of sprite 
};


//MISC FUNCTIONS

//Create a display window that prints out the status of a creature
void displayStatus(Sprite spr1, SDL_Renderer* gRenderer, int SCREEN_HEIGHT , SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s, SDL_Texture** item_tiles_t, SDL_Texture** misc_tiles);
void drawHorde(Sprite* spr1, SDL_Renderer* gRenderer, int draw_map_x, int draw_map_y, int display_n, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s, SDL_Texture** item_tiles_t); //draw's the sprites inventory in piles on the ground

#endif /* creature_hpp */
