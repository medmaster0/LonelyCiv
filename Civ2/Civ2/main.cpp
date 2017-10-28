/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
 and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <time.h>   
#include <algorithm>
#include <thread>

#include "dancez.hpp"
#include "creature.hpp"
#include "Item.hpp"
#include "tent.hpp"
#include "med_algo.hpp"
//#include <unistd.h>
/* time */

using std::vector;

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 480;
//const int SCREEN_WIDTH = 32;
//const int SCREEN_HEIGHT = 32;

//TileMap Stuff
int map_width;
int map_height;
SDL_Color back_col = {0,0,0}; //The background color
vector<vector<int>> colorz; //every randomly gen. resource (stone, weed) has it's own unique color
int blockable[2] = {301,304};
bool* block_map; //this is the map indicating whether a tile is blocked

//ItemTile Stuff
SDL_Texture** item_tiles_p; //primo item tiles
SDL_Texture** item_tiles_s; //seco item tiles
SDL_Texture** item_tiles_t; //terto item tiles
vector<SDL_Color> world_colors; //array of sdl_colors
vector<vector<Item>> map_items; //a list of list of items on a tile. Index corresponds to [y*map_width + x]

//Tent Stuff
SDL_Texture** tent_tiles_p; //primo tent tiles
SDL_Texture** tent_tiles_s; //seco tent tiles
vector<vector<Tent>> map_tents; //a list of list of tents on a tile. Index correspons to [y*map_width + x]

//Workshop Stuff
SDL_Texture** workshop_tiles_p; //primo tent tiles
SDL_Texture** workshop_tiles_s; //seco tent tiles
vector<vector<Workshop>> map_workshops; //a list of list of tents on a tile. Index correspons to [y*map_width + x]

//SDL Stuff
SDL_Window* gWindow = NULL;//The window we'll be rendering to
SDL_Renderer* gRenderer = NULL;//The window renderer

//TTF STUFF
TTF_Font * font1; //menu font
SDL_Color font1_clr = {255,255,255}; //color of font
int texW = 0;//constants used in displaying fonts
int texH = 0;//constants used in displaying fonts

//Interface Windows flags and vars
//These are for control of what "windows" are displayed
bool inventoryDisplayOn = false;
int item_display_index = 0; //counter used with cursors for inventory pos

/////////////////////////////////////////////////////////
////FUNCTIONS BEGIN////////////////////////////////////////
////////////////////////////////////////////////////////

//initialize screen
bool initScreen()
{
    //Initialization flag
    bool success = true;
    
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }
        
        //Create window
        gWindow = SDL_CreateWindow( "LONELELY CIV", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                
                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
            }
        }
    }
    
    //Initialize TTF Stuff
    if(TTF_Init()==-1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }
    font1 = TTF_OpenFont("Civ2/Civ2/DwarfFortressVan.ttf", 15);
    
    
    return success;
}

//Load a texture from a file path string
SDL_Texture* loadTexture( std::string path )
{
    //The final texture
    SDL_Texture* newTexture = NULL;
    
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
        
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    
    //Return success
    return newTexture;
}

