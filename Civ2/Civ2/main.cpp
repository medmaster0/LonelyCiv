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

#include "dancez.hpp"
#include "creature.hpp"
#include "Item.hpp"
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
vector<vector<int> > map;
SDL_Texture** tiles; //contains map tiles
int map_width;
int map_height;
vector<vector<int>> colorz; //main color list containing the colors corresponding to the tilez

//ItemTile Stuff
vector<Item> items; //holds all the items on map
SDL_Texture** item_tiles_p; //primo item tiles
SDL_Texture** item_tiles_s; //seco item tiles
//SDL_Color* world_colors; //array of sdl_colors

//SDL Stuff
SDL_Window* gWindow = NULL;//The window we'll be rendering to
SDL_Renderer* gRenderer = NULL;//The window renderer

//TTF STUFF
TTF_Font * font1; //menu font
SDL_Color font1_clr = {255,255,255}; //color of font
SDL_Rect inv_font_rect = { SCREEN_HEIGHT - 50, 0, SCREEN_HEIGHT, 200 };
int texW = 0;//constants used in displaying fonts
int texH = 0;//constants used in displaying fonts

//Interface Windows flags
//These are for control of what "windows" are displayed
bool inventoryDisplayOn = false;

/////////////////////////////////////////////////////////
////FUNCTIONS BEGIN////////////////////////////////////////
////////////////////////////////////////////////////////
///Important Declarations First
vector<vector<int>> findPathToCoord(vector<vector<int>>, int x1, int y1, int x2, int y2);

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

//Utility to check if an element is in list
bool isIntIn(int e, vector<int> v){
    if(std::find(v.begin(), v.end(), e) != v.end()) {
        return true;
    } else {
        return false;
    }
}

//Utility to check if a node - list(of ints) - is in a list of lists
//bool isNodeIn(vector<int> e, vector<vector<int>> v){
//    if(std::find(v.begin(), v.end(), e) != v.end()) {
//        return true;
//    } else {
//        return false;
//    }
//}
//bool isNodeIn(vector<int> e, vector<vector<int>> v){
//    bool check = false; //initialize to false
//    vector<int> temp; //Used to hold the nodes (list of ints)
//
//    for(std::vector<vector<int>>::iterator it = v.begin(); it!=v.end(); ++it){
////        for(std::vector<int>::iterator jt = it->begin(); jt!=it->end(); ++jt ){
////            std::cout << *jt;
////        }
//        printf("%d",it[0][0]);
//        temp = *it;
//        if(temp == e){
//            printf("equal");
//        }else{
//            printf("not equal");
//        }
//    }
//    
//    return check;
//}

bool isNodeIn(vector<int> e, vector<vector<int>> v){
    
    vector<int> temp; //used to hold the nodes...
    for(int i = 0; i<v.size(); i++){ //cycle through all elements of v
        if(e[0] == v[i][0] && e[1] == v[i][1]){
            return true;//means we found a match!
        }
    }
    return false; //means we cycled list and couldn't find match
}


