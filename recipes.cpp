//
//  recipes.cpp
//  Civ2
//
//  Created by Medz on 4/19/18.
//  Copyright © 2018 Medz. All rights reserved.
//

#include "recipes.hpp"
#include "story.hpp"

#include <stdio.h>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

using namespace std;

//Recipe Codes:
//NOT ALL ARE IMPLEMENETED YET!!!
// 0 - glasses
// 1 - salts
// 2 - oils
// 3 - wines (Recipe)
// 4 - cloths
// 5 - potions (Recipe)
// 6 - powders (Recipe)
// 7 - berries

//Resources Class
//These are for recipes made directly from raw resources
//They are named independently - i.e. they don't depend on other resource/recipe names

//This constructor fills out the Object's fields
//It assigns names according to the recipe types
Resource::Resource(int recipe_type_in){
    recipe_type = recipe_type_in;
    //Based on recipe_type, determine names, item types, etcs
    switch (recipe_type) {
        case 0: //glasses
            item_type = 312; //glasses are made of stone: Item tile code 312
            name = genGlassName(); //assign the glass a name
            colorPrim = {static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255)}; //assign a single random color
            colorSeco = {0,0,0,255}; //black
            break;
        case 1: //salts
            item_type = 312; //salts are made of stone: Item tile code 312
            name = genSaltName(); //assign the salt a name
            colorPrim = {static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255)}; //assign a single random color
            colorSeco = {0,0,0,255}; //black
            break;
        case 2: //oils
            item_type = 310; //oils are made of herbs; Item tile code 310
            name = genOilName(); //assign the oil a name
            colorPrim = {static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255)}; //assign a single random color
            colorSeco = {0,0,0,255}; //black
            break;
        case 3: //cloths
            item_type = 310; //cloths are made from herb: Item tile code 310
            name = genClothName(); //assign the salt a name
            colorPrim = {static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255)}; //assign a single random color
            colorSeco = {0,0,0,255}; //black
            break;
            
        default:
            break;
    }
    
}