//Load in al the tiles used for the map
void loadTiles(){
    
    item_tiles_p = new SDL_Texture* [307];
    item_tiles_s = new SDL_Texture* [307];
    item_tiles_t = new SDL_Texture* [307];
    tent_tiles_p = new SDL_Texture* [100];
    tent_tiles_s = new SDL_Texture* [100];
    workshop_tiles_p = new SDL_Texture* [100];
    workshop_tiles_s = new SDL_Texture* [100];
    //world_colors = new SDL_Color [200];
    SDL_Color temp_col;
    
    //ITEM TILES
    for(int i = 0 ; i < 100; i++){

        int r,g,b;
        r = rand()%255;
        g = rand()%255;
        b = rand()%255;
        colorz.push_back({r,g,b});
        
        //AS WE INTEGRATE THESE "MATERIALS" INTO ITEMS, WE ALSO UPDATE THOSE LISTS
        item_tiles_p[i] = loadTexture("Civ2/Civ2/weedz/"+std::to_string(i)+".png");
        item_tiles_s[i] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
        item_tiles_t[i] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
        temp_col = {static_cast<Uint8>(r),static_cast<Uint8>(g),static_cast<Uint8>(b)};
        //world_colors[i] = temp_col;
        world_colors.push_back(temp_col);
    }
    for(int i = 100 ; i < 200; i++){
        int r,g,b;
        r = rand()%255;
        g = rand()%255;
        b = rand()%255;
        colorz.push_back({r,g,b});
        
        //AS WE INTEGRATE THESE "MATERIALS" INTO ITEMS, WE ALSO UPDATE THOSE LISTS
        item_tiles_p[i] = loadTexture("Civ2/Civ2/stonez/"+std::to_string(i)+".png");
        item_tiles_s[i] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
        item_tiles_t[i] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
//        temp_col = {static_cast<Uint8>(r),static_cast<Uint8>(g),static_cast<Uint8>(b)};
//        world_colors[i] = temp_col;
        world_colors.push_back(temp_col);
        
    }

    item_tiles_p[300] = loadTexture("Civ2/Civ2/tiles/canPrim.png");
    item_tiles_s[300] = loadTexture("Civ2/Civ2/tiles/canSeco.png");
    item_tiles_t[300] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
    
    item_tiles_p[301] = loadTexture("Civ2/Civ2/tiles/adobePrim.png");
    item_tiles_s[301] = loadTexture("Civ2/Civ2/tiles/adobeSeco.png");
    item_tiles_t[301] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color

    item_tiles_p[302] = loadTexture("Civ2/Civ2/tiles/saxPrim.png");
    item_tiles_s[302] = loadTexture("Civ2/Civ2/tiles/saxSeco.png");
    item_tiles_t[302] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color

    
    item_tiles_p[303] = loadTexture("Civ2/Civ2/tiles/celloPrim.png");
    item_tiles_s[303] = loadTexture("Civ2/Civ2/tiles/celloSeco.png");
    item_tiles_t[303] = loadTexture("Civ2/Civ2/tiles/celloTert.png");
    
    item_tiles_p[304] = loadTexture("Civ2/Civ2/tiles/brickPrim.png");
    item_tiles_s[304] = loadTexture("Civ2/Civ2/tiles/brickSeco.png");
    item_tiles_t[304] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
    
    item_tiles_p[305] = loadTexture("Civ2/Civ2/tiles/capPrim.png");
    item_tiles_s[305] = loadTexture("Civ2/Civ2/tiles/capSeco.png");
    item_tiles_t[305] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
    
    item_tiles_p[306] = loadTexture("Civ2/Civ2/tiles/bow2Prim.png");
    item_tiles_s[306] = loadTexture("Civ2/Civ2/tiles/bow2Seco.png");
    item_tiles_t[306] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
    
    //TENT TILES
    tent_tiles_p[0] = loadTexture("Civ2/Civ2/tiles/tent0Prim.png");
    tent_tiles_s[0] = loadTexture("Civ2/Civ2/tiles/tent0Seco.png");
    
    //WORKSHOP TILES
    workshop_tiles_p[0] = loadTexture("Civ2/Civ2/doodadz/spinwhelPrim.png");
    workshop_tiles_s[0] = loadTexture("Civ2/Civ2/doodadz/spinwhelSeco.png");

    workshop_tiles_p[1] = loadTexture("Civ2/Civ2/doodadz/screwpressPrim.png");
    workshop_tiles_s[1] = loadTexture("Civ2/Civ2/doodadz/screwpressSeco.png");
}

//Generate tilez
//Call external python script,
//Then load the generated .png files
void generateTilez(){
    system("python Civ2/Civ2/weedz/WEEDZ.py");
    system("python Civ2/Civ2/weedz/outline.py");
    system("python Civ2/Civ2/stonez/STONEZ.py");
    system("python Civ2/Civ2/stonez/outline.py");
    system("python Civ2/Civ2/doodadz/spinwhel2.py");
    system("python Civ2/Civ2/doodadz/screwpress.py");
    system("python Civ2/Civ2/doodadz/outline.py");

}

//Print out a tile-based map give a vector 2D array
void drawVectorMap(){
    //Clear screen
    SDL_SetRenderDrawColor( gRenderer,back_col.r, back_col.g, back_col.b, back_col.a );
    SDL_RenderClear( gRenderer );

}

