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
#include "music.hpp"
#include "creature.hpp"
#include "Item.hpp"
#include "tent.hpp"
#include "med_algo.hpp"
#include "story.hpp"
#include "image_gen.hpp"
//#include <unistd.h>


using std::vector;

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 480;

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

//Creatures Stuff
vector<Sprite> map_creatures; //a list of all creatures on map

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
int creature_inventory_index = 0; //which creature in map_creature to display inventory for

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
    tent_tiles_p = new SDL_Texture* [100];
    tent_tiles_s = new SDL_Texture* [100];
    workshop_tiles_p = new SDL_Texture* [100];
    workshop_tiles_s = new SDL_Texture* [100];
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
    system("python Civ2/Civ2/fruitz/fruitz.py");
    system("python Civ2/Civ2/fruitz/outline.py");

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
    
//    //Create some random items!
//    for(int i = 0; i < 450; i++){
//        tempx = rand()%(map_width);
//        tempy = rand()%(map_height);
//        temp_tile = rand()%199;
//        //Item temp_item = Item(tempx, tempy, temp_tile, world_colors[temp_tile],{255,255,255,0} ); //temporary item
//        Item temp_item = Item(tempx, tempy, temp_tile, {static_cast<Uint8>(colorz[temp_tile][0]), static_cast<Uint8>(colorz[temp_tile][1]), static_cast<Uint8>(colorz[temp_tile][2]),255},{255,255,255,0} ); //temporary item
//        map_items[(tempy)*map_width+(tempx)].push_back(temp_item);
//        //printf("cc %d %d cc", tempx, tempy);
//        //printf("cc %d %d cc", map_items[(tempy-1)*map_width+(tempx)].back().x, temp_item.x);
//        
//    }
    
    //Create some random weedz with fruit!
    for(int i = 0; i < 450; i++){
        tempx = rand()%(map_width);
        tempy = rand()%(map_height);
//        tempx = 3;
//        tempy = 0;
        temp_tile = rand()%99;
        
        //Create the weed
        Item temp_item = Item(tempx, tempy, temp_tile, {static_cast<Uint8>(colorz[temp_tile][0]), static_cast<Uint8>(colorz[temp_tile][1]), static_cast<Uint8>(colorz[temp_tile][2]),255},{255,255,255,0} ); //temporary item
        map_items[(tempy)*map_width+(tempx)].push_back(temp_item);
        
        //then the fruits
        temp_item = Item(tempx, tempy, temp_tile+200, {static_cast<Uint8>(colorz[temp_tile+200][0]), static_cast<Uint8>(colorz[temp_tile+200][1]), static_cast<Uint8>(colorz[temp_tile+200][2]),255},{255,255,255,0} ); //temporary item
        map_items[(tempy)*map_width+(tempx)].push_back(temp_item);
        
        
    }
    
    
//    for(int b = 0 ; b<50; b++){
//        tempx = rand()%(map_width);
//        tempy = rand()%(map_height);
//        if(rand()%2 == 1){
//            temp_tile = 305; //+ (rand()%2);
//        }else{
//            temp_tile=306;
//        }
//        //temp_tile = rand()%7 + 300;
//        //if(temp_tile == 301 || temp_tile == 304){temp_tile = 305;}
//        Item temp_item = Item(tempx, tempy, temp_tile, {static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), 255},{static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255),255} ); //temporary item
//        map_items[(tempy)*map_width+(tempx)].push_back(temp_item);
//    }
    
    block_map = new bool[map_width*map_height](); //initialize a dynamically sized blocked map
    //Make a brick maze
    vector<vector<short>> gmap = gen_maze_corridor(30,80);
    int con_x = 0;//the starting point of construction
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
                //then we need a wall here
                Item con_item = Item(tempx, tempy, 304, p1, s1);
                map_items[(tempy*map_width)+tempx].push_back(con_item);
                block_map[(tempy*map_width)+tempx] = true;
            }
            
        }
    }
    
    
    //for testing inventory, specific tile...
    for(int b = 0 ; b<1; b++){
        tempx = 14;
        tempy = 0;
        if(rand()%2 == 1){
            temp_tile = 305; //+ (rand()%2);
        }else{
            temp_tile=306;
        }
        Item temp_item = Item(tempx, tempy, temp_tile, {static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), 255},{static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255),255} ); //temporary item
        map_items[(tempy)*map_width+(tempx)].push_back(temp_item);
    }
    
