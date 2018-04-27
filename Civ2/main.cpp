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
#include <queue>
#include <iostream>
#include <time.h>   
#include <algorithm>
#include <thread>

#include "dancez.hpp"
#include "music.hpp"
#include "creature.hpp"
#include "effects.hpp"
#include "Item.hpp"
#include "tent.hpp"
#include "med_algo.hpp"
#include "story.hpp"
#include "image_gen.hpp"
#include "building.hpp"
#include "recipes.hpp"
//#include <unistd.h>


using std::vector;
using std::queue;

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 680;

//TileMap Stuff
int map_width;
int map_height;
int map_floors; //the number of floors on map (the z dimension)
SDL_Color back_col = {0,0,0}; //The background color
vector<vector<int>> colorz; //every randomly gen. resource (stone, weed) has it's own unique color
//int blockable[2] = {301,304};
bool* block_map; //this is the map indicating whether a tile is blocked

//Draw Map Stuff
//These keep track of what parts of the map to draw
//The dimensinos of the draw map. These indices "swim" through the main map
//int draw_map_x, draw_map_y = 60;
int draw_map_x = 50;
int draw_map_y = 50;
int draw_map_z = 0;
int draw_map_width, draw_map_height;

//ItemTile Stuff
SDL_Texture** item_tiles_p; //primo item tiles
SDL_Texture** item_tiles_s; //seco item tiles
SDL_Texture** item_tiles_t; //terto item tiles
vector<SDL_Color> world_colors; //array of sdl_colors
vector<vector<Item>> map_items; //a list of list of items on a tile. Index corresponds to [y*map_width + x]
vector<vector<Item>> map_scenery_top; //a list of list of scenery on a tile. Index correspongs to [y*map_width + x]
                                  //THis is separate from the map_items list so creatures won't pick these items up
                                  //This version will be the last to be drawn on screen
vector<vector<Item>> map_scenery_bottom; //a list of list of scenery on a tile. Index correspongs to [y*map_width + x]
                                    //THis is separate from the map_items list so creatures won't pick these items up
                                    //This version will be the first to be drawn on screen

//Creatures Stuff
vector<Sprite> map_creatures; //a list of all creatures on map
vector<Shroom> map_shrooms; //a list of all shroom sprites on map

//Effects Stuff
vector<vector<Effect>> map_effects; //a list of list of effects on a tile. Index corresponds to [y*map_width + x]
//Animation Stuff
vector<vector<Animation>> map_animations; //a list of list of animations on a tile. Index corresponds to [y*map_width + x]

SDL_Texture** misc_tiles; //Symbols and Effects Tile Stuff misc. (tiles (symbols, effects, etc.)

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
int creature_inventory_index = 0; //which creature in map_creature to display inventory for
//-----------------
bool statusDisplayOn = false;
//-----------------
bool shroomDepoDisplayOn = false;
int shroom_inventory_index = 0; //which shroom in map_shrooms to display invontory for
//-----------------
bool consoleDisplayOn = false;
queue<string> consoleLog; //a queue of strings to keep track of console messages

//UI SETTINGS
double PLAYER_CREATURE_SPEED = 7.0;
bool is_player_wandering = false; //global flag to enable "player wander" thread

/////////////////////////////////////////////////////////
////FUNCTIONS BEGIN////////////////////////////////////////
////////////////////////////////////////////////////////