//A WORD ON MAPS..
//Here is as good a place as any to describe the map data structure
//map is a 2D array of ints (implemented with vector class)
//Each int is an id to the corresponding tile *type* (open space, wall, floor, etc...
//
//Another (2D) array (currently unimplemented) should hold the actual material *COLOR* of the tile *per floor*
//Probably should contain int index to another discrete world color palette
//Each map will have it's own set of world/geoprahical color palette
//All tiles in the world will be a color from this limited set/palette of colors
//Until this is implemented, the world is colorless...
//
//#MAP LEGEND:
// 0-99 - WEEDZ -  RANDOMLY GENERATED WEEDZ
// 100-199 - STONEZ - RANDOMLY GENERTATED STONEZ
// 200 - FLOOR - EMPTY/COMMA
// 201 - WALL - STRIPES
// 202 - WALL - ADOBE
//
// 299 - EMPTY
// 300 - ITEM (ANY ITEM, WHATSOEVER)
//
//
//Generates a map, based on window dimensions
//We generate tiles in tmap (what's on floor)
//And we also generate Items (specific objects)
vector<vector<int> > genMap(){
    
    vector<vector<int> > tmap; //the map we will return
    
    map_width = SCREEN_WIDTH/16;
    map_height = SCREEN_HEIGHT/16;
    
    //initialize map array
    tmap.resize(map_height);
    for (int i = 0; i<map_height; i++) {
        tmap[i].resize(map_width);
    }
    
//    tmap[1][1] = rand() % 10;
//    tmap[1][3] = rand() % 10;
//    tmap[1][4] = rand() % 10;
//    tmap[1][2] = rand() % 10;
    
    //ADD grid of squares
    //...Identify top left corner of square
    //...Branch out from there
//    for (int i = 0; i<map_height;i++){
//        for (int j = 0;j < map_width;j++){
//            if(i%8 == 0 && j%8 == 0){ //Identify the grid, so to speak
//                //tmap[i][j] = 1;
//                //Now fill out the square
//                for (int z = 0; z<6; z++){
//                    for (int y = 0; y<6; y++){
//                        tmap[i+z][j+y]=rand() %102; //random number between 0 and NUMBER OF TILES
//                        if(z>0&&z<5&&y>0&&y<5)tmap[i+z][j+y]=0;
//                    }
//                }
//            }
//        }
//    }
    
    //Fill out entire map with tilez
    for (int i = 0; i<map_height;i++){
        for (int j = 0;j < map_width;j++){
            if(rand()%2 == 1){
                tmap[i][j] = rand()%203;
                
                //Also create an item at that point
//                Item temp = Item::Item(j,i, tmap[i][j], {static_cast<Uint8>(colorz[120][0]),static_cast<Uint8>(colorz[120][1]),static_cast<Uint8>(colorz[120][2])}, {0xff,0xff,0xff,0xff} );
            
                
            }else{
                tmap[i][j] = 299;
            }
            
            //ARTIFICIAL FEATURE
            if(i>20){
                tmap[i][j] = 299;
            }
            
        }
    }
    
    
    return tmap;
}

//Load in al the tiles used for the map
void loadTiles(){
    tiles = new SDL_Texture *[303];
    
    item_tiles_p = new SDL_Texture* [303];
    item_tiles_s = new SDL_Texture* [303];
    //world_colors = new SDL_Color [200];
    SDL_Color temp_col;
    
    for(int i = 0 ; i < 100; i++){
        tiles[i] = loadTexture("Civ2/Civ2/weedz/"+std::to_string(i)+".png");
        int r,g,b;
        r = rand()%255;
        g = rand()%255;
        b = rand()%255;
        SDL_SetTextureColorMod( tiles[i], r, g, b); //modulate color, update to match the new one
        colorz.push_back({r,g,b});
        
        //AS WE INTEGRATE THESE "MATERIALS" INTO ITEMS, WE ALSO UPDATE THOSE LISTS
        item_tiles_p[i] = loadTexture("Civ2/Civ2/weedz/"+std::to_string(i)+".png");
        item_tiles_s[i] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
//        temp_col = {static_cast<Uint8>(r),static_cast<Uint8>(g),static_cast<Uint8>(b)};
//        world_colors[i] = temp_col;
    }
    for(int i = 100 ; i < 200; i++){
        tiles[i] = loadTexture("Civ2/Civ2/stonez/"+std::to_string(i)+".png");
        int r,g,b;
        r = rand()%255;
        g = rand()%255;
        b = rand()%255;
        SDL_SetTextureColorMod( tiles[i], r, g, b); //modulate color, update to match the new one
        colorz.push_back({r,g,b});
        
        //AS WE INTEGRATE THESE "MATERIALS" INTO ITEMS, WE ALSO UPDATE THOSE LISTS
        item_tiles_p[i] = loadTexture("Civ2/Civ2/stonez/"+std::to_string(i)+".png");
        item_tiles_s[i] = (SDL_Texture *)0x9999; //this is an escape code to indicate no color
//        temp_col = {static_cast<Uint8>(r),static_cast<Uint8>(g),static_cast<Uint8>(b)};
//        world_colors[i] = temp_col;
        
    }
    tiles[200] = loadTexture("Civ2/Civ2/tiles/map8.png");
    tiles[201] = loadTexture("Civ2/Civ2/tiles/map3.png");
    tiles[202] = loadTexture("Civ2/Civ2/tiles/map5.png");
    //Integration means we're also going to change tiles array....
    tiles[300] = loadTexture("Civ2/Civ2/tiles/map8.png");
    
    //initialize item tiles.
    //TODO: WEE NEED TO INTEGRATE THE WEEDZ AND STONES INTO THIS CLASS AS WELL!!!!!
    //initialize Item tile's (class static variables)
    //Item::Item initializer constructor takes in two arrays of tiles (SDL_Texture's)
    //initialize temporary array for textures
    //SDL_Texture** primTiles = new SDL_Texture* [1];
    //SDL_Texture** secoTiles = new SDL_Texture* [1];
    //Actual loading

    item_tiles_p[300] = loadTexture("Civ2/Civ2/tiles/canPrim.png");
    item_tiles_s[300] = loadTexture("Civ2/Civ2/tiles/canSeco.png");
    item_tiles_p[301] = loadTexture("Civ2/Civ2/tiles/adobePrim.png");
    item_tiles_s[301] = loadTexture("Civ2/Civ2/tiles/adobeSeco.png");
    
    //Item::tilesPrim = new SDL_Texture * [1];
    //Item::tilesPrim[0] = loadTexture("Civ2/Civ2/tiles/canPrim.png");
    //Item::tilesSeco[0] = loadTexture("Civ2/Civ2/tiles/canSeco.png");
}