//////DEBUG OR SOMEHTING LIKE IT
//generates the initial items on the map
void init_items(){
    
    //Calculate map (grid) dimensions
    map_width = SCREEN_WIDTH/16;
    map_height = SCREEN_HEIGHT/16;

    //Initialize our dimension-indexed arrays based on map dimensions
    map_items.resize(map_width*map_height);
    map_tents.resize(map_width*map_height);
    map_workshops.resize(map_width*map_height);
    //colors used throughout
    SDL_Color p1;
    SDL_Color s1;
    
    //we're going to create 300 random items (basic first kind)
    //we have to generate items, and also add them to the correct element in the array
    int tempx; //temporary x
    int tempy; //temporary y
    int temp_tile; //temporarily stores random tile index
    
    //Create some random items!
    for(int i = 0; i < 450; i++){
        tempx = rand()%(map_width);
        tempy = rand()%(map_height);
        temp_tile = rand()%199;
        //Item temp_item = Item(tempx, tempy, temp_tile, world_colors[temp_tile],{255,255,255,0} ); //temporary item
        Item temp_item = Item(tempx, tempy, temp_tile, {static_cast<Uint8>(colorz[temp_tile][0]), static_cast<Uint8>(colorz[temp_tile][1]), static_cast<Uint8>(colorz[temp_tile][2]),255},{255,255,255,0} ); //temporary item
        map_items[(tempy)*map_width+(tempx)].push_back(temp_item);
        //printf("cc %d %d cc", tempx, tempy);
        //printf("cc %d %d cc", map_items[(tempy-1)*map_width+(tempx)].back().x, temp_item.x);
        
    }
    
    for(int b = 0 ; b<50; b++){
        tempx = rand()%(map_width);
        tempy = rand()%(map_height);
        if(rand()%2 == 1){
            temp_tile = 305; //+ (rand()%2);
        }else{
            temp_tile=306;
        }
        //temp_tile = rand()%7 + 300;
        //if(temp_tile == 301 || temp_tile == 304){temp_tile = 305;}
        Item temp_item = Item(tempx, tempy, temp_tile, {static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), 255},{static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255),255} ); //temporary item
        map_items[(tempy)*map_width+(tempx)].push_back(temp_item);
    }
    
    block_map = new bool[map_width*map_height](); //initialize a dynamically sized blocked map
    //Make a brick maze
    vector<vector<short>> gmap = gen_maze_corridor(30,70);
    printMaze(gmap);
    int con_x = 20;//the starting point of construction
    int con_y = 10;//the starting point of construction
    //redefine color
    p1 = {static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255)};
    s1 = {static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255)};
    //BASIC RECIPE FOR MAKING A MAZE OUT OF PHYSICAL ITEMS
    //(Switch i and j for rotation)
    for(int j = 0 ; j < gmap.size(); j++){ //the ROWS of
        for(int i = 0; i<gmap[j].size();i++){ //the COLS of
//    for(int i = 0 ; i < gmap.size(); i++){ //the ROWS of
//        for(int j = 0; j<gmap[i].size();j++){ //the COLS of
    
            tempx = i + con_x;
            if(tempx >= map_width){continue;}
            tempy = j + con_y;
            if( tempy >= map_height){continue;}
            //Now check if the space is empty or wall
            if(gmap[j][i] == 1){
                printf("(%d,%d)",tempx, tempy);
                //then we need a wall here
                Item con_item = Item(tempx, tempy, 304, p1, s1);
                map_items[(tempy*map_width)+tempx].push_back(con_item);
                block_map[(tempy*map_width)+tempx] = true;
            }
            
        }
    }
    
    
    //for testing inventory, specific tile...
    for(int b = 0 ; b<450; b++){
        tempx = 14;
        tempy = 14;
        if(rand()%2 == 1){
            temp_tile = 305; //+ (rand()%2);
        }else{
            temp_tile=306;
        }
        Item temp_item = Item(tempx, tempy, temp_tile, {static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), 255},{static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255),255} ); //temporary item
        map_items[(tempy)*map_width+(tempx)].push_back(temp_item);
    }
    
    //Make some tents
    tempx = 23;
    tempy = 8;
    //Tent temp_tent = Tent(tempx, tempy, 0);
    Tent temp_tent = Tent(tempx, tempy, 0, {static_cast<Uint8>(127+rand()%127), static_cast<Uint8>(127+rand()%127), static_cast<Uint8>(127+rand()%127), 255},{static_cast<Uint8>(127+rand()%127), static_cast<Uint8>(127+rand()%127), static_cast<Uint8>(127+rand()%127),255});
    map_tents[(tempy)*map_width+(tempx)].push_back(temp_tent);
    
    //Make some workshops
    tempx = 25;
    tempy = 8;
    Workshop temp_workshop = Workshop(tempx, tempy, 0);
    map_workshops[(tempy)*map_width+(tempx)].push_back(temp_workshop);
    
    tempx = 27;
    tempy = 8;
    temp_workshop = Workshop(tempx, tempy, 1);
    map_workshops[(tempy)*map_width+(tempx)].push_back(temp_workshop);
    
    
}
//Draws all the items
void draw_items(){
    //You know what to do
    for(int i = 0; i < map_items.size(); i++){
        for(int j = 0 ; j < map_items[i].size(); j++){
            map_items[i][j].draw(gRenderer, item_tiles_p, item_tiles_s, item_tiles_t);
        }
    }
    
    //Draw all tents
    for(int i = 0; i < map_tents.size(); i++){
        for(int j = 0 ; j < map_tents[i].size(); j++){
            map_tents[i][j].draw(gRenderer, tent_tiles_p, tent_tiles_s);
        }
    }
    
    //Draw all workshops
    for(int i = 0; i < map_workshops.size(); i++){
        for(int j = 0 ; j < map_workshops[i].size(); j++){
            map_workshops[i][j].draw(gRenderer, workshop_tiles_p, workshop_tiles_s);
        }
    }
    
}

