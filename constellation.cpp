//
//  constellation.cpp
//  Civ2
//
//  Created by Medz on 9/1/18.
//  Copyright © 2018 Medz. All rights reserved.
//
//  A class to generate and draw constellations

#include <SDL2/SDL.h>
#include <vector>
#include "constellation.hpp"

using namespace std;

//check if a point, 1, is on line segment 2-3
//This assumes points 1,2,and 3 are colinear
bool onSegment(int x1, int y1, int x2, int y2, int x3, int y3){
    
    //check if the point 1 is within the bounds of the other two
    if(x1 <= max(x2,x3) && x1 >= min(x2,x3) &&
       y1 <= max(y2,y3) && y1 >= min(y2,y3) ){
        return true;
    }
    
    return false;
}

//Finds the orientation of the points in order
//Returns the following:
// 0 - three points are colinear
// 1 - clockwise
// 2 - counterclockwise
int orientation(int x1, int y1, int x2, int y2, int x3, int y3 ){
    
    //a formula for finding orientation of points
    int val = (y2-y1)*(x3-x2) - (x2-x1)*(y3-y2);
    
    if(val == 0){return 0;} //colinear
    return (val > 0) ? 1 : 2; //(counter?)clockwise\
    
}

//a function that determines if two lines intersect
//(points 1 & 2 are FIRST line, points 3 & 4 are SECOND line)
//
//You first check if the orientations of the first line segment and each point on the opposite segment are different
//Then you check if the orientations of the second line segment and each point on the opposite segment are different
//(if they have different orientation, it means the first part "straddles" the two other points)
//
//Also a special case if the two lines overlap perfectly on each other... colinear
//
bool doIntersect(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4){
    
    //Determine the four orientations needed to consider
    //FIRST line segment 1-2 then to each of the other two points
    int o1 = orientation(x1, y1, x2, y2, x3, y3);
    int o2 = orientation(x1, y1, x2, y2, x4, y4);
    //SECOND line segment 3-4 then to each of the other two points
    int o3 = orientation(x3, y3, x4, y4, x1, y1);
    int o4 = orientation(x3, y3, x4, y4, x2, y2);
    
    //general case
    if (o1 != o2 && o3 != o4){
        return true;
    }
    
//    //special case
//    //if the points are colinear and an end point from one segment lies on the other segment
//    if( o1 == 0 && onSegment(x1, y1, x3, y3, x4, y4) ){return true;}
//    if( o2 == 0 && onSegment(x1, y1, x4, y4, x2, y2) ){return true;}
//    if( o3 == 0 && onSegment(x3, y3, x1, y1, x4, y4) ){return true;}
//    if( o4 == 0 && onSegment(x3, y3, x2, y2, x4, y4) ){return true;}
    
    return false; //didn't fall in any of the other categories
    
}

//This constructor will generate and return a new constellation
//Constellaton generation works as follow:
//  * Pick random point (star)
//  * Attempt to connect to previous point - check if line intersection
//  * Possibly add  orbit - if doesn't intersect
//  * Repeat until satisfied
//  * Randomly sprinkle stars
Constellation:: Constellation(int SCREEN_WIDTH, int SCREEN_HEIGHT){
    
    printf("Generating NEW CONSTELLATION>>>>>\n");
    
    int x1,y1,x2,y2; //temp vars for construction
    vector<int> star_tiles = {39,40,41}; //list of valid tiles for stars
    vector<int> planet_tiles = {28,29,30,31,32,33,34,35,36,37,38}; //list of valid tiles for planets
    
    //Put A SINGLE point constellation
    x1 = rand()%SCREEN_WIDTH;
    y1 = rand()%SCREEN_HEIGHT;
    vertices_coords.push_back( {x1,y1} ); //add the coords to list
    vertices_types.push_back( star_tiles[rand()%star_tiles.size()] ); //pick a random star type for it
    //set color
    if(vertices_types.back() == 41){
        vertices_colors.push_back({255,255,255});
    }else{
        vertices_colors.push_back({255,255,255});
    }
    
    //DEBUG
//    //if(doIntersect(x1, y1, 0, 0, SCREEN_WIDTH/4, SCREEN_HEIGHT/4, 3*SCREEN_WIDTH/4, 3*SCREEN_HEIGHT/4)){
//    if( doIntersect(50, 50, 60, 50, 55, 90, 55, 60) == true ){
//        printf("liine intersect\n");
//    }else{
//        printf("liine no intersecty\n");
//    }
//    printf("%d,%d,\n",SCREEN_WIDTH,SCREEN_HEIGHT);
//    printf("%d,%d\n",x1,y1);
    
    
    
    //Now, cycle through creating more points (stars), but also connecting/checking them...
    for(int j = 0 ; j < 6; j++){
        //Keep trying until you can find a valid point
        while(true){
            x2 = rand()%SCREEN_WIDTH;
            y2 = rand()%SCREEN_HEIGHT;
            //Cycle through all edges and make sure this new points won't cause an intersection
            bool foundIntersection = false;
            if(edges_coords.size()>=2){ //make sure there are actully edges in list
                for( int i = 0; i < edges_coords.size()-1; i++ ){ //we can skip the very last one also
                    if(doIntersect(x1, y1, x2, y2, edges_coords[i][0], edges_coords[i][1], edges_coords[i][2], edges_coords[i][3]) == true){
                        //continue; //continue and try a different point
                        foundIntersection = true;
                    }
                    //break; //if we made it here, means, no intersections
                }
            }
            
            if(foundIntersection == true){continue;} //redo the loop if we found an intersection...
            printf("we break;");
            break;//if we made it here, means, no intersections
        }

        //Means point is valid and we can add it
        vertices_coords.push_back( {x2,y2} ); //add the coords to list
        vertices_types.push_back( star_tiles[rand()%star_tiles.size()] ); //pick a random star type for it
        //set color
        if(vertices_types.back() == 41){
            vertices_colors.push_back({255,255,255});
        }else{
            vertices_colors.push_back({255,255,255});
        }
        
        //Also add the edge to list
        edges_coords.push_back( {x1,y1,x2,y2} );
        
        //Potentially add an orbit (circle) between this and last point
        if(rand()%4 == 2){
            int radius = (int)( sqrt( pow(x1-x2, 2.0) + pow(y1-y2, 2.0) ) ) ;
            circles.push_back( {x2,y2,radius} );
        }
        
        //update "Point 1"
        x1 = x2;
        y1 = y2;
        printf("%d ", j);

        
        //Done adding a new star
    }
    
    //Now sprinkle with random planets
    int num_planets = 1+rand()%4;
    for(int i = 0; i < num_planets; i++){
        int planet_x = rand() % SCREEN_WIDTH;
        int planet_y = rand() % SCREEN_HEIGHT;
        planet_coords.push_back( {planet_x, planet_y} );
        planet_types.push_back( planet_tiles[rand()%planet_tiles.size()] );
    }
    
    //Possibly add branches??
    
    printf("Done creating constellation...\n");
    
}

