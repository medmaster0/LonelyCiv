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

using namespace std;

//GEN HISTORY ALGORITHMS
string genName(); //generates a random name
string giveMonickerPost(string name); //Adds a post monicker to a name
string genPrayer(); //generates a random prayer to a random saint/demon

string genLatinWord(); //generates a random latin word
string genLatinSentence(); //generates a random latin sentence
string genLatinSentence(int punctuation); //generates random latin sentence, allows you to specify which punctuation index to use

string genCuss(); //generates a random cuss

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

#endif /* story_hpp */