//////////////////////
//INTERACTION FUNCTIONS
//FUNCTIONS THAT OPERATE BETWEEN TWO GLOBALS

//a function that picks up an item from the map, and put's it in the specified Sprite's inventory
void pickUpItem(Sprite* sprite, int xpos, int ypos){
//    Item item_temp; // a temporary item for adding to inventory
//    int index; //a temporary index
//    vector<int> clr; //a temporary holder for colors used multiple times below
//    
//    index = map[ypos][xpos]; //get the tile number at the position
//    //check to make sure item is pick-upable
//    if(index > 199){
//        return;
//    }
//    
//    clr = colorz[index]; //get the color of the tile the cre is at
//    item_temp = {clr, index};
//    sprite->inventory.push_back(item_temp);
//    
//    //Also delete item from map
//    map[ypos][xpos] = 200; //blank tile
}

////////////////////////////
//THREADS - ACTIONS

//a thread for wandering to a random place
void wander_thread(Sprite* spr1){
//    for(int i = 0; i<401; i++){
//    //while(true){
//        //printf("%d", spr1.x );
//        printf("XXXXXXXX%dXXXXXXXX",spr1->x);
//    }
    
    while(true){
        
        if(spr1->path.empty()){ //if path is empty
            spr1->path = A_Star(block_map, map_width, map_height, spr1->x, spr1->y, rand()%map_width, rand()%map_height );
            //spr1->path = A_Star(block_map, map_width, map_height, spr1->x, spr1->y, 15, 0);
            //spr1->path = A_Star(block_map, map_width, map_height,  -15, -15 , spr1->x, spr1->y);
        }
        
        //Check if the search failed (error code (9999,9999)
        if(spr1->path[0][0] == 9999){
            spr1->path.pop_back();
            continue; //search failed, try again....
        }
        
        vector<int> next_step = spr1->path[spr1->path.size()-1]; //the last element of array/vector
        //Now actually move
        spr1->x = next_step[0];
        spr1->y = next_step[1];
        //pop off the step from path
        spr1->path.pop_back();
        
        SDL_Delay(500);
    }
    
    
}

//a thread for changing the background periodically
void background_color_thread(){
    
    short change = 1; //how much to change each component by
    while(true){
        
        //change is an opportunity for growth
        back_col.r=back_col.r+change;
        back_col.g=back_col.g+change;
        back_col.b=back_col.b+change;
        
        //bounds checking
        if(back_col.r>254){
            change = -1;
        }
        if(back_col.r==0){
            change = +1;
        }
        
        
        SDL_Delay(100); //Debug Quick
        //SDL_Delay(500); Nice looking
    }
    
}

//////////////////////
//OTHER FUNCTIONS

//Make an attempt to free up  all the memory
void close(){
    TTF_Quit();
    SDL_DestroyWindow(gWindow);
    SDL_DestroyRenderer(gRenderer);
    
}

