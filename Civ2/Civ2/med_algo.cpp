//
//  med_algo.cpp
//  Civ2
//
//  Created by Medz on 9/1/17.
//  Copyright © 2017 Medz. All rights reserved.
//

#include "med_algo.hpp"
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

using namespace std;

//generates a random corridor maze
//A corridor maze is moitititi inspired set of corridors
//There are two intermediate walls in open runs
// 1 - BRICK
// 0 - EMPTY SPACE
vector<vector<short>> gen_maze_corridor(){

    vector<vector<short>> tmaze; //temporarily create the maze
    vector<short> trow; //tmemporarily create a row for the maze
    int rows = 80; //how long the meze is
    int cols = 20; //how wide the maze is
    
    //these lists keep track where the mid walls are for each open row
    vector<short> walls1;
    vector<short> walls2;
    
    //Create alternating periods of walls and no walls.
    for(int i = 0; i<rows; i++){ //for every row
        //Decide whether to fill in cols with 1 or 0
        if(i%2 == 0){ //on even
            //fill in with 0s
            trow.clear(); //clear first
            for(int j = 0 ; j<cols; j++){
                trow.push_back(0);
            }
            //but also add the boundaries on sides
            trow[0] = 1;
            trow[cols - 1] = 1;
            tmaze.push_back(trow);
        }else{ //on odd
            //fill in with 1s
            trow.clear(); //clear first
            for(int k = 0 ; k<cols; k++){
                trow.push_back(1);
            }
            tmaze.push_back(trow);
        }
        
    }
    
    //Now we add the walls in the middle of the corridor
    short wall_pos; //temporarily holds the wall's position
    for(int i = 0; i<rows; i++){ //for every row
        //only do the evens
        if(i%2 !=0){
            walls1.push_back(0);//just as a place holder
            walls2.push_back(0);//just as a place holder
            continue;
        }
        
        wall_pos = rand()%(short)floor((0.75*cols)) + 1; //because of this, 0.25*cols need to be greater than 3!!!!!!
        tmaze[i][wall_pos] = 1;
        walls1.push_back(wall_pos);
        //Do it again for the second wall
        wall_pos = rand()%(cols-walls1[i]-2) + walls1[i] + 1;
        tmaze[i][wall_pos] = 1;
        walls2.push_back(wall_pos);

    }
    
    //With walls generated, we can determine where to place doors
    //Place Doors, based on walls lists
    vector<short> adj_walls; //a temp list to keep track of where adjacent walls are
    for(int i = 0; i<rows-2; i++){ //for every row , BUT DON"T DO IT FOR LAST ROW THAT IS INCOMPLETE
        //only do the odds
        if(i%2 ==0){
            continue;
        }
        
        //Now for every solid row, we look at the adjacnet open rows and determine where walls are
        adj_walls.clear();
        if(i-1>=0){
            adj_walls.push_back(walls1[i-1]);
            adj_walls.push_back(walls2[i-1]);
        }
        if (i+1<rows){
            adj_walls.push_back(walls1[i+1]);
            adj_walls.push_back(walls2[i+1]);
        }

        //make sure the nodes are in order
        sort(adj_walls.begin(), adj_walls.end());
        
    
        //between each adjacent wall, we need a doorway
        //pick a random spot between them (if there's enough room)
        //TODO::::::: currently hardcode each combination, could probably be more general
        short d; //a temporary values between the two "adjacent walls"
        //FIRST
        if(adj_walls[0]-0>1){
            d = rand()%(adj_walls[0]-1)+0+1;
            tmaze[i][d] = 3;
        }
        //SECOND
        if(adj_walls[1]-adj_walls[0]>1){
            d = rand()%(adj_walls[1]-adj_walls[0]-1)+adj_walls[0]+1;
            tmaze[i][d] = 3;
        }
        //THIRD, one row will only have two
        if(adj_walls[2]-adj_walls[1]>1){
            d = rand()%(adj_walls[2]-adj_walls[1]-1)+adj_walls[1]+1;
            tmaze[i][d] = 3;
        }
        //FOURTH, one row will only have two
        if(adj_walls[3]-adj_walls[2]>1){
            d = rand()%(adj_walls[3]-adj_walls[2]-1)+adj_walls[2]+1;
            tmaze[i][d] = 3;
        }
        //FIFTH,
        if(cols-1-adj_walls[3]>1){
            d = rand()%(cols-1-adj_walls[3]-1)+adj_walls[3]+1;
            tmaze[i][d] = 3;
        }
        
        
        
        //printf("sizeof: %d  ", adj_walls.size());
        
        
//            //print out shit
//            for(int i = 0 ; i<adj_walls.size(); i++){
//                    printf("%d", adj_walls[i]);
//        
//                printf("\n");
//            }
        
    }
    
    
    return tmaze;

    
}

//prints out a maze to console, simple utility
void printMaze(vector<vector<short>> maze){
    
    for(int i = 0 ; i<maze.size(); i++){
        for(int j = 0 ; j < maze[i].size(); j++){
            printf("%d", maze[i][j]);
            
        }
        printf("\n");
    }
    
    
    
}




























