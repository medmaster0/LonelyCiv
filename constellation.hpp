//
//  constellation.hpp
//  Civ2
//
//  Created by Medz on 9/1/18.
//  Copyright © 2018 Medz. All rights reserved.
//

#ifndef constellation_hpp
#define constellation_hpp

#include <stdio.h>
#include <SDL2/SDL.h>
#include <vector>

using namespace std;


class Constellation{
public:
    Constellation(int SCREEN_WIDTH, int SCREEN_HEIGHT); //constructor -> generate random constellation
    vector<vector<int>> vertices_coords; //list of coords of the vertices
    vector<int> vertices_types; //list of tile types of the vertices
    vector<SDL_Color> vertices_colors; //list of colors of the vertices
    vector<vector<int>> edges_coords; //list of lines (edges) connecting the vertices ( (x1,y1,x2,y2), (x1,y1,x2,y2)  )
    vector<vector<int>> planet_coords; //list of coords where the planets are
    vector<int> planet_types; //list of tile types corresponding to the planets
    
    vector<vector<int>> circles; //list of circles to draw { (x0,y0,r0), (x1,y1,r1), ...}
    
    //Handle branched  paths, (will be dotted later)
    vector<vector<int>> brach_vertices_coords; //list of coords of the vertices of the branched
    vector<int> branch_vertices_types; //lis of tiles types of the branched vertices
    vector<SDL_Color> branch_vertices_colors; //list of colors of the vertices
    vector<vector<int>> branch_edges_coords; //list of lines (edges) connecting the vertices ( (x1,y1,x2,y2), (x1,y1,x2,y2)  )
    
    void draw_constellation(SDL_Renderer* gRenderer, SDL_Texture** misc_tiles, SDL_Color back_ground_col); //draws a blank screen and the constellation
};






#endif /* constellation_hpp */