//initialize screen
bool initScreen()
{
    //Initialization flag
    bool success = true;
    
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
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
    misc_tiles = new SDL_Texture* [20];
    //world_colors = new SDL_Color [200];
    SDL_Color temp_col;
    
    //ITEM TILES
    for(int i = 0 ; i < 100; i++){

        int r,g,b,a;
        r = rand()%255;
        g = rand()%255;
        b = rand()%255;
//        r = 140;
//        g = 0;
//        b = 0;
        a = 255;
        colorz.push_back({r,g,b,a});

        item_tiles_p[i] = loadTexture("Civ2/Civ2/weedz/"+std::to_string(i)+".png");
        item_tiles_s[i] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
        item_tiles_t[i] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
        temp_col = {static_cast<Uint8>(r),static_cast<Uint8>(g),static_cast<Uint8>(b)};
        //world_colors[i] = temp_col;
        world_colors.push_back(temp_col);
    }
    for(int i = 100 ; i < 200; i++){
        int r,g,b,a;
        r = rand()%255;
        g = rand()%255;
        b = rand()%255;
        a = 255;
        colorz.push_back({r,g,b,a});

        item_tiles_p[i] = loadTexture("Civ2/Civ2/stonez/"+std::to_string(i)+".png");
        item_tiles_s[i] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
        item_tiles_t[i] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
        temp_col = {static_cast<Uint8>(r),static_cast<Uint8>(g),static_cast<Uint8>(b)};
        //world_colors[i] = temp_col;
        world_colors.push_back(temp_col);
        
    }
    for(int i = 200 ; i < 300; i++){
        int r,g,b,a;
        r = rand()%255;
        g = rand()%255;
        b = rand()%255;
        a = 255;
        colorz.push_back({r,g,b,a});

        item_tiles_p[i] = loadTexture("Civ2/Civ2/fruitz/"+std::to_string(i)+".png");
        item_tiles_s[i] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
        item_tiles_t[i] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
        temp_col = {static_cast<Uint8>(r),static_cast<Uint8>(g),static_cast<Uint8>(b)};
        //world_colors[i] = temp_col;
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
    
    item_tiles_p[307] = loadTexture("Civ2/Civ2/doodadz/staff.png");
    item_tiles_s[307] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
    item_tiles_t[307] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
    
    item_tiles_p[308] = loadTexture("Civ2/Civ2/doodadz/candlePrim.png");
    item_tiles_s[308] = loadTexture("Civ2/Civ2/doodadz/candleSeco.png");
    item_tiles_t[308] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
    
    item_tiles_p[309] = loadTexture("Civ2/Civ2/doodadz/flagPrim.png");
    item_tiles_s[309] = loadTexture("Civ2/Civ2/doodadz/flagSeco.png");
    item_tiles_t[309] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
    
    item_tiles_p[310] = loadTexture("Civ2/Civ2/doodadz/herbPrim.png");
    item_tiles_s[310] = loadTexture("Civ2/Civ2/doodadz/herbSecotry.png");
    item_tiles_t[310] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
    
    item_tiles_p[311] = loadTexture("Civ2/Civ2/doodadz/fruitPrim.png");
    item_tiles_s[311] = loadTexture("Civ2/Civ2/doodadz/fruitSeco.png");
    item_tiles_t[311] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
    
    item_tiles_p[312] = loadTexture("Civ2/Civ2/doodadz/stonePrim.png");
    item_tiles_s[312] = loadTexture("Civ2/Civ2/doodadz/stoneSeco.png");
    item_tiles_t[312] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
    
    item_tiles_p[313] = loadTexture("Civ2/Civ2/doodadz/bagPrim.png");
    item_tiles_s[313] = loadTexture("Civ2/Civ2/doodadz/bagSeco.png");
    item_tiles_t[313] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
    
    item_tiles_p[314] = loadTexture("Civ2/Civ2/doodadz/potionPrim.png");
    item_tiles_s[314] = loadTexture("Civ2/Civ2/doodadz/potionSeco.png");
    item_tiles_t[314] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
    
    item_tiles_p[315] = loadTexture("Civ2/Civ2/doodadz/coinPrim.png");
    item_tiles_s[315] = loadTexture("Civ2/Civ2/doodadz/coinSeco.png");
    item_tiles_t[315] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
    
    item_tiles_p[316] = loadTexture("Civ2/Civ2/doodadz/winePrim.png");
    item_tiles_s[316] = loadTexture("Civ2/Civ2/doodadz/wineSeco.png");
    item_tiles_t[316] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
    
    item_tiles_p[317] = loadTexture("Civ2/Civ2/tiles/curtainsPrim.png");
    item_tiles_s[317] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
    item_tiles_t[317] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
    
    //MISC TILES
    misc_tiles[0] = loadTexture("Civ2/Civ2/tiles/zodiac/aries.png");
    misc_tiles[1] = loadTexture("Civ2/Civ2/tiles/zodiac/taurus.png");
    misc_tiles[2] = loadTexture("Civ2/Civ2/tiles/zodiac/gemini.png");
    misc_tiles[3] = loadTexture("Civ2/Civ2/tiles/zodiac/cancer.png");
    misc_tiles[4] = loadTexture("Civ2/Civ2/tiles/zodiac/leo.png");
    misc_tiles[5] = loadTexture("Civ2/Civ2/tiles/zodiac/virgo.png");
    misc_tiles[6] = loadTexture("Civ2/Civ2/tiles/zodiac/libra.png");
    misc_tiles[7] = loadTexture("Civ2/Civ2/tiles/zodiac/scorpio.png");
    misc_tiles[8] = loadTexture("Civ2/Civ2/tiles/zodiac/sagittarius.png");
    misc_tiles[9] = loadTexture("Civ2/Civ2/tiles/zodiac/capricorn.png");
    misc_tiles[10] = loadTexture("Civ2/Civ2/tiles/zodiac/aquarius.png");
    misc_tiles[11] = loadTexture("Civ2/Civ2/tiles/zodiac/pisces.png");
    misc_tiles[12] = loadTexture("Civ2/Civ2/tiles/status_balance.png");
    misc_tiles[13] = loadTexture("Civ2/Civ2/tiles/status_health.png");
    misc_tiles[14] = loadTexture("Civ2/Civ2/tiles/status_smile.png");
    misc_tiles[15] = loadTexture("Civ2/Civ2/tiles/tarot_sword.png");
    misc_tiles[16] = loadTexture("Civ2/Civ2/tiles/tarot_wand.png");
    misc_tiles[17] = loadTexture("Civ2/Civ2/tiles/tarot_cup.png");
    misc_tiles[18] = loadTexture("Civ2/Civ2/tiles/tarot_pentacle.png");
    //Tiles for Effects
    misc_tiles[19] = loadTexture("Civ2/Civ2/tiles/heartsPrim.png");
    misc_tiles[20] = loadTexture("Civ2/Civ2/tiles/sparklesPrim.png");
    misc_tiles[21] = loadTexture("Civ2/Civ2/tiles/smokePrim.png");
    //Tiles for Fire Animation
    misc_tiles[22] = loadTexture("Civ2/Civ2/tiles/fire_animate/T1Prim.png");
    misc_tiles[23] = loadTexture("Civ2/Civ2/tiles/fire_animate/T2Prim.png");
    misc_tiles[24] = loadTexture("Civ2/Civ2/tiles/fire_animate/T3Prim.png");
    misc_tiles[25] = loadTexture("Civ2/Civ2/tiles/fire_animate/T4Prim.png");
    
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
    system("python Civ2/Civ2/fruitz/fruitz.py");
    system("python Civ2/Civ2/fruitz/outline.py");

}

//Print out a tile-based map give a vector 2D array
void drawVectorMap(){
    //Clear screen
    SDL_SetRenderDrawColor( gRenderer,back_col.r, back_col.g, back_col.b, back_col.a );
    SDL_RenderClear( gRenderer );
    
    //We also want to draw black boxes at boundaries...
    if(draw_map_x < 0){ //for left boundary
        SDL_SetRenderDrawColor( gRenderer,0,0,0,255 );
        SDL_Rect rect1 = { 0, 0, (-1*draw_map_x)*16, SCREEN_HEIGHT };
        SDL_RenderFillRect(gRenderer, &rect1 ); //calculate by how much draw_map is out of bounds and draw a rectangle over it
    }
    if(draw_map_x + draw_map_width >= map_width){ //for right boundary
        SDL_SetRenderDrawColor( gRenderer,0,0,0,255 );
        int difference = ((draw_map_x + draw_map_width) - map_width) * 16; //how much the right side of screen (draw_map) overlaps the end of main map (IN PIXELS)
        SDL_Rect rect1 = { SCREEN_WIDTH - difference, 0, SCREEN_WIDTH , SCREEN_HEIGHT };
        SDL_RenderFillRect(gRenderer, &rect1 ); //calculate by how much draw_map is out of bounds and draw a rectangle over it
    }
    
    if(draw_map_y < 0){ //for upper boundary
        SDL_SetRenderDrawColor( gRenderer,0,0,0,255 );
        //SDL_Rect rect1 = { 0, 0, (-1*draw_map_x)*16, SCREEN_HEIGHT };
        SDL_Rect rect1 = { 0, 0, SCREEN_WIDTH, (-1*draw_map_y)*16};
        SDL_RenderFillRect(gRenderer, &rect1 ); //calculate by how much draw_map is out of bounds and draw a rectangle over it
    }
    if(draw_map_y + draw_map_height > map_height){ //for BOTTOM boundary
        SDL_SetRenderDrawColor( gRenderer,0,0,0,255 );
        int difference = ((draw_map_y + draw_map_height) - map_height) * 16; //how much the bottom side of screen (draw_map) overlaps the end of main map (IN PIXELS)
        //SDL_Rect rect1 = { SCREEN_WIDTH - difference, 0, SCREEN_WIDTH , SCREEN_HEIGHT };
        SDL_Rect rect1 = { 0, SCREEN_HEIGHT- difference, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_RenderFillRect(gRenderer, &rect1 ); //calculate by how much draw_map is out of bounds and draw a rectangle over it
    }

}

//////MAKE this the standard Initialization script
//Initializes all items and creaures
//generates the initial items on the map based on shroom recipe resources
//This also initializes effects
void init_environment(){
    
    //Set map (grid) dimensions
    map_width = 200;
    map_height = 200;
    
    //draw_map DOES depend on screen dimension
    draw_map_width = SCREEN_WIDTH/16;
    draw_map_height = SCREEN_HEIGHT/16 + 1; //add one for that last extra bit

    //Initialize our dimension-indexed arrays based on map dimensions
    map_items.resize(map_width*map_height);
    map_scenery_top.resize(map_width*map_height);
    map_scenery_bottom.resize(map_width*map_height);
    map_effects.resize(map_width*map_height);
    map_animations.resize(map_width*map_height);
    
    //we're going to create 300 random items (basic first kind)
    //we have to generate items, and also add them to the correct element in the array
    int tempx; //temporary x
    int tempy; //temporary y
    int temp_tile; //temporarily stores random tile index

    //for dispersing random items on map
    for(int b = 0 ; b<125; b++){
        tempx = rand()%(map_width);
        tempy = rand()%(map_height);
        if(rand()%2 == 1){
            temp_tile = 310; //+ (rand()%2);
        }else{
            if(rand()%2==1){
                temp_tile=312;
            }else{
                temp_tile = 311;
            }
        }
        //temp_tile = rand()%7 + 300;
        //if(temp_tile == 301 || temp_tile == 304){temp_tile = 305;}
        Item temp_item = Item(tempx, tempy, temp_tile, {static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), 255},{255,255,255,0} ); //temporary item
        map_items[(tempy)*map_width+(tempx)].push_back(temp_item);
    }
    
    //Here is where we first initilize block_map...
    block_map = new bool[map_width*map_height](); //initialize a dynamically sized blocked map

    //create random scenery (from the randomly generated assets)
    for(int g = 0 ; g<225; g++){
        tempx = rand()%(map_width);
        tempy = rand()%(map_height);
        temp_tile = rand()%199;
        Item temp_item = Item(tempx, tempy, temp_tile,world_colors[temp_tile],{255,255,255,0} ); //temporary item (scenery)
        map_scenery_bottom[(tempy*map_width)+tempx].push_back(temp_item);
    }
    
    //Generate colors for neighboorhood/market/bazaar
    printf("Begin build sequence\n");
    SDL_Color build_col_p = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255};
    SDL_Color build_col_s = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255};
    SDL_Color floor_col_p = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255};
    SDL_Color floor_col_s = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255};
    SDL_Color door_col_s = {static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),static_cast<Uint8>(rand()%255),255};
    //Now build some houses
    build_circle_radius_door(&map_scenery_top, block_map, map_width,9, 13, 10, build_col_p, build_col_s, door_col_s);
    build_floor_radius(&map_scenery_bottom, block_map, map_width, map_height, 13, 10, 8, floor_col_p, floor_col_s); //notice how it is one less than wall build radius
    build_floor_path(&map_scenery_bottom, block_map, map_width, map_height, 13, 10, 30, 30, floor_col_p, floor_col_s);
    
    build_two_house_path(&map_scenery_top, &map_scenery_bottom, block_map, map_width, map_height, floor_col_p, floor_col_s, build_col_p, build_col_s, door_col_s);
    build_two_house_path(&map_scenery_top, &map_scenery_bottom, block_map, map_width, map_height, floor_col_p, floor_col_s, build_col_p, build_col_s, door_col_s);
    build_two_house_path(&map_scenery_top, &map_scenery_bottom, block_map, map_width, map_height, floor_col_p, floor_col_s, build_col_p, build_col_s, door_col_s);
    
    build_two_house_path(&map_scenery_top, &map_scenery_bottom, block_map, map_width, map_height, floor_col_p, floor_col_s, build_col_p, build_col_s, door_col_s);
    build_two_house_path(&map_scenery_top, &map_scenery_bottom, block_map, map_width, map_height, floor_col_p, floor_col_s, build_col_p, build_col_s, door_col_s);
    build_two_house_path(&map_scenery_top, &map_scenery_bottom, block_map, map_width, map_height, floor_col_p, floor_col_s, build_col_p, build_col_s, door_col_s);
    
    build_two_house_path(&map_scenery_top, &map_scenery_bottom, block_map, map_width, map_height, floor_col_p, floor_col_s, build_col_p, build_col_s, door_col_s);
    build_two_house_path(&map_scenery_top, &map_scenery_bottom, block_map, map_width, map_height, floor_col_p, floor_col_s, build_col_p, build_col_s, door_col_s);
    build_two_house_path(&map_scenery_top, &map_scenery_bottom, block_map, map_width, map_height, floor_col_p, floor_col_s, build_col_p, build_col_s, door_col_s);
    
    build_two_house_path(&map_scenery_top, &map_scenery_bottom, block_map, map_width, map_height, floor_col_p, floor_col_s, build_col_p, build_col_s, door_col_s);
    build_two_house_path(&map_scenery_top, &map_scenery_bottom, block_map, map_width, map_height, floor_col_p, floor_col_s, build_col_p, build_col_s, door_col_s);
    build_two_house_path(&map_scenery_top, &map_scenery_bottom, block_map, map_width, map_height, floor_col_p, floor_col_s, build_col_p, build_col_s, door_col_s);
    
    build_two_house_path(&map_scenery_top, &map_scenery_bottom, block_map, map_width, map_height, floor_col_p, floor_col_s, build_col_p, build_col_s, door_col_s);
    build_two_house_path(&map_scenery_top, &map_scenery_bottom, block_map, map_width, map_height, floor_col_p, floor_col_s, build_col_p, build_col_s, door_col_s);
    build_two_house_path(&map_scenery_top, &map_scenery_bottom, block_map, map_width, map_height, floor_col_p, floor_col_s, build_col_p, build_col_s, door_col_s);
    
    build_two_house_path(&map_scenery_top, &map_scenery_bottom, block_map, map_width, map_height, floor_col_p, floor_col_s, build_col_p, build_col_s, door_col_s);
    build_two_house_path(&map_scenery_top, &map_scenery_bottom, block_map, map_width, map_height, floor_col_p, floor_col_s, build_col_p, build_col_s, door_col_s);
    build_two_house_path(&map_scenery_top, &map_scenery_bottom, block_map, map_width, map_height, floor_col_p, floor_col_s, build_col_p, build_col_s, door_col_s);
    
    build_two_house_path(&map_scenery_top, &map_scenery_bottom, block_map, map_width, map_height, floor_col_p, floor_col_s, build_col_p, build_col_s, door_col_s);
    build_two_house_path(&map_scenery_top, &map_scenery_bottom, block_map, map_width, map_height, floor_col_p, floor_col_s, build_col_p, build_col_s, door_col_s);
    build_two_house_path(&map_scenery_top, &map_scenery_bottom, block_map, map_width, map_height, floor_col_p, floor_col_s, build_col_p, build_col_s, door_col_s);
    
    build_two_house_path(&map_scenery_top, &map_scenery_bottom, block_map, map_width, map_height, floor_col_p, floor_col_s, build_col_p, build_col_s, door_col_s);
    build_two_house_path(&map_scenery_top, &map_scenery_bottom, block_map, map_width, map_height, floor_col_p, floor_col_s, build_col_p, build_col_s, door_col_s);
    build_two_house_path(&map_scenery_top, &map_scenery_bottom, block_map, map_width, map_height, floor_col_p, floor_col_s, build_col_p, build_col_s, door_col_s);
    
    build_two_house_path(&map_scenery_top, &map_scenery_bottom, block_map, map_width, map_height, floor_col_p, floor_col_s, build_col_p, build_col_s, door_col_s);
    build_two_house_path(&map_scenery_top, &map_scenery_bottom, block_map, map_width, map_height, floor_col_p, floor_col_s, build_col_p, build_col_s, door_col_s);
    build_two_house_path(&map_scenery_top, &map_scenery_bottom, block_map, map_width, map_height, floor_col_p, floor_col_s, build_col_p, build_col_s, door_col_s);
    
    printf("Done with build aye\\\n");
    
    //Creatures
    int num_creatures = 25; //How many creatures are on the map
    //int num_creatures = 1; //DEBUG How many creatures are on the map
    for(int i = 0 ; i < num_creatures; i++){
        //Sprite temp_cre = Sprite(1+rand()%(map_width-2), 1+rand()%(map_height-2));
        Sprite temp_cre = Sprite(60 + rand()%4, 60 + rand()%5); //set them up in a specific spot
        temp_cre.loadFromFile("Civ2/Civ2/tiles/crePrim.png","Civ2/Civ2/tiles/creSeco.png", 16, 16);
        map_creatures.push_back(temp_cre);
        
        //randomly give hats
        if(rand()%2==1){
            Hat* temp_hat = new Hat(0, 0, 305+(rand()%2) ); //a temp Item to be added to cre's inventory
            map_creatures.back().hat = temp_hat;
        }
        //randomly give staffs (staves?)
        if(rand()%2==1){
            Staff* temp_staff = new Staff(0,0,307); //a temp Item to be added to the cre's equip inventory
            map_creatures.back().staff = temp_staff;
        }
        //randomly give lights
        if(rand()%2==1){
            Light* temp_light = new Light(0, 0, 308); //a temp Item to be added to the cre's equip inventory
            map_creatures.back().light = temp_light;
        }
    }
    
    //Shrooms
    int num_shrooms = 5; //How many shrooms are on the map
    //Sprite temp_shroom;
    for(int i = 0; i < num_shrooms; i++){
        int tx, ty;
        while(true){
            tx = rand()%map_width;
            ty = rand()%map_height;
            if(block_map[(ty*map_width)+tx]==false){ //if adjacent is NOT blocked
                Shroom temp_shroom = Shroom(tx,ty); //create new shroom
                temp_shroom.loadFromFile("Civ2/Civ2/tiles/shroomPrim.png","Civ2/Civ2/tiles/shroomSeco.png", 16, 16);
                map_shrooms.push_back(temp_shroom);
                break;
            }
        }
    }
    
    
}

//Draws all the items and creatures on map
//This function uses the indices:
//  draw_map_width, draw_map_height, draw_map_x, and draw_map_y
//
//It only cycles through though specific indices in the map lists
//HOWEVER,
//when it calls the draw function. It needs to call it in a certain way so as to draw it in the correct (translated) location...

