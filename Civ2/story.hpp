//
//  story.hpp
//  Civ2
//
//  Created by Medz on 11/19/17.
//  Copyright © 2017 Medz. All rights reserved.
//

#ifndef story_hpp
#define story_hpp

#include <stdio.h>
#include <string>

using namespace std;

//GEN HISTORY ALGORITHMS
string genName(); //generates a random name
string giveMonickerPost(string name); //Adds a post monicker to a name
string genPrayer(); //generates a random prayer to a random saint/demon

string genLatinWord(); //generates a random latin word
string genLatinSentence(); //generates a random latin sentence

string genCuss(); //generates a random cuss

#endif /* story_hpp */