//    //Make some tents
//    tempx = 23;
//    tempy = 8;
//    //Tent temp_tent = Tent(tempx, tempy, 0);
//    Tent temp_tent = Tent(tempx, tempy, 0, {static_cast<Uint8>(127+rand()%127), static_cast<Uint8>(127+rand()%127), static_cast<Uint8>(127+rand()%127), 255},{static_cast<Uint8>(127+rand()%127), static_cast<Uint8>(127+rand()%127), static_cast<Uint8>(127+rand()%127),255});
//    map_tents[(tempy)*map_width+(tempx)].push_back(temp_tent);
//    
//    //Make some workshops
//    tempx = 25;
//    tempy = 8;
//    Workshop temp_workshop = Workshop(tempx, tempy, 0);
//    map_workshops[(tempy)*map_width+(tempx)].push_back(temp_workshop);
//    
//    tempx = 27;
//    tempy = 8;
//    temp_workshop = Workshop(tempx, tempy, 1);
//    map_workshops[(tempy)*map_width+(tempx)].push_back(temp_workshop);
}

//Initialize Creatures
void init_creatures(){
    int num_creatures = 5; //How many creatures are on the ma
    
    for(int i = 0 ; i < num_creatures; i++){
        Sprite temp_cre = Sprite(rand()%map_width, rand()%map_height);
        temp_cre.loadFromFile("Civ2/Civ2/tiles/crePrim.png","Civ2/Civ2/tiles/creSeco.png", 16, 16);
        map_creatures.push_back(temp_cre);

        //randomly give hats
        if(rand()%2==1){
            Hat* temp_hat = new Hat(0, 0, 305+(rand()%2) ); //a temp Item to be added to cre's inventory
            map_creatures.back().hat = temp_hat;
        }
        
    }
}

//Draws all the items
void draw_items(){
    //You know what to do
    for(int i = 0; i < map_items.size(); i++){
        for(int j = 0 ; j < map_items[i].size(); j++){
            map_items[i][j].draw(gRenderer, item_tiles_p, item_tiles_s, item_tiles_t);
        }
    }
}