//Generate tilez
//Call external python script,
//Then load the generated .png files
void generateTilez(){
    system("python Civ2/Civ2/weedz/WEEDZ.py");
    system("python Civ2/Civ2/stonez/STONEZ.py");
#define NUM_WEEDZ 10

}

//Print out a tile-based map give a vector 2D array
void drawVectorMap(vector<vector<int> > map){
    //Clear screen
    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderClear( gRenderer );
    SDL_Rect* clip = NULL;
    
    for(int i = 0; i<map_height; i++){
        for(int j = 0; j<map_width; j++){
            if (map[i][j] == 299){
                //DO NOTHING
                j = j;
            }else{
                SDL_Rect renderQuad = { j*16, i*16,16, 16 }; //a dummy rectangle for drawing
                SDL_RenderCopy( gRenderer, tiles[map[i][j]], clip, &renderQuad );//Render to screen
            }
        }
    }

}

//
//HOW ADOBE WORKS:
//On map, adobe has a certain value, indicating there is an adobe wall there
//However, map DOES NOT contain the COLOR data.
//All adobe information (color, and position) is contained in Adobe struct
//All the walls on the map are contained in a single global Adobe array, adobes.
//
//
//FORGET ABOVE
//TRYING TO DO THIS WITH *ITEM* CLASS
vector<Item> adobe_items;


//DRAW ADOBE WALLS
//Go through the list of Adobes and draw all
void drawAdobe(){
    
    //Using a class
    for(int b = 0 ; b<adobe_items.size(); b++){
        adobe_items[b].draw(gRenderer, item_tiles_p, item_tiles_s);
    }
    
    return;
}
//FOR DEBUGGING MOSTLY
//DRAW SOME ADOBE IN HERE
void genAdobe(){
    
    //We're also going to want to gen some more elaborate constructs
    //TODO: Simple COrridor Maze???
    //Make new Adobe using Item class
    for(int p = 0 ; p < 299; p++){
        printf("%d\n",adobe_items.size());
        SDL_Color p1 = {0,0,0};
        SDL_Color s1 = {255,255,255};
        Item temp_item = Item(30+(p%6),12+p%4,301, p1, s1);
        //Item temp_item = Item(30+p%6,12,301);
        adobe_items.push_back(temp_item);
        //Item temp_adobe = *new Item(26,26,120,{static_cast<Uint8>(colorz[120][0]),static_cast<Uint8>(colorz[120][1]),static_cast<Uint8>(colorz[120][2])},{0xff,0xff,0xff,0x00});
        //adobes[p] = Item::Item(50, 23, 301,p1 , s1  );
        //adobe_items.push_back(*temp_adobe);
    }
    
    
}

//////////////////////
//INTERACTION FUNCTIONS
//FUNCTIONS THAT OPERATE BETWEEN TWO GLOBALS

//a function that picks up an item from the map, and put's it in the specified Sprite's inventory
void pickUpItem(Sprite* sprite, int xpos, int ypos){
    Material item_temp; // a temporary item for adding to inventory
    int index; //a temporary index
    vector<int> clr; //a temporary holder for colors used multiple times below
    
    index = map[ypos][xpos]; //get the tile number at the position
    //check to make sure item is pick-upable
    if(index > 199){
        return;
    }
    
    clr = colorz[index]; //get the color of the tile the cre is at
    item_temp = {clr, index};
    sprite->inventory.push_back(item_temp);
    
    //Also delete item from map
    map[ypos][xpos] = 200; //blank tile
}