//DISPLAY WINDOWS?RECTANGLES WITH INFO
//WE REALLY SHOULD CREATE A SOLID CLASS FOR ALL OF THESE...
//A function to display a little box showing what's in inventory
//int counterInventory = 0; //counter used with cursors for inventory pos
//SDL_Surface * surface; //used in drawing
//SDL_Texture * texture; //
//void showInventory(Sprite* sprite, SDL_Event * e){
//    
//    std::string words = "      inventory\n"; //char buffer that is displayed
//    
//    //Black out Box {x, y, w, h}
//    SDL_Rect r={0,SCREEN_HEIGHT,150,-150};
//
//    SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 255 ); //color of box
//    SDL_RenderFillRect( gRenderer, &r ); //copy box to renderer
//    
//    surface = TTF_RenderText_Blended_Wrapped(font1, words.c_str(), font1_clr, 125);
//    texture = SDL_CreateTextureFromSurface(gRenderer, surface);
//    
//    //Create text rect (text box)
//    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH); //This get's the dimensions of the font/text
//    SDL_Rect dstrect = { 0, SCREEN_HEIGHT+r.h, texW, texH }; //so we can make the proper rect to display it
//    SDL_RenderCopy(gRenderer, texture, NULL, &dstrect); //write
//    
//    //Free Memory!!!
//    SDL_DestroyTexture(texture);
//    SDL_FreeSurface(surface);
//    
//    ////////////FINISH PUTTING THE TITLE: inventory
//    
//    //Now we need to print out the items (in COLOR text!!!!)
//    std::string item_name;
//    int item_index; //keeps track of what item in inventory we are actually printing - depends on two factors, down below
//    SDL_Color item_color; //The color of the item
//    
////    if(sprite->inventory.size()!=0){counterInventory = counterInventory % sprite->inventory.size();} //this allows it to scroll back on itself//
//    if(counterInventory<0){counterInventory = sprite->inventory.size()-1;}
//    if(counterInventory>sprite->inventory.size()-1){   counterInventory = 0;}
//    
//    //We'll also reuse some of the previously used objects
//    for(int u = 0; u<7; u++){ //cycle through however many elements you wanna fit on screen
//        item_index = u+counterInventory; //Allow the user to control the scroll by referencing global counterInventory
//        if(u+counterInventory >= sprite->inventory.size()) {//check if sprite's inventory index is out of bounds
//            break;
//        }
//        
//        //Need to:
//        //Determine item type
//        //  Make string for item_type
//        //Determine color of item
//        //  Make color of item
//        //make surface with string, color, and pre-created font
//        //make texture from surface
//        //Query texture for dimensions
//        //create rectangle at correct position and with determined dimensions
//        //copy onto renderer
//        
//        //Determine item type
//        if(sprite->inventory[item_index].tile<100){ //Then it's a weed
//            item_name = "weed";
//        }else if(sprite->inventory[item_index].tile<200){ //Then it's a stone
//            item_name = "stone";
//        }
//        if(item_index == 0){item_name.append(" FIRST");} //Specify if first in list
//        else if(item_index+1 == sprite->inventory.size()){ item_name.append(" LAST"); }
//        if(u == 0){item_name.append(" <-");} //print out a cursor
//        //Determine Color
//        item_color = {static_cast<Uint8>(sprite->inventory[item_index].color[0]), static_cast<Uint8>(sprite->inventory[item_index].color[1]), static_cast<Uint8>(sprite->inventory[item_index].color[2]) }; //lots of conversion from int vector to SDL COLOR...
//        //Make surface and texture
//        surface = TTF_RenderText_Blended_Wrapped(font1, item_name.c_str(), item_color, 125);
//        texture = SDL_CreateTextureFromSurface(gRenderer, surface);
//        //Make the destination rectangle
//        SDL_QueryTexture(texture, NULL, NULL, &texW, &texH); //This get's the dimensions of the font/text
//        dstrect = { 0, SCREEN_HEIGHT+r.h + 15+(u*15), texW, texH }; //so we can make the proper rect to display it
//        //Render to screen
//        SDL_RenderCopy(gRenderer, texture, NULL, &dstrect); //write
//
//        //Free Memory!!!
//        SDL_DestroyTexture(texture);
//        SDL_FreeSurface(surface);
//        
//    }
//}



