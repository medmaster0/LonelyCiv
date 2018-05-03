//
//  recipes.hpp
//  Civ2
//
//  Created by Medz on 4/19/18.
//  Copyright © 2018 Medz. All rights reserved.
//
//Recipes and Resources
//
//These are more of a story element *attached* to the Item sprites
//
//* We give magical names to specific colors of items -
//  e.g. rgb(244,32,45) stones are made of "fofium hydoxide" and can be made into Qyrex glass
//
//* We also need to be able to combine these primitive named RESOURCES into more elaborate RECIPES.
//  e.g. Recipe for the Potion of Wisconsin Darkening of the Light, made of "kexonine" and in a "Qyrex" vial
//
// Types of Recipes and Resources will need a new coding system SEPARATE from the Item coding system...
// These Recipes and Resources will be integrated into each other, therefore...
// RECIPES and RESOURCES share the same coding table

#ifndef recipes_hpp
#define recipes_hpp

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
// 2 - wines (Recipe) made of 4  and 3
// 3 - writing (Recipe) made out of 3 and 4

//Resources Class
//These are for recipes made directly from raw resources
//They are named independently - i.e. they don't depend on other resource/recipe names
class Resource{
public:
    Resource(int recipe_type); //construct new resource of type accordng to input Recipe Code
    int resource_type; //a code indicating the type of resource in the context of recipes (powder OR glass? one or the other, both made from stone)
    int item_type; //the underlying Item type this resource comes from. e.g. Powder and Glass both have the same item type (stone)
    //int sub_type; //a more detailed description code of item -> e.g. a love potion or a health potion?
    SDL_Color colorPrim; //the Prim color of this resource
    SDL_Color colorSeco; //the Seco color of this resource (optional, defaults to {0,0,0,255})
    string name; //The magical name of this resource.
};

//Recipe class contains a collection of items and descriptions for products that can be made in rituals.
//Recipes are made up of Resources,
//The Recipe's properties derived from it's component Resources
class Recipe{
public:
    Recipe(int in_recipe_type, Resource ing1, Resource ing2); //construct new recipe
    int recipe_type; //a code indicating the product of recipe
    int item_type; //the underlying Item type of this recipe's PRODUCT
    int sub_type; //more detailed type of item -> e.g. a love potion or a health potion?
    Resource ingredient1; //the first resource ingredient
    Resource ingredient2; //the second resource ingredient (optional)
//    vector<int> ingredients; //a list of ints (item type codes) indicating what kind of ingredients are needed to make
//    vector<SDL_Color> colors; //a list of required (ideal) colors needed to make the recipe.
    string name; //the magical name of the recipe
    string description; //a string describing the item. generated off of Resource properties
    
};

#endif /* recipes_hpp */