//This method is not the most efficient (I imagine?) but will do for now.
void draw_environment(){

    //Draw all bottom items
    for(int i = draw_map_x ; i < (draw_map_x + draw_map_width) ; i++){ //cycle through x dimension
        if(i >= map_width || i <= 0){continue;} //bounds checking
        for(int j = draw_map_y ; j < (draw_map_y + draw_map_height); j++){ //cycle through y dimensiion
            if(j >= map_height || j < 0){continue;} //bounds checking
            
            int map_index = (j*map_width) + i; //calculate the index required to acces the location (i,j) on the map (since we use it so much)
            
            //Cycle through all the items in the map_scenery_bottom list at that location
            for(int k = 0; k < map_scenery_bottom[ map_index ].size(); k++ ){
                map_scenery_bottom[map_index][k].draw( (map_scenery_bottom[map_index][k].x - draw_map_x) , (map_scenery_bottom[map_index][k].y - draw_map_y), gRenderer, item_tiles_p, item_tiles_s); //call the draw function. We draw the item at a location translated from the current draw_map
            }
            
            //Cycle through all the items on the map
            for(int k = 0; k < map_items[ map_index ].size(); k++ ){
                map_items[map_index][k].draw( (map_items[map_index][k].x - draw_map_x) , (map_items[map_index][k].y - draw_map_y), gRenderer, item_tiles_p, item_tiles_s); //call the draw function. We draw the item at a location translated from the current draw_map
            }
            
        }
    } //End drawing bottom items, can draw creatures in a separate loop
    
    //Cycle through all shrooms and draw...
    for(int c = 0 ; c < map_shrooms.size(); c++){
        if (map_shrooms[c].x > draw_map_x && map_shrooms[c].x < draw_map_x + draw_map_width &&
            map_shrooms[c].y > draw_map_y && map_shrooms[c].y < draw_map_y + draw_map_height ) { //do bounds checking so don't draw out of screen
            map_shrooms[c].draw(map_shrooms[c].x - draw_map_x, map_shrooms[c].y - draw_map_y, item_tiles_p, item_tiles_s);
        }
    }
    
    //Cycle through all creatures and draw...
    for(int c = 0 ; c < map_creatures.size(); c++){
        if (map_creatures[c].x > draw_map_x && map_creatures[c].x < draw_map_x + draw_map_width &&
            map_creatures[c].y > draw_map_y && map_creatures[c].y < draw_map_y + draw_map_height ) { //do bounds checking so don't draw out of screen
            map_creatures[c].draw_movement(map_creatures[c].x - draw_map_x, map_creatures[c].y - draw_map_y, item_tiles_p, item_tiles_s);
        }
    }
    
    //Draw all top items
    for(int i = draw_map_x ; i < (draw_map_x + draw_map_width) ; i++){ //cycle through x dimension
        if(i >= map_width || i <= 0){continue;} //bounds checking
        for(int j = draw_map_y ; j < (draw_map_y + draw_map_height); j++){ //cycle through y dimensiion
            if(j >= map_height || j < 0){continue;} //bounds checking
            
            int map_index = (j*map_width) + i; //calculate the index required to acces the location (i,j) on the map (since we use it so much)
            
            //Cycle through all the items in the map_scenery_top list at that location
            for(int k = 0; k < map_scenery_top[ map_index ].size(); k++ ){
                map_scenery_top[map_index][k].draw( (map_scenery_top[map_index][k].x - draw_map_x) , (map_scenery_top[map_index][k].y - draw_map_y), gRenderer, item_tiles_p, item_tiles_s); //call the draw function. We draw the item at a location translated from the current draw_map
            }
            
            //Cycle through all animations on map
            for(int k = 0; k < map_animations[ map_index ].size(); k++ ){
                map_animations[map_index][k].draw( (map_animations[map_index][k].x - draw_map_x) , (map_animations[map_index][k].y - draw_map_y), gRenderer, misc_tiles);
            }
            
            //Cycle through all effects on map
            for(int k = 0; k < map_effects[ map_index ].size(); k++ ){
                map_effects[map_index][k].drawScroll( (map_effects[map_index][k].x - draw_map_x) , (map_effects[map_index][k].y - draw_map_y), gRenderer, misc_tiles);
            }
            
        }
    } //End drawing top items
    
    
    

}

//////////////////////
//INTERACTION FUNCTIONS
//FUNCTIONS THAT OPERATE BETWEEN TWO GLOBALS

//a function that picks up an item from the map, and put's it in the specified Sprite's inventory
void pickUpItem(Sprite* sprite, int xpos, int ypos){
    
    int num_items = map_items[(ypos*map_width)+xpos].size(); //the last element in the tile item list
    
    if(num_items!=0){
        
        Item temp_item = Item(999,999, map_items[(ypos*map_width)+xpos][num_items-1].type, {map_items[(ypos*map_width)+xpos][num_items-1].primColor.r,map_items[(ypos*map_width)+xpos][num_items-1].primColor.g,map_items[(ypos*map_width)+xpos][num_items-1].primColor.b,map_items[(ypos*map_width)+xpos][num_items-1].primColor.a}, {map_items[(ypos*map_width)+xpos][num_items-1].secoColor.r,map_items[(ypos*map_width)+xpos][num_items-1].secoColor.g,map_items[(ypos*map_width)+xpos][num_items-1].secoColor.b,map_items[(ypos*map_width)+xpos][num_items-1].secoColor.a});
        sprite->inventory.push_back(temp_item);
        map_items[(ypos*map_width) + xpos].pop_back();
        
    }
    
}
//overload - function picks up items, specifies index on tile-item stack (map_items) it wants
void pickUpItem(Sprite* sprite, int xpos, int ypos, int index){
    
    int num_items = map_items[(ypos*map_width)+xpos].size(); //the last element in the tile item list
    if(index>=num_items){return;} //bounds check
    
    Item temp_item = Item(999,999, map_items[(ypos*map_width)+xpos][index].type, map_items[(ypos*map_width)+xpos][index].primColor, map_items[(ypos*map_width)+xpos][index].secoColor);
    sprite->inventory.push_back(temp_item);
    map_items[(ypos*map_width) + xpos].erase(map_items[(ypos*map_width) + xpos].begin() + index);
        

}

//Helper function for below search routine
//returns true if element node e is in node list v
bool isNodeIn(vector<int> e, vector<vector<int>> v){
    
    vector<int> temp; //used to hold the nodes...
    for(int i = 0; i<v.size(); i++){ //cycle through all elements of v
        if(e[0] == v[i][0] && e[1] == v[i][1]){
            return true;//means we found a match!
        }
    }
    return false; //means we cycled list and couldn't find match
}

//searches for an item that is similar enough for a given color
//Flood searches from a given sprite's position for item that fits criteria
//(Flood search is a good choice here since we don't have a fixed destination we are trying to get to)
//Searches each tile on map_items - each tile has a stack of items that will be searched
//input: Sprite who will be searching
//        diff_threshold - the maximum difference in color acceptable
//output: steps from sprite to target
//        returns {{9999,9999}} error code if not found soon enough
//        steps returned as nodes: <xpos, ypos, steps_from_dest>
//        function returns a list of those nodes
//others: also sets target data on Sprite*
vector<vector<int>> faveColorSearch(Sprite* cre, int diff_threshold){
    
    vector<vector<int>> search_q; //the main "list"
    int k = 0; //the main counter used to keep track of where we are in the list
    vector<int> node; //The current node on the list we are looking at
    vector<int> temp; //Temp node used to push new nodes onto queues and later in filtering out irrelevant steps
    
    //Add starting node to search queue
    node = {cre->x, cre->y, 0}; //initialization
    search_q.push_back(node); //add first element to list
    bool findingTarget = true; //a flag signalling if we're still searching for target (and populating search_q)
    while(findingTarget){
        
        if(k<search_q.size()){ //bounds chhecking
            node = search_q[k]; //pull the next node <xpos, ypos, distance_from_start>
        }else{ //if we've got to the end of list
            //couldn't find targ
            cre->path = {{9999,9999}}; //set the path to indicate error code
            return {{9999,9999}} ; //didn't find / couldn't reach / searching too long
        }
        
        
        //Look at all nodes adjacent to the current node
        //LEFT
        if(node[0]-1 > 0){ //bounds checking
             if(block_map[((node[1])*map_width)+(node[0]-1)]==false){ //if adjacent is NOT blocked
                for(int i = 0; i < map_items[((node[1])*map_width)+(node[0]-1)].size() ; i++){ //iterate through all items on the adjacent node
                    if( (color_diff( map_items[((node[1])*map_width)+(node[0]-1)][i].primColor , cre->faveColor) < diff_threshold)
                       || ( color_diff( map_items[((node[1])*map_width)+(node[0]-1)][i].primColor , cre->faveColor2) < diff_threshold ) ){
                        //We found target!
                        temp = {node[0]-1, node[1], node[2]+1}; //temporary
                        search_q.push_back(temp); //add to search_q
                        findingTarget = false;
                        break;
                    }
                }//end for
             }
            
            if(findingTarget==false){break;} //?right???????
                
            //Otherwise, add node to search_q (if eligible, ofc)
            
            if(block_map[((node[1])*map_width)+(node[0]-1)]==false){ //if adjacent is NOT blocked
                //we also gotta make sure node isn't in queue with lower distance already...
                //cycle through the list and check for every lower distance...
                if(!isNodeIn({node[0]-1,node[1],0}, search_q)){
                    temp = {node[0]-1, node[1], node[2]+1};
                    search_q.push_back(temp);
                }
            }
        }
        //RIGHT
        if(node[0]+1 < map_width){ //bounds checking
            if(block_map[((node[1])*map_width)+(node[0]+1)]==false){ //if adjacent is NOT blocked
                for(int i = 0; i < map_items[((node[1])*map_width)+(node[0]+1)].size() ; i++){ //iterate through all items on the adjacent node
                    if( color_diff( map_items[((node[1])*map_width)+(node[0]+1)][i].primColor , cre->faveColor) < diff_threshold
                       || ( color_diff( map_items[((node[1])*map_width)+(node[0]+1)][i].primColor , cre->faveColor2) < diff_threshold )  ){
                        //We found target!
                        temp = {node[0]+1, node[1], node[2]+1}; //temporary
                        search_q.push_back(temp); //add to search_q
                        findingTarget = false;
                        break;
                    }
                }//end for
            }
            
            if(findingTarget==false){break;} //?right???????
            
            //Otherwise, add node to search_q (if eligible, ofc)
            
            if(block_map[((node[1])*map_width)+(node[0]+1)]==false){ //if adjacent is NOT blocked
                //we also gotta make sure node isn't in queue with lower distance already...
                //cycle through the list and check for every lower distance...
                if(!isNodeIn({node[0]+1,node[1],0}, search_q)){
                    temp = {node[0]+1, node[1], node[2]+1};
                    search_q.push_back(temp);
                }
            }
        }
        //UP
        if(node[1]-1 > 0){ //bounds checking
            if(block_map[((node[1]-1)*map_width)+(node[0])]==false){ //if adjacent is NOT blocked
                for(int i = 0; i < map_items[((node[1]-1)*map_width)+(node[0])].size() ; i++){ //iterate through all items on the adjacent node
                    if( color_diff( map_items[((node[1]-1)*map_width)+(node[0])][i].primColor , cre->faveColor) < diff_threshold
                       ||  (color_diff( map_items[((node[1]-1)*map_width)+(node[0])][i].primColor , cre->faveColor2) < diff_threshold) ){
                        //We found target!
                        temp = {node[0], node[1]-1, node[2]+1}; //temporary
                        search_q.push_back(temp); //add to search_q
                        findingTarget = false;
                        break;
                    }
                }//end for
            }
            
            if(findingTarget==false){break;} //?right???????
            
            //Otherwise, add node to search_q (if eligible, ofc)
            
            if(block_map[((node[1]-1)*map_width)+(node[0])]==false){ //if adjacent is NOT blocked
                //we also gotta make sure node isn't in queue with lower distance already...
                //cycle through the list and check for every lower distance...
                if(!isNodeIn({node[0],node[1]-1,0}, search_q)){
                    temp = {node[0], node[1]-1, node[2]+1};
                    search_q.push_back(temp);
                }
            }
        }
        //DOWN
        if(node[1]+1 < map_height){ //bounds checking
            if(block_map[((node[1]+1)*map_width)+(node[0])]==false){ //if adjacent is NOT blocked
                for(int i = 0; i < map_items[((node[1]+1)*map_width)+(node[0])].size() ; i++){ //iterate through all items on the adjacent node
                    if( color_diff( map_items[((node[1]+1)*map_width)+(node[0])][i].primColor , cre->faveColor) < diff_threshold
                       ||  (color_diff( map_items[((node[1]+1)*map_width)+(node[0])][i].primColor , cre->faveColor2) < diff_threshold) ){
                        //We found target!
                        temp = {node[0], node[1]+1, node[2]+1}; //temporary
                        search_q.push_back(temp); //add to search_q
                        findingTarget = false;
                        break;
                    }
                }//end for
            }
            
            if(findingTarget==false){break;} //?right???????
            
            //Otherwise, add node to search_q (if eligible, ofc)
            
            if(block_map[((node[1]+1)*map_width)+(node[0])]==false){ //if adjacent is NOT blocked
                //we also gotta make sure node isn't in queue with lower distance already...
                //cycle through the list and check for every lower distance...
                if(!isNodeIn({node[0],node[1]+1,0}, search_q)){
                    temp = {node[0], node[1]+1, node[2]+1};
                    search_q.push_back(temp);
                }
            }
        }
        k = k+1;
        if(k > 400){ //if we've been searching too long, give up
            k = search_q.size(); //just go to end of queue so we can return error code
        }
    
    }//Broke out of while loop means we found target
    
    //Now we need to go down the list (popping) and only keep nodes that apply to the CORRECT PATH
    //We create a list goind BACKWARD from target to current
    vector<vector<int>> path2target;//the list of nodes leading from target
    temp = search_q.back(); //get last element from list
    search_q.pop_back(); //now remove that element since we won't need it in list
    path2target.push_back(temp);//Add first node to path
    while(true){
        temp = search_q.back();
        search_q.pop_back(); //Get last value out of search queue
        if(temp[2] == 0){ //means 0 steps left and we found original node (our starting position
            break;
        }
        if(temp[2] == path2target.back()[2]- 1){ //if current node is actually a step closer
            //check if it's a step in the right direction (AND ONLY 1 unit away from last step)
            if(   ((abs(temp[0]-path2target.back()[0])==1)&&(abs(temp[1]-path2target.back()[1])==0)) || //check if x diff is 1
               ((abs(temp[0]-path2target.back()[0])==0)&&(abs(temp[1]-path2target.back()[1])==1)) ){ //check if y diff is 1
                path2target.push_back(temp);
            }
            
        }
        
    }
    
    //return search_q;
    //reverse(path2target.begin(), path2target.end());
    cre->path = path2target;
    
    return path2target; //return the list backwards so you can pop_back to get the next step needed
    
}

