//
//  dancez.hpp
//  Civ2
//
//  Created by Medz on 8/16/17.
//  Copyright © 2017 Medz. All rights reserved.
//

#ifndef dancez_hpp
#define dancez_hpp

#include <stdio.h>
#include "dancez.hpp"

#endif /* dancez_hpp */
//#include <iostream>
#include <vector>

using namespace std;

std::vector<std::vector<int>> generateChaCha() ; //ChaCha

//DANCE TYPES:
// 0 - CIRCLE DANCE (CCW x 3)
// 1 - CHA-CHA
vector<vector <int>> dancePathGen(int refx, int refy, int refz, int type); //generates a list of steps to take for a dance
