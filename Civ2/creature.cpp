//
//  creature.cpp
//  Civ2
//
//  Created by Medz on 8/17/17.
//  Copyright © 2017 Medz. All rights reserved.
//

#include "creature.hpp"
#include "dancez.hpp"
#include "Item.hpp"
#include <vector>
#include <iostream>

#include <SDL2/SDL.h>

using namespace std;

//////////////////
//SPRITE CLASS
//class Sprite{
//public:
//    Sprite(int xp, int yp);//Initializes variables
//    void free(); //For clearing texture memory
//    bool loadFromFile(std::string path1,std::string path2, int w, int h);//Load a texture from file for the sprite
//    void draw(); //Draws the sprite to screen using MAP COORDS
//    void moveDown();
//    void moveUp();
//    void moveRight();
//    void moveLeft();
//    void randomMove();
//    int x,y,z; //Position of sprite (MAP COORDS!)
//    vector<Item> inventory; //list of items the sprite is carrying
//    void moveTo(int x1, int y1);
//private:
//    SDL_Texture* primTexture;//primary texture of the sprite
//    SDL_Texture* secoTexture;//secondary texture of the sprite
//    int mWidth;//Image dimensions - pixels
//    int mHeight;//Image dimensions - pixels
//    int r,g,b; //colors of the sprite
//    vector<vector<int>> path; //a path to whatever target it has
//    //Some vars for movement timing
//    long int timer; //will later store a time stamp
//    int moveSpeedPeriod; //how long (ms) it takes to move 1 step
//    
//};
//Constructor
Sprite::Sprite(int xp, int yp)
{
    //Initialize
    primTexture = NULL;
    secoTexture = NULL;
    mWidth = 0;
    mHeight = 0;
    x = xp;
    y = yp;
    z = 0;
    r = rand() %255;
    g = rand() %255;
    b = rand() %255;
    
}
//Destructor
void Sprite::free(){
    if (secoTexture!=NULL){
        SDL_DestroyTexture( secoTexture );
    }
    if (primTexture!=NULL){
        SDL_DestroyTexture( primTexture );
    }
}
//Load a texture from file for the sprite
bool Sprite::loadFromFile( std::string path1, std::string path2, int w, int h )
{
    primTexture = loadTexture(path1);
    secoTexture = loadTexture(path2);
    SDL_SetTextureColorMod( secoTexture, r, g, b); //modulate color, update to match the new one
    mWidth = w;
    mHeight = h;
    return primTexture != NULL;
    
}
//Draws the sprite to screen using private MAP COORDS
void Sprite::draw()
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x*16, y*16, mWidth, mHeight };
    SDL_Rect* clip = NULL;
    SDL_RenderCopy( gRenderer, primTexture, clip, &renderQuad );//Render to screen
    SDL_RenderCopy( gRenderer, secoTexture, clip, &renderQuad );//Render to screen
}
//Move Down (1 step)
void Sprite::moveDown(){
    y = y +1;
    return;
}
//Move Up (1 step)
void Sprite::moveUp(){
    y = y -1;
    return;
}
//Move Right (1 step)
void Sprite::moveRight(){
    x=x+1;;
    return;
}
////Move Right (1 step) CHECKS BLOCKED
//void Sprite::moveRight(int* blocked, vector<vector<Item>> map_items){
//    x=x+1;;
//    return;
//}
//Move Left (1 step)
void Sprite::moveLeft(){
    x=x-1;;
    return;
}
//Move TO
void Sprite::moveTo(int x1, int y1){
    x = x1;
    y = y1;
}
//Random movement
//Sprite follows a random path
void Sprite::randomMove(){
    
    //get a new random path if need be
    if(path.empty()){
        //path = findPathToCoord(map, x, y, (rand()%map_width)-1, (rand()%map_height)-1);
        //Need to make sure it's small steps for now...
        path = findPathToCoord(map, x, y, rand()%23, rand()%23);
    }
    
    //Now move to next step in the path (if move Period has been reached
    vector<int> next = path.back();
    x = next[0];
    y = next[1];
    path.pop_back();
    SDL_Delay(100);
    
}

//Sprite's random path will be dance pattern
void Sprite::randomDance(){
    vector<int> step; //a temporary step for the path
    int curx = x; //used as a reference for finding next stepp
    int cury = y; //y'know
    
    //get a new dance if need be
    if(path.empty()){
        vector<vector<int>> routine = generateChaCha();
        for(int i = 0; i < routine.size(); i++){
            curx = curx+routine[i][0];
            cury = cury+routine[i][1];
            step = {curx,cury};  //the routine merely says where we go *relative* to where we are
            path.push_back(step);
            
        }
    }
    
    //Now move to next step in the path (if move Period has been reached
    vector<int> next = path.front();
    x = next[0];
    y = next[1];
    path.erase(path.begin());
    SDL_Delay(100);
    
}

//Draw the sprite's inventory
void Sprite::drawInventory(SDL_Renderer* gRenderer, SDL_Texture** item_tiles_p, SDL_Texture** item_tiles_s){
    for(int i = 0; i < inventory.size(); i++){
        inventory[i].x = x;
        inventory[i].y = y;
        inventory[i].draw(gRenderer, item_tiles_p, item_tiles_s);
    }
}

//END OF SPRITE CLASS
///////////////////////////





