//////////////////////
//OTHER FUNCTIONS

//Make an attempt to free up  all the memory
void close(){
    TTF_Quit();
    SDL_DestroyWindow(gWindow);
    SDL_DestroyRenderer(gRenderer);
    
}

//The searching Algorithm - One coord to another, considering blocked paths
//Flood Search
//Input - map, coord1, coord2
//Output - a list of coords to fllow to get to target - BACKWARD - so POP element from list to find first step
vector<vector<int>> findPathToCoord(vector<vector<int>>, int x1, int y1, int x2, int y2){
    vector<vector<int>> search_q; //the main "list"
    vector<int> blocked = {201}; //list containing tile "types" that are blocked
    int k = 0; //the main counter used to keep track of where we are in the list
    vector<int> node; //This is a single node in the list representing <xpos, ypos, steps_from_dest>. Always changing
    vector<int> temp; //Temp node used to push new nodes onto queues AND LATER in filtering out irrelevant steps
    
    node = {x1, y1, 0}; //initialization
    search_q.push_back(node); //add first element to list
    bool findingTarget = true; //a flag signalling if we're still searching for target (and populating search_q)
    while(findingTarget){
        try{
            node = search_q[k]; //go to the next node in the queue
        }catch(...){
            return {{}}; //return an empty list signalling we tried all reachable points but couldn't find target
        }
        
//            printf("one%d",node[0]-1);
//            printf("two%d",x2);
//            printf("three%d",node[1]);
//            printf("four%d",y2);
        
        //Find all adjacent nodes.
        //Check if adjacent node is in fact target coord.
        //If not, Add it to search_q if is is eligible space (not blocked).
        //LEFT
        try{ //we use try-catch so we don't have to worry about bounds errors...
            if(node[0]-1 == x2 && node[1] == y2){ //means we found target!
                temp = {node[0]-1, node[1], node[2]+1};
                search_q.push_back(temp);
                findingTarget = false;
                break;
            }
            
            if(!(node[0]-1<=0)){
                if(!isIntIn(map[node[1]][node[0]-1], blocked)){//if index is *not* in blocked list
                    //we also gotta make sure node isn't in queue with lower distance already...
                    //cycle through the list and check for every lower distance...
                    if(!isNodeIn({node[0]-1,node[1],0}, search_q)){
                        temp = {node[0]-1, node[1], node[2]+1};
                        search_q.push_back(temp);
                    }
                }
            }
            
        }catch(...){
            k = k; //do nothing pretty much
        }
        
        //RIGHT
        try{ //we use try-catch so we don't have to worry about bounds errors...
            if(node[0]+1 == x2 && node[1] == y2){ //means we found target!
                temp = {node[0]+1, node[1], node[2]+1};
                search_q.push_back(temp);
                findingTarget = false;
                break;
            }
            if(!isIntIn(map[node[1]][node[0]+1], blocked)){//if index is *not* in blocked list
                //we also gotta make sure node isn't in queue with lower distance already...
                //cycle through the list and check for every lower distance...
                if(!isNodeIn({node[0]+1,node[1]}, search_q)){
                    temp = {node[0]+1, node[1], node[2]+1};
                    search_q.push_back(temp);
                }

            }
        }catch(...){
            k = k; //do nothing pretty much
        }
        
        //UP
        try{ //we use try-catch so we don't have to worry about bounds errors...
            if(node[0] == x2 && node[1]-1 == y2){ //means we found target!
                temp = {node[0], node[1]-1, node[2]+1};
                search_q.push_back(temp);
                findingTarget = false;
                break;
            }
            if(node[1]-1 >= 0){
                if(!isIntIn(map[node[1]-1][node[0]], blocked)){//if index is *not* in blocked list
                    //we also gotta make sure node isn't in queue with lower distance already...
                    //cycle through the list and check for every lower distance...
                    if(!isNodeIn({node[0],node[1]-1}, search_q)){
                        temp = {node[0], node[1]-1, node[2]+1};
                        search_q.push_back(temp);
                    }
                    
                }
            }
            
        }catch(...){
            k = k; //do nothing pretty much
        }
        
        //DOWN
        try{ //we use try-catch so we don't have to worry about bounds errors...
            if(node[0] == x2 && node[1]+1 == y2){ //means we found target!
                temp = {node[0], node[1]+1, node[2]+1};
                search_q.push_back(temp);
                findingTarget = false;
                break;
            }
            if(!(node[1]+1 > map_height-1)){ //bounds checking
                if(!isIntIn(map[node[1]+1][node[0]], blocked)){//if index is *not* in blocked list
                    //we also gotta make sure node isn't in queue with lower distance already...
                    //cycle through the list and check for every lower distance...
                    if(!isNodeIn({node[0],node[1]+1}, search_q)){
                        temp = {node[0], node[1]+1, node[2]+1};
                        search_q.push_back(temp);
                    }
                }
            }
        }catch(...){
            k = k; //do nothing pretty much
        }

        
        k++; //increase the index
    
    }//broke out of while loop means we found target
    
    
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
    return path2target;
}

