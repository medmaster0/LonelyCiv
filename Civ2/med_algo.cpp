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


//Path finding using A*
//input: block_map: 1-D map array indicating if tile blocked
//       x1,y1: starting coords
//       x2,y2: target coords
//output: list of coords leading to target
struct a_node{
    a_node *parent;
    int x, y;
    int f, g, h;
};
vector<vector<int>> findPathToCoord_A_Star(bool block_map[] , int x1, int y1, int x2, int y2){
    
    vector<vector<int>> search_q; //the main "list"
    
    //CORDS: (x,y)
    a_node t_node; //temportary coordinate to add to lists...
    vector<a_node> closed_set; //list of closed coords
    vector<a_node> open_set; //list of open coords
    
    //Add start node to open set
    t_node = {0x0000, x1, y1, 0, 0, 0};
    open_set.push_back(t_node);
    
    //Iteation
    //a_node q; //A temporary node to be used throughout ITERATION
    int index; //a temp index used throughout
    bool searching = true; //
    //more counters used throughout, declared here for optimization??
    //while(open_set.size()>0){
    while(searching){
    
        //Find the node in open_set with least f, make it q
        index = 0;
        int least_f = 9999999;
        for(int i = 0; i<open_set.size(); i++){
            if(open_set[i].f < least_f){
                least_f = open_set[i].f;
                index = i; //the index of the node we want.
            }
        }
        a_node q = open_set[index]; //q now has the node with the lowest f, muthafyucka
        closed_set.push_back(q); //Now add that node to the closed set
        //printf(" %d,%d then ",open_set[index].x,open_set[index].y);
        
        printf("\nopen set bitch fucker:");
        for(int i = 0; i<open_set.size(); i++){
            if(!((open_set[i].x==x1)&&(open_set[i].y==x1))){
                printf(" ( %d, %d, %d,%d )", open_set[i].x, open_set[i].y, open_set[i].parent->x, open_set[i].parent->y);
        
            }
        }
        
//        printf("\nafter set bitch vfudfsak;fklas:");
//        for(int i = 0; i<open_set.size(); i++){
//            printf(" ( %d, %d, %d )", open_set[i].x, open_set[i].y, open_set[i].f);
//        }
        if(!((q.x==x1)&&(q.y==x1))){
            printf("\n the q: (%d,%d, %d,%d )", q.x, q.y, q.parent->x, q.parent->y);
        }
        printf("\nclose set bitch fucker:");
        for(int i = 1; i<closed_set.size(); i++){
            printf(" ( %d, %d, %d,%d )", closed_set[i].x, closed_set[i].y, closed_set[i].parent->x, closed_set[i].parent->y);
        }
        
        //printf("%d,%d.",open_set[index].x,open_set[index].y);
        //printf(" %d ", q.f);
        
        //Now, generate nodes for the 4 neighbors around q (set their pointers to the parent)
        vector<a_node> neighbors;
        //Left...
        t_node = {&closed_set[closed_set.size()-1], q.x-1, q.y, 9999, 9999, 9999}; //create the t_node (let's initialize the distance values to some code and calculate later
        neighbors.push_back(t_node);
        //Right...
        t_node = {&closed_set[closed_set.size()-1], q.x+1, q.y, 9999, 9999, 9999}; //create the t_node (let's initialize the distance values to some code and calculate later
        neighbors.push_back(t_node);
        //Up...
        t_node = {&closed_set[closed_set.size()-1], q.x, q.y-1, 9999, 9999, 9999}; //create the t_node (let's initialize the distance values to some code and calculate later
        neighbors.push_back(t_node);
        //Down...
        t_node = {&closed_set[closed_set.size()-1], q.x, q.y+1, 9999, 9999, 9999}; //create the t_node (let's initialize the distance values to some code and calculate later
        neighbors.push_back(t_node);
        
        //Now perform the calculation on each neighbor to check if we add it to the open list...
        for(int i = 0; i<neighbors.size(); i++){
            
//            //check where neighbor is pointing to
//            printf("pointing: %d,%d,%d \n", neighbors[i].parent->x, neighbors[i].parent->y, neighbors[i].parent->f);
            
            //If the node is at the goal, then stop!
            if((neighbors[i].x == x2)&&(neighbors[i].y == y2)){
                printf("okay done");
                closed_set.push_back(neighbors[i]);
                searching = false;
                break;
            }
            
            //CALCULATE G
            neighbors[i].g = q.g + 1;
            
            //CALCULATE H
            neighbors[i].h = abs(x2 - neighbors[i].x)+(y2 - neighbors[i].y);
            
            //CALCULATE F
            neighbors[i].f = neighbors[i].g + neighbors[i].h;
            
            //Now check if neighbor is in closed or open set with lower f...
            bool in_set = false; //A flag indicating whether the neighbor is in one of the sets already (with lower f)
            for(int j = 0; j<open_set.size(); j++){
                if( (open_set[j].f <= neighbors[i].f) && (open_set[j].x==neighbors[i].x) && (open_set[j].y==neighbors[i].y) ){
                //if((open_set[j].x==neighbors[j].x) && (open_set[j].y==neighbors[j].y) ){
                    in_set = true;
                }
            }
            for(int j = 0; j<closed_set.size(); j++){
                //if((closed_set[j].x==neighbors[i].x) && (closed_set[j].y==neighbors[i].y) ){
                if( (closed_set[j].f <= neighbors[i].f) && (closed_set[j].x==neighbors[i].x) && (closed_set[j].y==neighbors[i].y) ){
                    in_set = true;
                }
            }
            //If we couldn't find it in the other sets, add it to open_list
            if(in_set == false){
                open_set.push_back(neighbors[i]);
                printf("\nokay push: ");
                printf("%d,%d,%d,%d",neighbors[i].x,neighbors[i].y, neighbors[i].parent->x, neighbors[i].parent->y  );
            }
            
            //printf("%d", open_set.size());
            
        }
        
        open_set.erase(open_set.begin() + index ); //also erase from list
        if(open_set.size()<=0){
            printf("open_set empty , possibly means couldn't reach target???? but could also mean other stuff too!");
            searching = false;
        }
        
        //printf("%d",closed_set.size());
        
        //reconstruct path back from closed set
//        printf("\n closed bitchass set: ");
//        for(int w = 0; w<closed_set.size(); w++){
//            printf("(%d,%d,%d)",closed_set[w].x,closed_set[w].y,closed_set[w].f);
//        }
//        a_node temp = closed_set.back();
//        printf("\npresent to you path:" );
//        while(true){
//            printf("(%d,%d,%d)", temp.x, temp.y, temp.f);
//            temp = *temp.parent;
//            
//        }
        
        
        //break;
    }

    printf("\n closed bitchass set: ");
    for(int w = 1; w<closed_set.size(); w++){
        printf("(%d,%d,%d,%d)",closed_set[w].x,closed_set[w].y,closed_set[w].parent->x, closed_set[w].parent->y);
    }
    a_node temp = closed_set.back();
    printf("\npresent to you path:" );
//    temp = *(temp.parent);
//    temp = *(temp.parent);
//    temp = *(temp.parent);
//    temp = *(temp.parent);
    printf("%d,%d,%d", temp.x, temp.y, temp.f);
//    printf("%d", temp.parent);
    //temp = {temp.parent, temp.x, temp.y, temp.f, temp.g, temp.h};
    //temp = *(temp.parent);
    temp = *(temp.parent->parent);
//    printf(" %d", temp.parent);
    while(true){
        printf("(%d,%d,%d)", temp.x, temp.y, temp.f);
        temp = *temp.parent;
        
    }
    
    
    
    
    
    return search_q;
    
    
}



//An even better A*, gash damn!
vector<vector<int>> A_star(bool block_map[], int x1, int y1, int x2, int y2){
    vector<vector<int>> search_q; //the main "list"
    return search_q;
}
















