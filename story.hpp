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
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

using namespace std;

//GEN HISTORY ALGORITHMS
string genName(); //generates a random name
string giveMonicker(string name); //Adds a post monicker to a name
string genPrayer(); //generates a random prayer to a random saint/demon

string genLatinWord(); //generates a random latin word
string genLatinSentence(); //generates a random latin sentence
string genLatinSentence(int punctuation); //generates random latin sentence, allows you to specify which punctuation index to use

string genCuss(); //generates a random cuss

string roman_numerals(int number); //returns the roman numeral equivalent of an entered value

//LAWS

//Article struct - most basic unit of law
struct article {
    string author;
    string text;
    int section_index; //what section it's in (bookkeeping)
    int article_index; //what article number it is (bookkeeping)
    //if it references another article
    int ref_section = NULL;
    int ref_article = NULL;
    
    article(): author(), text(), section_index(), article_index(), ref_section(), ref_article() {}
    article(string author, string text, int sect_id, int art_id, int ref_sect, int ref_art) : author(author), text(text), section_index(sect_id), article_index(art_id), ref_section(ref_sect), ref_article(ref_art) {}; 
};

//Law class contains sections of articles
//provides methods for writing and revising
class Law{
public:
    Law(string author); //construct new Law
    vector<vector<article>> articles; //a grid of articles: articles[section_id][article_id]
    string author; //the originial author of the law
    void addRandArticle(string author); //adds a random article by author
    void printLaw(); //prints itself out
private:

};

//Magic
vector<int> return_fave_rgb(); //returns the rgb value of a random ravorite color
string rgb_to_trigram(int r, int g, int b); //return the equivalent I ching trigram of the color
string genPotionName(); //generates random name of a potion
string genOilName(); //Generates -ine name. These can be the names of the active ingrediant in the above potions
string genSaltName(); //Generates "-ium -ide" name, These can be the names of the active ingrediant in the above powders
string genGlassName(); //Generates a glass name //either a (Pyr)-ex name or a (boro)-silicate name
string genClothName(); //Generates a cloth name // consonant - vowel - consonant - consonant
int find_compatibility(int z1, int z2); //returns the compatibility of the entered zodiac sign codes: 0 - Good, 1 - Fine, 2 - Bad
int find_zodiac_element(int z); //returns the element of the entered zodiac sign code: 1: Air, 2: Fire, 3: Water, 4:Earth

//HOTEL
string genWine(); //Generates a wine name with a saint, vintage year, sweetness level, and fruit/aroma taste
string genWineName(); //Only generates the wine name (saint, vintage year)
string genWineTaste(); //Generates the taste of the wine - sweetness level and fruit/aroma tastes

string genBerryName(); //Gen name of berries //Ex. Lychee, fofroo

#endif /* story_hpp */