int main( int argc, char* args[] ){
    
    //initialization
    initScreen();
    //...Random
    time_t seconds;
    time(&seconds);
    srand((unsigned int) seconds);
    //...Controls
    bool quit = false;
    SDL_Event e; //event handler
    //...Map
    generateTilez(); //call this before loadTiles()
    loadTiles();
    //...Sprites
    Sprite* cre1 = new Sprite(10,10);
    cre1->loadFromFile("Civ2/Civ2/tiles/crePrim.png","Civ2/Civ2/tiles/creSeco.png", 16, 16);
    Sprite* cre2 = new Sprite(10,11);
    cre2->loadFromFile("Civ2/Civ2/tiles/crePrim.png","Civ2/Civ2/tiles/creSeco.png", 16, 16);
    Sprite* cre3 = new Sprite(10,10);
    cre3->loadFromFile("Civ2/Civ2/tiles/crePrim.png","Civ2/Civ2/tiles/creSeco.png", 16, 16);
    Sprite* cre4 = new Sprite(7,10);
    cre4->loadFromFile("Civ2/Civ2/tiles/crePrim.png","Civ2/Civ2/tiles/creSeco.png", 16, 16);
    Sprite* shroom1 = new Sprite(3,3);
    shroom1->loadFromFile("Civ2/Civ2/tiles/shroomPrim.png","Civ2/Civ2/tiles/shroomSeco.png", 16, 16);
    Sprite* lov1 = new Sprite(5,5);
    lov1->loadFromFile("Civ2/Civ2/tiles/lovPrim.png", "Civ2/Civ2/tiles/lovSeco.png", 16, 16);
    //...Items
    Item* can1 = new Item(25,25,300);
    Item* item1 = new Item(24,24,301);
    Item* item2 = new Item(26,26,120,{static_cast<Uint8>(colorz[120][0]),static_cast<Uint8>(colorz[120][1]),static_cast<Uint8>(colorz[120][2])},{0xff,0xff,0xff,0x00});

    //TEST SHIT
    //genAdobe();
    //vector<vector<short>> maze = gen_maze_corridor();
    //printMaze(maze);
    init_items();
    Hat temp_accessory = Hat(0, 0, 305); //a temp Item to be added to cre's inventory
    Hat temp_accessory2 = Hat(0, 0, 306); //a temp Item to be added to cre's inventory
    cre1->hat = &temp_accessory; //give him a hat
    cre3->hat = &temp_accessory2; //give him a hat
    //test the name gen
//    while(true){
//        //printf("%s\n",genName());
//        cout << genName() << '\n';
//    }
    
    //vector<vector<int>> test = findPathToCoord(map, 8, 3, 1, 0);
    //TEST A* STAR ALGO
    //bool block_map[map_width*map_height];
//    for(int i = 0; i<map_width*map_height; i++){
//        block_map[i] = false;
//        //Test the algo block detection
////        if(   (i == (5*map_width) + 8) || (i == (6*map_width) + 8) || (i == (4*map_width) + 8) || (i == (3*map_width) + 8)  ){
////            block_map[i] = true;
////        }
//    }
    //make a square of blocked map entries
//    for(int i = 0 ; i < 6 ; i++){
//        for(int j = 0; j < 6; j++) {
//            block_map[(1+j)*map_width + (1 + i)]= true;
//        }
//    }
    //vector<vector<int>> path_test = findPathToCoord_A_Star(block_map, 5, 5, 18, 5);
    //while(true){
    
    //path test
//        vector<vector<int>> path_test = A_Star(block_map,map_width, map_height, 7, 29, 17, 29);
//        //vector<vector<int>> path_test = A_star(block_map,map_width, 5, 5, 5, 8);
//        
//        //print out the steps:
//        for(int i = 0 ; i < path_test.size(); i++){
//            printf("(%d,%d)",path_test[i][0],path_test[i][1]);
//        }
    
    //
    //DEBUG TEST : SPAWN SOME THREAD DAWGd
    //This thread wanders the input creature
    std::thread wanderObj(wander_thread, std::ref(cre4));
    wanderObj.detach();
    //This thread updates the background color
    std::thread backObj(background_color_thread);
    backObj.detach();
    
    //wanderObj.join();
    
//    int diff_test = color_diff({255,255,255}, {0,0,0});
//    printf("DEBUGCOLOR DIFF: %d", diff_test);
    //vector<int> fight_test = color_fight({255,0,255}, {0,255,0} );
    //printf("DEBUGFIGHT:%d, %d",fight_test[0], fight_test[1]);
    
    /////////END DEBUG AREA
    
    //Variables
    bool shiftDown = false; //a flag holding state of shift key
    vector<int> clr; //a temporary holder for colors used multiple times below
    
    //MAIN GAME LOOP
    while(!quit){
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }
            //On keypress change rgb values
            else if( e.type == SDL_KEYDOWN )
            {
                switch( e.key.keysym.sym )
                {
                    case SDLK_q:
                        break;
                        
                    case SDLK_w:
                        if(cre1->y<1){ //bounds check
                            break;
                        }
                        //blocked check
                        if(block_map[(cre1->y-1)*map_width+cre1->x]==true){
                            break;
                        }
                        if(shiftDown){ //pick up above tile
                            pickUpItem(cre1, cre1->x, (cre1->y)-1); //pick up item above
                            break;
                        }
                        cre1->moveUp();
                        break;
                        
                    case SDLK_b: //FOR DEBUGGING!!!!
                        for(int i = 0; i < map_width*map_height; i++) cout << block_map[i];
                        break;
                        
                    case SDLK_a:
                        if(cre1->x<1){ //bounds check
                            break;
                        }
                        //blocked check
                        if(block_map[(cre1->y)*map_width+cre1->x-1]==true){
                            break;
                        }
                        if(shiftDown){ //get info of left tile
                            pickUpItem(cre1, (cre1->x)-1, (cre1->y)); //pick up item left
                            break;
                        }
                        cre1->moveLeft();
                        break;
                        
                    case SDLK_s:
                        if(cre1->y>map_height-2){ //bounds check
                            break;
                        }
                        //blocked check
                        if(block_map[(cre1->y+1)*map_width+cre1->x]==true){
                            break;
                        }
                        if(shiftDown){ //get info of above tile
                            pickUpItem(cre1, (cre1->x), (cre1->y)+1); //pick up item down
                            break;
                        }
                        cre1->moveDown();
                        break;
                        
                    case SDLK_d:
                        //bool willMove = true; //if the cre will move this turn
                        //bounds check
                        if(cre1->x >= map_width-1){
                            //willMove = false;
                            break;
                        }
                        //blocked check
                        if(block_map[(cre1->y)*map_width+cre1->x+1]==true){
                            break;
                        }
        
                        if(shiftDown){ //get info of above tile
                            pickUpItem(cre1, (cre1->x)+1, (cre1->y)); //pick up item right
                            break;
                        }
                        cre1->moveRight();
                        break;
                        
                    case SDLK_SPACE:{
                        break;
                    }
                        
                    case SDLK_i:{
                        inventoryDisplayOn = !inventoryDisplayOn;
                        break;
                    }
                        
                    case SDLK_LSHIFT:
                    case SDLK_RSHIFT:
                        shiftDown = true;
                        break;

                        
                }
            }else if(e.type == SDL_KEYUP){
                switch(e.key.keysym.sym){
                    case SDLK_LSHIFT:
                    case SDLK_RSHIFT:
                        shiftDown = false;
                        break;
                }
            }

            //process display controls
            if(inventoryDisplayOn){ //only if the flag is set
                //Process input on this screen
                if( e.type == SDL_KEYDOWN ){
                    switch( e.key.keysym.sym ){
                        case SDLK_EQUALS:
                            item_display_index++;
                            break;
                        case SDLK_MINUS:
                            item_display_index--;
                            break;
                    }
                }
            }
            
            
        }//End event while loop
        
        //Update Positions...
        //lov1->randomDance();
        //cre3->randomDance();
        //cre2->randomDance();
        
        drawVectorMap();
        //Draw all the sprites
        cre1->draw();
        cre1->drawHat(gRenderer, item_tiles_p, item_tiles_s);
        cre2->draw();
        cre3->draw();
        cre3->drawHat(gRenderer, item_tiles_p, item_tiles_s);
        cre4->draw();
        shroom1->draw();
        lov1->draw();
        //Draw all the items
        draw_items();
        //DEBUG: Test generic wonky ass items not in list (BAD)
        can1->draw(gRenderer, item_tiles_p, item_tiles_s);
        item1->draw(gRenderer, item_tiles_p, item_tiles_s);
        item2->draw(gRenderer, item_tiles_p, item_tiles_s);
        
        //Draw Display windows
        if(inventoryDisplayOn){
            //showInventory(cre1, &e);
            displayItemList(map_items[14*map_width+14], gRenderer, SCREEN_HEIGHT);
        }
        
        SDL_RenderPresent( gRenderer ); //Update screen
        
        
    }//END MAIN GAME LOOP
    
    //Free up sprites and everything else
    cre1->free();
    shroom1->free();
    lov1->free();
    close();
    
    return 0;
}
