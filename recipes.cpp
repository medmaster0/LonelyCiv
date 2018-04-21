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

//Resource Codes:
//NOT ALL ARE IMPLEMENETED YET!!!
// 0 - glasses
// 1 - salts
// 2 - oils
// 3 - cloths
// 4 - berries

//Recipe Codes:
//NOT ALL ARE IMPLEMENTED YET!!!
// 0 - potions (Recipe) made of 2 and 0
// 1 - powders (Recipe) made of 1 and 3
// 2 - wines (Recipe) made of 4 and 3

//Resources Class
//These are for recipes made directly from raw resources
//They are named independently - i.e. they don't depend on other resource/recipe names

//This constructor fills out the Object's fields
//It assigns names according to the recipe types
Resource::Resource(int resource_type_in){
    resource_type = resource_type_in;
    //Based on recipe_type, determine names, item types, etcs
    switch (resource_type) {
        case 0: //glasses
            item_type = 312; //glasses are made of stone: Item tile code 312
            name = genGlassName(); //assign the glass a name
            colorPrim = {static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),255}; //assign a single random color
            colorSeco = {0,0,0,255}; //black
            break;
        case 1: //salts
            item_type = 312; //salts are made of stone: Item tile code 312
            name = genSaltName(); //assign the salt a name
            colorPrim = {static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),255}; //assign a single random color
            colorSeco = {0,0,0,255}; //black
            break;
        case 2: //oils
            item_type = 310; //oils are made of herbs; Item tile code 310
            name = genOilName(); //assign the oil a name
            colorPrim = {static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),255}; //assign a single random color
            colorSeco = {0,0,0,255}; //black
            break;
        case 3: //cloths
            item_type = 310; //cloths are made from herb: Item tile code 310
            name = genClothName(); //assign the salt a name
            colorPrim = {static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),255}; //assign a single random color
            colorSeco = {0,0,0,255}; //black
            break;
            
        case 4: //berries
            item_type = 311; //cloths are made from herb: Item tile code 310
            name = genBerryName(); //assign the salt a name
            colorPrim = {static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),static_cast<Uint8>(rand() %255),255}; //assign a single random color
            colorSeco = {0,0,0,255}; //black
            break;
            
        default:
            break;
    }
}

//RECIPE CLASS
//Recipe class contains a collection of items and descriptions for products that can be made in rituals.
//Recipes are made up of Resources,
//The Recipe's properties derived from it's component Resources
Recipe::Recipe(int in_recipe_type, Resource ing1, Resource ing2) : ingredient1(0), ingredient2(1){
    recipe_type = in_recipe_type;
    ingredient1 = ing1;
    ingredient2 = ing2;
    //Now process the recipe_type-specific properties
    switch (recipe_type) {
        case 0: //For potions
            item_type = 314;
            name = genPotionName();
            description = "This is the potion, " + name + ". It is made of " + ing1.name + ". It comes in a " + ing2.name + " glass vial.";
            break;
        case 1: //For powders
            item_type = 313; 
            name = genPotionName();
            description = "This is the powder, " + name + ". It is made of " + ing1.name + ". It comes in a " + ing2.name + " cloth bag.";
            break;
        case 2: //For wines
            item_type = 316; 
            name = genWineName();
            description = "This is the wine, " + name + ". " + genWineTaste(); 
            break;
        
        default:
            break;
    }
    
    
}