//searches for an item of a specific tile_type
//Flood searches from a given sprite's position for item that fits criteria
//(Flood search is a good choice here since we don't have a fixed destination we are trying to get to)
//Searches each tile on map_items - each tile has a stack of items that will be searched
//input: Sprite who will be searching
//        item_type - the type of item we are trying to find
//output: steps from sprite to target
//        returns {{9999,9999}} error code if not found soon enough
//        steps returned as nodes: <xpos, ypos, steps_from_dest>
//        function returns a list of those nodes
//others: also sets target data on Sprite*
vector<vector<int>> itemTypeSearch(Sprite* cre, int item_type){

    vector<vector<int>> search_q; //the main "list"
    int k = 0; //the main counter used to keep track of where we are in the list
    vector<int> node; //The current node on the list we are looking at
    vector<int> temp; //Temp node used to push new nodes onto queues and later in filtering out irrelevant steps

    //Add starting node to search queue
    node = {cre->x, cre->y, 0}; //initialization
    search_q.push_back(node); //add first element to list
    bool findingTarget = true; //a flag signalling if we're still searching for target (and populating search_q)
    while(findingTarget){

        if(k<search_q.size()){ //bounds chhecking
            node = search_q[k]; //pull the next node <xpos, ypos, distance_from_start>
        }else{ //if we've got to the end of list
            //couldn't find targ
            cre->path = {{9999,9999}}; //set the path to indicate error code
            return {{9999,9999}} ; //didn't find / couldn't reach / searching too long
        }


        //Look at all nodes adjacent to the current node
        //LEFT
        if(node[0]-1 > 0){ //bounds checking
            if(block_map[((node[1])*map_width)+(node[0]-1)]==false){ //if adjacent is NOT blocked
                for(int i = 0; i < map_items[((node[1])*map_width)+(node[0]-1)].size() ; i++){ //iterate through all items on the adjacent node
                    if( map_items[((node[1])*map_width)+(node[0]-1)][i].type == item_type ){
                        //We found target!
                        temp = {node[0]-1, node[1], node[2]+1}; //temporary
                        search_q.push_back(temp); //add to search_q
                        findingTarget = false;
                        break;
                    }
                }//end for
            }

            if(findingTarget==false){break;} //?right???????

            //Otherwise, add node to search_q (if eligible, ofc)

            if(block_map[((node[1])*map_width)+(node[0]-1)]==false){ //if adjacent is NOT blocked
                //we also gotta make sure node isn't in queue with lower distance already...
                //cycle through the list and check for every lower distance...
                if(!isNodeIn({node[0]-1,node[1],0}, search_q)){
                    temp = {node[0]-1, node[1], node[2]+1};
                    search_q.push_back(temp);
                }
            }
        }
        //RIGHT
        if(node[0]+1 < map_width){ //bounds checking
            if(block_map[((node[1])*map_width)+(node[0]+1)]==false){ //if adjacent is NOT blocked
                for(int i = 0; i < map_items[((node[1])*map_width)+(node[0]+1)].size() ; i++){ //iterate through all items on the adjacent node
                    if( map_items[((node[1])*map_width)+(node[0]+1)][i].type == item_type){
                        //We found target!
                        temp = {node[0]+1, node[1], node[2]+1}; //temporary
                        search_q.push_back(temp); //add to search_q
                        findingTarget = false;
                        break;
                    }
                }//end for
            }

            if(findingTarget==false){break;} //?right???????

            //Otherwise, add node to search_q (if eligible, ofc)

            if(block_map[((node[1])*map_width)+(node[0]+1)]==false){ //if adjacent is NOT blocked
                //we also gotta make sure node isn't in queue with lower distance already...
                //cycle through the list and check for every lower distance...
                if(!isNodeIn({node[0]+1,node[1],0}, search_q)){
                    temp = {node[0]+1, node[1], node[2]+1};
                    search_q.push_back(temp);
                }
            }
        }
        //UP
        if(node[1]-1 > 0){ //bounds checking
            if(block_map[((node[1]-1)*map_width)+(node[0])]==false){ //if adjacent is NOT blocked
                for(int i = 0; i < map_items[((node[1]-1)*map_width)+(node[0])].size() ; i++){ //iterate through all items on the adjacent node
                    if( map_items[((node[1]-1)*map_width)+(node[0])][i].type == item_type){
                        //We found target!
                        temp = {node[0], node[1]-1, node[2]+1}; //temporary
                        search_q.push_back(temp); //add to search_q
                        findingTarget = false;
                        break;
                    }
                }//end for
            }

            if(findingTarget==false){break;} //?right???????

            //Otherwise, add node to search_q (if eligible, ofc)

            if(block_map[((node[1]-1)*map_width)+(node[0])]==false){ //if adjacent is NOT blocked
                //we also gotta make sure node isn't in queue with lower distance already...
                //cycle through the list and check for every lower distance...
                if(!isNodeIn({node[0],node[1]-1,0}, search_q)){
                    temp = {node[0], node[1]-1, node[2]+1};
                    search_q.push_back(temp);
                }
            }
        }
        //DOWN
        if(node[1]+1 < map_height){ //bounds checking
            if(block_map[((node[1]+1)*map_width)+(node[0])]==false){ //if adjacent is NOT blocked
                for(int i = 0; i < map_items[((node[1]+1)*map_width)+(node[0])].size() ; i++){ //iterate through all items on the adjacent node
                    if( map_items[((node[1]+1)*map_width)+(node[0])][i].type == item_type){
                        //We found target!
                        temp = {node[0], node[1]+1, node[2]+1}; //temporary
                        search_q.push_back(temp); //add to search_q
                        findingTarget = false;
                        break;
                    }
                }//end for
            }

            if(findingTarget==false){break;} //?right???????

            //Otherwise, add node to search_q (if eligible, ofc)

            if(block_map[((node[1]+1)*map_width)+(node[0])]==false){ //if adjacent is NOT blocked
                //we also gotta make sure node isn't in queue with lower distance already...
                //cycle through the list and check for every lower distance...
                if(!isNodeIn({node[0],node[1]+1,0}, search_q)){
                    temp = {node[0], node[1]+1, node[2]+1};
                    search_q.push_back(temp);
                }
            }
        }
        k = k+1;
        if(k > 400){ //if we've been searching too long, give up
            k = search_q.size(); //just go to end of queue so we can return error code
        }

    }//Broke out of while loop means we found target

    //Now we need to go down the list (popping) and only keep nodes that apply to the CORRECT PATH
    //We create a list goind BACKWARD from target to current
    vector<vector<int>> path2target;//the list of nodes leading from target
    temp = search_q.back(); //get last element from list
    search_q.pop_back(); //now remove that element since we won't need it in list
    path2target.push_back(temp);//Add first node to path
    while(true){
        temp = search_q.back();
        search_q.pop_back(); //Get last value out of search queue
        if(temp[2] == 0){ //means 0 steps left and we found original node (our starting position
            break;
        }
        if(temp[2] == path2target.back()[2]- 1){ //if current node is actually a step closer
            //check if it's a step in the right direction (AND ONLY 1 unit away from last step)
            if(   ((abs(temp[0]-path2target.back()[0])==1)&&(abs(temp[1]-path2target.back()[1])==0)) || //check if x diff is 1
               ((abs(temp[0]-path2target.back()[0])==0)&&(abs(temp[1]-path2target.back()[1])==1)) ){ //check if y diff is 1
                path2target.push_back(temp);
            }

        }

    }

    //return search_q;
    //reverse(path2target.begin(), path2target.end());
    cre->path = path2target;

    return path2target; //return the list backwards so you can pop_back to get the next step needed

}