//DISPLAY WINDOWS?RECTANGLES WITH INFO
//WE REALLY SHOULD CREATE A SOLID CLASS FOR ALL OF THESE...
//A function to display a little box showing what's in inventory
int counterInventory = 0; //counter used with cursors for inventory pos
SDL_Surface * surface; //used in drawing
SDL_Texture * texture; //
void showInventory(Sprite* sprite, SDL_Event * e){
    
    std::string words = "      inventory\n"; //char buffer that is displayed
    
    //Black out Box {x, y, w, h}
    SDL_Rect r={0,SCREEN_HEIGHT,150,-150};

    SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 255 ); //color of box
    SDL_RenderFillRect( gRenderer, &r ); //copy box to renderer
    
    surface = TTF_RenderText_Blended_Wrapped(font1, words.c_str(), font1_clr, 125);
    texture = SDL_CreateTextureFromSurface(gRenderer, surface);
    
    //Create text rect (text box)
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH); //This get's the dimensions of the font/text
    SDL_Rect dstrect = { 0, SCREEN_HEIGHT+r.h, texW, texH }; //so we can make the proper rect to display it
    SDL_RenderCopy(gRenderer, texture, NULL, &dstrect); //write
    
    //Free Memory!!!
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    
    ////////////FINISH PUTTING THE TITLE: inventory
    
    //Now we need to print out the items (in COLOR text!!!!)
    std::string item_name;
    int item_index; //keeps track of what item in inventory we are actually printing - depends on two factors, down below
    SDL_Color item_color; //The color of the item
    