//draws a circle
void DrawCircle(SDL_Renderer *Renderer, int _x, int _y, int radius){
    int x = radius - 1;
    int y = 0;
    int tx = 1;
    int ty = 1;
    int err = tx - (radius << 1); // shifting bits left by 1 effectively
    // doubles the value. == tx - diameter
    while (x >= y)
    {

        
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(Renderer, _x + x, _y - y);
        SDL_RenderDrawPoint(Renderer, _x + x, _y + y);
        SDL_RenderDrawPoint(Renderer, _x - x, _y - y);
        SDL_RenderDrawPoint(Renderer, _x - x, _y + y);
        SDL_RenderDrawPoint(Renderer, _x + y, _y - x);
        SDL_RenderDrawPoint(Renderer, _x + y, _y + x);
        SDL_RenderDrawPoint(Renderer, _x - y, _y - x);
        SDL_RenderDrawPoint(Renderer, _x - y, _y + x);
        
        if (err <= 0)
        {
            y++;
            err += ty;
            ty += 2;
        }
        if (err > 0)
        {
            x--;
            tx += 2;
            err += tx - (radius << 1);
        }
    }
}

//Draw Constellations
//draws a blank screen and the constellation
void Constellation::draw_constellation(SDL_Renderer* gRenderer, SDL_Texture** misc_tiles, SDL_Color back_ground_col){
    
    //Draw Blank Background to Screen
    
    //Clear screen
    SDL_SetRenderDrawColor( gRenderer,back_ground_col.r, back_ground_col.g, back_ground_col.b, back_ground_col.a );
    SDL_RenderClear( gRenderer );
    
    //Get ready to draw black
    SDL_SetRenderDrawColor(gRenderer, 0.0,0.0,0.0,0.0);
    
    //Go THrough and draw each circle
    for(int i = 0 ; i < circles.size(); i++){
        DrawCircle(gRenderer, circles[i][0], circles[i][1], circles[i][2]);
    }
    
    //Go through and draw each Edge
    for(int i = 0 ; i < edges_coords.size(); i++){
        
        //Draw a dotted line
        SDL_RenderDrawLine(gRenderer, edges_coords[i][0], edges_coords[i][1], edges_coords[i][2], edges_coords[i][3]);
        
    }
    
    //Go Through and draw each Vertex (remember to center it!)
    for(int i = 0 ; i < vertices_coords.size(); i++ ){
        
        //Draw Star Symbol
        SDL_Rect renderQuad = {vertices_coords[i][0] - 8, vertices_coords[i][1] - 8, 16, 16 };
        SDL_Rect* clip = NULL;
        SDL_RenderCopy(gRenderer, misc_tiles[vertices_types[i]], clip, &renderQuad); //Render to screen
        
    }
    
    //Go through and draw each planet (rmember to center it)
    for(int i = 0 ; i < planet_coords.size(); i++ ){
        
        //Draw Planet Symbol
        SDL_Rect renderQuad = {planet_coords[i][0] - 8, planet_coords[i][1] - 8, 16, 16 };
        SDL_Rect* clip = NULL;
        SDL_RenderCopy(gRenderer, misc_tiles[planet_types[i]], clip, &renderQuad); //Render to screen
        
    }
    

        
    
    //DrawCircle(gRenderer, vertices_coords[3][0], vertices_coords[3][1], 50);
    
}