//Free Path'- deletes the path of a given creature
//Gotta delete each individually?
void free_path(Sprite cre){
    for(int i = 0; i < cre.path.size(); i++){
        cre.path.erase(cre.path.begin()+cre.path.size()-1);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//THREADS - ACTIONS ////////////////////////////////////////////////////////////////////////////////////

//a thread for wandering to a random place
void wander_thread(Sprite* spr1){
    //    for(int i = 0; i<401; i++){
    //    //while(true){
    //        //printf("%d", spr1.x );
    //        printf("XXXXXXXX%dXXXXXXXX",spr1->x);
    //    }
    
    //Standard movement timing stuff
    spr1->move_timer = SDL_GetTicks(); //start move timer
    int steps_to_pop = 0; //how many steps need to be popped off path
    int carry_over = 0; //how many ticks were rounded off
    
    while(true){
        
        //check if thread should expire
        if( difftime(time(NULL) , spr1->thread_timer) > 30.0 ){
            spr1->inThread = false;
            break;
        }
        
        if(spr1->path.empty()){ //if path is empty
            spr1->path = A_Star(block_map, map_width, map_height, spr1->x, spr1->y, rand()%map_width, rand()%map_height );
        }
    
        //Check if the search failed (error code (9999,9999)
        if(spr1->path[0][0] == 9999){
            spr1->path.pop_back();
            continue; //search failed, try again....
        }
        
        //Determine how many steps have to be moved while time has passed
        steps_to_pop = ( (SDL_GetTicks() - spr1->move_timer)*(spr1->move_speed/1000.0) );
        if(steps_to_pop <= 0){
            continue; //no steps need to be taken, continue
        }
        //Now pop off that many steps
        for(int j = 0 ; j < steps_to_pop-1 ; j++){
            if(spr1->path.size()>1){ //if the path list is non-empty
                //Need to register the skipped steps in "prev values"
                spr1->moveTo(spr1->path[spr1->path.size()-1][0], spr1->path[spr1->path.size()-1][1]);
                spr1->path.pop_back(); //pop off the last element (skip it)
            }
        }
    
        vector<int> next_step = spr1->path[spr1->path.size()-1]; //the last element of array/vector
        //Now actually move
        spr1->moveTo(next_step[0], next_step[1]);
        //pop off the step from path
        spr1->path.pop_back();
        
        //update timer
        //First, determine how much carry-over we need to keep (time we haven't accounted for due to rounding
        carry_over = (SDL_GetTicks() - spr1->move_timer); //How much time we have on timer, total
        carry_over = carry_over - (steps_to_pop*1000.0/spr1->move_speed); //now subtract how much time we've accounted for
                                                                          //carry_over now has how many ticks we haven't updated for
        spr1->move_timer = SDL_GetTicks() - carry_over; //Now update the timer and considering unaccounted for time
        
    }
    return;
}

//A thread for finding the nearest shroom depot
//Call the AStar search algorithm
//Travels to the given shroom
//Transfers item from inventory
void shroom_depo_thread(Sprite* spr1){
    
    //STNDARD-ISSUE MOVEMENT TIMING VARIABLES
    spr1->move_timer = SDL_GetTicks(); //start move timer
    int steps_to_pop = 0; //how many steps need to be popped off path
    int carry_over = 0; //how many ticks were rounded off
    
    int shroom_index = rand()%map_shrooms.size(); //pick a random shroom to go to
    
    //trying to find a path to shroom
    while(true){
        if(spr1->path.empty()){ //if path is empty
            //find a new target path
            spr1->path = A_Star(block_map, map_width, map_height, spr1->x, spr1->y, map_shrooms[shroom_index].x, map_shrooms[shroom_index].y );
        }
        
        //Check if the search failed (error code (9999,9999)
        if(spr1->path[0][0] == 9999){
            spr1->path.pop_back();
            printf("Search failed in shroom depo");
            shroom_index = rand()%map_shrooms.size(); //pick another random shroom to go to
            continue; //search failed, try again....
        }
        
        
        break;
    }
    
    //now the actual going to the shroom
    while(true){
        
        //STANDARD MOVEMENT TIMING CALCULATIONS
        //Determine how many steps have to be moved while time has passed
        steps_to_pop = ( (SDL_GetTicks() - spr1->move_timer)*(spr1->move_speed/1000.0) );
        if(steps_to_pop <= 0){
            continue; //no steps need to be taken, continue
        }
        //Now pop off that many steps
        for(int j = 0 ; j < steps_to_pop-1 ; j++){
            if(spr1->path.size()>1){ //if the path list is non-empty
                //Need to register the skipped steps in "prev values"
                spr1->moveTo(spr1->path[spr1->path.size()-1][0], spr1->path[spr1->path.size()-1][1]);
                spr1->path.pop_back(); //pop off the last element (skip it)
            }
        }
        //END MOVEMENT TIMING
        
        vector<int> next_step = spr1->path[spr1->path.size()-1]; //the last element of array/vector
        //Now actually move
        spr1->moveTo(next_step[0], next_step[1]);
        //pop off the step from path
        spr1->path.pop_back();
        
        //Check if done with path
        if(spr1->path.empty()){ //then it's empty
            
            //now switch the item into the shroom inventory
            map_shrooms[shroom_index].inventory.push_back(spr1->inventory[ spr1->inventory.size() - 1 ]);
            spr1->inventory.pop_back();
            
            spr1->inThread = false;
            break;
        }
        
        //STANDARD MOVEMENT TIMER UPDATE SEQUENCE
        //First, determine how much carry-over we need to keep (time we haven't accounted for due to rounding
        carry_over = (SDL_GetTicks() - spr1->move_timer); //How much time we have on timer, total
        carry_over = carry_over - (steps_to_pop*1000.0/spr1->move_speed); //now subtract how much time we've accounted for
        //carry_over now has how many ticks we haven't updated for
        spr1->move_timer = SDL_GetTicks() - carry_over; //Now update the timer and considering unaccounted for time
        
    }
    return;
    
}

//a thread for picking up items of your favorite color
//Call the faveColorSearch algorithm
//Travels to proper tile
//picks up item (if it's still there)
void gather_thread(Sprite* spr1){

    //STNDARD-ISSUE MOVEMENT TIMING VARIABLES
    spr1->move_timer = SDL_GetTicks(); //start move timer
    int steps_to_pop = 0; //how many steps need to be popped off path
    int carry_over = 0; //how many ticks were rounded off

    int color_thresh = 100; //the maximum difference between item color and fave color,  allowable
    //int search_item = 313; //the item type we are searching for

    //spr1->path.clear(); //clear path and start clean

    while(true){

        //find a new target path
        if(spr1->path.empty()){ //if path is empty
            faveColorSearch(spr1, color_thresh);
            //itemTypeSearch(spr1, search_item);
        }

        //Check if the search failed (error code (9999,9999)
        if(spr1->path[0][0] == 9999){
            spr1->path.pop_back();
            //continue; //search failed, try again....
            //search failed
            //EXPIRE THREAD:
            printf("can't gather");
            spr1->inThread = false;
            break;
        }

        //STANDARD MOVEMENT TIMING CALCULATIONS
        //Determine how many steps have to be moved while time has passed
        steps_to_pop = ( (SDL_GetTicks() - spr1->move_timer)*(spr1->move_speed/1000.0) );
        if(steps_to_pop <= 0){
            continue; //no steps need to be taken, continue
        }
        //Now pop off that many steps
        for(int j = 0 ; j < steps_to_pop-1 ; j++){
            if(spr1->path.size()>1){ //if the path list is non-empty
                //Need to register the skipped steps in "prev values"
                spr1->moveTo(spr1->path[spr1->path.size()-1][0], spr1->path[spr1->path.size()-1][1]);
                spr1->path.pop_back(); //pop off the last element (skip it)
            }
        }
        //END MOVEMENT TIMING

        vector<int> next_step = spr1->path[spr1->path.size()-1]; //the last element of array/vector
        //Now actually move
        spr1->moveTo(next_step[0], next_step[1]);
        //        spr1->x = next_step[0];
        //        spr1->y = next_step[1];
        //pop off the step from path
        spr1->path.pop_back();

        //If we've reach target...
        if(spr1->path.empty()){

            //now look on tile and check if item is still there and pick up
            for(int i = 0; i < map_items[(spr1->y*map_width)+spr1->x].size(); i++){
                if(color_diff(map_items[(spr1->y*map_width)+spr1->x][i].primColor, spr1->faveColor)<color_thresh ||
                   color_diff(map_items[(spr1->y*map_width)+spr1->x][i].primColor, spr1->faveColor2)<color_thresh){
                    //if(map_items[(spr1->y*map_width)+spr1->x][i].type == search_item){

                    pickUpItem(spr1, spr1->x, spr1->y, i); //then pick up the item

                    //Start trek to shroom depot...
                    //we're going to transfer it to another thread...
                    free_path(*spr1); //clear old path
                    //spr1->inThread = false; //we can keep the status as true since we're changing threads.( normally have to set this flag)
                    //This thread makes the creature go to shroom
                    //std::thread shroomDepoObj(shroom_depo_thread, spr1);
                    std::thread shroomDepoObj(shroom_depo_thread, spr1);
                    shroomDepoObj.detach();
                    if(spr1->isNeededByThread){printf("is neeeded'\n");}
                    return;

                }

            }

            //break;
        }

        //STANDARD MOVEMENT TIMER UPDATE SEQUENCE
        //First, determine how much carry-over we need to keep (time we haven't accounted for due to rounding
        carry_over = (SDL_GetTicks() - spr1->move_timer); //How much time we have on timer, total
        carry_over = carry_over - (steps_to_pop*1000.0/spr1->move_speed); //now subtract how much time we've accounted for
        //carry_over now has how many ticks we haven't updated for
        spr1->move_timer = SDL_GetTicks() - carry_over; //Now update the timer and considering unaccounted for time
        //SDL_Delay(500);

    }
    spr1->inThread = false; //if it reaches here and exits ,then clear flag
    return;
}

//A thread that moves the input sprite along it's stored path
//Thread is timed to reflect Sprite's movement speed
//The sprite should have it's path set before starting this thread (or else it does nothing)
//THe sprite's inThread flag is set to false when this thread finishes running
void walk_path_thread(Sprite* spr1){
    //STNDARD-ISSUE MOVEMENT TIMING VARIABLES
    spr1->move_timer = SDL_GetTicks(); //start move timer
    int steps_to_pop = 0; //how many steps need to be popped off path
    int carry_over = 0; //how many ticks were rounded off
    
    //Check to make sure path is non-empty
    if(spr1->path.size() <= 0){
        spr1->inThread = false;
        return;
    }
    
    while( true ){
        //Determine how many steps have to be moved while time has passed
        steps_to_pop = ( (SDL_GetTicks() - spr1->move_timer)*(spr1->move_speed/1000.0) ); // (elapsed time * movement speed) / 1000 ms
        if(steps_to_pop <= 0){
            continue; //no steps need to be taken, continue
        }
        //Now pop off that many steps
        for(int j = 0 ; j < steps_to_pop-1 ; j++){
            if(spr1->path.size()>1){ //if the path list is non-empty (and has at least 1 element, which will be saved for actual movement)
                //Need to register the skipped steps in "prev values"
                spr1->moveTo(spr1->path[spr1->path.size()-1][0], spr1->path[spr1->path.size()-1][1]);
                spr1->path.pop_back(); //pop off the last element (skip it)
            }
        }
        
        vector<int> next_step = spr1->path[spr1->path.size()-1]; //the last element of array/vector
        //Now actually move
        spr1->moveTo(next_step[0], next_step[1]);
        //pop off the step from path
        spr1->path.pop_back();
        
        //Check if done with path
        if(spr1->path.size()<1){ //then it's empty
            spr1->inThread = false;
            break;
        }
        
        //update timer
        //First, determine how much carry-over we need to keep (time we haven't accounted for due to rounding
        carry_over = (SDL_GetTicks() - spr1->move_timer); //How much time we have on timer, total
        carry_over = carry_over - (steps_to_pop*1000.0/spr1->move_speed); //now subtract how much time we've accounted for
        //carry_over now has how many ticks we haven't updated for
        spr1->move_timer = SDL_GetTicks() - carry_over; //Now update the timer and considering unaccounted for time
    }
    
    free_path(*spr1); //clear old path
    return;
    
}

//Thread that performs a ritual
//spawns other threads (walkTo) and also handles isNeededByThread flag
void perform_ritual_thread(Sprite* spr1){
    
    int loc_x, loc_y; //the location of ritual
    
    //determine random ritual location
    while(true){
        //TODO: Make sure location isn't blocked or have an animation on currently
        loc_x = 1+(rand()%(map_width-2));
        loc_y = 1+(rand()%(map_height-2));
        //Check to make sure location isn't blocked or has an animation on it currently
        if( (block_map[(loc_y*map_width)+loc_x]==true) || map_animations[(loc_y*map_width)+loc_x].size() > 0){
            continue;
        }else{
            break;
        }
    }
    
    
    //Firstly, indicate to the sprites that we need them...
    //the isNeededByThread Flag is used to indicate to the task_creatures_thread not to schedule it anything new (we'll handle that)
    //mainly used for OTHER creatures, outside thread...
    spr1->isNeededByThread = true;
    
    //START WALKING TO LOCATION
    
    //Find a path to target
    free_path(*spr1); //clear path on sprite for starters
    if(spr1->path.empty()){ //if path is empty
        spr1->path = A_Star(block_map, map_width, map_height, spr1->x, spr1->y, loc_x+1, loc_y );
    }

    //Check if the search failed (error code (9999,9999)
    if(spr1->path[0][0] == 9999){
        spr1->path.pop_back();
        printf("search failed, no path to ritual");
        
        //Just exit and try something else
        spr1->inThread = false;
        spr1->isNeededByThread = false;
        return; //search failed, try again....
    }
        

    //Now start the thread that will actually walk the path to target
    std::thread walkPathObj(walk_path_thread, spr1);
    walkPathObj.detach();
    //walkPathObj.join(); //block (wait for it to complete)
    
    //Wait for spr1 to get to destination (walk_to_thread signals end by setting inThread to false)
    while(true){
        
//        //Check if spr1 reached the proper desitnation??
//        if( (spr1->x-1 == loc_x) && (spr1->y == loc_y) ){
//            printf("got there\n");
//        }
        
        //if done with walk_to_thread
        if(spr1->inThread == false){
            
            //Start a fire Animation (standard protocol)
            int list[4] = {22, 23, 24, 25};
            Animation temp_animation = Animation(loc_x, loc_y, list);
            map_animations[ ( (loc_y) * map_width ) + (loc_x) ].push_back(temp_animation);
            
            //Start Timer on sprite
            spr1->thread_timer = SDL_GetTicks();
            
            break; //break out of wait loop
        }
        
    }
    
    //Now wait a certain amount of time to complete ritual
    while(true){
        
        if(SDL_GetTicks() > spr1->thread_timer + 10000){ //waiting 10 seconds
            
            //BREAK OUT OF RITUAL
            
            //Turn off flags
            spr1->inThread = false;
            spr1->isNeededByThread = false;
            
            //Delete Animation Object
            // erase the 6th element
            //myvector.erase (myvector.begin()+5);
            if(map_animations[(loc_y*map_width)+loc_x].size()>0){ //make sure the animation in question is still there...
                //printf("im at: %d,%d\n",spr1->x,spr1->y);
                map_animations[(loc_y*map_width)+loc_x].erase(map_animations[(loc_y*map_width)+loc_x].begin());
            }
            
            break;
            
        }
        
    }
    
    //at this point, spr1 is elegible for new tasks and Animation is deleted
    //We now want to create a new item in it's place
    //Pick random item from shroom recipe list
    //Create a basic dual-tone item based off shroom recipe resource list
    Shroom temp_shroom = map_shrooms[rand()%map_shrooms.size()]; //pick a random shroom
    Recipe temp_recipe = temp_shroom.recipe_list[rand()%temp_shroom.recipe_list.size()]; //pick a random resource from list
    
    //Create an dual-tone item
    Item temp_item = Item(loc_x, loc_y, temp_recipe.item_type, temp_recipe.ingredient1.colorPrim,temp_recipe.ingredient2.colorPrim ); //create temporary item from recipe's fields
    map_items[loc_y*map_width+loc_x].push_back(temp_item);
    
    //Let's announce the items's creation
    cout << "\n" << temp_recipe.name << "\n";
    cout << temp_recipe.description << "\n";
    
    
    //we also wanna add some effects to spruce things up for a bit
    Effect temp_effect = Effect(loc_x, loc_y - 1, 1); //create the effect above the newly created item. Effect code for sparkles is 1 (last argument)
    map_effects[((loc_y - 1)*map_width)+loc_x].push_back(temp_effect); //add the effect to global queue so it can be drawn
    
    //Now wait 10 seconds to delete the effect
    int ritual_timer = SDL_GetTicks(); //start the timer
    while(true){
        
        if(SDL_GetTicks() > ritual_timer+10000){ //waiting 10 seconds
            if(map_effects[((loc_y - 1)*map_width)+loc_x].size() > 0){
                map_effects[((loc_y - 1)*map_width)+loc_x].erase(map_effects[((loc_y - 1)*map_width)+loc_x].begin());
            }
            
            break;
            
        }
        
    }
    
    return;
    
}

//periodically gives a new task to creatures
//This function starts threads and sets inThread flag = true
//**Individual spawned threads are responsible for inThread flag = false again
void task_creatures_thread(){
    
    short choice = 0; //decides which thread to run
    
    while(true){
        
        //cycle through all creatures
        for(int i = 0; i < map_creatures.size(); i++){
            
            //Check if isNeededByThread flag is set (indicating another thread will take responsibility for setting flags )
            if(map_creatures[i].isNeededByThread == true){
                continue; //don't do anything to creature
            }
            
            if(map_creatures[i].inThread == false){ //we need to assign it a new thread
                //map_creatures[i].path.clear(); //clear the old path
                free_path(map_creatures[i]);//clear old path, fuction to individually delete path
                map_creatures[i].thread_timer = time(NULL);
                map_creatures[i].inThread = true;
                //choice = rand()%3;
                choice = rand()%2 * 2; 
                switch(choice){
                    case 0: {
                        //This thread makes the creature gather
                        //std::thread gatherObj(gather_thread, &map_creatures[i]);
                        std::thread gatherObj(gather_thread, &map_creatures[i]);
                        gatherObj.detach();
                        break;
                    }
                    case 1: {
                        //This thread wanders the input creature
                        //std::thread wanderObj(wander_thread, &map_creatures[i]);
                        std::thread wanderObj(wander_thread, &map_creatures[i]);
                        wanderObj.detach();
                        break;
                    }
                        
                    case 2: {
                        //This thread makes the creatures walk somewhere specifc
                        std::thread ritualObj(perform_ritual_thread, &map_creatures[i]);
                        ritualObj.detach();
                        break;
                    }
                }
            }
        }
        
        
        SDL_Delay(2000); //doesn't have to run all the time
        
    }
}

void background_color_thread(){
    
    //STNDARD-ISSUE MOVEMENT TIMING VARIABLES
    int steps_to_pop = 0; //how many steps need to be ticked on the colors
    int carry_over = 0; //how many ticks were rounded off
    int BACKGROUND_CHANGE_SPEED = 500; //constant used to throttle how oftern background color changes
    int animate_timer = SDL_GetTicks();
    
    //How much we change the colors
    int r_inc = 1;
    int g_inc = 1;
    int b_inc = 1;
    
    //THESE COLORS SHOULD RANGE FROM 1 to 254 (NO 0 or 255!!!!)
    //beginning color
    int r1 = 1;
    int g1 = 1;
    int b1 = 1;
    //stop color
    int r2 = (rand()%223) + 30;
    int g2 = (rand()%223) + 30;
    int b2 = (rand()%223) + 30;
    //BOUNCES BACK AND FORTH BETWEeN THE VALUES
    
    //initialize background color (global)
    back_col = {static_cast<Uint8>(rand()%(r2-5)), static_cast<Uint8>(rand()%(g2-5)), static_cast<Uint8>(rand()%(b2-5))};
    
    //THE ACTUAL DRAW LOOP ////////////////////////////////////
    //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    int new_r, new_g, new_b; //temporary variables used t check rgb values before writing them
    while(true){
        
        //////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////
        ///// FIRST PART: Update background color
        
        //Determine how many steps have to be moved while time has passed
        steps_to_pop = ( SDL_GetTicks() - animate_timer ) / BACKGROUND_CHANGE_SPEED; //ONe tick every 100 ms
        
        //NOW apply that many
        new_r = back_col.r + (steps_to_pop*r_inc);
        new_g = back_col.g + (steps_to_pop*g_inc);
        new_b = back_col.b + (steps_to_pop*b_inc);
        //bounds checkings
        if(new_r > r2){
            r_inc = r_inc * -1;
            new_r = r2;
        }
        if(new_r < r1){
            r_inc = r_inc * -1;
            new_r = r1;
        }
        
        if(new_g > g2){
            g_inc = g_inc * -1;
            new_g = g2;
        }
        if(new_g < g1){
            g_inc = g_inc * -1;
            new_g = g1;
        }
        
        if(new_b > b2){
            b_inc = b_inc * -1;
            new_b = b2;
        }
        if(new_b < b1){
            b_inc = b_inc * -1;
            new_b = b1;
        }
        //Now that values have been checked, we can write them
        back_col.r = new_r;
        back_col.g = new_g;
        back_col.b = new_b;
        
        
        //Update animate timer
        //Figure out how much time not accounted for
        carry_over = SDL_GetTicks() - animate_timer; //How much time has elapsed since last timing
        carry_over = carry_over - (steps_to_pop * BACKGROUND_CHANGE_SPEED); //don't count the time we *have* accounted for
        //Now check new timer but also remember the time we haven't accounted for
        animate_timer = SDL_GetTicks() - carry_over; //Now update the timer, but set it back for unaccounted time
        //////////////////////////
        //DONE TAKING CARE OF BACKGROUND UPDATES
        
        
    }
    
}

//regenerates weedz periodically
void regen_weedz_thread(){
    
    int x, y, item_id = 0;
    while(true){
        //pick a random spot
        x = rand()%map_width;
        y = rand()%map_height;
        
        if(!block_map[y*map_width+x]){ //check if bloecked
            if(map_items[y*map_width+x].size()<2){ //if tile doesn't have too many items on it
                
                //If there is an herb there, plant a fruit
                for(int y = 0; y < map_items[y*map_width+x].size(); y++){ //cycle through items on tiles
                    if(map_items[y*map_width+x][y].type == 310){ //if item is a weed
                        //create a fruit here
                        item_id = 311; //id for fruit
                        printf("spawn: fruit\n", item_id);
                        Item temp_fruit = Item(x, y, item_id, {static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255),255},{255,255,255,0} ); //temporary item
                        map_items[y*map_width+x].push_back(temp_fruit);
                        SDL_Delay(5000);
                        continue;
                    }
                }
                
//                //Create an dual-tone item
//                //item_id = rand()%300; //pick a random item id
//                item_id = (313+rand()%3); //pick a random item id
//                printf("spawn: %d\n", item_id);
//                //Item temp_item = Item(x, y, item_id, {static_cast<Uint8>(colorz[item_id][0]), static_cast<Uint8>(colorz[item_id][1]), static_cast<Uint8>(colorz[item_id][2]),255},{255,255,255,0} ); //temporary item
//                //Item temp_item = Item(x, y, item_id, {static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255),255},{255,255,255,0} ); //temporary item
//                Item temp_item = Item(x, y, item_id, {static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255),255},{static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255),255} ); //temporary item
//                map_items[y*map_width+x].push_back(temp_item);
//                SDL_Delay(5000);
                
                x = rand()%map_width;
                y = rand()%map_height;
                
                //Create a basic single-tone item based off shroom recipe resource list
                Shroom temp_shroom = map_shrooms[rand()%map_shrooms.size()]; //pick a random shroom
                Resource temp_recipe = temp_shroom.resource_list[rand()%temp_shroom.resource_list.size()]; //pick a random resource from list
                //assign that item to be created
                item_id = temp_recipe.item_type;
                SDL_Color temp_col = temp_recipe.colorPrim;
                Item temp_item = Item(x, y, item_id, temp_col,{255, 255, 255,0} ); //temporary item
                map_items[y*map_width+x].push_back(temp_item);
                SDL_Delay(5000);
                
            }
        }
        
    }
    
}

//A thread that randomly moves chracter around map
void wander_player_thread(Sprite* cre1){
    
    int choice = rand() % 4; ; //holds a randome choice
    while(true){
        
        //Don't do anything if wander isn't enabled.
        if(is_player_wandering == false){
            continue;
        }
        
        switch (choice) {
            case 0:
                //move up
                //bounds checking
                if(cre1->y < 1){
                    break;
                }
                //blocked_check
                if(block_map[(cre1->y-1)*map_width+(cre1->x)]==true){
                    break;
                }
                //If we made it here, then it's okay to move
                cre1->moveUp();
                break;
            case 1:
                //move left
                //First, do bounds checking
                if(cre1->x < 1){
                    break;
                }
                //blocked_check
                if(block_map[(cre1->y)*map_width+(cre1->x-1)]==true){
                    break;
                }
                //If we made it here, then it's okay to move
                cre1->moveLeft();
                break;
            case 2:
                //Move down
                //Also move one step to seem more responsive upon first down
                //First, do bounds checking
                if(cre1->y >= map_height-1){
                    break;
                }
                //blocked_check
                if(block_map[(cre1->y+1)*map_width+(cre1->x)]==true){
                    break;
                }
                //If we made it here, then it's okay to move
                cre1->moveDown();
                break;
            case 3:
                //Move right
                //Also move one step to seem more responsive upon first down
                //First, do bounds checking
                if(cre1->x >= map_width-1){
                    break;
                }
                //blocked_check
                if(block_map[(cre1->y)*map_width+(cre1->x+1)]==true){
                    break;
                }
                //If we made it here, then it's okay to move
                cre1->moveRight();
                break;
                
            default:
                break;
        }
        
        //update choice for next iteration
        choice = rand()%4;
        
        //Finally, update draw_map indices
        draw_map_y = cre1->y - draw_map_height/2;
        draw_map_x = cre1->x - draw_map_width/2;
        
        SDL_Delay(500);
        
    }
    
}

//////////////////////////
//Display/Interface Functions

void addToConsoleLog(string msg){
    consoleLog.push(msg);
    if(consoleLog.size() > 9){ //if queue is too big, pop last element
        consoleLog.pop();
    }
}

//Create a display window for game story console messages
SDL_Surface* console_display_surface;
SDL_Texture* console_display_texture;
SDL_Color console_font_clr = {255,255,255}; //color of font
int console_texW = 650;//constants used in displaying fonts
int console_texH = 0;//constants used in displaying fonts
void displayConsole(SDL_Renderer* gRenderer, queue<string> consoleLog){
    string console_text = ""; //Char buffer that is displayed
    
    //We're gonna have to pick out the proper strings to print out to console
    //WITH THIS CONFIGURATION, we only have about 9 lines
    queue<string> temp_log = consoleLog;
    for(int i=0; i<consoleLog.size(); i++){
        console_text += temp_log.front() + "\n";
        temp_log.pop();
    }
    
    //Blackout Box {x,y,w,h}
    SDL_Rect r = {SCREEN_WIDTH-650,SCREEN_HEIGHT-150,SCREEN_WIDTH,150};
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderFillRect(gRenderer, &r);
    console_display_surface = TTF_RenderText_Blended_Wrapped(font1, console_text.c_str(), console_font_clr, 1550);
    console_display_texture = SDL_CreateTextureFromSurface(gRenderer, console_display_surface);
    //create text rect (text box)
    SDL_QueryTexture(console_display_texture, NULL, NULL, &console_texW, &console_texH); //gets the dimenstions of the font/text
    SDL_Rect dstrect = {SCREEN_WIDTH-650,SCREEN_HEIGHT-150, console_texW, console_texH }; //so we can make the proper rect to dispay it
    SDL_RenderCopy(gRenderer, console_display_texture, NULL, &dstrect); //write
    //FREE MEMORY
    SDL_DestroyTexture(console_display_texture);
    SDL_FreeSurface(console_display_surface);
    //FINISH PUtting the Title: Status

    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE); //turn alpha back off
    
}