//Draws all the creatures
void draw_creatures(){
    //Cycle through all creatures and draw
    for(int i = 0; i < map_creatures.size(); i++){
        map_creatures[i].draw();
        map_creatures[i].drawHat(gRenderer, item_tiles_p, item_tiles_s);
    }
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
                    if( color_diff( map_items[((node[1])*map_width)+(node[0]-1)][i].primColor , cre->faveColor) < diff_threshold){
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
                    if( color_diff( map_items[((node[1])*map_width)+(node[0]+1)][i].primColor , cre->faveColor) < diff_threshold){
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
                    if( color_diff( map_items[((node[1]-1)*map_width)+(node[0])][i].primColor , cre->faveColor) < diff_threshold){
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
                    if( color_diff( map_items[((node[1]+1)*map_width)+(node[0])][i].primColor , cre->faveColor) < diff_threshold){
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
        
        //check if thread should expire
        if( difftime(time(NULL) , spr1->thread_timer) > 30.0 ){
            printf("done wandering\n");
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
        
        vector<int> next_step = spr1->path[spr1->path.size()-1]; //the last element of array/vector
        //Now actually move
        spr1->x = next_step[0];
        spr1->y = next_step[1];
        //pop off the step from path
        spr1->path.pop_back();
        
        SDL_Delay(500);
    }
}

//a thread for picking up items of your favorite color
//Call the faveColorSearch algorithm
//Travels to proper tile
//picks up item (if it's still there)
void gather_thread(Sprite* spr1){
    
    int color_thresh = 100; //the maximum difference between item color and fave color,  allowable
    
    //spr1->path.clear(); //clear path and start clean
    
    while(true){
        
        //find a new target path
        if(spr1->path.empty()){ //if path is empty
            faveColorSearch(spr1, color_thresh);
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
        

        
        vector<int> next_step = spr1->path[spr1->path.size()-1]; //the last element of array/vector
        //Now actually move
        spr1->x = next_step[0];
        spr1->y = next_step[1];
        //pop off the step from path
        spr1->path.pop_back();
        
        //If we've reach target...
        if(spr1->path.empty()){

            //now look on tile and check if item is still there and pick up
            for(int i = 0; i < map_items[(spr1->y*map_width)+spr1->x].size(); i++){
                if(color_diff(map_items[(spr1->y*map_width)+spr1->x][i].primColor, spr1->faveColor)<color_thresh ){
                    
                    pickUpItem(spr1, spr1->x, spr1->y, i); //then pick up the item
                }
                
            }
            
            
            //break;
        }
        
        SDL_Delay(500);
        
    }
    
}

//periodically gives a new task to creatures
void task_creatures_thread(){
    
    short choice = 0; //decides which thread to run
    
    while(true){
        
        //cycle through all creatures
        for(int i = 0; i < map_creatures.size(); i++){
            if(map_creatures[i].inThread == false){ //we need to assign it a new thread
                map_creatures[i].thread_timer = time(NULL);
                map_creatures[i].inThread = true;
                choice = rand()%2;
                switch(choice){
                    case 0: {
                        //This thread makes the creature gather
                        std::thread gatherObj(gather_thread, &map_creatures[i]);
                        gatherObj.detach();
                        break;
                    }
                    case 1: {
                        //This thread wanders the input creature
                        std::thread wanderObj(wander_thread, &map_creatures[i]);
                        wanderObj.detach();
                        break;
                    }
                }
                
            }
        }
        
        
        SDL_Delay(2000); //doesn't have to run all the time
        
    }
}

//void music_thread(int ToneHz){
//    playToneOnce(ToneHz, 2000);
//    SDL_Delay(2000);
//    
//}

//a thread for changing the background periodically
void background_color_thread(){
    
    short change = 1; //how much to change each component by
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
//    int r2 = 143;
//    int g2 = 133;
//    int b2 = 254;
    int r2 = (rand()%154) + 1;
    int g2 = (rand()%154) + 1;
    int b2 = (rand()%154) + 1;
    
    while(true){
        
        //change is an opportunity for growth
//        back_col.r=back_col.r+change;
//        back_col.g=back_col.g+change;
//        back_col.b=back_col.b+change;
//        
//        //bounds checking
//        if(back_col.r>254){
//            change = -1;
//        }
//        if(back_col.r==0){
//            change = +1;
//        }
//


        //oscilates between the two
        
        //change is an opportunity for growth
        //let's only change one color - nice effect :)
        back_col.r=back_col.r+r_inc;
        back_col.g=back_col.g+g_inc;
        back_col.b=back_col.b+b_inc;
        
        //bounds checking
        if( (back_col.r>r2) || (back_col.r<r1) ){
            r_inc = r_inc * -1;
        }
        if( (back_col.g>g2) || (back_col.g<g1) ){
            g_inc = g_inc * -1;
        }
        if( (back_col.b>b2) || (back_col.b<b1) ){
            b_inc = b_inc * -1;
        }
        
        
        //SDL_Delay(100); //Debug Quick
        SDL_Delay(500); //Nice looking
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
                item_id = rand()%100; //pick a random item id
                Item temp_item = Item(x, y, item_id, {static_cast<Uint8>(colorz[item_id][0]), static_cast<Uint8>(colorz[item_id][1]), static_cast<Uint8>(colorz[item_id][2]),255},{255,255,255,0} ); //temporary item
                map_items[y*map_width+x].push_back(temp_item);
                SDL_Delay(5000);
            }
        }
        
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
    Sprite* shroom1 = new Sprite(3,3);
    shroom1->loadFromFile("Civ2/Civ2/tiles/shroomPrim.png","Civ2/Civ2/tiles/shroomSeco.png", 16, 16);
    Sprite* lov1 = new Sprite(5,5);
    lov1->loadFromFile("Civ2/Civ2/tiles/lovPrim.png", "Civ2/Civ2/tiles/lovSeco.png", 16, 16);

    //TEST SHIT
    //genAdobe();
    //vector<vector<short>> maze = gen_maze_corridor();
    //printMaze(maze);
    init_items();
    init_creatures();
    Hat temp_accessory = Hat(0, 0, 305); //a temp Item to be added to cre's inventory
    Hat temp_accessory2 = Hat(0, 0, 306); //a temp Item to be added to cre's inventory
    cre1->hat = &temp_accessory; //give him a hat
    cre3->hat = &temp_accessory2; //give him a hat
    
    //DEBUG AUDIOOOOOOOOOOOOOOOO
//    // NOTE: Sound test
    int SamplesPerSecond = 48000;
//    int ToneHz = 240;
//    int ToneVolume = 3000;
//    uint RunningSampleIndex = 0;
//    int SquareWavePeriod = SamplesPerSecond / ToneHz;
//    int HalfSquareWavePeriod = SquareWavePeriod / 2;
    int BytesPerSample = sizeof(int) * 2;
//    // Open our audio device:
    initAudio(48000, SamplesPerSecond * BytesPerSample / 60);
    //playToneCont(440);
    //playToneOnce(110, 2000);
    //playSquare(116.6, 2000);
    //playToneOnce(116, 2000);
//    playToneOnce(117, 2000);
    
    
//    playToneOnce(220, 2000);
    //while(true){
//        playTwoChord(200, 300, 2000);
    
//    while(true){
//        playTwoChord(200, 300, 2000);
//        playToneOnce(200, 2000);
//    }
    SDL_PauseAudio(0);
    
//    time_t timev = time(nullptr);
//    cout << asctime(localtime( &timev )) << timev;
//    vector<vector<int>> test_path = faveColorSearch(cre5);
//    for(int i = 0; i < test_path.size(); i++){
//        printf("\n(%d,%d,%d)", test_path[i][0],test_path[i][1]);
//    }
//    
//    timev = time(nullptr);
//    cout << asctime(localtime( &timev )) << timev;
    
    //playToneList_Phase({440,880,3*440, 4*440, 5*440, 550, 2*550, 3*550, 4*550, 5*550}, 2000);
//    while(true){
//        printf("\n");
//        printScale(genIonian(0));
//        printf("\n");
//    }
    
//    for(int i = 0; i < 10 ; i++){
//        playScaleOnce(genMinorChord(0));
//        playSquare(116.6, 2000);
//    }
    
//    while(true){
//        //cout << giveMonickerPost( genName() ) << '\n';
//        //cout << genPrayer() << '\n';
//        //cout << genLatinSentence() << '\n';
//        //cout << genCuss() << '\n';
//        Law* law1 = new Law(genName());
//        for(int y = 0; y < 77; y++){
//           law1->addRandArticle( giveMonicker( genName() ) );
//        }
//        law1->printLaw();
//        
//    }


    //SDL_Delay(2000);
    //SDL_PauseAudio(0);
    
    //test the name gen
//    while(true){
//        //printf("%s\n",genName());
//        cout << genPrayer() << '\n';
//    }
    
    //genPNG();
    //SDL_Texture* text1 = genTexture(gRenderer, gWindow);
    

    
    //
    //DEBUG TEST : SPAWN SOME THREAD DAWGd
    //This thread updates the background color
    std::thread backObj(background_color_thread);
    backObj.detach();
    //This thread updates the audio
//    std::thread audioObj(compose_music_thread);
//    audioObj.detach();
    //This thread regenerates weeds
    std::thread regenObj(regen_weedz_thread);
    regenObj.detach();
    //This thread schedules new threads for halted creatures!
    std::thread taskObj(task_creatures_thread);
    taskObj.detach();
    
    //std::thread audioObj1(music_thread, std::ref(440));
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
                        //cycle through different creature's inventories
                        case SDLK_PERIOD:
                            creature_inventory_index = creature_inventory_index+1;
                            if(creature_inventory_index > map_creatures.size()){
                                creature_inventory_index = creature_inventory_index-1;
                            }
                            break;
                        case SDLK_COMMA:
                            creature_inventory_index = creature_inventory_index-1;
                            if(creature_inventory_index < 0){
                                creature_inventory_index = 0;
                            }
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
        cre2->draw();
        cre3->draw();
        shroom1->draw();
        lov1->draw();
        
        draw_items();
        draw_creatures();
        
        //drawTexture(gRenderer, text1, 5, 5);
        
        cre1->drawHat(gRenderer, item_tiles_p, item_tiles_s);
        cre3->drawHat(gRenderer, item_tiles_p, item_tiles_s);
        
        
        
        //Draw Display windows
        if(inventoryDisplayOn){
            //showInventory(cre1, &e);
            //displayItemList(map_items[3], gRenderer, SCREEN_HEIGHT);
            displayItemList(map_creatures[creature_inventory_index].inventory, gRenderer, SCREEN_HEIGHT);
        }
        
        SDL_RenderPresent( gRenderer ); //Update screen        
        
        
    }//END MAIN GAME LOOP
    
    //Free up sprites and everything else
    cre1->free();
    cre2->free();
    cre3->free();
    shroom1->free();
    lov1->free();
    close();
    
    return 0;
}