//    if(sprite->inventory.size()!=0){counterInventory = counterInventory % sprite->inventory.size();} //this allows it to scroll back on itself//
    if(counterInventory<0){counterInventory = sprite->inventory.size()-1;}
    if(counterInventory>sprite->inventory.size()-1){   counterInventory = 0;}
    
    //We'll also reuse some of the previously used objects
    for(int u = 0; u<7; u++){ //cycle through however many elements you wanna fit on screen
        item_index = u+counterInventory; //Allow the user to control the scroll by referencing global counterInventory
        if(u+counterInventory >= sprite->inventory.size()) {//check if sprite's inventory index is out of bounds
            break;
        }
        
        //Need to:
        //Determine item type
        //  Make string for item_type
        //Determine color of item
        //  Make color of item
        //make surface with string, color, and pre-created font
        //make texture from surface
        //Query texture for dimensions
        //create rectangle at correct position and with determined dimensions
        //copy onto renderer
        
        //Determine item type
        if(sprite->inventory[item_index].tile<100){ //Then it's a weed
            item_name = "weed";
        }else if(sprite->inventory[item_index].tile<200){ //Then it's a stone
            item_name = "stone";
        }
        if(item_index == 0){item_name.append(" FIRST");} //Specify if first in list
        else if(item_index+1 == sprite->inventory.size()){ item_name.append(" LAST"); }
        if(u == 0){item_name.append(" <-");} //print out a cursor
        //Determine Color
        item_color = {static_cast<Uint8>(sprite->inventory[item_index].color[0]), static_cast<Uint8>(sprite->inventory[item_index].color[1]), static_cast<Uint8>(sprite->inventory[item_index].color[2]) }; //lots of conversion from int vector to SDL COLOR...
        //Make surface and texture
        surface = TTF_RenderText_Blended_Wrapped(font1, item_name.c_str(), item_color, 125);
        texture = SDL_CreateTextureFromSurface(gRenderer, surface);
        //Make the destination rectangle
        SDL_QueryTexture(texture, NULL, NULL, &texW, &texH); //This get's the dimensions of the font/text
        dstrect = { 0, SCREEN_HEIGHT+r.h + 15+(u*15), texW, texH }; //so we can make the proper rect to display it
        //Render to screen
        SDL_RenderCopy(gRenderer, texture, NULL, &dstrect); //write

        //Free Memory!!!
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
        
    }
    
    


    
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
    map = genMap();
    generateTilez(); //call this before loadTiles()
    loadTiles();
    //...Sprites
    Sprite* cre1 = new Sprite(10,10);
    cre1->loadFromFile("Civ2/Civ2/tiles/crePrim.png","Civ2/Civ2/tiles/creSeco.png", 16, 16);
    Sprite* cre2 = new Sprite(10,11);
    cre2->loadFromFile("Civ2/Civ2/tiles/crePrim.png","Civ2/Civ2/tiles/creSeco.png", 16, 16);
    Sprite* shroom1 = new Sprite(3,3);
    shroom1->loadFromFile("Civ2/Civ2/tiles/shroomPrim.png","Civ2/Civ2/tiles/shroomSeco.png", 16, 16);
    Sprite* lov1 = new Sprite(5,5);
    lov1->loadFromFile("Civ2/Civ2/tiles/lovPrim.png", "Civ2/Civ2/tiles/lovSeco.png", 16, 16);
    //...Items
    Item* can1 = new Item(25,25,300);
    Item* item1 = new Item(24,24,301);
    Item* item2 = new Item(26,26,120,{static_cast<Uint8>(colorz[120][0]),static_cast<Uint8>(colorz[120][1]),static_cast<Uint8>(colorz[120][2])},{0xff,0xff,0xff,0x00});

    //TEST SHIT
    genAdobe();
    gen_maze();
    
    
    vector<vector<int>> test = findPathToCoord(map, 8, 3, 1, 0);

    //printf("DEBUG%d",test.size());
    
    //Variables
    bool shiftDown = false; //a flag holding state of shift key
    vector<int> clr; //a temporary holder for colors used multiple times below
    
    //MAIN GMAE LOOP
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
                        //r += 32;
                        break;
                        
                    case SDLK_w:
                        if(cre1->y<1){ //bounds check
                            break;
                        }
                        if(shiftDown){ //pick up above tile
                            pickUpItem(cre1, cre1->x, (cre1->y)-1); //pick up item above
                            break;
                        }
                        cre1->moveUp();
                        break;
                        
                    case SDLK_e:
                        //b += 32;
                        break;
                        
                    case SDLK_a:
                        if(cre1->x<1){ //bounds check
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
                        if(shiftDown){ //get info of above tile
                            pickUpItem(cre1, (cre1->x), (cre1->y)+1); //pick up item down
                            break;
                        }
                        cre1->moveDown();
                        break;
                        
                    case SDLK_d:
                        if(cre1->x > map_width-1){ //bounds check
                            break;
                        }
                        if(shiftDown){ //get info of above tile
                            pickUpItem(cre1, (cre1->x)+1, (cre1->y)); //pick up item right
                            break;
                        }
                        cre1->moveRight();
                        break;
                        
                    case SDLK_SPACE:{
                        clr = colorz[map[cre1->y][cre1->x]]; //get the color of the tile the cre is at
                        printf("color%d %d %d", clr[0],clr[1],clr[2] );
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
                            counterInventory++;
                            break;
                        case SDLK_MINUS:
                            counterInventory--;
                            break;
                    }
                }
            }
            
            
        }//End event while loop
        
        //Update Positions...
        //lov1->randomMove();
        //lov1->randomDance();
        //cre1->randomDance();
        //cre2->randomDance();
        
        drawVectorMap(map);
        drawAdobe();
        //Draw all the sprites
        cre1->draw();
        cre2->draw();
        shroom1->draw();
        lov1->draw();
        //Draw all the items
        can1->draw(gRenderer, item_tiles_p, item_tiles_s);
        item1->draw(gRenderer, item_tiles_p, item_tiles_s);
        item2->draw(gRenderer, item_tiles_p, item_tiles_s);
        
        //Draw Display windows
        if(inventoryDisplayOn){
            showInventory(cre1, &e);
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