//////////////////////
//OTHER FUNCTIONS

//Make an attempt to free up  all the memory
void close(){
    TTF_Quit();
    SDL_DestroyWindow(gWindow);
    SDL_DestroyRenderer(gRenderer);
    
}

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
    init_environment();
    
    //...Sprites
    Sprite* cre1 = new Sprite(draw_map_x + (int)(draw_map_width/2.0),draw_map_y + (int)(draw_map_height/2.0) ); //start creaturein the middle of the screen
    printf("%d,%d\n",(int)(draw_map_width), (int)(draw_map_height/2.0));
    cre1->loadFromFile("Civ2/Civ2/tiles/crePrim.png","Civ2/Civ2/tiles/creSeco.png", 16, 16);
    
    Hat temp_accessory = Hat(0, 0, 305); //a temp Item to be added to cre's inventory
    cre1->hat = &temp_accessory; //give him a hat
    
    //Give cre1 a flag
    Staff temp_staff = Staff(0,0,309); //a temp Item to be added to the cre's equip inventory
    cre1->staff = &temp_staff;

    //RECIPES
    for(int j = 0; j <3; j++){

        //Generate some new resource recipes
        for(int w = 0; w < 5; w++){
            //Generates a recipe and generatess a required item
            Resource temp_resource_recipe1 = Resource(rand()%4);
            cout << temp_resource_recipe1.name << "\n";
            Item temp_item = Item(10+j, 5+w, temp_resource_recipe1.item_type, temp_resource_recipe1.colorPrim, temp_resource_recipe1.colorSeco);
            map_items[(temp_item.y * map_width) + temp_item.x].push_back(temp_item);
        }
        
    }
    
    //Generate some new recipe types
    for(int w = 0; w < 5; w++){
        //Generates a recipe and generates an example item
        Recipe temp_recipe1 = Recipe(0, Resource(2), Resource(0));
//        cout << temp_recipe1.name << "\n";
        cout << temp_recipe1.description << "\n";
        Item temp_item = Item(3, 5+w, temp_recipe1.item_type, temp_recipe1.ingredient1.colorPrim, temp_recipe1.ingredient2.colorPrim);
        map_items[(temp_item.y * map_width) + temp_item.x].push_back(temp_item);
        
        //Generates a recipe and generates an example item
        Recipe temp_recipe2 = Recipe(1, Resource(1), Resource(3));
//        cout << temp_recipe2.name << "\n";
        cout << temp_recipe2.description << "\n";
        temp_item = Item(4, 5+w, temp_recipe2.item_type, temp_recipe2.ingredient1.colorPrim, temp_recipe2.ingredient2.colorPrim);
        map_items[(temp_item.y * map_width) + temp_item.x].push_back(temp_item);
        
        //Generates a recipe and generates an example item
        Recipe temp_recipe3 = Recipe(2, Resource(4), Resource(2));
//        cout << temp_recipe3.name << "\n";
        cout << temp_recipe3.description << "\n";
        temp_item = Item(5, 5+w, temp_recipe3.item_type, temp_recipe3.ingredient1.colorPrim, temp_recipe3.ingredient2.colorPrim);
        map_items[(temp_item.y * map_width) + temp_item.x].push_back(temp_item);
        
        cout << genBerryName() << "\n";
        
    }
    
