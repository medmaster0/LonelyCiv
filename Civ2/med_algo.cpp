//
//  med_algo.cpp
//  Civ2
//
//  Created by Medz on 9/1/17.
//  Copyright © 2017 Medz. All rights reserved.
//

#include "med_algo.hpp"
#include <vector>
//#include <list>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string>

#include <iostream>

#include <SDL2/SDL.h>

using namespace std;

//generates a random corridor maze
//A corridor maze is moitititi inspired set of corridors
//There are two intermediate walls in open runs
// 1 - BRICK
// 0 - EMPTY SPACE
// Inputs: Specify how many rows, cols
// Output: maze[rows][cols]
vector<vector<short>> gen_maze_corridor(int rows_in, int cols_in){

    vector<vector<short>> tmaze; //temporarily create the maze
    vector<short> trow; //tmemporarily create a row for the maze
    int rows = rows_in; //how long the meze is
    int cols = cols_in; //how wide the maze is
    
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
//            //but also add the boundaries on sides
//            trow[0] = 1;
//            trow[cols - 1] = 1;
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
        
        
    }
    
    //clear last wall
    for(int d = 0; d<cols_in; d++){
        //tmaze[rows].push_back(0);
        tmaze[rows-1][d] = 0;
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



///////////////////////////
//Implemented linked list function

//need struct for linked list node, list_node
struct list_node{
    list_node *next; //next in list, in whatever linked list it's in
    list_node *prev; //previous in list, whatver linked list it's in
    list_node *parent; //the parent (in regards to A* algo)
    int x,y,z;
    int f,g,h;
    list_node(){
        next = 0;
        prev = 0;
        parent = 0;
        x,y,f,g,h = 0;
    }
    list_node(list_node* i_next, list_node* i_prev, list_node* i_parent, int i_x, int i_y, int i_f, int i_g , int i_h){
        next = i_next;
        prev = i_prev;
        parent = i_parent;
        x = i_x;
        y = i_y;
        f = i_f;
        g = i_g;
        h = i_h;
    }
    list_node(list_node* i_next, list_node* i_prev, list_node* i_parent, int i_x, int i_y, int i_z, int i_f, int i_g , int i_h){
        next = i_next;
        prev = i_prev;
        parent = i_parent;
        x = i_x;
        y = i_y;
        z = i_z;
        f = i_f;
        g = i_g;
        h = i_h;
    }
};

//generic struct for linked lists
struct list{
    list_node* first;
    list_node* last;
};

//An even better A*, gash damn!
//Returns a list (vector) of steps from x1,y1 to x2,y2
//Steps are in form of (x,y)
//returns error code {{9999,9999}} if can't find path
vector<vector<int>> A_Star(bool block_map[],int map_width, int map_height, int x1, int y1, int x2, int y2){
    
    //DEBUG
    int num_news = 0; //keeps track of calls to new
    
    list open_set = {nullptr,nullptr}; //the start of the open_set
    list closed_set = {nullptr,nullptr};; //the start of the closed_set
    
    num_news = num_news + 1;
    //create first node
    list_node* temp = new list_node;
    *temp = {nullptr, nullptr, nullptr, x1, y1, 0, 0, 0};
    
    //add it to open_set
    open_set.first = temp;
    open_set.last = temp;
    
    bool search_main = true; //flag to indicate searching - turns off when target is reached
    //while open_set isn't empty, keep picking off the node with the smallest f
    while( (open_set.first!=nullptr)  && (search_main)){
        
        //find node in open_set with least f, make it q
        ///////////////////////////////////
        bool search_f = true;
        list_node* lowest_f = nullptr; //points to the node with lowest f
        int least_f = 999999; //start with high number
        list_node* current = open_set.first; //points to the current node, we're searching
        while(search_f){
            
            if(current == nullptr){
                break;
            }
            
            if(current->f<=least_f){ //if, it has a lesser f, point to it... THIS ALSO ENSURES THE LAST ONE POPPED ON GETS PICKED
                lowest_f = current;
                least_f = lowest_f->f;
            }
            if(current==open_set.last){ //if cuurently pointing to last nodee, then stop search
                search_f = false;
            }else{
                current = current->next;
            }
        }
        //when done with search, lowest_f should point to node with lowest f
        
        //Find lowest_f node in open_set and call it q
        ////////////////////////////
        list_node* q = lowest_f; //q has the lowest f
        
        //POPping off q from OPEN_SET
        ////////////////////////
        if(q->prev!=nullptr){ //if lowest_f is not FIRST in list
            (q->prev)->next = q->next; //update the previous node in list
        }else{ //if lowest_f is first in list
            open_set.first = q->next; //update the open_set struct
        }
        if(q->next!=nullptr){ //if lowest_f is not LAST in list
            (q->next)->prev = q->prev; //update the next node in list
        }else{ //if lowest_f is last in list
            open_set.last = q->prev; //update the open_set struct
        }
        //open_set (and it's elements) are correct and up to date
        
        num_news = num_news + 4;
        list_node* neighbors[4]; //The FOUR neighbors
        neighbors[0] = new list_node;
        *neighbors[0] = {nullptr, nullptr, q, (q->x)-1 , q->y , 9999, 9999, 9999};
        neighbors[1] = new list_node;
        *neighbors[1] = {nullptr, nullptr, q, (q->x)+1 , q->y , 9999, 9999, 9999};
        neighbors[2] = new list_node;
        *neighbors[2] = {nullptr, nullptr, q, q->x , (q->y)-1 , 9999, 9999, 9999};
        neighbors[3] = new list_node;
        *neighbors[3] = {nullptr, nullptr, q, q->x , (q->y)+1 , 9999, 9999, 9999};
     
        //ADD q to CLOSED_SET
        q->next = nullptr; //indicate q is at end of list
        q->prev = closed_set.last; //update q who it's behind
        if(closed_set.last!=nullptr){closed_set.last->next = q;} //the (former) last item in list now points to neighbor
        closed_set.last= q; //the closed_set is updated
        if(closed_set.first==nullptr){closed_set.first= q;}; //if this is the first item in open_set
        
        //For each neighbor/successor
        for(int i = 0; i<4; i++){
            
            //if neighbor is out of bounds then we can throw it away
            if( (neighbors[i]->y < 0) || (neighbors[i]->x < 0) || (neighbors[i]->x >= map_width) || (neighbors[i]->y >= map_height)    ){
                delete neighbors[i]; //delete what we've created!!!
                num_news = num_news - 1;
                continue; //move on to next neighbor
            }
            
            //if neighbor tile is blocked then we can stop
            if(block_map[((neighbors[i]->y)*map_width)+neighbors[i]->x]==true){
                delete neighbors[i]; //delete what we've created!!!
                num_news = num_news - 1;
                continue; //move on to next neighbor
            }
            
            //if neighbor is TARGET, then we can stop
            if((neighbors[i]->x==x2)&&(neighbors[i]->y==y2)){

                //ADD neighbor to CLOSED_SET
                neighbors[i]->next = nullptr; //indicate neighbor is at end of list
                neighbors[i]->prev = closed_set.last; //update neighbor who it's behind
                if(closed_set.last!=nullptr){closed_set.last->next = neighbors[i];} //the (former) last item in list now points to neighbor
                closed_set.last = neighbors[i]; //the closed_set is updated
                if(closed_set.first==nullptr){closed_set.first= neighbors[i];}; //if this is the first item in open_set
                
                //we also need to delete the rest of the neighbors that we've created but haven't had a chance to process
                int j = i + 1;
                while(j<4){
                    delete neighbors[j];
                    num_news = num_news - 1;
                    j = j + 1;
                }
                
                search_main = false;
                break;
            }
            
            //Calculate The 3 Values: f, g, h
            neighbors[i]->g = q->g + 1;
            neighbors[i]->h = abs(y2-(neighbors[i]->y)) + abs(x2-(neighbors[i]->x));
            neighbors[i]->f = neighbors[i]->g + neighbors[i]->h;
            
            //Cycle Through OPEN_SET and check if the neighbor is already in there (with lower f or does it matter here?)
            bool search_o = true; //search flag
            bool in_open =  false; //flag indicating if neighbor is in OPNE_SET
            current = open_set.first; //start at beginning
            while(search_o){
                
                //Check if at end of list
                if(current==nullptr){
                    search_o = false;
                    break;
                }
                
                //Check x,y values in node
                if( (current->x==neighbors[i]->x) && (current->y==neighbors[i]->y)){
                    in_open = true;
                }
              
                //Move on to search next node in list
                current = current->next;
                
            }
            if(in_open){
                delete neighbors[i]; //delete temporary neighbor
                num_news = num_news - 1;
                continue; //skip this neighbor
            }
            
            //Cycle Through CLOSED_SET and check if the neighbor is already in there (with lower f or does it matter here?)
            bool search_c = true; //search flag
            bool in_closed =  false; //flag indicating if neighbor is in CLOSED_SET
            current = closed_set.first; //start at beginning
            while(search_c){
                
                //Check if at end of list
                if(current==nullptr){
                    search_c = false;
                    break;
                }
                
                //Check x,y values in node
                if( (current->x==neighbors[i]->x) && (current->y==neighbors[i]->y)){
                    in_closed = true;
                }
                
                //Move on to search next node in list
                current = current->next;
                
            }
            if(in_closed){
                delete neighbors[i]; //delete what we've created!!!
                num_news = num_news - 1;
                continue; //skip this neighbor
            }
            
            //If we've made it to this point, it isn't in CLOSED or OPEN SET
            //ADD neighbor to OPEN_SET
            neighbors[i]->next = nullptr; //indicate neighbor is at end of list
            neighbors[i]->prev = open_set.last; //update neighbor who it's behind
            if(open_set.last!=nullptr){open_set.last->next = neighbors[i];} //the (former) last item in list now points to neighbor
            open_set.last = neighbors[i]; //the open_set is updated
            if(open_set.first==nullptr){open_set.first= neighbors[i];}; //if this is the first item in open_set

            
        }//Done cycling through neighbors
        
        //THIS MEANS WE HIT THE END OF OPEN_SET AND HAVENT FOUND TARGET
        if(open_set.first == nullptr){
            
            //DELETIION
            //Delete out the closed and open_sets
            //...the closed set
            bool erasing = true;
            list_node* cur = closed_set.first; //the current node we mpointing at
            while(erasing){
                if(cur==nullptr){
                    erasing = false;
                    break;
                }
                list_node* temp = cur; //Fo deletion
                cur = cur->next;
                delete temp; //Also fo deletion
                num_news = num_news - 1;
            }
            //...the open set
            erasing = true;
            cur = open_set.first;
            while(erasing){
                if(cur==nullptr){
                    erasing = false;
                    break;
                }
                list_node* temp = cur; //fo deletion
                cur = cur->next;
                delete temp; //also fo deletion
                num_news = num_news - 1;
                
            }
            
            printf("A star flat couldn't find target\n");
            return {{9999,9999}};
        }
        
        
    }//end open_list empty
    
    
    //CCONSTRUCT VECTOR TO RETURN BY TRAVERSING THE CLOSED LIST
    vector<vector<int>> search_q; //the main "list", could really be turned into what ever datat type you wantx
    vector<int> coord; //a temp coord to add to the search_q.
    list_node* travel = closed_set.last; //temp pointer for traversing the lists
    while(true){
        coord = {travel->x,travel->y};
        search_q.push_back(coord);
        travel = travel->parent; //move on to the parent
        if(travel == nullptr){ //if at end of list
            break; //bust outta the while loop
        }
    }
    
    
    //DELETIION
    //Print out the closed and open_sets
    //...the closed set
    bool printing = true;
    list_node* current = closed_set.first;
    while(printing){
        if(current==nullptr){
            printing = false;
            break;
        }
        list_node* temp = current; //Fo deletion
        current = current->next;
        delete temp; //Also fo deletion
        num_news = num_news - 1;
        
    }
    //...the open set
    printing = true;
    current = open_set.first;
    while(printing){
        if(current==nullptr){
            printing = false;
            break;
        }
        list_node* temp = current; //fo deletion
        current = current->next;
        delete temp; //also fo deletion
        num_news = num_news - 1;
        
    }
    
    return search_q;
}

//
////An A* algorithm that is aware of the z-dimension (can climb ladders) .
// (Ladders appear on map_scenery_top list in main function, btw)
// Deals with 3D points (as opposed to 2D in original)
// While adding steps to search set, also checks if each step is a ladder.
// Still uses Manhattan distance as a heuristic
// Same inputs except also takes map_scenery_top list (for ladders)
//Returns a list (vector) of steps from x1,y1,z1 to x2,y2,z2
//Steps are in form of (x,y,z)
//returns error code {{9999,9999}} if can't find path

vector<vector<int>> A_Star_Z(bool block_map[], vector<vector<Item>>* map_scenery_top, int map_width, int map_height,
                                  int x1, int y1, int z1, int x2, int y2, int z2){
    
    //DEBUG
    int num_news = 0; //keeps track of calls to new
    
    list open_set = {nullptr,nullptr}; //the start of the open_set
    list closed_set = {nullptr,nullptr};; //the start of the closed_set
    
    num_news = num_news + 1;
    //create first node
    list_node* temp = new list_node;
    *temp = {nullptr, nullptr, nullptr, x1, y1, z1, 0, 0, 0};
    
    //add it to open_set
    open_set.first = temp;
    open_set.last = temp;
    
    bool search_main = true; //flag to indicate searching - turns off when target is reached
    //while open_set isn't empty, keep picking off the node with the smallest f
    while( (open_set.first!=nullptr)  && (search_main)){
        
        //find node in open_set with least f, make it q
        ///////////////////////////////////
        bool search_f = true;
        list_node* lowest_f = nullptr; //points to the node with lowest f
        int least_f = 999999; //start with high number
        list_node* current = open_set.first; //points to the current node, we're searching
        while(search_f){
            
            if(current == nullptr){
                break;
            }
            
            if(current->f<=least_f){ //if, it has a lesser f, point to it... THIS ALSO ENSURES THE LAST ONE POPPED ON GETS PICKED
                lowest_f = current;
                least_f = lowest_f->f;
            }
            if(current==open_set.last){ //if cuurently pointing to last nodee, then stop search
                search_f = false;
            }else{
                current = current->next;
            }
        }
        //when done with search, lowest_f should point to node with lowest f
        
        //Find lowest_f node in open_set and call it q
        ////////////////////////////
        list_node* q = lowest_f; //q has the lowest f
        
        //POPping off q from OPEN_SET
        ////////////////////////
        if(q->prev!=nullptr){ //if lowest_f is not FIRST in list
            (q->prev)->next = q->next; //update the previous node in list
        }else{ //if lowest_f is first in list
            open_set.first = q->next; //update the open_set struct
        }
        if(q->next!=nullptr){ //if lowest_f is not LAST in list
            (q->next)->prev = q->prev; //update the next node in list
        }else{ //if lowest_f is last in list
            open_set.last = q->prev; //update the open_set struct
        }
        //open_set (and it's elements) are correct and up to date
        
        num_news = num_news + 4;
        list_node* neighbors[4]; //The FOUR neighbors
        neighbors[0] = new list_node;
        *neighbors[0] = {nullptr, nullptr, q, (q->x)-1 , q->y, q->z, 9999, 9999, 9999};
        neighbors[1] = new list_node;
        *neighbors[1] = {nullptr, nullptr, q, (q->x)+1 , q->y, q->z, 9999, 9999, 9999};
        neighbors[2] = new list_node;
        *neighbors[2] = {nullptr, nullptr, q, q->x , (q->y)-1, q->z, 9999, 9999, 9999};
        neighbors[3] = new list_node;
        *neighbors[3] = {nullptr, nullptr, q, q->x , (q->y)+1, q->z, 9999, 9999, 9999};
        
        //For each neighbor/successor
        for(int i = 0; i<4; i++){
            
            //if neighbor is out of bounds then we can throw it away
            if( (neighbors[i]->y < 0) || (neighbors[i]->x < 0) || (neighbors[i]->x >= map_width) || (neighbors[i]->y >= map_height)    ){
                delete neighbors[i]; //delete what we've created!!!
                num_news = num_news - 1;
                continue; //move on to next neighbor
            }
            
            //if neighbor tile is blocked then we can stop
            if(block_map[((neighbors[i]->y)*map_width)+neighbors[i]->x]==true){
                delete neighbors[i]; //delete what we've created!!!
                num_news = num_news - 1;
                continue; //move on to next neighbor
            }
            
            //if neighbor is TARGET, then we can stop (and must delete rest of neighbors)
            if((neighbors[i]->x==x2)&&(neighbors[i]->y==y2)&&(neighbors[i]->z==z2)){
                
                //ADD neighbor to CLOSED_SET
                neighbors[i]->next = nullptr; //indicate neighbor is at end of list
                neighbors[i]->prev = closed_set.last; //update neighbor who it's behind
                if(closed_set.last!=nullptr){closed_set.last->next = neighbors[i];} //the (former) last item in list now points to neighbor
                closed_set.last = neighbors[i]; //the closed_set is updated
                if(closed_set.first==nullptr){closed_set.first= neighbors[i];}; //if this is the first item in open_set
                
                //we also need to delete the rest of the neighbors that we've created but haven't had a chance to process
                int j = i + 1;
                while(j<4){
                    delete neighbors[j];
                    num_news = num_news - 1;
                    j = j + 1;
                }
                
                search_main = false;
                break;
            }
            
            //Calculate The 3 Values: f, g, h
            neighbors[i]->g = q->g + 1;
            neighbors[i]->h = abs(y2-(neighbors[i]->y)) + abs(x2-(neighbors[i]->x)) + abs(z2-(neighbors[i]->z));
            neighbors[i]->f = neighbors[i]->g + neighbors[i]->h;
            
            //Cycle Through OPEN_SET and check if the neighbor is already in there (with lower f or does it matter here?)
            bool search_o = true; //search flag
            bool in_open =  false; //flag indicating if neighbor is in OPNE_SET
            current = open_set.first; //start at beginning
            while(search_o){
                
                //Check if at end of list
                if(current==nullptr){
                    search_o = false;
                    break;
                }
                
                //Check x,y values in node
                if( (current->x==neighbors[i]->x) && (current->y==neighbors[i]->y) && (current->z==neighbors[i]->z) ){
                    in_open = true;
                }
                
                //Move on to search next node in list
                current = current->next;
                
            }
            if(in_open){
                delete neighbors[i]; //delete temporary neighbor
                num_news = num_news - 1;
                continue; //skip this neighbor
            }
            
            //Cycle Through CLOSED_SET and check if the neighbor is already in there (with lower f or does it matter here?)
            bool search_c = true; //search flag
            bool in_closed =  false; //flag indicating if neighbor is in CLOSED_SET
            current = closed_set.first; //start at beginning
            while(search_c){
                
                //Check if at end of list
                if(current==nullptr){
                    search_c = false;
                    break;
                }
                
                //Check x,y values in node
                if( (current->x==neighbors[i]->x) && (current->y==neighbors[i]->y) && (current->z==neighbors[i]->z)){
                    in_closed = true;
                }
                
                //Move on to search next node in list
                current = current->next;
                
            }
            if(in_closed){
                delete neighbors[i]; //delete what we've created!!!
                num_news = num_news - 1;
                continue; //skip this neighbor
            }
            
            //If we've made it to this point, it isn't in CLOSED or OPEN SET
            //ADD neighbor to OPEN_SET
            neighbors[i]->next = nullptr; //indicate neighbor is at end of list
            neighbors[i]->prev = open_set.last; //update neighbor who it's behind
            if(open_set.last!=nullptr){open_set.last->next = neighbors[i];} //the (former) last item in list now points to neighbor
            open_set.last = neighbors[i]; //the open_set is updated
            if(open_set.first==nullptr){open_set.first= neighbors[i];}; //if this is the first item in open_set
            
            
        }//Done cycling through neighbors
        
        
        ///////////////////////////////////////////////////
        //Now check if there are LADDERS on the tile
        //
        for(int i = 0; i < map_scenery_top->at( q->z*(map_height*map_width) + q->y*map_width + q->x ).size(); i++ ){
            if( map_scenery_top->at( q->z*(map_height*map_width) + q->y*map_width + q->x ).at(i).type == 318){ //check if a ladder is on tile.
                //DEBUG:
                printf("found a ladder\n");
                
                //check if ladder can go up
                for(int j = 0; j < map_scenery_top->at( (q->z+1)*(map_height*map_width) + q->y*map_width + q->x ).size() ; j++){ //for looop for up items
                    if(map_scenery_top->at( (q->z+1)*(map_height*map_width) + q->y*map_width + q->x ).at(j).type == 318){
                        //create temporary node
                        list_node* ladder_node = new list_node;
                        *ladder_node = {nullptr, nullptr, q, q->x , q->y, q->z + 1, 9999, 9999, 9999};
                        
                        //If it's the target, then we can stop
                        if( ladder_node->x == x2 && ladder_node->y == y2 && ladder_node->z == z2){
                            //ADD neighbor to CLOSED_SET
                            ladder_node->next = nullptr; //indicate ladder_node is at end of list
                            ladder_node->prev = closed_set.last; //update ladder_node with who it's behind
                            if(closed_set.last!=nullptr){closed_set.last->next = ladder_node;} //the (former) last item in list now points to node
                            closed_set.last = ladder_node; //the closed_set is updated
                            if(closed_set.first==nullptr){closed_set.first= ladder_node;}; //if this is the first item in closed_set
                            search_main = false;
                            break;
                            
                        }
                    
                        //Calculate The 3 Values: f, g, h
                        ladder_node->g = q->g + 1;
                        ladder_node->h = abs(y2-(ladder_node->y)) + abs(x2-(ladder_node->x)) + abs(z2-(ladder_node->z));
                        ladder_node->f = ladder_node->g + ladder_node->h;
                        
                        //Cycle Through OPEN_SET and check if the ladder_node is already in there
                        bool search_o = true; //search flag
                        bool in_open =  false; //flag indicating if ladder_node is in OPEN_SET
                        current = open_set.first; //start at beginning
                        while(search_o){
                            
                            //Check if at end of list
                            if(current==nullptr){
                                search_o = false;
                                break;
                            }
                            
                            //Check x,y,z values in node
                            if( (current->x== ladder_node->x) && (current->y==ladder_node->y) && (current->z==ladder_node->z) ){
                                in_open = true;
                            }
                            
                            //Move on to search next node in list
                            current = current->next;
                            
                        }
                        if(in_open){
                            delete ladder_node; //delete temporary ladder_node
                            num_news = num_news - 1;
                            continue; //skip this ladder_node
                        }
                        
                        //Cycle Through CLOSED_SET and check if the ladder_node is already in there
                        bool search_c = true; //search flag
                        bool in_closed =  false; //flag indicating if ladder_node is in CLOSED_SET
                        current = closed_set.first; //start at beginning
                        while(search_c){
                            
                            //Check if at end of list
                            if(current==nullptr){
                                search_c = false;
                                break;
                            }
                            
                            //Check x,y values in node
                            if( (current->x==ladder_node->x) && (current->y==ladder_node->y) && (current->z==ladder_node->z)){
                                in_closed = true;
                            }
                            
                            //Move on to search next node in list
                            current = current->next;
                            
                        }
                        if(in_closed){
                            delete ladder_node; //delete what we've created!!!
                            num_news = num_news - 1;
                            continue; //skip this neighbor
                        }
                        
                        //If we've made it to this point, it isn't in CLOSED or OPEN SET
                        //ADD ladder_node to OPEN_SET
                        ladder_node->next = nullptr; //indicate ladder_node is at end of list
                        ladder_node->prev = open_set.last; //update ladder_node who it's behind
                        if(open_set.last!=nullptr){open_set.last->next = ladder_node;} //the (former) last item in list now points to ladder_node
                        open_set.last = ladder_node; //the open_set is updated
                        if(open_set.first==nullptr){open_set.first= ladder_node;}; //if this is the first item in open_set
                        
                    } //for loop for up items
                } //END LADDER GO UP
                
                //check if ladder can go down (bounds check first)
                if(q->z <= 0)continue; //BOUNDS CHECK!!!!!!
                if(map_scenery_top->at( (q->z-1)*(map_height*map_width) + q->y*map_width + q->x ).at(i).type == 318){
                    //create temporary node
                    //create temporary node
                    list_node* ladder_node = new list_node;
                    *ladder_node = {nullptr, nullptr, q, q->x , q->y, q->z - 1, 9999, 9999, 9999};
                    
                    //If it's the target, then we can stop
                    if( ladder_node->x == x2 && ladder_node->y == y2 && ladder_node->z == z2){
                        //ADD neighbor to CLOSED_SET
                        ladder_node->next = nullptr; //indicate ladder_node is at end of list
                        ladder_node->prev = closed_set.last; //update ladder_node with who it's behind
                        if(closed_set.last!=nullptr){closed_set.last->next = ladder_node;} //the (former) last item in list now points to node
                        closed_set.last = ladder_node; //the closed_set is updated
                        if(closed_set.first==nullptr){closed_set.first= ladder_node;}; //if this is the first item in closed_set
                        search_main = false;
                        break;
                        
                    }
                    
                    //Calculate The 3 Values: f, g, h
                    ladder_node->g = q->g + 1;
                    ladder_node->h = abs(y2-(ladder_node->y)) + abs(x2-(ladder_node->x)) + abs(z2-(ladder_node->z));
                    ladder_node->f = ladder_node->g + ladder_node->h;
                    
                    //Cycle Through OPEN_SET and check if the ladder_node is already in there
                    bool search_o = true; //search flag
                    bool in_open =  false; //flag indicating if ladder_node is in OPEN_SET
                    current = open_set.first; //start at beginning
                    while(search_o){
                        
                        //Check if at end of list
                        if(current==nullptr){
                            search_o = false;
                            break;
                        }
                        
                        //Check x,y,z values in node
                        if( (current->x== ladder_node->x) && (current->y==ladder_node->y) && (current->z==ladder_node->z) ){
                            in_open = true;
                        }
                        
                        //Move on to search next node in list
                        current = current->next;
                        
                    }
                    if(in_open){
                        delete ladder_node; //delete temporary ladder_node
                        num_news = num_news - 1;
                        continue; //skip this ladder_node
                    }
                    
                    //Cycle Through CLOSED_SET and check if the ladder_node is already in there
                    bool search_c = true; //search flag
                    bool in_closed =  false; //flag indicating if ladder_node is in CLOSED_SET
                    current = closed_set.first; //start at beginning
                    while(search_c){
                        
                        //Check if at end of list
                        if(current==nullptr){
                            search_c = false;
                            break;
                        }
                        
                        //Check x,y values in node
                        if( (current->x==ladder_node->x) && (current->y==ladder_node->y) && (current->z==ladder_node->z)){
                            in_closed = true;
                        }
                        
                        //Move on to search next node in list
                        current = current->next;
                        
                    }
                    if(in_closed){
                        delete ladder_node; //delete what we've created!!!
                        num_news = num_news - 1;
                        continue; //skip this neighbor
                    }
                    
                    //If we've made it to this point, it isn't in CLOSED or OPEN SET
                    //ADD ladder_node to OPEN_SET
                    ladder_node->next = nullptr; //indicate ladder_node is at end of list
                    ladder_node->prev = open_set.last; //update ladder_node who it's behind
                    if(open_set.last!=nullptr){open_set.last->next = ladder_node;} //the (former) last item in list now points to ladder_node
                    open_set.last = ladder_node; //the open_set is updated
                    if(open_set.first==nullptr){open_set.first = ladder_node;}; //if this is the first item in open_set
                    
                    //Must do: Check if the above step is in CLOSED or OPEN Sets. (or if it's target)
                }
                
            }
        }
        ////////////////////////////////////////////////
        //End ladder search
        
        

        
        //And Now done with q and can go on to next step.
        //ADD q to CLOSED_SET
        q->next = nullptr; //indicate q is at end of list
        q->prev = closed_set.last; //update q who it's behind
        if(closed_set.last!=nullptr){closed_set.last->next = q;} //the (former) last item in list now points to neighbor
        closed_set.last= q; //the closed_set is updated
        if(closed_set.first==nullptr){closed_set.first= q;}; //if this is the first item in open_set
        
        //THIS MEANS WE HIT THE END OF OPEN_SET AND HAVENT FOUND TARGET
        //->DELETE EVERYTHING AND RETURN ERROR
        if(open_set.first == nullptr){
            
            
            //DELETIION
            //Delete out the closed and open_sets
            //...the closed set
            bool erasing = true;
            list_node* cur = closed_set.first; //the current node we mpointing at
            while(erasing){
                if(cur==nullptr){
                    erasing = false;
                    break;
                }
                list_node* temp = cur; //Fo deletion
                cur = cur->next;
                delete temp; //Also fo deletion
                num_news = num_news - 1;
            }
            //...the open set
            erasing = true;
            cur = open_set.first;
            while(erasing){
                if(cur==nullptr){
                    erasing = false;
                    break;
                }
                list_node* temp = cur; //fo deletion
                cur = cur->next;
                delete temp; //also fo deletion
                num_news = num_news - 1;
                
            }
            
            printf("A_star_z can't find target...\n");
            return {{9999,9999,0}};
        }
        
        
    }//end open_list empty, time to move on to a new STEP and navigating from there (finding neighbors, searching for ladders, etc.)
    
    
    //CCONSTRUCT VECTOR TO RETURN BY TRAVERSING THE CLOSED LIST
    vector<vector<int>> search_q; //the main "list", could really be turned into what ever datat type you wantx
    vector<int> coord; //a temp coord to add to the search_q.
    list_node* travel = closed_set.last; //temp pointer for traversing the lists
    while(true){
        coord = {travel->x,travel->y,travel->z};
        search_q.push_back(coord);
        travel = travel->parent; //move on to the parent
        if(travel == nullptr){ //if at end of list
            break; //bust outta the while loop
        }
    }
    
    
    //DELETIION
    //Print out the closed and open_sets
    //...the closed set
    bool printing = true;
    list_node* current = closed_set.first;
    while(printing){
        if(current==nullptr){
            printing = false;
            break;
        }
        list_node* temp = current; //Fo deletion
        current = current->next;
        delete temp; //Also fo deletion
        num_news = num_news - 1;
        
    }
    //...the open set
    printing = true;
    current = open_set.first;
    while(printing){
        if(current==nullptr){
            printing = false;
            break;
        }
        list_node* temp = current; //fo deletion
        current = current->next;
        delete temp; //also fo deletion
        num_news = num_news - 1;
        
    }
    
    return search_q;
    
}

//COLOR ALGORITHMS!

//Compute the difference in colors
//the sum of the individual differences in r,g, and b.
int color_diff(SDL_Color col1, SDL_Color col2){
    
    int diff = 0;
    
    diff += abs(col1.r - col2.r) + abs(col1.g - col2.g) + abs(col1.b - col2.b);
    
    return diff;
    
}

//determine the superior color
//For cases where one color has to be matched up against another.
//You find out which one has the greatest ROTATIONAL distance over the other
//
//Ex. For R component
//col1.r is greater!
// Consider col1.r - col2.r = e1 (how much col1 ahead of col2)
//but also col2.r + (256 - col1.r) = e2 (how much col2 ahead of col1)
//But when considering all 3 components, you have to keep track of which color is e1 or e2
//
//Input 2 colors
//Output[0] = how much col1 LEADS col2
//Output[1] = how much col2 LEADS col1
vector<int> color_fight(SDL_Color col1, SDL_Color col2){
    vector<int> lead_values; //2 element array containing how much each color leads the other
    
    int e1 = 0; //how much col1 leads col2
    int e2 = 0; //how much col2 leads col1
    
    //Compute how much col1 LEADS col2
    //R component
    if(col1.r == col2.r){ //components are equal
        e1 = e1;
        e2 = e2;
    }else if(col1.r > col2.r){ //col1.r is greater!
        e1 = e1 + (col1.r - col2.r);
        e2 = e2 + (col2.r + (256 - col1.r));
    }else{ //col2.r is greater
        e2 = e2 + (col2.r - col1.r);
        e1 = e1 + (col1.r + (256 - col2.r));
    }
    //G component
    if(col1.g == col2.g){ //components are equal
        e1 = e1;
        e2 = e2;
    }else if(col1.g > col2.g){ //col1.g is greater!
        e1 = e1 + (col1.g - col2.g);
        e2 = e2 + (col2.g + (256 - col1.g));
    }else{ //col2.g is greater
        e2 = e2 + (col2.g - col1.g);
        e1 = e1 + (col1.g + (256 - col2.g));
    }
    //B component
    if(col1.b == col2.b){ //components are equal
        e1 = e1;
        e2 = e2;
    }else if(col1.b > col2.b){ //col1.b is greater!
        e1 = e1 + (col1.b - col2.b);
        e2 = e2 + (col2.b + (256 - col1.b));
    }else{ //col2.b is greater
        e2 = e2 + (col2.b - col1.b);
        e1 = e1 + (col1.b + (256 - col2.b));
    }
    
    lead_values.push_back(e1);
    lead_values.push_back(e2);

    
    return lead_values;
    
}

//belnd two colors
//blends each color by taking the average of each RGB component
SDL_Color color_blend(SDL_Color col1, SDL_Color col2){
    SDL_Color colF = {static_cast<Uint8>(((col1.r+col2.r)/2)),static_cast<Uint8>(((col1.g+col2.g)/2)),static_cast<Uint8>(((col1.b+col2.b)/2))};
    return colF;
}

//generates a random shade of brown
//Returns a randmo SDL_Color
//First picks a red between 150 & 200
//Then picks a green that is 50 - 100 lower than red
//Then picks a blue that is lower than green.
SDL_Color generate_brown(){
    int r = rand()%50 + 150;
    int g = (r - rand()%50 ) - 50;
    int b = rand()%g;
    
    SDL_Color brown = {static_cast<Uint8>(r),static_cast<Uint8>(g),static_cast<Uint8>(b),255};
    return brown;
}

//generates a random shade of pink
//Returns a random shade of pink
//Choose a red component
//Choose a Blue, less than Red
//Choose a Green, less than Blue
SDL_Color generate_pink(){
    int r = 125+rand()%126;
    int b = rand()%(r+1);
    int g = rand()%(b+1);
    
    SDL_Color pink = {static_cast<Uint8>(r),static_cast<Uint8>(g),static_cast<Uint8>(b),255};
    return pink;
    
}

//generates a random shade of green
SDL_Color generate_green(){
    int g = rand()%255;
    if(g == 0){g++;} //so we don't % by zero
    int r = rand()%g;
    int b = rand()%g;
    
    SDL_Color green = {static_cast<Uint8>(r),static_cast<Uint8>(g),static_cast<Uint8>(b),255};
    return green;
    
}

////given n input color, convert it to a pastel version (increase the lightness)
////This can be used to simulate metallic shiny color (think rose gold, silver gold, etc.)
////This new color is above 180 in all components.
////The original color is mapped onto the last 55 values.
//SDL_Color color_to_pastel(SDL_Color in_col){
//    
//    Uint8 r = 180;
//    Uint8 g = 180;
//    Uint8 b = 180;
//    
//    //mapping on to last 55 of the color value
//    r = r + ((75.0/255)*in_col.r);
//    g = g + ((75.0/255)*in_col.g);
//    b = b + ((75.0/255)*in_col.b);
//    
//    SDL_Color pastel = {r,g,b,255};
//    return pastel;
//}

//given n input color, convert it to a pastel version (increase the lightness)
//This can be used to simulate metallic shiny color (think rose gold, silver gold, etc.)
//This new color is above whatever the input low_gray is.
//The original color is mapped onto the last values leading up to 255
SDL_Color color_to_pastel(SDL_Color in_col, SDL_Color low_gray){
    
    //Think, "It's at the very least, these values."
    Uint8 r = low_gray.r;
    Uint8 g = low_gray.g;
    Uint8 b = low_gray.b;
    
    //mapping on to the last values of the 255 scale
    r = r + ( ( (255.0-low_gray.r) / 255) * in_col.r );
    g = g + ( ( (255.0-low_gray.g) / 255) * in_col.g );
    b = b + ( ( (255.0-low_gray.b) / 255) * in_col.b );
    
    SDL_Color pastel = {r,g,b,255};
    return pastel;
}

//takes a color and gradually changes it along the redish/orange spectrum
//Takes a redHue as input to use as a jumping off point
//randomly adjust the r,g,b channels
//Red needs to be higher than green by at least 50
//Green has to be above 75
//Blue can be anything below 125
SDL_Color redNoise(SDL_Color redHue){
    
    SDL_Color temp_red = redHue;
    
    //if current_red is out of the spectrum, initialize at a default red_shade
    if(redHue.g < 75 || redHue.b > 125 || redHue.r < redHue.g+50){
        printf("adjusted red. \n");
        temp_red = {200,76,91,255 } ;
    }
    
    //Now start manipulating it
    int r_inc = rand() % 2;
    int g_inc = rand() % 2;
    int b_inc = rand() % 2;
    int change = 1 + (-2 * (rand()%2) ); //either 1 or -1
    
    temp_red.r = temp_red.r + (change * r_inc);
    change = 1 + (-2 * (rand()%2) ); //either 1 or -1
    temp_red.g = temp_red.g + (change * g_inc);
    change = 1 + (-2 * (rand()%2) ); //either 1 or -1
    temp_red.r = temp_red.b + (change * b_inc);
    
    //bounds checking
    if(temp_red.g < 75){
        temp_red.g = 75;
    }
    if(temp_red.g > 205){
        temp_red.g = 205;
    }
    if(temp_red.b < 0){
        temp_red.b = 0;
    }
    if(temp_red.b > 125){
        temp_red.b = 125;
    }
    if(temp_red.r < temp_red.g + 50){
        temp_red.r = temp_red.g + 50;
    }
    if(temp_red.r > 255){
        temp_red.r = 255;
    }
    
    return temp_red;
    
}

//take a color and gradually (slightly) change the RGB values
//input the plus or minus variance value
SDL_Color warpColor(SDL_Color inputHue, int variance){
    
    int r,g,b; //temp vars for the color values
    
    r = inputHue.r + rand()%(2*variance) - (variance); //2 to -2
    g = inputHue.g + rand()%(2*variance) - (variance); //2 to -2
    b = inputHue.b + rand()%(2*variance) - (variance); //2 to -2
    
    //Bounds check (and set)
    
    //Bounds checks
    if(r > 255) r = 255;
    if(g > 255) g = 255;
    if(b > 255) b = 255;
    if(r < 0) r = 0;
    if(g < 0) g = 0;
    if(b < 0) b = 0;
    
    SDL_Color outputHue = {static_cast<Uint8>(r),static_cast<Uint8>(g),static_cast<Uint8>(b),255};
    return outputHue;
    
}