//    //DEBUG: Figure out wine colors. Will make a nice wine and herb display
//    //wine made of herb and berry
//    for(int r = 0 ; r < 5; r ++){
//        //Generate them
//        Resource berry = Resource(4); //generate a berry
//        Resource cloth = Resource(3); //generate a cloth (for label)
//        Recipe wine = Recipe(2, berry, cloth); //create a recipe from the generated resources.
//
//        Item temp_berry = Item(16, 5+r, berry.item_type, berry.colorPrim, berry.colorSeco);
//        map_items[(temp_berry.y * map_width) + temp_berry.x].push_back(temp_berry);
//        Item temp_cloth = Item(17, 5+r, cloth.item_type, cloth.colorPrim, cloth.colorSeco);
//        map_items[(temp_cloth.y * map_width) + temp_cloth.x].push_back(temp_cloth);
//        Item temp_wine = Item(18, 5+r, wine.item_type, wine.ingredient1.colorPrim, wine.ingredient2.colorPrim);
//        map_items[(temp_wine.y * map_width) + temp_wine.x].push_back(temp_wine);
//
//        cout << wine.name << "\n";
//        cout << wine.description << "\n";
//
//    }

//    //DEBUG: Figure out potion colors. Will make a nice potion and herb display
//    //potion made out of oil and glass
//    for(int r = 0 ; r < 5; r ++){
//        //Generate them
//        Resource oil = Resource(2); //generate a berry
//        Resource glass = Resource(0); //generate a cloth (for label)
//        Recipe potion = Recipe(0, oil, glass); //create a recipe from the generated resources.
//
//        Item temp_oil = Item(16, 5+r, oil.item_type, oil.colorPrim, oil.colorSeco);
//        map_items[(temp_oil.y * map_width) + temp_oil.x].push_back(temp_oil);
//        Item temp_glass = Item(17, 5+r, glass.item_type, glass.colorPrim, glass.colorSeco);
//        map_items[(temp_glass.y * map_width) + temp_glass.x].push_back(temp_glass);
//        Item temp_potion = Item(18, 5+r, potion.item_type, potion.ingredient1.colorPrim, potion.ingredient2.colorPrim);
//        map_items[(temp_potion.y * map_width) + temp_potion.x].push_back(temp_potion);
//
//        cout << potion.name << "\n";
//        cout << potion.description << "\n";
//
//    }
    
    //DEBUG: Figure out powder colors. Will make a nice powder and stone display
    //powder made out of stone and cloth
    for(int r = 0 ; r < 5; r ++){
        //Generate them
        Resource salt = Resource(1); //generate a salt
        Resource cloth = Resource(3); //generate a cloth (for label)
        Recipe powder = Recipe(1, salt, cloth); //create a recipe from the generated resources.
        
        Item temp_salt = Item(16, 5+r, salt.item_type, salt.colorPrim, salt.colorSeco);
        map_items[(temp_salt.y * map_width) + temp_salt.x].push_back(temp_salt);
        Item temp_cloth = Item(17, 5+r, cloth.item_type, cloth.colorPrim, cloth.colorSeco);
        map_items[(temp_cloth.y * map_width) + temp_cloth.x].push_back(temp_cloth);
        Item temp_powder = Item(18, 5+r, powder.item_type, powder.ingredient1.colorPrim, powder.ingredient2.colorPrim);
        map_items[(temp_powder.y * map_width) + temp_powder.x].push_back(temp_powder);
        
        cout << powder.name << "\n";
        cout << powder.description << "\n";
        
    }

    
    //
    //DEBUG TEST : SPAWN SOME THREAD DAWGd
    //This thread updates the background color
    std::thread backObj(background_color_thread);
    backObj.detach();
    
    //This thread regenerates weeds
    std::thread regenObj(regen_weedz_thread);
    regenObj.detach();
    //This thread schedules new threads for halted creatures!
    std::thread taskObj(task_creatures_thread);
    taskObj.detach();
    //This thread wanders the player character around the map
    std::thread playerWanderObj(wander_player_thread, cre1);
    playerWanderObj.detach();
    
    //Variables
    bool shiftDown = false; //a flag holding state of shift key
    bool sKeyDown = false; //a flag holding the state of the s key
    int sKeyTimer = SDL_GetTicks();
    bool dKeyDown = false; //a flag holding the state of the d key
    int dKeyTimer = SDL_GetTicks();
    bool aKeyDown = false; //a flag holding the state of the a key
    int aKeyTimer = SDL_GetTicks();
    bool wKeyDown = false; //a flag holding the state of the w key
    int wKeyTimer = SDL_GetTicks();
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
                        if(wKeyDown == false //when you first press down the w key
                           && ~sKeyDown && ~dKeyDown && ~aKeyDown ){
                            wKeyDown = true;
                            wKeyTimer = SDL_GetTicks(); //start timer for w_key down
                            //Also move one step to seem more responsive upon first down
                            //First, do bounds checking
                            if(cre1->y < 1){
                                break;
                            }
                            //blocked_check
                            if(block_map[(cre1->y-1)*map_width+(cre1->x)]==true){
                                break;
                            }
                            //If we made it here, then it's okay to move
                            cre1->moveUp();
                        }
                        break;
                        
                    case SDLK_a:
                        if(aKeyDown == false //when you first press down the a key
                           && ~sKeyDown && ~dKeyDown && ~wKeyDown){
                            aKeyDown = true;
                            aKeyTimer = SDL_GetTicks(); //start timer for a_key down
                            //Also move one step to seem more responsive upon first down
                            //First, do bounds checking
                            if(cre1->x < 1){
                                break;
                            }
                            //blocked_check
                            if(block_map[(cre1->y)*map_width+(cre1->x-1)]==true){
                                break;
                            }
                            //If we made it here, then it's okay to move
                            cre1->moveLeft();
                        }
                        break;
                        
                    case SDLK_s:
                        if(sKeyDown == false //when you first press down the s key
                           && ~aKeyDown && ~dKeyDown && ~wKeyDown ){ //make sure only one movement key pressed at a time
                            sKeyDown = true;
                            sKeyTimer = SDL_GetTicks(); //start timer for s_key down
                            //Also move one step to seem more responsive upon first down
                            //First, do bounds checking
                            if(cre1->y >= map_height-1){
                                break;
                            }
                            //blocked_check
                            if(block_map[(cre1->y+1)*map_width+(cre1->x)]==true){
                                break;
                            }
                            //If we made it here, then it's okay to move
                            cre1->moveDown();
                        }
                        break;
                        
                    case SDLK_d:
                        if(dKeyDown == false //when you first press down the d key
                           && ~aKeyDown && ~sKeyDown && ~wKeyDown){
                            dKeyDown = true;
                            dKeyTimer = SDL_GetTicks(); //start timer for d_key down
                            //Also move one step to seem more responsive upon first down
                            //First, do bounds checking
                            if(cre1->x >= map_width-1){
                                break;
                            }
                            //blocked_check
                            if(block_map[(cre1->y)*map_width+(cre1->x+1)]==true){
                                break;
                            }
                            //If we made it here, then it's okay to move
                            cre1->moveRight();
                        }
                        break;
                        
                    case SDLK_SPACE:{
                        break;
                    }
                        
                    case SDLK_m:
                        addToConsoleLog("This is the potion, "+ genPotionName()+". It is made of "+ genOilName() +".");
                        addToConsoleLog("This is the powder, "+ genPotionName()+". It is made of "+ genSaltName() +".");
                        break;
                        
                    case SDLK_p:{
                        shroomDepoDisplayOn = !shroomDepoDisplayOn;
                        break;
                    }
                        
                    case SDLK_c:{
                        consoleDisplayOn = !consoleDisplayOn;
                        break;
                    }
                        
                    case SDLK_g:{
                        is_player_wandering = !is_player_wandering;
                        break;
                    }
                        
                    //cycle through different creature's inventories
                    case SDLK_PERIOD:
                        creature_inventory_index = creature_inventory_index+1;
                        if(creature_inventory_index >= map_creatures.size()){
                            creature_inventory_index = creature_inventory_index-1;
                        }
                        break;
                    case SDLK_COMMA:
                        creature_inventory_index = creature_inventory_index-1;
                        if(creature_inventory_index < 0){
                            creature_inventory_index = 0;
                        }
                        break;
                        
                    case SDLK_t:{
                        statusDisplayOn = !statusDisplayOn;
                        break;
                    }
                        
                    case SDLK_LSHIFT:
                    case SDLK_RSHIFT:
                        shiftDown = true;
                        break;

                        
                }
            //These key press events only happen when a key goes from being down to up (single not continuous)
            }else if(e.type == SDL_KEYUP){
                switch(e.key.keysym.sym){
                    case SDLK_LSHIFT:
                    case SDLK_RSHIFT:
                        shiftDown = false;
                        break;
                    case SDLK_s:
                        sKeyDown = false;
                        break;
                    case SDLK_d:
                        dKeyDown = false;
                        break;
                    case SDLK_a:
                        aKeyDown = false;
                        break;
                    case SDLK_w:
                        wKeyDown = false;
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
            if(statusDisplayOn){
                //process input for status display
            }
            if(shroomDepoDisplayOn){
                //Process input for shroomDepoDisplay
                if(e.type == SDL_KEYDOWN){
                    switch(e.key.keysym.sym){
                        case SDLK_COMMA:
                            shroom_inventory_index--;
                            //bounds checking
                            if(shroom_inventory_index <= 0){
                                shroom_inventory_index = 0;
                            }
                            break;
                        case SDLK_PERIOD:
                            shroom_inventory_index++;
                            //bounds checking
                            if(shroom_inventory_index >= map_shrooms.size()){
                                shroom_inventory_index = shroom_inventory_index-1;
                            }
                            break;
                        case SDLK_EQUALS:
                            item_display_index++;
                            break;
                        case SDLK_MINUS:
                            item_display_index--;
                            break;
                    }
                    
                }
            }
            
            //Process Movement Buttons Here (They need timers for seamless motion)
            //S-KEY TIMER
            if(sKeyDown){ //process down key movement
                //STNDARD-ISSUE MOVEMENT TIMING VARIABLES
                int steps_to_pop = 0; //how many steps need to be popped off path
                int carry_over = 0; //how many ticks were rounded off
                
                //Determine how many steps have to be moved while time has passed
                steps_to_pop = ( (SDL_GetTicks() - sKeyTimer)*(PLAYER_CREATURE_SPEED/1000.0) ); // (elapsed time * movement speed) / 1000 ms
                //NOW MOVE THOSE STEPS
                for(int j = 0 ; j < steps_to_pop; j++){ //process each step, one, by one
                    
                    //First, do bounds checking
                    if(cre1->y >= map_height-1){
                        break;
                    }
                    //blocked_check
                    if(block_map[(cre1->y+1)*map_width+cre1->x]==true){
                        break;
                    }
                    //If we made it here, then it's okay to move
                    cre1->moveDown();
                    
                    
                }//DONE moving steps
                //update timer
                //First, determine how much carry-over we need to keep (time we haven't accounted for due to rounding
                carry_over = (SDL_GetTicks() - sKeyTimer); //How much time we have on timer, total
                carry_over = carry_over - (steps_to_pop*1000.0/PLAYER_CREATURE_SPEED); //now subtract how much time we've accounted for
                //carry_over now has how many ticks we haven't updated for
                sKeyTimer = SDL_GetTicks() - carry_over; //Now update the timer and considering unaccounted for time
                
                //Finally, update draw_map indices
                draw_map_y = cre1->y - draw_map_height/2;
                
            }//end s key processing
            //D-KEY TIMER
            if(dKeyDown){ //process down key movement
                //STNDARD-ISSUE MOVEMENT TIMING VARIABLES
                int steps_to_pop = 0; //how many steps need to be popped off path
                int carry_over = 0; //how many ticks were rounded off
                
                //Determine how many steps have to be moved while time has passed
                steps_to_pop = ( (SDL_GetTicks() - dKeyTimer)*(PLAYER_CREATURE_SPEED/1000.0) ); // (elapsed time * movement speed) / 1000 ms
                
                //NOW MOVE THOSE STEPS
                for(int j = 0 ; j < steps_to_pop; j++){ //process each step, one, by one

                    //First, do bounds checking
                    if(cre1->x >= map_width-1){
                        break;
                    }
                    //blocked_check
                    if(block_map[(cre1->y)*map_width+(cre1->x+1)]==true){
                        break;
                    }
                    //If we made it here, then it's okay to move
                    cre1->moveRight();


                }//DONE moving steps
               
                //update timer
                //First, determine how much carry-over we need to keep (time we haven't accounted for due to rounding
                carry_over = (SDL_GetTicks() - dKeyTimer); //How much time we have on timer, total
                carry_over = carry_over - (steps_to_pop*1000.0/PLAYER_CREATURE_SPEED); //now subtract how much time we've accounted for
                //carry_over now has how many ticks we haven't updated for
                dKeyTimer = SDL_GetTicks() - carry_over; //Now update the timer and considering unaccounted for time
                
                //Finally, update draw_map indices
                draw_map_x = cre1->x - draw_map_width/2;
                
            }//end d key processing
            
            //A-KEY TIMER
            if(aKeyDown){ //process left key movement
                //STNDARD-ISSUE MOVEMENT TIMING VARIABLES
                int steps_to_pop = 0; //how many steps need to be popped off path
                int carry_over = 0; //how many ticks were rounded off
                
                //Determine how many steps have to be moved while time has passed
                steps_to_pop = ( (SDL_GetTicks() - aKeyTimer)*(PLAYER_CREATURE_SPEED/1000.0) ); // (elapsed time * movement speed) / 1000 ms
                
                //NOW MOVE THOSE STEPS
                for(int j = 0 ; j < steps_to_pop; j++){ //process each step, one, by one
                    
                    //First, do bounds checking
                    if(cre1->x < 1){
                        break;
                    }
                    //blocked_check
                    if(block_map[(cre1->y)*map_width+(cre1->x-1)]==true){
                        break;
                    }
                    //If we made it here, then it's okay to move
                    cre1->moveLeft();
                }//DONE moving steps
                //update timer
                //First, determine how much carry-over we need to keep (time we haven't accounted for due to rounding
                carry_over = (SDL_GetTicks() - aKeyTimer); //How much time we have on timer, total
                carry_over = carry_over - (steps_to_pop*1000.0/PLAYER_CREATURE_SPEED); //now subtract how much time we've accounted for
                //carry_over now has how many ticks we haven't updated for
                aKeyTimer = SDL_GetTicks() - carry_over; //Now update the timer and considering unaccounted for time
                
                //Finally, update draw_map indices
                draw_map_x = cre1->x - draw_map_width/2;
                
            }//end a key processing
            
            //W-KEY TIMER
            if(wKeyDown){ //process up key movement
                //STNDARD-ISSUE MOVEMENT TIMING VARIABLES
                int steps_to_pop = 0; //how many steps need to be popped off path
                int carry_over = 0; //how many ticks were rounded off
                
                //Determine how many steps have to be moved while time has passed
                steps_to_pop = ( (SDL_GetTicks() - wKeyTimer)*(PLAYER_CREATURE_SPEED/1000.0) ); // (elapsed time * movement speed) / 1000 ms
                
                //NOW MOVE THOSE STEPS
                for(int j = 0 ; j < steps_to_pop; j++){ //process each step, one, by one
                    
                    //First, do bounds checking
                    if(cre1->y < 1){
                        break;
                    }
                    //blocked_check
                    if(block_map[(cre1->y-1)*map_width+(cre1->x)]==true){
                        break;
                    }
                    //If we made it here, then it's okay to move
                    cre1->moveUp();
                }//DONE moving steps
                //update timer
                //First, determine how much carry-over we need to keep (time we haven't accounted for due to rounding
                carry_over = (SDL_GetTicks() - wKeyTimer); //How much time we have on timer, total
                carry_over = carry_over - (steps_to_pop*1000.0/PLAYER_CREATURE_SPEED); //now subtract how much time we've accounted for
                //carry_over now has how many ticks we haven't updated for
                wKeyTimer = SDL_GetTicks() - carry_over; //Now update the timer and considering unaccounted for time
                
                //Finally, update draw_map indices
                draw_map_y = cre1->y - draw_map_height/2;
                
            }//end a key processing
            
            
        }//End event while loop
        
        //BEGIN DRAW ROUTINES
        
        drawVectorMap();
        draw_environment();
        //Draw all the sprites
        //#
        cre1->draw_movement(cre1->x - draw_map_x, cre1->y - draw_map_y, item_tiles_p, item_tiles_s);
        
        //Draw Display windows
        if(inventoryDisplayOn){
            //showInventory(cre1, &e);
            //displayItemList(map_items[3], gRenderer, SCREEN_HEIGHT);
            displayItemList(map_creatures[creature_inventory_index].inventory, gRenderer, SCREEN_HEIGHT);
        }
        if(statusDisplayOn){
            //show status (of creature)
            displayStatus(map_creatures[creature_inventory_index], gRenderer, SCREEN_HEIGHT, item_tiles_p, item_tiles_s, misc_tiles);
        }
        if(shroomDepoDisplayOn){
            //display inventory of shroom
            displayItemList(map_shrooms[shroom_inventory_index].inventory, gRenderer, SCREEN_HEIGHT);
        }
        if(consoleDisplayOn){
            displayConsole(gRenderer,consoleLog);
        }

        SDL_RenderPresent( gRenderer ); //Update screen
        
        
    }//END MAIN GAME LOOP
    
    //Free up sprites and everything else
    cre1->free();
    close();
    
    return 0;
}
