//
//  story.cpp
//  Civ2
//
//  Created by Medz on 11/19/17.
//  Copyright © 2017 Medz. All rights reserved.
//

#include "story.hpp"
#include <math.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>

 #include<fstream>

using namespace std;

/////
//TODO:

/////////////////
//COW LATIN
//the syllables
vector<string> latin_syls = {"est","ius","onos","el","ia","ium","eri","itus","us","nius","tum","um","en","eno","er","era","a","o","u","y"};
//the consonants
vector<string> latin_cons = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","pl","th","ca","pa"};
//the end punctuation
vector<string> punctuation_end = {".","?","!"};

//generates a random latin word
string genLatinWord(){
    
    int syls = (rand()%5) + 1; //The number of syllables the word will have
    
    string word = ""; //the word we will be adding to
    for(int i = 0 ; i < syls; i ++){
        if(rand()%3 == 1){
            word += latin_cons[rand()%latin_cons.size()];
        }
        word += latin_syls[rand()%latin_syls.size()];
    }
    
    return word;
}

//generates a random latin sentence
string genLatinSentence(){
    
    int words = (rand()%9) + 1;
    
    string sentence = ""; //the sentence we will be returning
    for(int i = 0; i < words; i++){
        
        sentence += " " + genLatinWord();
        
        
    }
    sentence += punctuation_end[rand()%punctuation_end.size()];
    sentence[1] = toupper(sentence[1]);
    
    return sentence;
    
}
//generates random latin sentence, allows you to specify which punctuation index to use
string genLatinSentence(int punctuation){
    
    int words = (rand()%9) + 1;
    
    string sentence = ""; //the sentence we will be returning
    for(int i = 0; i < words; i++){
        
        sentence += " " + genLatinWord();
        
        
    }
    sentence += punctuation_end[punctuation];
    sentence[1] = toupper(sentence[1]);
    
    return sentence;
    
}

vector<string> cuss_symbols = {"!","@","#","$","%","^","&","*","z","x","0"};

//generates a radom cuss
string genCuss(){
    int letters = (rand()%10)+1;
    
    string cuss = "";
    for(int i = 0 ; i < letters ; i++){
        cuss += cuss_symbols[rand()%cuss_symbols.size()];
    }
        
    return cuss;
    
}

vector<string> written_by_syns = {"written by", "according to", "as proclaimed by", "in the words of"}; //synynoms for "written by" - to spruce things ups

//returns the roman numeral equivalent of an entered value
//TODO Extend ROMAN NUMERAL system beyond the digit M...
// I,
// V, X
// L, C
// D, M
//New Extended System: J, W, Y, K, B, E, N (letter afterwards (unless taken, then you go backwards (L->M->K instead) )
string roman_numerals(int number){
    string roman_numerals = "";
    
    //MAGNITUDE: 1000's
    //DETERMINE M's (Simple, Inelegant... TODO: Come up with further ROMAN NUMERALS)
    int num_M = number / 1000;
    number = number - (num_M * 1000);
    for(int i = 0; i<num_M; i++){
        roman_numerals.append("M");
    }
    
    //MAGNITUDE: 100's
    //DETERMINE C's and D's (maybe CM/CD)
    int num_C = number / 100;
    number = number - (num_C * 100);
    //If 9, put CM
    if(num_C == 9){
        roman_numerals.append("CM");
        num_C = 0; //set to 0, so we don't add any more in this 10's magnitude
    }
    //Determine if need D ( > 5 )
    if(num_C >= 5){
        roman_numerals.append("D");
        num_C = num_C - 5;
    }
    //If 4, put CD
    if(num_C == 4){
        roman_numerals.append("CD");
        num_C = 0;
    }
    //Put remaining C's
    for(int i = 0; i<num_C; i++){
        roman_numerals.append("C");
    }
    
    //MAGNITUDE: 10's
    //DETERMINE X's and L's (maybe XC/XL)
    int num_X = number / 10;
    number = number - (num_X * 10);
    //If 9, put XC
    if(num_X == 9){
        roman_numerals.append("XC");
        num_X = 0; //set to 0, so we don't add any more in this 10's magnitude
    }
    //Determine if need L ( > 5 )
    if(num_X >= 5){
        roman_numerals.append("L");
        num_X = num_X - 5;
    }
    //If 4, put XL
    if(num_X == 4){
        roman_numerals.append("XL");
        num_X = 0;
    }
    //Put remaining X's
    for(int i = 0; i<num_X; i++){
        roman_numerals.append("X");
    }
    
    //MAGNITUDE: 1's
    //DETERMINE I's and V's (maybe IX/IV)
    int num_I = number / 1;
    number = number - (num_I * 1);
    //If 9, put IX
    if(num_I == 9){
        roman_numerals.append("IX");
        num_I = 0; //set to 0, so we don't add any more in this 10's magnitude
    }
    //Determine if need V ( > 5 )
    if(num_I >= 5){
        roman_numerals.append("V");
        num_I = num_I - 5;
    }
    //If 4, put IV
    if(num_I == 4){
        roman_numerals.append("IV");
        num_I = 0;
    }
    //Put remaining I's
    for(int i = 0; i<num_I; i++){
        roman_numerals.append("I");
    }
    
    return roman_numerals;
}



/////////////////
//LAW CLASS
//Constructor
Law::Law(string in_author){
    //constructor stuff go here
    
    author = in_author; //original author of the LAW
    
    //create the first article (section 0, article 0)
    article temp_art = {author, genLatinSentence(0), 0,0, NULL, NULL };
    
    vector<article> temp_sect; //a temp section to add to article
    temp_sect.push_back(temp_art); //add article to section
    articles.push_back(temp_sect); //add section to law
    
}
//prints out the law
void Law::printLaw(){
    
    //preamble
    //
    cout << "This is the Law according to " << author << "\n";
    cout << "---------------------------------------\n\n"; 
    
    //print out articles
    for(int i = 0 ; i < articles.size(); i++){
        for(int j = 0 ; j < articles[i].size(); j++){
            cout << "Section " << i << ", Article " << j << " according to " << articles[i][j].author << ":\n\n\t";
            cout << articles[i][j].text;
            //if the article references another
            if(articles[i][j].ref_section!=NULL){
                cout << " ( In reference to §" << articles[i][j].ref_section << "." << articles[i][j].ref_article  << " )";
            }
            cout << "\n\n";
        }
    }
}
//add random article to law (either in new section or existing)
void Law::addRandArticle(string author){
 
    int temp_ref_sect = NULL;
    int temp_ref_art = NULL;
    //decide if we reference previous article
    if(rand()%6==1){
        
        //choose a random existing section and article
        temp_ref_sect = rand()%articles.size();
        temp_ref_art = rand()%articles[temp_ref_sect].size();
        
    }
    
    //decide if we add to existing section or new section
    if(rand()%3 == 1){
        //new section
        vector<article> temp_sect;
        
        //new article
        article temp_art = {author, genLatinSentence(0), static_cast<int>( articles.size() ), 0, temp_ref_sect, temp_ref_art };
        
        temp_sect.push_back(temp_art);
        articles.push_back(temp_sect);
        
        
    }else{
        //add to existing section
        int sect_id = rand()%articles.size();
        
        //new article
        article temp_art = {author, genLatinSentence(0), sect_id, static_cast<int>(articles[sect_id].size()) , temp_ref_sect, temp_ref_art};
        
        articles[sect_id].push_back(temp_art);
        
    }
    
    
}


////////////////
//HISTORY
//
//
//
//

//generates a randmo name
//For now, 3 - syllables
string genName(){
    
    char consonants[] = "bcdfghjklmnpqrstvwxyz";
    char vowels[] = "aeiou";
    
    char c; //temp char for string building
    string name1; //the name we constructing
    
    //Add first letter, consonant
    c = consonants[rand()%21];
    name1 += toupper(c); //capitalize first letter
    //Add second letter, vowel
    c = vowels[rand()%5];
    name1 += c;
    //Add third letter, consonant
    c = consonants[rand()%21];
    name1 += c;
    name1 += c; //fourth is same
    //Add fifth letter, vowel
    c = vowels[rand()%5];
    name1 += c;
    
    
    
    return name1;
    
    
}

vector<string> street_monickers_post = {"Dopest","Dope","Baddest","Bad","Slickest","Slick","Mostest","Rad","Clown","Killa","Slizza","Blizza","Snow","Product","Biggie","Down","Chiller","Bomb","Bombest","Funny","Punk","Chill","Junkhead","Cracker","Lowlife","Thug","Thuggin","Pimpin","Chief","Pill","Rocker","Baller","Insane","Moco","Snoop","JoJo","Fly","Real Deal","Peep","Smalls", "Illest","Dude","Duderino","Baby","Vato","Joker","Homie","Flow","Slug"};

vector<string> street_monickers_pre = {"Mista", "Lil", "Supa", "Fitty", "Champ", "Kid", "Wiz", "Babyface","Cousin"};

//Adds a post monicker to a name
//Ex. Jugga the Fly
string giveMonicker(string name){
    
    string full_name = ""; //the string we will return
    
//    //Give a pre monicker
//    if(rand()%10 == 1){
//        full_name = full_name + street_monickers_pre[rand()%street_monickers_pre.size()] + " ";
//    }
//
//    //The original Name
//    full_name += name;
//
//    //Give a post monicker
//    if(rand()%7 == 1){
//        full_name += " the " + street_monickers_post[rand()%street_monickers_post.size()];
//    }
    
    //Give a pre monicker
    if(rand()%10 >7 ){
        full_name = full_name + street_monickers_pre[rand()%street_monickers_pre.size()] + " ";
        full_name += name;
    }

    //Give a post monicker
    else{
        full_name += name;
        full_name += " the " + street_monickers_post[rand()%street_monickers_post.size()];
    }
    
    return full_name;
    
}

vector<string> saints = {"Saint Aaron of Brettany","Saint Aaron of Caerleon","Blessed Aaron of Cracow","Saint Abachum of Persia","Saint Attracta of Killaraght","Saint Anthony the Abbot","Saint Cyrus the Physician","Saint Abban the Hermit","Saint Abban","Saint Abbán of Magh-Armuidhe","Saint Abban of New Ross","Saint Abban of Kill-Abban","Saint Goeric of Metz","Saint Abbo of Auxerre","Saint Abbo of Fleury","Saint Elpidius the Cappadocian","Blessed Henry Abbot","Saint Adheritus","Saint Abdiesus the Deacon","Saint Abdon","Saint Abel of Rheims","Saint Abel McAedh","Abel the Patriarch","Blessed Thomas Abel","Blessed Andrew Abellon","Blessed Nathalan of Aberdeen","Saint Abercius Marcellus","Saint Mazota of Abernethy","Saint Abibas","Saint Joseph Abibos","Saint Abibus of Samosata","Abigail the Matriarch","Saint Abilius of Alexandria","Saint Abo of Tblisi","Saint Abundius the Sacristan","Saint Abundius of Como","Saint Abra","Saint Abraham of Harran","Saint Abraham of Abela","Saint Abraham of Cyriacus","Saint Abraham of Ephesus","Saint Abraham of Persia","Saint Abraham Kidunaia","Saint Abraham of Kratia","Saint Abraham of Rostov","Saint Abraham of Smolensk",
    "Blessed Abraham of Strelna","Saint Abraham the Poor","Abraham the Patriarch","Abrahamite Monks","Saint Abrosimus","Saint Absolon of Caesarea","Saint Peter Absalon","Saint Frumentius of Ethiopia","Saint Abundantius of Rome","Saint Abundantius of Africa","Saint Abundius of Ananelos","Saint Abundius the Martyr","Saint Abundius the Priest","Saint Abundius","Saint Abundius of Spain","Saint Abundius of Rome","Saint Acacius Agathangelos of Melitene","Saint Acacius of Amida","Saint Acasius of Prusa","Saint Acacius of Byzantium","Saint Acacius the Martyr","Saint Acacius of Miletus","Saint Acacius of Sebaste","Blessed Marie of the Incarnation","Saint Acca of Hexham","Saint Aceolus of Amiens","Saint Aceptismas of Hnaita","Saint Acepsimas","Saint Acestes","Saint Aichardus","Saint Acharius of Tournai","Saint Achilleus of Terracina","Saint Achileo Kiwanuka","Saint Achillas of Sketis","Saint Achillas","Saint Bernard of Lérida","Saint Ahmed the Calligrapher","Saint Acius of Amiens","Saint Flavian of Acquapendente","Blessed Rudolph Aquaviva","Blessed Guido of Acqui","Saint Actinea of Volterra","Saint Accursies of Morocco","Saint Acutius of Pozzuoli","Saint Acutus of Syrmium","Saint Ada of Le Mans","Saint Adelard of Corbie","Saint Adalard the Younger","Saint Adalbald of Ostrevant","Saint Adalbero of Augsburg",
    "Blessed Adalbero of Lambach","Saint Adalbero of Metz","Blessed Adalbero of Liège","Blessed Adalbert Nierychlewski","Saint Adalbert of Casauria","Saint Adalbert of Egmond","Saint Adalbert of Magdeburg","Saint Adalbert of Prague","Saint Adaler of Erfurt","Saint Alferius of La Cava","Saint Adalgis of Thiérarche","Saint Adalgott of Chur","Saint Aldegrin of Baume","Saint Adalric of Bèze","Saint Adalsindis of Bèze","Saint Adalsindis of Hamay","Blessed Adam of San Sabine","Blessed Adam Bargielski","Saint Albert Chmielowski","Blessed Adam of Saxony","Adam the Patriarch","Saint Roman Adame Rosales","Saint Adamnan of Iona","Blessed Adamnan of Coldingham","Blessed John Adams","Saint Adauctus of Rome","Saint Adauctus","Saint Adaucus of Phrygia","Saint Addai","Saint Adela","Blessed Maria Stella of the Most Blessed Sacrament","Saint Adela of Messines","Saint Adela of Pfalzel","Saint Adelaide of Burgundy","Saint Adelaide of Guelders","Saint Aleydis of Schaerbeek","Saint Adelelmus","Saint Aldegundis","Blessed Adelheidis of Tennenbach","Saint Hadelin of Sees","Saint Adelina","Saint Adeloga of Kitzingen","Saint Deodatus of Nevers","Pope Saint Adeodatus I","Saint Aldhelm of Sherborne","Blessed Adilio Daronch","Saint Adjutor of Vernon","Saint Adjutor of Africa","Saint Ado of Vienne","Saint Adolofu Mukasa Ludigo","Saint Adolphus of Osnabrück","Saint Aniceto Adolfo","Blessed Adolph Kolping","Saint Adolphus of Cordoba","Saint Adrian of Numidia","Saint Adrian of Caesarea","Saint Fricor","Pope Saint Adrian III","Blessed Adrian Fortescue","Saint Adrian of Canterbury","Blessed Adrian of Dalmatia","Saint Adrian of Maastricht","Saint Adrian of May","Saint Adrian of Nicomedia","Venerable Adriano Osmolowski","Saint Mavilus of Adrumetum","Saint Adventor of Turin","Saint Ebbe the Elder","Saint Ebbe of Minster-in-Thanet","Saint Ebbe the Younger","Saint William of Eskilsoe","Saint Aedan of Ferns","Saint Aedesius of Alexandria","Saint Aedh Dubh","Saint Aedh mac Bricc","Saint Macartan of Clogher","Saint Aedh McLugack","Saint Ethelwald of Lindisfarne","Saint Ethelbert of Kent","Saint Edwin of Northumbria","Saint Aegidius","Saint Giles Mary-of-Saint-Joseph","Blessed Giles of Assisi","Saint Eigil of Fulda","Saint Athanasia of Aegina","Saint Mary of Egypt",
    "Saint Alphege the Bald","Saint Alfric of Canterbury","Saint Aelia Flaccilla","Saint Aelred of Rievaulx","Saint Aemilianus the Martyr","Saint Emilian of Saujon","Saint Macanisius","Saint Ethelhard of Canterbury","Saint Afan of Wales","Saint Dafrosa of Acquapendente","Saint Afra of Augsburg","Saint Donatian of Africa","Saint Eugenius of Africa","Saint Ignatius of Africa","Saint Longinus of Africa","Martyrs in Africa","Saint Moses of Africa","Saint Vindemialis of Africa","Saint Gaudiosus of Naples","Saint Africanus of Carthage","Saint Agabus the Prophet","Saint Aganus of Airola","Saint Agape the Martyr","Saint Agape of Thessalonica","Saint Agapes of Nicomedia","Pope Saint Agapitus I","Saint Agapitus the Martyr","Saint Agapitus the Deacon","Saint Agapius of Novara","Saint Agapitus of Rome","Saint Agapitus of Synnada","Saint Agapius of Cirta","Saint Agapius of Caesarea","Saint Agapius of Caesarea","Saint Agatha Chon Kyong-Hyob","Saint Agatha of Wimborne","Saint Agatha Chon Kyong-Hyob","Saint Agatha Hildegard of Carinthia","Saint Agatha Kim A-Gi","Saint Agatha Kwon Chin-I","Saint Agatha Lin","Saint Agatha Yi Kan-Nan","Saint Agatha Yi Kyong-I","Saint Agatha Yi So-Sa","Blessed Agathangelus Nourry","Saint Agathangelus","Pope Saint Agatho","Saint Agatho of Sicily","Saint Agathoclia","Saint Agathopus the Deacon","Saint Agathopus of Crete","Saint Victor of Agaunum","Saint Vitalis of Agaunum","Saint Ageranus of Bèze","Saint Agericus of Verdun","Saint Aggaeus the Martyr","Haggai the Prophet","Saint Aggai","Saint Philip of Agira","Saint Agia","Saint Agia of Hainault","Saint Agigulf","Saint Agilbert of Paris","Saint Agileus of Carthage","Saint Agilus","Venerable Agnelo Gustavo Adolfo de Souza","Saint Agnellus of Naples","Saint Agnellus of Naples","Blessed Agnellus of Pisa","Saint Rafqa","Saint Agnes De","Blessed Agnès of Jesus","Saint Teresa of Calcutta","Saint Agnes of Assisi","Saint Agnes of Prague","Saint Agnes of Montepulciano","Saint Agnes of Poitiers","Saint Agnes of Rome","Blessed Ines de Beniganim","Saint Agnofleta","Saint Agobard of Lyon","Saint Agofredus","Blessed Clare Agolanti of Rimini","Saint Agostina Petrantoni","Blessed Augustine Novello","Saint Agostino Roscelli","Blessed Zefirino Agostini","Saint Agrecius of Trier","Saint Agricola of Châlon-sur-Saône","Saint Agrepe","Saint Agricola of Bologna","Saint Agricola of Pannonia","Saint Agricola of Ravenna","Saint Agricola of Avignon","Saint Agrippina of Rome","Saint Agrippinus of Autun","Saint Agrippinus of Naples","Saint Agrippinus of Pannonia","Saint Agustin Caloca Cortes","Saint Aidan of Lindesfarne","Saint Aiden","Saint Aigulf of Bourges","Saint Aigulf","Saint Aigulphus of Lérins","Saint Ailbe of Emly","Saint Aileran of Clonard","Saint Aimo of Savigny","Blessed Aimo Taparelli","Saint Apollinaris of Valence","Blessed Airaldus of Maurienne","Saint Basil of Aix","Saint Alexander Akimetes","Saint Marcellus the Righteous","Saint Mary of Jesus Crucified","Saint Alanus of Lavaur","Saint Margaret Mary Alacoque","Blessed Alacrinus of Casamari","Saint Albaud of Toul","Blessed Alan de Solminihac","Blessed Theobald Roggeri","Saint Alban Bartholomew Roe","Saint Alban of Mainz","Blessed Henry of Albano","Blessed Matthew of Albano","Blessed Alvarez of Cordova","Saint Albian of Albee","Blessed Nicholas Albergati","Saint Alberic of Citeaux","Saint Alberic of Utrecht","Saint Alberic Crescitelli","Blessed Alberic of Gladbach","Blessed Giacomo Alberione","Blessed Maria Pilar Izquierdo Albero","Saint Albert of Jerusalem","Saint Albert Quadrelli","Blessed Albert of Bergamo","Blessed Albert of Bologna","Saint Albert of Butrio","Saint Albert of Cashel","Saint Albert of Como","Saint Albert of Gambron","Blessed Albert of Haigerloch","Saint Albert of Louvain","Saint Albert of Montecorvino","Saint Erhard of Regensburg","Saint Albert of Sicily","Blessed Frederick Albert","Blessed Albertina Berkenbrock","Blessed Albertinus of Fonte Avellana","Venerable Alberto Capellán Zuazo","Saint Alberto Hurtado Cruchaga","Blessed Alberto Marvelli","Blessed Louise degli Albertoni","Saint Albert the Great","Saint Carissima of Albi","Saint Salvius of Albi","Saint Albina of Caesarea","Saint Albinus of Angers","Saint Albinus of Angers","Blessed Magdalen Albrizzi","Saint Alburga of Wilton","Saint Avitus of Vienne","Saint Alcmund of Hexham","Saint Juan Alcober Figuera","Blessed Alcuin","Blessed Alda of Siena","Saint Aldate of Gloucester","Saint Aldebrandus of Fossombrone","Saint Aldemar the Wise","Saint Aldric of Le Mans","Saint Ouen of Rouen","Saint Aldwine of Peartney","Saint Almedha","Saint Aleth of Dijon","Blessed Aleksy Sobaszek","Saint Alena","Blessed Alericus","Venerable Alessandro Luzzago","Saint Alexis Falconieri","Saint Enogatus of Aleth","Saint Alexander of Goman","Saint Alexander of Edessa","Saint Alexander of Rome","Saint Alexander of Alexandria","Saint Alexander of Apema","Saint Alexander","Saint Alexander of Vienne","Saint Alexander of Fiesole","Saint Alexander of Perga","Saint Alexander of Sabine","Saint Alexander of Trier","Saint Alexander of Salonica","Saint Alexander","Blessed Alexander of Lugo","Saint Alexander Briant","Pope Saint Alexander I","Saint Alexander Sauli","Saint Alexander Newski","Blessed Alexander Vincioli","Saint Alexander of Bergamo","Saint Alexander of Caesarea","Saint Alexander of Jerusalem","Saint Alexander the Charcoal Burner","Saint Alexander of Constantinople","Saint Alexander of Corinth","Saint Alexander of Fermo","Blessed Alexander of Foigny","Saint Alexander of Rome","Saint Alexander of Rome","Saint Alexander of Rome","Saint Alexander of the Via Claudia","Martyrs Under Alexander","Saint Alexandra of Amisus","Saint Ambrose of Alexandria","Saint Anatolius of Alexandria","Saint Basilides of Alexandria","Saint Clement of Alexandria","Saint Dionysius of Alexandria","Saint Leucius of Alexandria","Saint Macarius of Alexandria","Martyrs of Alexandria","Saint Maximus of Alexandria","Saint Metranus of Alexandria","Saint Nemesius of Alexandria","Saint Pantaenus of Alexandria","Saint Peter of Alexandria","Saint Serapion of Alexandria","Saint Severus of Alexandria","Blessed Alexandrina Maria da Costa","Saint Alexius of Rome","Blessed Alexius Sanbashi Saburo","Blessed Thomas Alfield","Blessed Alfons Maria Mazurek","Blessed Alfonso Navarrete-Benito","Blessed Alfonso Lopez","Saint Alfonso Maria Fusco","Blessed Alphonsus Pacheco","Saint André Bessette","Saint Alfred the Great","Saint Etheldritha","Blessed Alfredo Ildefonso Schuster","Saint Julián Alfredo","Saint Magnus of Füssen","Saint Alipius of Tagaste","Saint Alkmund of Northumbria","Blessed Joseph Allamano","Saint Elian of Brittany","Saint Illidius of Clermont","Saint Bavo of Ghent","Saint Telemachus","Saint John Almond","Blessed James the Almsgiver","Blessed John Rainuzzi","Saint Alodia of Huesca","Venerable Anastasius Hartmann","Saint Luigi Guanella","Blessed Luigi Maria Monti","Blessed Alojzije Stepinac","Blessed Alojzy Liguda","Saint Alonso de Orozco Mena","Saint Alonso Rodriguez","Blessed Stephen Bellesini","Saint Aloysius Gonzaga","Saint Luigi Scrosoppi of Udine","Blessed Aloysius Luis Rabata","Blessed Louis Tezza","Saint Luigi Versiglia","Blessed Alphais of Cudot","Saint Alphege of Winchester","Saint Alphius of Lentini","Saint Alphonsa of India","Saint Alphonsus Maria de Liguori","Saint Alpinian of Limoges","Blessed Gertrude of Altenberg","Saint Altinus","Saint Altman of Passau","Blessed Bartholomew Alvarez","Saint Julio Álvarez Mendoza","Blessed John of Alvernia","Venerable Emmanuel-Maurice d’Alzon","Saint Paulina do Coração Agonizante de Jesus","Saint Amabilis of Auvergne","Saint Amabilis of Rouen","Saint Bartholomew degli Amidei","Blessed Amadeus of Clermont","Blessed Amadeus of Lausanne","Saint Amaethlu of Anglesey","Saint Amalburga of Mauberge","Saint Amandus of Avignon","Saint Amandus of Beaumont","Saint Amand of Grisalba","Saint Amandus of Boixe","Saint Amandus of Bordeaux","Saint Amand of Coly","Saint Amand of Maastricht","Saint Amandus of Lérins","Saint Amandus of Limoges","Saint Amand of Moissac","Saint Amand of Nantes","Saint Amandus of Rennes","Saint Amandus of Rodez","Saint Amandus of Strasbourg","Saint Amandus of Worms","Saint Marie Amandine","Saint Amator of Auxerre","Blessed Marcolino of Forli","Saint Amantius of Wintershoven","Saint Amantius of Noyon","Blessed Gonzalo de Amarante","Saint Amarinus of Clermont","Saint Basileus of Amasea","Saint Amasius of Teano",
    "Saint George of Amastris","Saint Amaswinthus of Málaga","Saint Amata of Assisi","Blessed Amata of San Sisto","Saint Amator of Autun","Saint Amatus","Saint Ambrose of Milan","Saint Hugh of Ambronay","Saint Ambrose Edward Barlow","Blessed Ambrose Fernandez","Saint Ambrosio Kibuuka","Blessed Ambrose Sansedoni of Siena","Saint Ambrose of Sens","Blessed Frances d’Amboise","Blessed Ambrosio Francisco Ferro","Venerable Amelia of Saint Joseph","Saint Firmina of Amelia","Blessed Lucy Bufalari","Saint Margaret of Amelia","Blessed John Amero","Blessed John Amias","Saint Firminus of Amiens","Saint Godfrey of Amiens","Saint Salvius of Amiens","Saint Ulphia of Amiens","Saint Ammianus the Martyr","Saint Ammon of Alexandria","Saint Ammon of Alexandria","Saint Ammon the Great","Saint Ammonaria of Alexandria","Saint Ammonius","Saint Ammonius the Deacon","Saint Amoes of Sketis","Saint Felice of Nicosia","Saint Ampelius of Milan","Saint Amphibalus of Verulam","Saint Amphion of Nicomedia","Saint Ansgar","Blessed Anacleto González Flores","Pope Saint Cletus","Saint Ananias of Arbela","Saint Ananias of Damascus","Saint Anastasia of Sirmium","Saint Anastasia the Patrician","Saint Anastasius of Antioch","Saint Anastasius of Suppentonia","Saint Astricus of Esztergom","Saint Anastasius","Pope Saint Anastasius I","Saint Anastasius II of Antioch","Saint Anastasius VIII","Saint Anastasius of Sens","Saint Anastasius of Bourges","Saint Anastasius of Brescia","Saint Anastasius of Cordoba","Saint Anastasius the Fuller","Saint Anastasius the Persian","Blessed Anastazy Jakub Pankiewicz","Saint Anatoli Kiriggwajjo","Saint Anatolia of Thora","Saint Anatolius of Constantinople","Saint Anatolius of Salins","Saint Julian of Anazarbus","Saint José de Anchieta","Saint Calogerus of Fragalata","Blessed Giovanni Giovenale Ancina","Saint Marcellinus of Ancona","Saint Nilus the Elder","Blessed Andrea Carlo Ferrari","Saint Andeolus of Smyrna","Saint Guy of Anderlecht","Blessed William Richardson","Blessed Robert Anderton","Saint Teresa of the Andes","Blessed William Andleby","Blessed Diogo de Andrade","Saint Angadrisma of Beauvais","Blessed André de Soveral","Blessed Andrea Gallerani","Blessed Andrea Giacinto Longhin","Blessed Andrii Ischak","Saint Andrew the Apostle","Blessed Osanna Andreasi","Saint Andrew Corsini","Saint Augusto Andrés","Blessed Andrés Solá Molist","Saint Andrew of Constantinople","Saint Andrew Avellino","Saint Andrew Bobola","Blessed Andrew Dotti","Saint Anrê Tran An Dung","Saint André-Hubert Fournet","Blessed Andrew of Peschiera","Saint Andrew Kim Taegon","Saint Anrê Nguyen Kim Thông","Andre Oexner of Rinn","Blessed Andrew Sushinda","Blessed Andreas Murayama Tokuan","Saint Anrê Tran Van Trông","Blessed Andrew of Antioch","Saint Andrew of Crete","Blessed Andrew the Catechist","Saint Andrew of Poland","Saint Andrew of Strumi","Saint Andrew of Trier","Saint Andrew of Fiesole","Saint Richard of Andria","Saint Andronicus","Blessed Gabrielle Androuin","Blessed Perrine Androuin","Blessed Suzanne Androuin","Saint Anectus of Corinth","Saint Anesius","Blessed Engelmar Unzeitig","Blessed Jerome Ranuzzi","Blessed Sebastian of Aparicio","Saint Leontius of Caesarea","Saint Angela Merici","Blessed Angela Truszkowska","Saint Angela of the Cross Guerrero","Blessed Francisca Desamparados Honorata Lloret Martí","Saint Angela of Foligno","Blessed Columba of Rieti","Saint Angelelmus of Auxerre","Saint Thomas Aquinas","Blessed Fra Angelico","Blessed Angelina di Marsciano","Blessed Jerome de Angelis","Blessed Angelo Carletti","Saint Benedetto Menni","Pope Saint John XXIII","Venerable Francesco Paoli","Saint John Angeloptes","Guardian Angels","Our Lady of the Angels","Venerable Vittoria Gisella Gregoris","Blessed Angelus Agostini Mazzinghi","Blessed Angelus de Scarpetti","Blessed Angelus of Furci","Blessed Angelus of Gualdo Tadino","Saint Angelus of Jerusalem","Blessed John Angelus","Saint Benedict of Angers","Saint Angilbert of Centula","Saint Simon Stock","Saint Caesarius of Angoulême","Saint Anianus the Deacon","Saint Anianas of Chartres","Blessed Anicet Hryciuk","Blessed Anicet Koplinski","Pope Saint Anicetus","Saint Christina of Bolsena","Saint Aninus of Syria","Saint Maria Bertilla Boscardin","Blessed Anne-Joseph Leroux","Blessed Clara Isabella Fornari","Blessed Maria Fortunata Viti","Blessed Anna Maria Janer Anglarill","Saint Teresa Margaret Redi","Blessed Anne Marie Taigi","Blessed Marie Therese of Saint Joseph","Blessed Anna Rosa Gattorno","Saint Anna Schaeffer","Blessed Anna of the Angels Monteagudo","Saint Anna the Prophetess","Saint Anne","Blessed Anne Catherine Emmerich","Blessed Anne of Saint Bartholomew","Saint Anne Line","Blessed Anne Hamard","Venerable Anne Madeleine Remuzat","Blessed Anne Mary Javouhey","Blessed Anne Maugrain","Saint Marie de Saint Just","Saint Anne of Constantinople","Blessed Anne Pelras","Saint John Wall","Saint Marie Adolphine Dierks","Saint Marie-Eugénie de Jésus","Blessed Anne-Françoise de Villeneuve","Blessed Anne-Marie-Madeleine Thouret","Saint Hannibal Mary di Francia","Saint Anno II","Saint Anno of Verona","Blessed Titus Brandsma","Saint Giacinto Ansalone","Saint Auxanus of Milan","Saint Ansanus the Baptizer","Saint Ansbald of Prüm","Saint Ansbert of Rouen","Saint Ansegisus","Saint Anselm of Canterbury","Saint Anselm of Lucca the Younger","Saint Anselm of Nonantola","Blessed Anselmo Polanco","Saint Ansfrid of Utrecht","Saint Ansovinus of Camerino","Saint Anstrudis of Laon","Saint Ansuinus of Bèze","Saint Ansurius of Orense","Saint Antanansio Bazzekuketta","Pope Saint Antherus","Saint Anthelm of Belley","Saint Anthemius","Saint Anthimus of Rome","Saint Anthony of Antioch","Blessed Anthony Baldinucci","Venerable Anthony Battie","Saint Anthony Kauleas","Saint Anthony Daniel","Saint Antôn Nguyen Ðích","Saint Antonino Fantosati","Saint Antonio González","Blessed Anthony Manzi","Saint Anthony Mary Claret","Saint Anthony Mary Gianelli","Saint Antony Mary Pucci","Saint Anthony Mary Zaccaria","Blessed Antony Page","Saint Antôn Nguyen Huu Quynh","Blessed Anthony Turner","Saint Anthony of Froidemont","Saint Anthony of Lérins","Saint Anthony of Padua","Saint Anthony the Roman","Blessed Anthony of Saxony","Blessed Anthony of Stroncone","Blessed Antonio of Saint Bonaventure","Saint Armentarius of Antibes","Saint Antigonus of Rome","Saint Antimus of Brantôme","Saint Asclas of Antinoe","Saint Talida of Antinoë","Saint Clerus of Antioch","Saint Evodius of Antioch","Saint George of Antioch","Saint Hesychius of Antioch","Saint Isidore of Antioch","Saint Joseph of Antioch","Saint Lucian of Antioch","Saint Macarius of Antioch","Saint Maximilian of Antioch","Saint Palladius of Antioch","Saint Paulinus of Antioch","Saint Pelagia of Antioch","Saint Philogonius of Antioch","Saint Theodolus of Antioch","Saint Theodosius of Antioch","Saint Antipas of Pergamon","Blessed Antoine Fournier","Blessed Anton Maria Schwartz","Blessed Anton Martin Slomsek","Blessed Antoni Beszta-Borowski","Blessed Antoni Julian Nowowiejski","Blessed Antoni Leszczewicz","Blessed Antoni Rewera","Blessed Antoni Swiadek","Saint Antonia of Cirta","Blessed Antonia Mesina","Blessed Antonia of Florence","Saint Antonina of Bithynia","Saint Antonina of Constantinople","Blessed Antonio of Tlaxcala","Venerable Antonio Amumdarain Garmendia","Saint Antonio of Saint Anne","Pope Saint Pius V","Venerable Antonio Margil","Blessed Antonio Pérez Lários","Blessed Louis-Antoine-Rose Ormières Lacase","Blessed Antonio Rosmini-Serbati","Blessed Bonaventura of Potenza","Saint Antoninus of Syria","Saint Antoninus of Pamiers","Saint Antoninus of Piacenza","Saint Antonius of Rocher","Saint Antoninus of Sorrento","Saint Antoninus the Martyr","Saint Antonius of Florence","Saint Anthony of Rome","Saint Antony","Blessed Antony Bonfadini","Saint Antony Deynan","Blessed Antony Fatati","Saint Antony Primaldo","Blessed Antony of Pavoni","Saint Antony of Vilna","Saint Anysia of Salonika","Saint Anysius of Thessalonica","Saint Hugh of Anzy-le-Duc","Saint Gratus of Aosta","Saint Ursus of Aosta","Saint Austregisilus of Bourges","Saint Miguel González de Aozaraza de Leibar","Saint Julian of Apamea","Saint Apelles of Smyrna","Saint Aper of Sens","Saint Aprus of Toul","Saint Apodemus of Saragossa","Saint Apollinaris of Ravenna","Saint Apollinaris the Apologist","Saint Apollinaris of Rheims","Blessed Apolinar Franco","Blessed Apollinaris of Posat","Saint Apollinaris Syncletica","Saint Apollonia of Alexandria","Saint Apollo of Heliopolis","Saint Apollonius of Braga","Saint Apollonius of Benevento","Saint Apollonius of Brescia","Blessed Vilmos Apor","Saint John the Apostle","Saint Peter the Apostle","Pope Saint Leo IX","Saint Gaspare Bertoni","Blessed Bonaventure Tornielli","Saint Apphia","Saint Appian of Comacchio","Saint Appian of Alexandria","Saint Appian of Caesarea","Saint Apronia","Saint Apronian the Executioner","Saint Peter Balsam","Saint Auspicius of Apt","Saint Castor of Apt","Saint Stephen of Apt","Saint Aquila of Caesarea","Saint Aquila the Tent Maker","Saint Aquila of Philadelphia","Saint Aquila the Martyr","Saint Aquila of Egypt","Saint Maximus of Aquila","Saint Raynerius of Aquila","Saint Chromatius of Aquileia","Saint Paulinus of Aquileia","Saint Aquilina of Syria",
    "Saint Aquilinus of Africa","Saint Aquilinus of Milan","Saint Prosper of Aquitaine","Saint Arbogast of Strasbourg","Saint Arator of Verdun","Saint Arbeo of Freising","Saint Peter Arbues","Saint Arcadius","Saint Arcadius of Salamanca","Saint Arcadius of Mauretania","Saint Arcangelo Tadini","Saint Arcanus","Gabriel the Archangel","Michael the Archangel","Raphael the Archangel","Blessed Archangela Girlani","Saint Archelais the Martyr","Saint Archelaus of Mesopotamia","Saint Archippus of Colossi","Saint Namphanion the Archmartyr","Saint Armogastes of Africa","Saint Arcontius of Capua","Saint Arcontius of Viviers","Saint Mel of Ardagh","Saint Ardanus of Tournus","Saint Declan of Ardmore","Saint Ardo of Aniane","Saint Aretas","Saint Arethas","Saint Mark of Arethusa","Book of Saints – Donatus of Corfu","Saint Osmanna","Blessed Ida of Argensolles","Saint Argeus of Tomi","Saint Aria of Rome","Saint Ariadne","Saint Arianell","Blessed Arigius of Gap","Saint Ariston of Campania","Saint Aristarchus of Thessalonica","Pope Saint Evaristus","Saint Macarius of Petra","Saint Harlindis of Arland","Saint Aurelian of Arles","Saint Caesaria of Arles","Saint Caesarius of Arles","Saint Genesius of Arles","Saint Hilary of Arles","Saint Honoratus of Arles","Saint Trophimus of Arles","Saint Celsus of Armagh","Saint Colman of Armagh","Saint Thomian of Armagh","Saint Ormond of Mairé","Blessed Peter Armengol","Saint Hermengaudius of Urgell","Saint Caesarius of Armenia","Forty Martyrs of Armenia","Martyrs of Armenia","Saint Armentarius of Pavia","Saint Arnold","Saint Arnold Janssen","Blessed Arnold of Gemblours","Saint Arnold of Hiltensweiler","Saint Arnulf of Metz","Blessed Arnold Reche","Saint Arnulf of Mainz","Saint Arnulf of Soissons","Saint Arnulph of Gap","Saint Enda of Arran","Blessed Frederick of Saint-Vanne","Saint Wulganus","Saint Theophilus of Corte","Blessed Henry of Treviso","Blessed Hildemar of Arrouaise","Saint Edmund Arrowsmith","Saint John Mary Vianney","Saint Arsenius of Alexandria","Saint Arsenius the Great","Saint Arsenius of Corfu","Saint Artaxus of Syrmium","Saint Artemas of Pozzuoli","Blessed Artemide Zatti","Saint Artemius Megalomartyr","Saint Artemius of Clermont","Saint Arthelais of Benevento","Blessed Arthur Bell","Blessed Roger Filcock","Saint Asaph of Llanelwy","Saint Martin of the Ascension","Saint Mark the Ascetic","Saint Asicus of Elphin","Saint Asclepiades","Saint Serafino of Montegranaro","Saint Asella of Rome","Blessed Ralph Ashley","Venerable Roger Ashton","Saint Aspasius of Auch","Saint Aspren of Naples","Saint Felix of Cantalice","Saint Clare of Assisi","Saint Francis of Assisi","Saint Rufinus of Assisi","Blessed Sylvester of Assisi","Blessed Mary Assunta","Saint Asterius of Caesarea","Saint Evasius of Asti","Saint Malcoldia of Asti","Saint Secundus of Asti","Saint Christina the Astonishing","Blessed Maria Angela Astorch","Saint Turibius of Astorga","Saint John de Atarés","Saint Tathai","Saint Athanasia of Canope","Saint Athanasius of Cilicia","Saint Athanasius of Alexandria","Saint Athanasius of Jerusalem","Saint Athanasius of Modon","Saint Athanasius of Naples","Saint Athanasius of Sorrento","Saint Hilary of Poitiers","Saint Athanasius the Athonite","Saint Athelm of Canterbury","Saint Dionysius the Aeropagite","Saint Attilanus of Zamora","Blessed Thomas Atkinson","Niño de Atocha","Saint Peter of Atroa","Saint Attalas of Bobbio","Saint Attalia","Saint Attalus of Catania","Saint Atticus of Constantinople","Saint Atticus","Saint Atto of Oca","Saint Aubert of Arras","Saint Ethelburga of Faremoutier","Saint Silvinus of Auchy","Saint Audax of Thora","Saint Audifax of Persia","Saint Othmar of Saint Gall","Saint Omer","Saint Augebert of Champagne","Saint Eugendus of Condat","Saint Simbert of Augsburg","Saint Ulric of Augsburg","Saint Augurius the Martyr","Saint Augusta of Treviso","Saint Augustalis","Saint Auguste Chapdelaine","Saint Augustine of Sens","Blessed Ambroise-Augustin Chevreux","Saint Augustine Tchao","Blessed Augustine Fangi","Blessed Augustine Gazotich","Saint Augustinô Phan Viet Huy","Saint Augustine Moi Van Nguyen","Saint Augustine of Canterbury","Saint Augustine of Hippo","Blessed Augustine Ota","Saint Augustine Schoffler","Blessed Augustine Thevarparampil","Saint Augustine Webster","Saint Augustus of Campania","Saint Eulalia of Merida","Blessed Simon of Aulne","Saint Aurea of Paris","Saint Áurea of San Millán","Saint Aurelia","Saint Petronilla of Rome","Saint Aurelia of Strasbourg","Saint Aurelian of Limoges","Saint Aurelian of Lyons","Venerable Aureliano of the Blessed Sacrament","Saint Paul Aurelian","Blessed Aurelio da Vinalesa","Saint Aurelius of Cordoba","Saint Aureus of Mainz","Saint Auspicius of Toul","Saint Auspicius of Trier","Saint Dagobert II","Saint Sigebert III of Austrasia","Saint Austrebertha of Pavilly","Saint Austremonius","Saint Rupert of Salzburg","Blessed Charles of Austria","Saint Leopold III","Saint Austriclinian of Limoges","Saint Autbert of Avranches","Saint Autbert of Landevennec","Saint Autonomus","Saint Cassian of Autun","Saint Eugenian of Autun","Saint John of Autun","Blessed Leonianus of Autun","Saint Racho of Autun","Saint Symphorian of Autun","Saint Julian of Auvergne","Saint Auxentius of Bithynia","Saint Auxentius of Mopsuetia","Saint Curcodomus of Auxerre","Saint Germanus of Auxerre","Saint Gregory of Auxerre","Saint Helladius of Auxerre","Saint Jovinian of Auxerre","Saint Optatus of Auxerre","Saint Peregrinus of Auxerre","Saint Valerian of Auxerre","Saint Vigilius of Auxerre","Saint Auxibius","Saint Eulalia of Barcelona","Saint Hugh of Lincoln","Saint Bertha of Avenay","Saint Marius Aventicus","Saint Aventinus of Chartres","Saint Aventinus of Troyes","Saint Everild of Everingham","Saint Avertinus of Tours","Blessed Mark of Aviano","Saint Francisca Salesia","Saint John of Avila","Saint Avitus","Saint Severus of Avranches","Saint Evan of Ayrshire","Saint Lutgardis","Blessed Juana de Aza","Saint Azadanes the Deacon","Saint Azades the Eunuch","Blessed Ignatius de Azevedo","Saint Lukka Baanabakintu","Saint Babylas of Antioch","Saint Bacchus","Blessed Ferdinando Maria Baccilieri","Saint Balay","Venerable Chiara Badano","Saint Bademus","Saint Benedict Biscop","Blessed Francisco Alvares","Saint Bagne of Terouanne","Blessed Joan of Bagno di Romagna","Saint Bonaventure of Bagnoregio","Saint Bairfhion","Saint Baoithin of Ennisboyne","Saint Baithen of Iona","Saint Finbar","Blessed Vitaliy Bayrak","Saint Bajulus of Rome","Blessed Isidore Bakanja","Saint David Lewis","Saint Josephine Bakhita","Saint Balbina of Rome","Blessed Notkar Balbulus","Saint Balda of Jouarre","Blessed Balthassar of Chiavari","Saint Ubaldus Baldassini","Saint Bega","Saint Baldomerus of Saint Justus","Saint Baldred of Strathclyde","Saint Baldred the Hermit","Saint Balther of Lindisfarne","Blessed Christopher Bales","Blessed Jan Adalbert Balicki","Saint Joseph Mukasa","Saint Cronan Mochua","Blessed Simon Ballachi","Blessed Balsamus of Cava","Saint Balthasar","Blessed Walto of Wessobrünn","Blessed Edward Bamber","Saint Otto of Bamberg","Saint Beornwald of Bampton","Blessed Giovanni Dominici","Saint Deiniol of Bangor","Saint Fionnchu of Bangor","Saint Baoithin","Saint Candida of Bañoles","Saint Emerius of Bañoles","Saint John the Baptist","Blessed John Baptist Turpin du Cormier","Saint Barachisius","Saint Berach of Kilbarry","Blessed Severian Baranyk","Saint Madeline Sophie Barat","Blessed Maria Candida of the Eucharist","Saint Barbara","Saint Marianne Cope","Saint Gregory Barbarigo","Saint Raymond of Barbastro","Saint Barbasymas","Saint Barbatian of Ravenna","Saint Barbatus of Benevento","Saint Barbea of Edessa","Saint Jaime Hilario Barbal","Blessed Dominic Barberi","Saint Clelia Barbieri","Saint Pacian of Barcelona","Saint Severus of Barcelona","Saint Joseph Oriol","Saint Bardo of Mainz","Saint Lleudadd of Bardsey","Saint Tortgith of Barking","Blessed Mark Barkworth","Saint Barlaam of Antioch","Saint Barnabas the Apostle","Saint Brannock","Saint Barrog the Hermit","Blessed Jeanne-Louise Barré","Blessed Nicolas Barré","Blessed Florentino Asensio Barroso","Blessed Oddino Barrotti","Saint Barsabas","Saint Joseph Barsabas","Saint Barsanuphius of Gaza","Saint Barsimaeus of Edessa","Saint Bartholomea Capitanio","Blessed Bartholomew Fanti of Mantua","Blessed Bartholomew Longo","Blessed Bartlomiej Osypiuk","Blessed Bartholomew of Vicenza","Saint Bartholomew of Farne","Saint Bartholomew of Rossano","Saint Sergius of Moscow","Blessed Bartholomew of Tuscany","Saint Bartholomew the Apostle","Saint Barypsabas","Venerable Cesare Maria Barzaghi","Saint Basil","Saint Basil of Constantinople","Blessed Basil Hopko","Blessed Basil Anthony Marie Moreau","Saint Basil of Bologna","Saint Basil the Great","Blessed Vasyl Vsevolod Velychkovskyi","Saint Basileus the Martyr","Saint Basilian of Laodicea","Saint Basilides of Crete","Saint Basilissa of Øhren","Saint Basiliscus of Pontus","Saint Basillisa","Saint Basilissa of Galatia","Saint Basinus of Trier","Blessed Jean Bassano","Saint Bassian of Lodi","Saint Bassus of Nice","Blessed John del Bastone","Saint Bathilde","Saint Luis Batiz Sainz","Venerable Tommasina Vernazza","Saint Baudacarius of Bobbio","Saint Baudolino","Blessed Victoire Bauduceau épouse Révélière","Blessed Odile Baumgarten","Saint Pascal Baylon","Saint Gerard de Bazonches","Saint Bean","Saint Breandan","Saint Beoc","Saint Brogan of Mothil","Blessed James Bird","Saint Beatrice da Silva Meneses","Saint Beatrice of Rome","Saint Beatrix of Bohemia","Blessed Beatrix of Lens","Blessed Beatrix of Este the Elder","Blessed Beatrix of Este the Younger","Blessed Beatrix of Ornacieux","Blessed Beatrix of Valfleury","Saint Beatus","Saint Beatus of Lungern","Blessed Flora of Beaulieu","Our Lady of Beauraing","Saint Julian of Beauvais","Saint Justus of Beauvais","Saint Lucian of Beauvais","Saint Maximian of Beauvais","Blessed Odo of Beauvais","Blessed Giovanni da Fabriano Becchetti","Blessed John Beche","Saint Bede the Venerable","Blessed George Beesley","Saint Cronan Beg","Saint Begga of Andenne","Blessed Hesso of Beinwil","Saint Belina","Blessed James Bell","Blessed Thomas Bellacci","Blessed Françoise Bellanger","Saint Robert Bellarmine","Saint Berlindis of Meerbeke","Saint Ignatius of Santhia","Venerable Benedetta Bianchi Porro","Saint Benedicta Cambiagio Frassinello","Blessed Jacopone da Todi","Blessed Pope Innocent XI","Saint Benedict of Nursia","Pope Saint Benedict II","Saint Joseph Benedict Cottolengo","Saint Benedict Crispus of Milan","Saint Benedict Joseph Labre","Saint Benedict dell’Antella","Saint Benedict of Aniane","Saint Benedict of Cagliari","Saint Benedict of Campagna","Saint Benito de Jesus","Saint Benedict the Black","Blessed Benedict Revelli","Blessed Benedict Ricasoli","Saint Benedicta of Rome","Saint Benedicta of Assisi","Saint Benignus of Armagh","Saint Offa of Benevento","Saint Benevenuto Scotivoli of Osimo","Saint Benezet the Bridge Builder","Blessed James Benefatti","Saint Benignus of Dijon","Saint Benildus","Saint Benildis of Cordoba","Blessed Benincasa of Cava","Saint Catherine of Siena","New Catholic Dictionary – Venerable Ursula Benincasa","Saint Philip Benizi","Saint Benjamin the Deacon","Saint Benjamín Julián","Saint Beuno Gasulsych","Saint Benno of Meissen","Saint Winwallus","Blessed Thomas Hunt","Blessed Bentivoglio de Bonis","Blessed Benvenuta Bojani",
    "Saint Brithwald of Canterbury","Saint Birstan","Saint Berard of Bèze","Saint Berard of Bèze","Saint Berardes of Morocco","Saint Berarius I of Le Mans","Saint Bercharius","Saint John Berchmans","Blessed Richard Bere","Saint Beregisus","Saint Gianna Beretta Molla","Saint John of Bergamo","Saint Viator of Bergamo","Saint Sunniva of Bergen","Saint Wiltrudis of Bergen","Saint Birinus of Dorchester","Saint Zedislava Berka","Saint Bernadette of Lourdes","Saint Bernadine of Siena","Saint Bernadine Realino","Venerable Bernard Francis Casey","Saint Bênadô Võ Van Duê","Saint Bernard of Carinola","Saint Bernard of Clairvaux","Saint Bernard of Corleone","Saint Bernard of Menthon","Blessed Bernard of Toulouse","Saint Bernard of Valdeiglesias","Blessed Bernard of Vienne","Saint Bernard Valeara of Teramo","Blessed Bernard Scammacca","Saint Bernardo Tolomei","Blessed Bernardina Maria Jablonska","Blessed Bernardine of Feltre","Saint Bernardo degli Uberti","Saint Siméon-François Berneux","Blessed Bernhard Lichtenberg","Saint Berno of Cluny","Blessed Constantius of Fabriano","Saint Bernold of Ottobeuren","Saint Bernward of Hildesheim","Saint Philip Berruyer","Saint William of Bourges","Saint Solange of Bourges","Saint Berthaldus","Blessed Bertha de’Alberti of Cavriglia","Saint Bertha of Blangy","Blessed Denis of the Nativity","Saint Bertrand of Le Mans","Saint Bertilia of Mareuil","Saint Bertille","Saint Bertin the Great","Blessed Joscius Roseus","Blessed Simon of Saint Bertin","Saint Bertinus the Younger","Saint Bertoara","Blessed James Bertoni","Saint Bettelin","Saint Cirilo Bertrán","Blessed Luis Bertrán","Saint Louis Bertrand","Saint Bertrand of Aquileia","Saint Bertrand of Comminges","Blessed Bertrand of Garrigue",
    "Saint Bertrand of Saint Quentin","Blessed Innocent of Berzo","Saint Donatus of Besancon","Saint Germanus of Besancon","Saint Besa of Egypt","Saint Besas of Alexandria","Blessed Perrine Besson","Saint Pedro de San Jose Betancur","Blessed Elizabeth Achler","Saint Lazarus of Bethany","Saint Betto","Saint Budoc of Brittany","Saint John of Beverley","Blessed Jan Beyzym","Saint Gerald of Beziers","Saint Blaise","Saint Francesco Saverio Maria Bianchi","Saint Bibiana","Saint Byblig","Saint Bicor","Blessed Olympia Bida","Saint Bieuzy of Brittany","Saint Jósef Bilczewski","Saint Julia Billiart","Blessed Gandulphus of Binasco","Blessed Veronica of Milan",
    "Saint Bridget of Sweden","Blessed Brigida of Jesus","Saint Brendan of Birr","Blessed Sibyllina Biscossi","Saint Constantius the Bishop","Saint Gerland the Bishop","Saint Humilis of Bisignano","Blessed Jakov Varingez","Saint Blane","Saint Ewald the Black","Saint Moses the Black","Saint Blidulf of Bobbio","Saint Bladus",
    "Saint Blaesilla","Saint Blaise of Veroli","Saint Blaithmaic of Iona","Saint Gwen","Black Nazarene","Saint Blanchard","Saint Gwen of Tagarth","Saint Blanda of Rome","Saint Blandina the Slave","Saint Blath of Kildare","Saint Blitharius of Seganne","Saint Blitmund of Bobbio","Blessed Maria Anna Blondin","Saint Boadin","Saint John Boste",
    "Saint Columbanus","Saint Bobinus of Troyes","Saint Bodfan of Wales","Blessed Luigi Boccardo","Saint Bodagisil of Aquitaine","Blessed John Bodey","Blessed Severinus Boethius","Blessed Geremia Oghlou Boghos","Saint Boswell","Blessed Edmund Bojanowski","Saint Bolcan of Derken","Blessed Boleslas Strzelecki","Saint Felix of Bologna","Blessed Helen Duglioli",
    "Saint Jucundus of Bologna","Saint Juliana of Bologna","Saint Zama of Bologna","Saint Bona of Pisa","Blessed Bonannus of Roio","Saint Bonaventure of Miyako","Blessed Bonavita of Lugo","Saint Isidore the Farmer","Saint Bonitus of Clermont","Saint Buonfiglio Monaldi","Blessed Pierre Bonhomme","Saint Boniface","Saint Boniface the Martyr","Saint Boniface of Rome","Pope Saint Boniface I","Pope Saint Boniface IV","Saint Boniface Curitan","Saint Boniface of Ferentino","Saint Boniface of Tarsus","Saint Bonifacia Rodriguez Castro","Blessed Pietro Bonilli","Blessed Josephina Gabriella Bonino","Saint John-Louis Bonnard","Blessed Françoise Bonneau","Blessed Hugh of Bonnevaux","Saint Buono","Saint John Camillus the Good","Saint Delphinus of Bordeaux","Saint Francis Borgia","Saint Boris of Kiev","Saint Brendan the Navigator","Saint Charles Borromeo","Saint Bosa of York","Saint John Bosco",
    "Venerable Margaret Bosco","Blessed Thomas Bosgrave","Blessed Vikentij Bosilkov","Saint Botulph of Ikanhoe","Saint Botwid of Sudermannland","Blessed Elias of Bourdeilles","Saint Stephen du Bourg","Saint Marguerite Bourgeous","Saint Felix of Bourges","Saint Simplicius of Bourges","Blessed Gerard of Bourgogne","Blessed Hyacinthe-Augustine-Gabrielle Bourla","Blessed Marmaduke Bowes","Saint Martin of Braga","Venerable Flora Bracaval","Blessed Teresa Bracco","Saint Virginia Centurione Bracelli","Blessed Maria Caridad Brader","Saint Fructuosus of Braga","Saint Peter of Braga","Blessed Sanctes Brancasino","Saint Theodorus of Apamea","Saint Maria Cristina dell’Immacolata Concezione","Saint Branwallader of Jersey","Saint Braulio of Saragossa","Martyrs of Brazil","Saint John Bread-and-Water","Saint Uni of Bremen","Saint Willehad of Bremen","Saint Brenach of Carn-Engyle","Blessed Francesc Castells Areny","Saint Calocerus of Brescia","Saint Deusdedit of Brescia","Saint Dominic of Brescia","Saint Felix of Brescia","Saint Gaudiosus of Brescia","Saint Herculanus of Brescia","Saint Honorius of Brescia","Saint Latinus of Brescia","Saint Titian of Brescia","Saint Ursicinus of Brescia","Saint Bretannion of Tomi","Saint Hubert of Bretigny","Blessed Brian Lacey","Saint Briarch of Bourbriac","Saint Brice of Tours","Saint Brigid of Ireland","Blessed Marie-Anne-Françoise Brideau","Saint John of Bridlington","Saint Brieuc of Brittany","Saint Brigid of Kilbride","Saint Brigid of Fiesole","Saint Lawrence of Brindisi","Saint Leucius of Brindisi","Saint Brithwald of Ramsbury","Saint Britta","Blessed Julian Maunoir","Saint Efflam of Brittany","Saint Casilda of Toledo","Blessed Hartmann of Brixen","Saint Brocard","Blessed Lucy of Narni","Saint Brogan","Saint Brogan","Saint Brogan","Saint Brogan of Ross Tuirc","Saint Gerard of Brogne","Saint Bron of Cassel-Irra","Blessed Bronislava of Poland","Blessed Bronislaw Markiewicz","Saint Brothen","Blessed Hildegun of Schönau","Blessed Isidore of Saint Joseph","Blessed Daniel Brottier","Saint Bruno of Querfort","Blessed Bruna Pellesi","Saint Bruno","Saint Bruno the Great","Saint Bruno of Ebsdorf","Saint Bruno of Ottobeuren","Saint Bruno of Segni","Saint Bruno of Würzburg","Saint Bruno Sserunkuuma","Saint Gudule of Brussels","Blessed Christina of Stommeln","Blessed Henry the Shoemaker","Saint John Jones","Blessed Nikita Budka","Blessed John of Rieti","Saint Gaspare Bufalo","Saint Edburga of Thanet","Saint Theophilus of Bulgaria","Blessed Thomas Bullaker","Blessed Nicholas Bunkerd Kitbamrung","Saint John Buonagiunta Monetti","Blessed John Buoni","Blessed John of Parma","Saint Burchard of Würzburg","Saint Burgundofara","Pope Saint Callistus II","Saint Sigismund of Burgundy","Saint Gregory of Burtscheid","Blessed Cesar de Bus","Saint James Buzabaliao","Saint Honorius of Buzançais","Blessed María del Tránsito de Jesús Sacramentado","Saint Frances Xavier Cabrini","Blessed John Sordi","Blessed Johanna of Cáceres","Our Lady of Charity of El Cobre","Saint Caedmon","Blessed Peter de la Cadireta","Saint Cadoc of Llancarvan","Saint Cadroe","Blessed Roger Cadwallador","Saint Caecilian of Saragossa","Saint Caedwalla of Wessex","Saint Caellainn","Saint Cybi of Caenarvon","Saint Kentigerna","Saint Caerealis","Saint Dionysius of Caesarea","Saint Eupsychius of Caesarea","Saint Julian of Caesarea","Saint Pausis of Caesarea","Saint Romulus of Caesarea","Saint Timolaus of Caesarea","Saint Caesarius of Africa","Saint Caesarius of Damascus","Saint Caesarius of Nanzianzen","Saint Giuseppe Cafasso","Saint Julian of Cagliari","Blessed Gérard Cagnoli","Saint Ursicinus of Cahors","Saint Caidoc","Saint Caillin","Saint Caimin of Lough Derg","Saint Canice","Saint Cannera of Inis Cathaig","Saint Cairlon of Cashel","Saint Gaius of Moesia","Pope Saint Caius I","Saint Kichi Franciscus","Saint Caius of Apema","Saint Cajetan","Saint Gaetano Catanoso","Blessed Gaetana Sterni","Saint John Calabytes","Saint Giovanni Calabria","Saint Eustochia Calafato","Saint Joseph Calasanz","Saint Caldia of Amisus","Saint Calepodius of Rome","Saint Caletricus of Chartres","Blessed Calimerius of Montechiaro","Pope Saint Callistus I","Saint Callinica of Galatia","Saint Callinicus of Apollonia","Saint Callinicus of Paphlagonia","Saint Callisthene","Saint Callistus of Rome","Saint Callistus Caravario","Blessed Charles-Regis-Mathieu de la Calmette de Valfons","Saint Calocerus of Rome","Saint Pedro Calungsod","Saint Calupan","Saint Erlafrid of Hirschau","Blessed Peter Cambiano","Blessed Walembert of Cambrai","Blessed Odo of Cambrai","Saint Vindician of Cambrai","Saint Waltmann of Cambrai","Blessed Daniel of Cambron","Saint Venantius of Camerino","Saint Cameron","Saint Camilla","Saint Camilla Battista Varano","Saint Camillus of Lellis","Blessed Christina of Spoleto","Martyrs of Campania","Saint Edmund Campion","Martyrs of North America","Saint Candida of Rome","Saint Candida Maria de Jesus Cipitria y Barriola","Saint Candida of Carthage","Saint Candida of Naples","Saint Candida of Whitchurch","Saint Candida the Elder","Saint Candidus","Saint Candidus the Theban","Saint Candidus the Martyr","Saint Candres of Maastricht","Blessed Hugh Canefro","Blessed Jacobinus de Canepaci","Saint Giuse Hoàng Luong Canh","Saint Canio","Saint John of Kanty","Saint Ruggero of Canne","Saint Kanten","Saint Dunstan of Canterbury","Saint Honoratus of Canterbury","Saint Justus of Canterbury","Saint Lawrence of Canterbury","Saint Odo the Good","Saint Peter of Canterbury","Saint Kentigern","Saint Knud","Saint Canute Lavard","Saint Francis Ferdinand de Capillas","Saint John of Capistrano","Venerable Ignazio Eustachio Capizzi","Blessed James of Viterbo","Saint Gordius of Cappadocia","Martyrs of Cappadocia","Saint Theodosius the Cenobiarch","Blessed Godfrey of Cappenberg","Saint Caprasius of Lérins","Saint Germanus of Capua","Blessed Raymond of Capua","Saint Vitalian of Capua","Blessed Peter of Città di Castello","Saint Francis Caracciolo","Blessed Jean of Caramola","Saint Caran of Scotland","Blessed Cosmas da Carboniam","Martyrs of Cardeña","Blessed John Carey","Saint Calais of Anisola","Saint Virgilius of Salzburg","Venerable Carla Ronci","Blessed Carlos Eraña Guruceta","Venerable Marino Sanguineti","Venerable Carlo Gnocchi","Blessed Carlo Liviero","Saint Charles of Sezze","Blessed Carlos Manuel Cecilio Rodriguez Santiago","Blessed Charlemagne","Carmelite Martyrs of Compiègne","Blessed Carmelo Sastre y Sastre","Blessed Mykolai Charnetskyi","Blessed Charlotte Lucas","Blessed Charlotte Davy","Blessed Karolina Gerhardinger","Blessed Karoliny Kózkówny","Saint Caron","Saint Carpophorus","Saint Florentius of Carracedo","Blessed Matthew Carreri","Blessed William Carter","Saint Carterius of Caesarea","Saint Carthach the Elder","Saint Carthage the Younger","Saint Celerinus of Carthage","Saint Constantine of Carthage","Saint Deogratias of Carthage","Saint Catulinus of Carthage","Blessed Diego Carvalho","Saint Luke Casali","Blessed Guy of Casauria","Saint Casdoe","Saint Casimir of Poland","Venerable Casimiro Barello Morello",
    "Blessed Pierre-Joseph Cassant","Saint Cassian of Rome","Saint Cassian of Rome","Saint Cassian of Tangiers","Saint Cassian of Imola","Blessed Cassian Vaz Lopez-Neto","Saint John Cassian","Saint Cassius","Saint Giles of Castaneda","Saint Jacinto Castañeda Puchasóns","Blessed Marie-Céline of the Presentation","Saint Catellus of Castellammare","Blessed Peter of Castelnau","Saint Castor of Africa","Saint Castora Gabrielli","Saint Castorus","Saint Juan de Sahagún","Saint Castritian of Milan","Saint Pedro Poveda Castroverde","Saint Castulus","Saint Castulus of Rome","Saint Castus of Sinuessa","Saint Castus of Ancyra","Saint Catald of Taranto","Venerable Catalina of Mary","Saint Catherine of Palma","Blessed Caterina Cittadini","Blessed Caterina Dominici","Saint Caterina Volpicelli","Saint Elizabeth of the Trinity","Blessed Hosanna of Cattaro","Blessed Catherine Cottanceau","Blessed Catherine du Verdier de la Sorinière","Saint Vincentia Gerosa","Blessed Catherine Jarrige","Saint Catherine of Bologna","Saint Catherine del Ricci","Saint Catherine Labouré",
    "Saint Catherine of Alexandria","Saint Catherine of Genoa","Saint Catherine of Sweden","Saint Kateri Tekakwitha","Blessed Gratia of Cattaro","Saint Catus","Blessed Constabilis of Cava","Blessed Helen del Cavalcanti","Saint Cawrdaf","Saint Ceallach of Killala","Saint Cera of Kilkeary","Saint Cearan the Devout","Saint Ceccardus of Luni","Saint Cecilia","Saint Cecilia Yu Sosa","Saint Cecilius of Granada","Saint Cedd","Blessed Ceferino Jimenez-Malla","Blessed Mykola Tsehelskyi","Saint Cellach of Armagh","Saint Keyna","Saint Cele-Christ","Saint Maximilian of Celeia","Saint Celestine of Alexandria","Pope Saint Celestine V","Blessed Celestine of the Mother of God","Blessed Celina Chludzinska","Blessed Gregory Celli of Verucchio","Saint Mochelloc of Kilmallock","Saint Celsus of Antioch","Saint Celsus of Rome","Saint Cornelius the Centurion","Saint Marcellus the Centurion","Saint Ceolfrid","Saint Ceolwulph","Saint Kieran the Younger","Saint Ceratius of Grenoble","Saint Cerbonius of Populonia","Blessed Humiliana de’ Cerchi","Saint Serenus the Gardener","Blessed James of Cerqueto","Blessed James of Certaldo","Blessed Julia of Certaldo","Saint Maurus of Cesena","Blessed Ceslas Odrowaz","Saint Cettin of Oran","Blessed Juan of Cetina","Saint Cewydd","Saint Noel Chabanel","Saint Chad of Mercia","Saint Chaeremon of Nilopolis","Saint John of Châlon","Saint Lupus of Châlons","Saint Maria Chaira","Blessed William Joseph Chaminade","Saint Marcellin-Joseph-Benoît Champagnat","Saint Peter Chanel","Saint Charalampias","Saint Charbel Makhlouf","Saint Charity","Saint Martin de Porres","Blessed Virgin Mary","Saint Jean-Charles Cornay","Saint Charles Garnier","Saint Charles of Mount Argus","Saint Charles Lwanga","Blessed Charles Meehan","Blessed Charles Spinola","Blessed Charles Steeb","Blessed Charles de Foucauld","Blessed Charles of Blois","Blessed Charles of Sayn","Blessed Charles the Good","Blessed Ivo of Chartres","Saint Malard of Chartres","Saint Jacques Honoré Chastán","Saint Lucy the Chaste","Saint Kuriakose Elias Chavara","Saint Theuderius","Saint Werburgh of Chester","Blessed Isnard de Chiampo","Blessed Josaphat Chichkov","Saint Colgan","Saint Justin of Chieti","Saint Killian","Saint Chillien of Aubigny","Saint John of Chinon","Saint Chionia of Thessalonica","Saint Isidore of Chios","Our Lady of Chiquinquirá","Saint Nerses Glaietsi","Saint Phêrô Võ Ðang Khoa","Blessed Rose-Chretien de Neuville","Saint Gregory of Nazianzen","Blessed Christian of Perugia","Blessed Christian O’Conarchy","Saint Christiana","Saint Nino of Georgia","Blessed Oringa of the Cross","Saint Christina of Persia","Blessed Christina Ciccarelli","Saint Christina of Markyate","Saint Christina of Tyre","Saint Christopher","Blessed Christopher Robinson","Blessed Christopher of Milan","Saint Chrodegang of Metz","Blessed Fidelis Jerome Chojnacki","Saint Theophanes the Chronographer","Saint Chrysanthus","Saint Nicholas Chrysoberges","Saint Chrysogonus","Saint Peter Chrysologus","Saint John Chrysostom","Blessed Jan Nepomucen Chrzan","Saint Cian of Wales","Saint Kenan of Damleag","Saint Cillene","Venerable Vincenzo Cimatti","Blessed John Cini della Pace","Saint Cinnia of Ulster","Venerable Giovanni Battista Cioni","Feast of the Circumcision of the Lord","Saint John Cirita","Saint Zita of Lucca","Blessed Margaret of Castello","Saint Kigwe","Saint Clarus of Vienne","Saint Clair of Tours","Blessed Gerard of Clairvaux","Saint Clether","Venerable Clara Fey","Saint Clare of Montefalco","Saint Clarentius of Vienne","Blessed James Claxton","Saint Cloud","Saint Claude de la Colombiere","Saint Claudine Thevenet","Saint Victoriano Pío","Blessed Claudio Granzotto","Saint Claudius of Rome","Saint Claudius","Saint Claudius the Martyr","Saint Claudius of Troyes","Blessed Clemens August von Galen","Saint Clement of Ireland","Pope Saint Clement I","Saint Clement Mary Hofbauer","Blessed Clement of Dunblane","Saint Clement of Ohrid","Blessed Klymentii Sheptytskyi","Blessed Clemente Marchisio","Saint Cleomenes","Saint Cleonicus of Pontus","Saint Mary of Cleophas","Saint Genesius, Bishop of Clermont","Saint Genesius, Count of Clermont","Blessed Venerandus of Clermont","Saint Jean-François-Régis Clet","Saint John Climacus","Saint Clydog","Saint Margaret Clitherow","Saint Gluvias","Saint Tigernach of Clogher","Saint Finnian of Clonard","Saint Samthann of Clonbroney","Saint Colman of Cloyne","Saint Odilo of Cluny","Saint Odo of Cluny","Saint Ulric of Zell","Blessed Susanna Chobyoe","Saint Codratus of Corinth","Saint Govan","Saint Gowan of Wales","Saint Theotonius of Coimbra","Saint Collen of Denbighshire","Blessed Edward Coleman","Saint Colette","Saint Francisco Coll Guitart","Blessed Juliana of Collalto","Saint Gerard Sagredo","Saint Colman of Thuringia","Saint Colman of Stockerau","Saint Colman McRhoi","Saint Colman of Dromore","Saint Colman of Glendalough","Saint Colman of Kilmacduagh","Saint Colman of Kilroot","Saint Colman of Lindisfarne","Saint Colman of Lismore","Saint Columba of Iona","Saint John the Short","Saint Engelbert of Cologne","Blessed Henry of Cologne","Saint John of Cologne","Blessed Colomba Matylda Gabriel","Blessed John Colombini","Blessed Margaret Colonna","Blessed Columba Marmion","Saint Columba of Cordova","Saint Columba of Sens","Saint Columba of Terryglass","Saint Comasia","Saint Daniel Comboni","Saint Comgall of Bangor","Saint Connat","Saint Felix of Como","Saint Fidelis of Como","Saint John of Meda","Saint Provinus of Como","Saint Comus of Crete","Saint John Baptist Con","Saint Conan of Iona","Saint Concordia of Rome","Saint Concordius of Ravenna","Saint Concordius of Spoleto","Saint Romanus of Condat","Saint Leopold Bogdan Mandic","Saint Conval of Strathclyde","Saint Maximus the Confessor","Saint Guido Maria Conforti","Saint Congar","Saint Conindrus","Saint Conleth of Kildare","Saint Conon of Pamphylia","Saint Conon the Elder","Saint Conon the Younger","Saint Conrad of Constance","Blessed Conrad of Frisach","Blessed Conrad of Heisterbach","Saint Conrad of Parzham","Saint Conrad of Piacenza","Blessed Conrad of Seldenbüren","Saint Conrad of Trier","Blessed Conrad of Urach","Blessed Marie-Geneviève Meunier","Saint Gebhard of Constance","Saint Gulstan","Saint Constant","Saint Constantia of Nocera","Saint Constantine of Javron","Saint Cyril","Saint Constantine of Gap","Saint Constantine II","Saint Constantine of Cornwall","Saint Cyril of Constantinople","Saint Germanus of Constantinople","Saint Ignatius of Constantinople","Saint John of Constantinople","Saint Justina of Constantinople","Saint Menas Kallikelados","Saint Methodius of Constantinople","Saint Nicarete of Constantinople","Saint Nicephorus of Constantinople","Saint Silvia of Constantinople","Saint Tarasius of Constantinople","Saint Zoticus of Constantinople","Saint Constantius of Ancona","Saint Constantius of Rome","Saint Constantius of Trier","Saint Constantius of Perugia","Blessed Contardo Ferrini","Saint Contentius of Bayeux","Blessed Herman the Cripple","Venerable Fernando de Contreras","Blessed Convoyon of Redon","Blessed Hugh Faringdon","Saint Joseph of Cupertino","Saint Stephen of Sweden","Saint Theofrid of Corbie","Blessed Ralph Corby","Saint Corbinian","Saint Corbmac","Saint Miguel Febres Cordero Muñoz","Saint John of Cordoba","Saint Leocritia of Cordoba","Saint Corentius of Quimper","Martyrs of Corfu","Saint Tommaso da Cori","Saint Cornelia the Martyr","Pope Saint Cornelius","Saint Cornelius of Catania","Blessed John Cornelius","Blessed Juliana of Mont Cornillon","Saint Mawnan of Cornwall","Saint Mateo Correa-Magallanes","Blessed Maria Corsini Beltrame Quattrocchi","Saint Cosmas","Saint Cosmas Takeya","Blessed Costanza Starace","Blessed Thomas Cottam","Saint Cottidus of Cappadocia","Saint Marie Victoire Therese Couderc","Saint Lauto of Coutances","Saint Guillermo Courtet","Saint Germaine Cousin","Our Lady of Copacabana","Saint Michael Kozaki","Saint Thomas Kozaki","Blessed Vincent Kadlubek","Blessed John of the Grating","Saint Emmanuel of Cremona","Saint Crescens of Corinth","Saint Crescens of Galatia","Saint Crescens of Tomi","Saint Crescens of North Africa","Saint Crescentia of Lucania","Saint Maria Crescentia Höss","Saint Crescentian of Campania","Saint Crescentian of North Africa","Saint Crescentian of Rome","Saint Crescentinus","Saint Crescentius of Trier","Saint Crescentius of North Africa","Saint Cresconius of North Africa","Ten Martyrs of Crete","Saint Myron the Wonder Worker","Saint Crewanna","Saint Crispian","Saint Crispin","Saint Crispin I of Pavia","Saint Crispin II of Pavia","Saint Crispin of Viterbo","Saint Crispina","Saint Crispus of Rome","Blessed Cristobal","Saint Cristobal Magallanes Jara","Saint Nathy","Saint Cronan of Roscrea","Blessed Jerome of the Cross de Torres","Saint Guthlac of Croyland","Blessed Maria Crocifissa Curcio","Saint Finnian of Moville","Saint Juan Diego","Saint Julian of Cuenca","Martyrs of Cuncolim","Saint Kinga","Saint Cunegundes","Saint Drithelm","Blessed John of Cupramontana","Saint Curcodomus","Saint Curig of Llanbadarn","Saint Curonotus","Saint Gudwall","Blessed Giacomo Cusmano","Saint Cuthbert of Lindisfarne","Saint Cuthbert Mayne","Saint Cuthman","Saint Cutias of Rome","Saint Kingsmark","Saint Cynibild of Laestingaeu","Saint Cyprian of Corinth","Saint Cyprian of Antioch","Saint Cyprian of Toulon","Saint Cyprian","Blessed Cyprian Michael Iwene Tansi","Saint Cyprian of Carthage","Saint Cyprian of Périgueux","Saint Isaac of Cyprus","Saint Meletius of Cyprus","Saint Paul of Cyprus","Saint Cyriacus of Iconium","Saint Cyran of Brenne","Saint Cyrilla of Cyrene","Saint Theodore of Cyrene","Saint Cyrenia of Tarsus","Saint Cyriacus the Martyr","Saint Cyriaca of Nicomedia","Saint Cyriacus of Alexandria","Saint Cyriacus of Pamphylia","Saint Cyril of Philadelphia","Saint Cyril of Antioch","Saint Cyril of Alexandria","Saint Cyril of Jerusalem","Saint Cyril of Turov","Saint Cyrinus of Cyzicus","Saint Cyrinus of Lentini","Saint Cyrion","Saint Quirinus of Rome","Saint Tryphaena of Cyzicus","Saint John IV of Naples","Saint Dabius","Saint Dacius of Damascus","Saint Dadas of Persia","Saint Daig MacCairaill","Blessed Robert Dalby","Saint Dallan Forghaill","Saint Marinus","Saint Dalmatius","Saint Dalmatius of Pavia","Saint Dalmatius of Rodez","Saint John Damascene","Pope Saint Damasus I","Saint Damhnade","Saint Damian of Africa","Saint Damian of Rome","Saint Damian","Saint Dyfan","Blessed Damian dei Fulcheri","Saint Joseph de Veuster","Saint Thomas Xico","Saint Daniel of Cilicia","Blessed Daniel Karmasz","Saint Daniel of Padua","Blessed Daniel of Villiers","Saint Daniel the Stylite","Saint Joan of Arc","Saint Darlaugdach of Kildare","Saint Darerca","Saint Daria","Blessed Darío Acosta Zurita","Saint Darius of Nicaea","Blessed Marcelina Darowska","Saint Gioan Ðat","Saint Dathus of Ravenna","Saint Datius of Milan","Saint Datius of North Africa","Saint Dativa the Martyr","Blessed Daudi Okelo","Pope Blessed Victor III","King David","Saint David Galván-Bermúdez","Saint David of Wales","Blessed David Oglu David","Saint David Roldán-Lara","Saint David Uribe-Velasco","Blessed Toros Oghlou David",
    "Blessed Vicente Vilar David","Blessed John Davy","Saint Day","Saint Diodorus the Deacon","Saint James the Deacon","Saint Luke the Deacon","Saint Peter the Deacon","Saint Philip the Deacon","Saint Palladia","Blessed William Dean","Deborah the Prophetess","Saint Gregory Decapolites","Saint Declan","Saint Deicola","Venerable Leo Gustav Dehon","Saint Servus Dei","Saint Ita of Killeedy","Saint Jeanne of the Cross Delanoue","Blessed Pierre Delépine","Saint Clemente Ignacio Delgado Cebrián","Blessed Delphine","Saint Demetria of Africa","Saint Demetrius of Antioch","Saint Demetrius of Fragalata","Saint Demetrius of Ostia","Saint Demetrius of Sermium","Saint Demetrius of Veroli","Saint Dentlin of Soignies","Saint Aengus the Culdee","Saint Dionysius of Aquileia","Blessed Dionysius Fugixima","Saint Denis Ssebuggwawo","Saint Denis of Paris","Blessed Iacobus Matsuo Denji","Saint Deochar","Saint Deodatus of Lagny","Saint Derfel Gadarn","Saint Dermot of Inis Clothrann","Saint Derphuta of Amisus","Saint Joseph of Leonessa","Saint Desiderius of Pozzuoli","Saint Desiderius of Fontenelle","Blessed Didier of Thérouanne","Saint Dismas","Saint Deusdedit of Canterbury","Saint Devota","Pope Saint Gregory the Great","Blessed Diana d’Andalo","Saint Diarmaid of Armagh",
    "Saint Didacus","Saint Dichu","Blessed Diégo Josef of Cádiz","Saint Didorus","Saint Didymus of Alexandria","Saint James Kisai","Blessed Diego Ventaja Milán","Blessed Diego Oddi","Saint Vinh Son Nguyen The Ðiem","Blessed Jutta of Disibodenberg","Saint Digna of Cordoba","Saint Digna of Rome","Saint Vincent of Digne","Saint Paschasia of Dijon","Saint William of Dijon","Saint Diman Dubh of Connor","Blessed Dimbalac Oghlou Vartavar","Saint Ninian","Saint Dinooth","Saint Diomedes the Martyr","Saint Diomma of Kildimo","Saint Dionysia the Martyr","Saint Dionysia of Alexandria","Saint Dionysius of Corinth","Saint Hilary of Aquileia","Saint Dionysius of Asia","Pope Saint Dionysius","Saint Dioscorus of Kynopolis","Saint Dioscorus","Blessed John Discalceat","Saint Disibod of Disenberg","Blessed Raymond Lull","Saint Duje","Saint Domangard of Maghera","Blessed Domingo Iturrate Zubero","Saint Domenico Lentini",
    "Saint Domitius of Phrygia","Saint Dominic of Silos","Saint Domingo Henares de Zafra Cubero","Saint Domingo Ibáñez de Erquicia","Saint Modomnoc","Saint Dominic of North Africa","Saint James of the Marches","Saint Ðaminh Nguyen Van Hanh","Blessed Dominic Nihachi","Saint Dominic Savio","Blessed Dominic Shobyoye","Blessed Dominic Spadafora","Saint Ðaminh Trach Ðoài","Saint Dominic de Guzman","Saint Dominic de la Calzada","Blessed Dominic Magoshichi","Saint Dominic of Sora","Blessed Dominic of the Holy Rosary","Blessed Dominic Tomaki","Saint Dominic Uy Van Bui","Saint Ðaminh Nguyen Van Xuyên","Saint Domitian of Huy","Saint Domitian of Lerins","Saint Domitian of Philadelphia","Saint Flavia Domitilla of Terracina","Saint Domitius of Caesarea","Saint Domna of Nicomedia","Saint Domnina of Syria","Saint Domnio of Rome","Saint Donatian of Rheims","Saint Donat","Saint Donata of Rome","Saint Donatus of Ripacandida","Saint Donatus the Martyr","Saint Donatus of Africa","Saint Donatus the Martyr","Saint Donatus of Alexandria","Saint Donatus of Fiesoli","Blessed Petrus Donders","Saint Dwynwen","Saint Fintan of Doon","Saint Dorothy of Caesarea","Saint Julius the Veteran","Saint Dorotheus the Younger","Saint Dorotheus of Tyre","Blessed Dorothy of Montau","Saint Dorymedon the Senator","Saint Dositheus of Gaza","Saint Duthus of Ross","Blessed George Douglas",
    "Saint Drausinus of Soissons","Blessed Robert Drury","Saint Katharine Drexel","Saint Droctoveus","Saint Drogo","Saint Drostan","Saint Drouet of Auxerre","Saint Drusus of Syria","Saint Drusus of Tripoli","Saint Rose Philippine Duchesne","Blessed James Duckett","Blessed John Duckett","Blessed Peter de Duenas","Saint Jan of Dukla","Saint Dulcissima of Sutri","Saint Pierre Rose Ursule Dumoulin Borie","Saint Dunchaid O’Braoin","Blessed John Duns Scotus","Blessed Marco Antonio Durando","Blessed Guy of Durnes","Blessed Marie Rose Durocher","Blessed Giuseppe Benedetto Dusmet","Saint Godfried van Duynen","Saint Theneva","Saint Dymphna","Blessed Pavel Dzidzov","Saint Edbert of Lindisfarne","Saint Eanfleda of Whitby","Blessed Richard Leigh","Saint Edmund of East Anglia","Blessed Notburga","Saint Eberhard of Salzburg","Blessed Margareta Ebner","Saint Ebrulf of Ouche","Martyrs of Ebsdorf","Saint Egbert of Rathemigisi","Saint Egwin of Worcester","Saint Fulgentius of Ecija","Saint Miguel de Sanctis","Blessed Liduina Meneguzzi","Saint Edana of West Ireland","Saint Oudoceus","Venerable Edel Mary Quinn","Saint Edeyrn","Saint John of Edessa","Saint Mary of Edessa","Saint Edgar the Peaceful","Saint Osmund","Saint Edith of Tamworth","Saint Edith of Wilton","Saint Teresa Benedicta of the Cross","Saint Edmund Gennings","Blessed Edmund Rice","Saint Edmund Rich","Blessed Edoardo Giuseppe Rosaz","Blessed Edward James","Blessed Edward Oldcorne","Blessed Edward Joannes Maria Poppe","Blessed Edward Powell","Blessed Edward Shelley",
    "Blessed Edward Stransham","Saint Edward the Confessor","Saint Edward the Martyr","Blessed Edward Waterson","Saint Hedwig of Andechs","Blessed Edward Fulthrop","Saint Edwen of Northumbria","Saint Euphrosyne of Polotsk","Blessed Ekbert of Muensterschwarzach","Saint Egelwine of Athelney","Saint John of Lycopolis","Saint Julian of Egypt","Saint Macarius of Egypt","Martyrs of Upper Egypt","Saint Theodore of Egypt","Saint Isidore the Egyptian","Saint Eigrad","Saint Einhildis of Hohenburg","Saint Gregory of Einsiedeln","Saint Meinrad of Einsiedeln","Saint Eithne","Saint Elaeth the King","Saint Gertrude the Elder","Saint Gregory Nazianzen the Elder","Saint Melania the Elder","Saint Pachomius of Tabenna","Saint Swithbert","Saint Heldrad of Novalese","Blessed Eleanora","Eleazar the Patriarch","Venerable Elena Silvestri","Pope Saint Eleuterus","Saint Eleutherius","Saint Eleutherius of Rocca d’Arce","Saint Eleutherius of Tournai","Saint Elgiva of Shaftesbury","Saint Elian ap Erbin","Saint Elias of Caesarea","Saint Elias of Phunon","Blessed Elias del Socorro Nieves","Saint Eligius of Noyon","Blessed Helinand of Pronleroy","Saint Elizabeth of Hungary","Saint Elizabeth of the Visitation","Saint Elizabeth Ann Seton","Blessed Elizabeth of Mantua","Saint Jeanne Elizabeth des Bichier des Anges","Saint Elizabeth of Portugal","Saint Elizabeth of Schönau","Blessed Elizabeth Rose","Blessed Elizabeth Salviati","Blessed Roger of Ellant","Saint Ellidius","Saint Erasmus","Saint Eloque","Saint Elpidius the Martyr","Saint Expeditus of Melitene","Saint Elpidius of Lyon","Saint Illtyd","Saint Eufrasia of the Sacred Heart of Jesus Eluvathingal","Saint Elvan","Saint Gregory of Elvira","Saint Elzear","Saint Emebert of Cambrai","Saint Emerentiana","Saint Emerita of Rome","Saint Emidius of Ascoli Piceno","Saint Emilas of Cordoba","Saint Emilian of Lagny","Saint Emilian of Citra","Blessed Emilian Kovch","Saint Emiliana of Rome","Saint Jerome Emiliani","Saint Emily de Rodat","Blessed Émilie Tavernier Gamelin","Saint Emilina of Boulancourt","Saint Emily de Vialar","Saint Emma of Bremen","Blessed Gemma of Goriano","Blessed Hemma of Gurk","Blessed Maria Euthymia Üffing","Saint Emmanuel","Blessed Emmanuel d’Abreu","Saint Manuel Moralez","Saint Emmanuel Nguyen Van Trieu","Saint Emmeramus of Regensburg","Saint Iñigo of Oña","Saint Margaret of England","Saint Kennocha of Fife","Saint Enric de Osso y Cervello","Blessed Helen Enselmini","Saint Eobán of Utrecht","Saint Eochod of Galloway","Saint Éoghan of Ardstraw","Saint Eparchius of Caesarea","Saint Eparchius of Périgord","Martyrs of Ephesus","Saint Ephrem of Syria","Saint Ephysius of Sardinia","Saint Epigmenius of Rome","Saint Epimachus","Saint Epimachus of Rome","Saint Epiphanius of Pavia","Saint Epiphanius of Salamis","Saint Epipodius of Lyon","Saint Epictetus of Africa","Saint Epolonius","Saint Erbin of Cornwall","Saint Hermenland","Saint Ergnad of Ulster","Saint Erconwald of London","Saint Erembert of Toulouse","Saint Erentrude","Saint Erfyl","Saint Eric of Sweden","Saint Erinhard","Saint Ermenilda of Ely","Saint Hermylus","Saint Ermine","Saint Richard Pampuri","Saint Erminold of Prüfening","Saint Gaetano Errico","Blessed Gregorio Escribano","Saint Exuerantia of Troyes","Saint Ethelbert of East Anglia","Saint Etheldreda","Saint Ethelwin of Lindsey","Saint Ethelwold of Winchester","Saint Eubulus of Caesarea","Saint Eucharius of Trier","Saint Eucherius of Orleans","Saint John Eudes","Saint Eudocia of Heliopolis","Blessed Eufrasio of the Child Jesus","Saint Eugene of Africa","Saint Eugene","Saint Eugene","Saint Eugene of Cappadocia","Pope Saint Eugene I","Blessed Pope Eugene III","Saint Eugene de Mazenod","Saint Eugene of Milan","Saint Eugene the Martyr","Saint Eugenia of Rome","Blessed Eugenia Picco","Blessed Eugenia Ravasco","Blessed Eugenie Smet","Saint Eulogius of Constantinople","Saint Eulogius of Cordoba","Saint Eulogius of Tarragona","Saint Eunician of Crete","Saint Euphemia of Amisus","Saint Euphemia of Chalcedon","Saint Euphrasia","Saint Euphrasia of Amisus","Saint Euphrasius of Corfu","Saint Euphrosyne of Alexandria","Saint Euporus of Crete","Saint Euprepis of Rome","Saint Eurosia","Blessed Eurosia Fabris","Saint Eusebia of Hamage","Saint Eusebia of Marseilles","Saint Eusebius of Rome","Blessed Eusebia Palomino Yenes","Saint Eusebius of Fano","Saint Eusebius of Samosata","Saint Eusebius of Rome","Pope Saint Eusebius","Saint Eusebius of Cremona","Saint Eusebius of Vercelli","Saint Eustace of North Africa","Saint Eustace of Vilna","Blessed John Eustace","Saint Eustachius","Blessed Eustáquio van Lieshout","Saint Eustathius of Antioch","Saint Teresa Eustochio Verzeri","Saint Eustochium","Blessed Eustochium of Padua","Saint Eustochius of Tours","Saint Eustorgius II of Milan","Saint Eustorgius of Milan","Saint Eustosius of Antioch","Saint Euthymius the Great","Saint Euthymius the Illuminator","Saint Euthymius of Nicomedia","Saint Eutropia of Rheims","Saint Eutropius","Saint Eutropius of Pontus","Saint Eutychian the Martyr","Pope Saint Eutychian","Saint Eutychianus of Salamanca","Saint Eutychius of Pozzuoli","Saint Eutychius","Saint Evagrius","Saint Philip Evans","Saint Evaristus of Crete","Saint Eve of Dreux","Eve the Matriarch","Saint Eventius of Rome","Saint Eventius of Saragossa","Blessed Everard Hanse","Saint Evermod of Ratzeburg","Saint Wystan of Evesham","Saint Ewald the Fair","Saint Hywyn of Aberdaron","Blessed William Exmew","Saint Hermes the Exorcist","Saint Exsuperius of Pamphylia","Saint Exuperantius of Spoleto","Saint Exuperius","Saint Peter Julian Eymund","Blessed John Eynon","Saint Ezekiel Moreno y Diaz","Saint Peter Faber","Pope Saint Fabian","Saint Fabian of Catania","Saint Fabiola of Rome","Saint Fulgentius of Ruspe","Blessed John Righi of Fabriano","Saint Fachanan of Ross","Saint Faciolus","Blessed Fazzio of Verona","Saint Nicolás Factor-Estaña","Saint Foillan of Fosses","Saint Fanchea of Rossory","Blessed Margaret of Faenza","Saint Failbhe of Iona","Saint Foila","Saint Faith","Saint Faith","Saint Falco of Maastricht",
    "Blessed Falco of Cava","Blessed Falco the Hermit","Blessed Nazju Falzon","Saint Famianus of Compostela","Holy Family","Saint Fandilas of Penamelaria","Saint Phanurius","Blessed Mark Fantucci","Saint Thomas of Farfa","Saint Giovanni Antonio Farina","Saint Faro of Meaux","Saint Francesco Antonio Fasani","Blessed Maria Teresa Fasce","Blessed Leonid Feodorov","Blessed James Salomone","Saint Fausta of Sirmium","Saint Faustianus of Corfu","Saint Faustina Kowalska","Saint Faustinus","Saint Faustinus the Martyr","Saint Faustinus of Spoleto","Saint Faustus","Saint Faustus of Alexandria","Saint Faustus of Syracuse","Blessed Iacobus Hayashida","Blessed Richard Featherstone","Saint Febronia of Nisibis","Saint Waningus of Fécamp","Saint Fechin of Fobhar","Saint Phelim","Saint Felician of Africa","Saint Felician the Martyr","Saint Felician of Foligno","Saint Felicianus","Saint Felicissima the Blind","Saint Felicissimus of Campania","Saint Felicissimus the Deacon","Saint Felicissimus of Perugia","Saint Felicity of Carthage","Saint Felicity of Rome","Blessed Félicité Pricet","Saint Felinus of Perugia","Blessed Zygmunt Szcesny Felinski","Saint Felipe de Jesus","Blessed Felipe Ripoll Morata","Saint Felix of Heraclea","Saint Felix of Africa","Saint Felix of Lyons","Saint Felix of Aquileia","Saint Felix the Martyr","Saint Felix the Martyr","Saint Felix of Rome","Saint Felix of Cordoba","Saint Felix of Campania","Saint Felix of Rome","Saint Felix of Rome","Saint Felix of Nocera","Saint Felix","Saint Felix of Rome","Venerable Félix de Jesús Rougier","Pope Saint Felix I","Pope Saint Felix III","Pope Saint Felix IV","Saint Felix of Burgundy","Saint Felix of Fondi","Saint Felix of Monte Cassino","Saint Felix of Nicomedia","Saint Felix of Nola","Saint Felix of Rhuys","Saint Felix of Saragossa","Saint Felix of Seville","Saint Felix of Thyniss","Saint Felix of Valois","Blessed John Felton","Blessed James Fenn","Blessed John Fenwick","Saint Feock","Blessed Ferdinand of Portugal","Saint Ferdinand III of Castille","Saint Ferréolus of Grenoble","Saint Fergus the Pict","Saint Jacques Fermin","Saint Molagga of Fermoy","Blessed Isabella Fernandes","Blessed João Fernandes","Blessed João Fernandes","Saint José Fernández de Ventosa","Saint Ferreolus the Tribune","Saint Ferreolus of Uzès","Blessed Gabriel Ferretti","Saint Ferrutius","Saint Festus of Pozzuoli","Saint Festus of Tuscany","Saint Fiacre","Saint Fillan","Saint Fiace","Saint Maidoc of Fiddown","Saint Fidelis of Merida","Saint Fidelis the Martyr","Saint Fidelis of Sigmaringen","Saint Fidweten","Blessed Filip Geryluk","Saint Lucia Filippini","Saint Filippo Smaldone","Saint Tillo of Solignac","Saint Philomena","Saint Marciano José","Blessed Michaël Himonoya","Blessed Paul Himonoya","Saint Seraphina","Saint Finan of Iona","Saint Finbar of Wexford","Blessed John Finch","Blessed Ladislaus Findysz","Saint Gwinear","Saint Fingar of Hayle","Saint Finian Munnu","Saint Finian Lobhar","Saint Finlugh of Derry","Saint Henry of Uppsala","Saint Fintán of Clonenagh","Blessed Joachim of Fiore","Blessed Ioachim Díaz Hirayama","Saint William Firmatus","Saint Firminus of Gabales","Saint Firmus the Martyr","Saint Firmus","Saint Firmus of Rome","Saint John Fisher","Saint Raymond of Fitero","Saint William of York","Saint Winnoc of Wormhoult","Saint Florentius of Bangor","Saint Flavian of Carthage","Saint Flavian of Civitavecchia","Saint Flavitas","Saint Flocellus","Saint Flora the Martyr","Saint Flora of Cordoba","Saint Florentius the Venerable","Saint Florentian of North Africa","Saint Florentius of Carthage","Saint Florentius","Saint Florentius of Rheims","Saint Gregory of Tours","Saint Florentius of Seville","Saint Florentius of Strasbourg","Saint Florentius of Vienne","Saint Florentius of Vienne the Martyr","Saint Florentius the Martyr","Saint Jose Isabel Flores Varela","Saint Florian of Lorch","Saint Florian of Palestine","Saint Florus of Ostia","Saint Flos of Catania","Saint Flosculus of Orleans","Saint Nicholas of Flüe","Saint Francesco Fogolla","Blessed Thomas of Foligno","Blessed Fulk of Toulouse","Saint Honoratus of Fondi","Blessed Marie-Madeleine Fontaine","Blessed Mary Fontanella","Venerable Oreste Fontanella","Saint Wando of Fontenelle","Blessed Fortis Gabrielli","Blessed John Forest","Saint Padre Pio","Saint Fortchern of Trim","Blessed Victoria Strata","Saint Paula of Saint Joseph of Calasanz","Saint Fortunatus of Smyrna","Saint Fortunatus the Martyr","Saint Fortunatus","Saint Fortunatus of Aquileia","Saint Fortunatus of Rome","Saint Venantius Fortunatus","Forty Martyred Soldiers at Rome","Blessed Jordan Forzatei","Saint Vaast of Arras","Four Crowned Martyrs","Saint Peter Fourier","Saint Fragan","Saint Franca Visalta","Saint Jane of Valois","Blessed Francisco Martín Fernández de Posadas","Saint Frances of Rome","Blessed Francesca of Gubbio","Blessed Francesco Pianzola","Saint Gabriel of Our Lady of Sorrows","Blessed Francis Xavier Seelos","Blessed Francesco Spoto","Blessed Francesco dei Maleficii","Saint Francis Blanco","Saint Phanxicô Ðo Van Chieu",
    "Blessed Francis Galvez","Saint Francesc Gil de Federich de Sans","Saint François-Isidore Gagelin","Saint François Jaccard","Saint Francis of Girolamo","Saint Phanxicô Xaviê Hà Trong Mau","Venerable François-Marie-Paul Libermann","Blessed Francis Page","Blessed Francis Palau y Quer","Blessed Francis Patrizzi","Blessed Franciszek Rogaczewski","Saint Francis Solano","Saint Francis Trung Von Tran","Saint Francis Xavier","Saint Francis Xavier Can Nguyen","Saint Francis de Montmorency Laval","Saint Francis de Sales","Saint Francis of Nagasaki","Saint Francis of Paola","Saint Francisco of Saint Michael","Saint Francisco Marto","Venerable Francisco of the Child Jesus","Saint Franciso Shoyemon","Blessed François Peltier","Blessed Françoise Michau","Blessed Françoise Micheneau veuve Gillot","Blessed Françoise Pagis épouse Railleau","Blessed Françoise Suhard veuve Ménard","Blessed Ignacy Franczuk","Blessed Franz Jägerstätter","Blessed Pier Giorgio Frassati","Saint Paula Frassinetti","Blessed Frederick of Hallum","Saint Frederick of Utrecht","Blessed Frederick of Ratisbon","Saint Frideswide","Saint Joseph Freinademetz","Blessed Joseph of Freising","Saint Leontius of Fréjus","Blessed Pierre Frémond","Blessed Stefan Wincenty Frelichowski","Saint Friard","Saint Frithbert of Hexham","Saint Fridolin","Saint Frigidian of Lucca","Saint Lebuin of Deventer","Saint Wigbert of Fritzlar","Saint Frodobert of Troyes","Saint Froilan","Saint Fronto of Saragossa","Saint Fructuosus of Tarragona","Saint Fructos of Segovia","Saint Fulcran of Lodève","Saint Sturmi of Fulda","Saint Fursey of Péronne","Blessed Gundisalvus Fusai Chozo","Saint Fuscian","Blessed Tommaso Maria Fusco","Saint Gabdelas of Persia","Saint Gabinus","Saint Gavino of Sardinia","Blessed Michael Ghebre","Saint Gabriel de Duisco","Saint Gabriel Gowdel","Saint Jean-Gabriel-Taurin Dufresse","Saint Gabriel Lalemant","Blessed Gabriel Mary","Blessed Gabriel of Saint Magdalen","Saint Jean-Gabriel Perboyre","Saint Gaugericus of Cambrai","Saint Gaius of Alexandria","Saint Gaius of Nicomedia","Saint Gaius of Apamea","Saint Gaius of Saragossa","Saint Gaius of Milan","Blessed Gaius Xeymon","Saint Gajan","Saint Gall of Clermont","Saint Gal of Clermont","Saint Galation","Blessed Hippolytus Galantini","Saint Galdinus of Milan","Saint Walfrid","Saint Gall","Saint Tutilo of Saint Gall","Saint Galla of Rome","Saint Gallgo","Saint Gallicanus of Embrun","Saint Gallicanus","Venerable Silvio Gallotti","Saint Gamaliel","Blessed Gamelbert of Michaelsbuch","Saint Gamo of Brittany","Saint Godo of Oye","Blessed Jakob Gapp","Blessed John of Vercelli","Saint Garbhan","Blessed Garcia of Arlanza","Saint Gundisalvus Garcia","Saint Manuel González García","Blessed German Gardiner","Saint Hiduard","Saint Garibaldus of Regensburg","Saint Michael Garicoïts","Saint Gerinus","Saint Gervadius","Saint Thomas Garnet","Venerable Manuel Lozano Garrido","Saint Gatianus of Tours","Saint Gaucherius","Saint Gaudentia","Saint Gaudentius of Brescia","Saint Gaudentius of Arezzo","Saint Gaudentius of Gniezno","Saint Gaudentius of Novara","Saint Gaudentius of Ossero","Saint Gaudentius of Rimini","Saint Gaudentius of Verona","Saint Gaudiosus of Salerno","Saint Gaudiosus of Tarazona","Blessed Gaufridus","Saint Martin of Tours","Saint John Gualbert","Blessed Walter of Himmerode","Blessed Gausmarus of Savigny","Saint Walter of Pontoise","Blessed John Gavan","Saint Raymond of Toulouse","Saint Vitalis of Gaza","Saint Zeno of Gaza","Saint Gébétrude of Remiremont","Saint Gebizo","Blessed Gebrand of Klaarkamp","Saint Gebuinus of Lyons","Saint Gedeon of Besancon","Gideon the Judge","Saint Gelasius of Armagh","Saint Gelasius of Crete","Pope Saint Gelasius I","Pope Saint Gelasius II","Saint Julius of Gelduba","Saint Gemma Galgani","Saint Gemellus of Ancyra","Saint Geminian of Rome","Saint Geminian of Modena","Saint Geminus of Africa","Saint Geminus","Saint Gemus","Saint Genebald of Laon","Saint Gerebernus","Saint Generosus of Poitou","Saint Generosus","Saint Genesius of Lyon","Saint Genesius of Bèze","Saint Genesius of Rome","Saint Genevieve","Saint Gengulphus of Burgundy","Saint Genistus of Limoges","Saint Genitus","Saint Gennadius of Astorga","Saint Gennard","Saint Januarius of Naples","Saint Syrus of Genoa","Saint Valentine of Genoa","Saint Genulfus","Saint Gentian","Blessed Gentilis of Toringa","Saint Genuinus of Sabion","Blessed Godfrey of Le Mans","Saint Godfried van Melveren","Blessed Geoffrey of Peronne","Saint George","Saint George of Cordoba","Blessed George Gervase","Blessed George Haydock","Saint George Limniotes","Blessed Jurgis Matulaitis-Matulewicz","Blessed George Napper","Saint George Preca","Saint George of Lodeve","Saint George of Vienne","Blessed George Swallowell","Saint George the Younger","Saint John the Georgian","Saint Gerontius","Saint Gerald of Aurillac","Saint Gerald of Sauve-Majeure","Saint Gerald of Ostia","Saint Gerald of Mayo","Saint Gerald of Mâcon","Blessed Gerald of Fossanuova","Saint Gerald of Braga","Blessed Gerard of Sassoferrato","Saint Gerard of Gallinaro","Blessed Gerard Tintorio","Saint Gerard of La-Charite","Saint Gerard Majella","Blessed Gerard of Lunel","Blessed Gerard of Villamagna","Blessed Gerard of Orchimont","Saint Gerard of Potenza","Saint Gerard of Toul","Blessed Jeanne Gérard","Blessed Gerardesca","Saint Gherardino Sostegni","Saint Gerasimus of Palestine","Saint Gerbald of Saint Bertin","Saint Gerbold of Bayeux","Saint Gerebald of Chalons","Saint Geremarus","Blessed Peter de Geremia","Saint Gereon","Blessed James Gerius","Saint Gerlac of Valkenburg","Saint Gerland","Saint Germanus of Man","Saint Germana of Numidia","Saint Grimonia of Picardy","Saint Germanicus of Smyrna","Saint Germanus of Granfield","Saint Germanus of Normandy","Saint Germanus of Montfort","Saint Germanus of Paris","Saint Germerius of Toulouse","Saint Germoc","Saint Gerold of Evreux","Saint Gerold of Cologne","Saint Gerontius of Numidia","Saint Gerontius of Cervia","Saint Gertrude of Nivelles","Saint Gertrude the Great","Blessed Gertrude van Oosten","Saint Gerulph","Saint Geruntius of Italica","Saint Geruntius of Milan","Saint Gervase","Saint Gervinus","Blessed Gervinus of Aldenberg","Saint Getulius of Tivoli","Blessed Gezelin of Schlebusch","Saint Henry of Gheest","Saint Hildebert of Ghent","Blessed John of Ghent","Blessed Guy de Gherardesca","Madonna del Ghisallo","Saint Gislenus","Saint Godelieve","Saint Gibardus of Luxeuil","Saint Gibitrudis","Saint Gibrian","Blessed Ludwik Roch Gietyngier","Saint Gilbert de Moray","Blessed Gilbert of Neufontaines","Blessed Gilbert of Valenciennes","Saint Gilbert of Meaux","Saint Gilbert of Sempringham","Blessed Gilbert the Theologian","Saint Machabeo of Armagh","Saint Gildard of Rouen","Saint Gildas the Wise","Saint Gilduin of Dol","Saint Giles","Blessed Giles of Laurenzana","Blessed Giles of Santarem","Saint Joseph","Blessed Josaphata Mykhailyna Hordashevska","Saint Jane Antide Thouret","Saint Jeanne de Chantal","Saint Jeanne-Marie Kerguin","Blessed Giovanna Maria Bonomo","Blessed Jeanne Marie de Maille","Blessed Jeanne-Reine Prin","Blessed Jane Scopelli","Blessed Giovanna Soderini","Blessed Jane of Orvieto","Blessed Jane of Portugal","Blessed Jane of Segna","Blessed Jane of Toulouse","Blessed Giovanni Battista Mazzucconi","Saint Girald of Fontenelle","Blessed Josefa Naval Girbes","Saint Gregory of Girgenti","Saint Jerome","Saint Gisela","Blessed Gisella of Ungarn","Saint Gistilian","Blessed Giulia Valle","Saint Giulia Salzano","Saint Veronica Giuliani","Saint Joseph Luu","Venerable Giuseppe Carlo Vaser","Saint Joseph Marello","Saint Joseph Pignatelli","Blessed Giuseppina Nicoli","Saint Lawrence Giustiniani","Saint Justus of Trieste","Saint Gladys","Saint Glaphyra","Saint Glastian of Kinglassie",
    "Saint Gleb","Saint Kevin of Glendalough","Saint Glycerius of Milan","Saint Glodesind of Metz","Saint Glunshallaich","Saint Glyceria of Trajanopolis","Saint Glycerius of Nicomedia","Saint Romanos the Melodist","Saint William Gnoffi","Saint Goar of Aquitaine","Saint Goban","Saint Goban Gobhnena","Saint Gobert of Apremont","Saint Gobnata","Saint Gobrain of Vannes","Saint Godehard of Hildesheim","Saint Godeberta of Noyon","Saint Godric of Finchale","Saint Godwin of Stavelot","Saint Goeznoveus of Léon","Saint Gohardus of Nantes","Blessed Pavol Gojdic","Saint Golvinus of Leon","Blessed Gomidas Keumurjian","Saint Gonéri of Tréguier","Saint Guntramnus","Saint Gonzaga Gonza","Blessed Manuel Gómez González","Blessed Gundisalvus of Silos","Saint Goran","Saint Gorazd","Saint Zygmunt Gorazdowski","Saint Luke Alonso Gorda","Blessed Jordan of Saxony","Saint Gordian the Judge","Saint Gordian of Noviodunum","Saint Maria Goretti","Saint Gorfor of Llanover","Saint Gorgonia","Saint Gorgonius the Martyr","Saint Gorgonius of Nicomedia","Martyrs of Gorkum","Saint Gorman of Schleswig","Saint Gormeal of Ardoilen","Saint John of Gorze","Saint Gosbert of Osnabruck","Saint Goscelinus of Turin","Saint Goswin","Blessed Goswin of Clairvaux","Saint John of the Goths","Saint John Soan de Goto","Saint Gotteschalk","Saint Guesnoveus of Quimper","Saint Grace of Saragossa","Saint Gracia of Lérida","Saint Grace of Cornwall","Blessed John of Grace-Dieu",
    "Saint Gracilian","Blessed Giovanni Gradenigo","Saint Graecina of Volterra","Saint Theodule of Grammont","Saint John Grande","Saint Stephen of Muret","Saint Grata of Bergamo","Saint Gratian of Perugia","Saint Gratus of Chalons","Saint Gratus","Saint Gratus of Oloron","Saint Isaac the Great","Saint Nerses the Great","Blessed William of Maleval","Saint Gredifael","Blessed Hugh Green","Blessed Thomas Reynolds","Blessed William Greenwood","Blessed Hryhorii Khomyshyn","Blessed Gregory of Besians","Saint Gregory of Fragalata","Pope Saint Gregory II","Pope Saint Gregory III","Blessed Hryhorii Lakota","Blessed Gregorio López","Saint Gregory Makar","Saint Gregory of Langres","Blessed Gregory of Nonantula","Saint Gregory of Nyssa","Saint Gregory of Ostia","Saint Gregory of Spoleto","Saint Gregory of Terracina","Blessed Gregory of Traguio","Saint Gregory of Utrecht","Saint Gregory Thaumaturgus","Saint Gregory the Illuminator","Pope Saint Gregory VII","Pope Blessed Gregory X","Saint Hugh of Grenoble","Blessed James Grissinger","Saint Grimbald","Saint Grimoaldus of Pontecorvo","Saint Mary Hermina Grivot","Blessed Nikolaus Gross","Blessed John Grove","Saint Grwst","Our Lady of Guadalupe","Blessed Guala of Brescia","Blessed Vivaldus","Blessed Hugolinus of Gualdo Cattaneo","Blessed Gualfardus of Augsburg","Blessed Pascual Araguàs y Guàrdia","Saint Guarinus of Sion","Saint Guarinus of Palestrina","Saint Guasacht","Saint Rudolph of Gubbio","Saint Gudelia","Saint Plechelm of Guelderland","Saint Gundelindis of Niedermünster","Blessed Iacobus Bunzo Gengoro","Saint Guénhaël","Saint Guenninus","Saint Guerembaldus","Saint Theodore Guerin","Saint Guevrock","Blessed Elena Guerra","Blessed Guerricus","Saint Guibertus of Gorze","Saint Wiborada of Gall","Saint Guy of Pomposa","Blessed Guy of Vicogne","Saint Gwerir of Liskeard","Blessed Guillaume Répin","Blessed Guilminus","Saint Winifred of Wales","Saint Guinizo","Saint Guinoc","Saint Guitmarus","Saint Rafael Guízar y Valencia","Saint Gumesindus","Saint Gummarus","Saint Gundebert of Sens","Saint Gundebert of Gumber","Blessed Gundekar of Eichstätt","Saint Gundenis of Carthage","Blessed Gundisalvus of Azebeyro","Blessed Gonçalo Henriques","Blessed Gundisalvus of Lagos","Saint Gundisalvus","Saint Gundulphus","Saint Gunifort","Blessed Gunther","Saint Gunthiern","Saint Gunthildis of Ohrdruf","Saint Gurias of Edessa","Saint Gurloes of Sainte-Croix","Saint Guthagon","Saint Guy of Baume","Blessed Guy Maramaldi","Blessed Guy Vignotelli","Saint Gwen","Saint Gwendoline","Saint Gwinoc","Saint Gwrnerth","Saint Richard Gwyn","Saint Gwynllyw","Saint Vimin of Holywood","Saint Gyavire","Habakkuk the Prophet","Saint Habentius of Cordoba","Blessed Haberilla","Saint Herbert of Tours","Saint Habet-Deus","Saint Mechtilde of Helfta","Saint Hadelin of Chelles","Saint Hadelin of Lobbes","Saint Haduin of Le Mans","Saint Hadulph of Arras","Saint Hadulph","Saint Hermes of Moesia","Blessed John Haile","Saint Hallvard of Oslo","Saint Thorfinn","Saint Libentius of Hamburg","Blessed Thomas Holland","Saint Olaf II","Blessed Levkadia Harasymiv","Saint Hardoin of Brittany","Blessed Harduin","Saint Hardulph","Saint Herulph of Langres","Harold of Gloucester","Blessed William Harrington","Blessed William Hart","Blessed Hartwig of Salzburg","Saint Haruch of Werden","Saint Herve","Saint Trudo of Hesbaye","Saint Hatebrand","Blessed Hatto of Ottobeuren","Saint Hedda the Abbot","Saint Hedda of Wessex","Saint Hedwig, Queen of Poland","Saint Hegesippus of Jerusalem","Blessed Herman of Heidelberg","Blessed Otto of Heidelberg","Blessed Jutta of Heiligenthal","Saint Heimrad","Saint Helanus","Saint Helen of Auxerre","Blessed Helen of Poland","Saint Helen of Oehren","Saint Helen of Burgos","Blessed Helen of Hungary","Saint Helen of Skofde","Saint Olga of Kiev","Saint Helena","Saint Hélier of Jersey","Saint Joachim","Saint Heliconis of Thessalonica","Saint Helimenas","Saint Heliodorus the Martyr","Saint Heliodorus","Saint Heliodorus of Altinum","Saint Heliodorus of Persia","Saint Heliodorus of Antioch","Saint Heliodorus of Pamphylia","Saint Helladius","Saint Helladius of Rome","Saint Helladius of Toledo","Saint Ivo of Kermartin","Blessed Helwisa","Saint Emeterius of Calahorra","Saint Henedina of Sardinia","Blessed Henry the Hagiographer","Saint Henry II","Saint Henry Morse","Saint Henry of Coquet",
    "Blessed Henry of Zweifalten","Blessed Henry Suso","Saint Henry Walpole","Blessed Henry Zdick","Blessed Henryk Kaczorowski","Saint Theopemptus of Nicomedia","Saint Heraclas of Alexandria","Saint Heracleas of Thrace","Saint Heraclides of Alexandria","Saint Heraclius of Sens","Saint Heraclius of Porto Romano","Saint Heraclius of Todi","Saint Heraclius the Martyr","Saint Heraclius of Carthage","Saint Heradius of Noyon","Saint Irais","Saint Martin of Vertou","Saint Herbert Hoscam","Saint Herculanus of Porto Romano","Saint Herculanus the Soldier","Saint Herculanus of Perugia","Blessed Herculanus of Piegare","Saint Herebald of Bretagne","Saint Herena the Martyr","Saint Herenia of North Africa","Saint Hereswitha","Saint Heribaldus of Auxerre","Saint Heribert of Cologne","Saint Herlindis","Blessed Herluin","Saint Hermagorus of Aquileia","Blessed Herman Joseph","Blessed Herman of Zahringen","Blessed Herman of Scheda","Saint Hermas of Rome","Saint Hermas of Rome","Saint Hermas of Myra","Saint Hermenegild","Saint Hermengild","Saint Hermes of Numidia","Saint Hermes of Rome","Saint Hermes of Heraclea","Saint Hermias of Comana","Saint Hermione","Saint Hermippus","Saint James the Hermit","Saint Victor the Hermit","Saint Hermocrates","Saint Hermogenes of Melitene","Saint Hermogenes of Melitene","Saint Hermogenes of Syracuse","Saint Hermogenes of Alexandria","Saint Hermogenes","Saint Hermogius of Tuy","Saint Hermolaus","Saint Jerome Hermosilla","Saint Hernan","Saint Herodion of Patras","Saint Heron of Alexandria","Saint Heron of Antioch","Saint Heron of Alexandria","Saint Heros of Satala","Saint Herundo of Rome","Saint Herveus of Tours","Blessed Herveus","Saint Mary Elizabeth Hesselblad","Saint John the Silent","Saint Hesychius of Gibraltar","Saint Hesychius of Durostorum","Saint Hesychius","Saint Hesychius the Martyr","Saint Hesychius","Saint Hesychius of Mytilene","Saint Hesychius of Alexandria","Blessed John Hewett","Saint Ia","Saint Hygbald",
    "Saint Hidulf of Moyenmoutier","Saint Hidulphus of Hainault","Saint Jarlath","Saint Giuse Hiên","Saint Hieron of Mytilene","Saint Hieron of Mytilene","Saint Hieronides of Alexandria","Saint Jerome Lu","Saint Hierotheus","Saint Hieu","Blessed Peter Higgins","Saint Hilaria of Augsburg","Saint Hilaria the Martyr","Saint Hilaria of Rome","Saint Hilarinus of Ostia","Saint Hilarinus","Saint Hilarion of Gaza","Saint Hilarion of Ancyra","Pope Saint Hilary","Saint Hilary the Martyr","Saint Hilary of Galeata","Saint Hilary of Pavia","Saint Hilary of Toulouse","Saint Hilary of Carcassone","Saint Hilary the Hermit","Saint Hilary of Vulturno","Saint Hilary of Dijon","Saint Hilary of Mende","Saint Hilary of Viterbo","Saint Hilda of Whitby","Saint Hildebrand of Saint-Gilles","Saint Hildegard von Bingen","Blessed Hildegard the Empress","Saint Hildegrin of Chalons","Saint Hildegund","Saint Hildelith of Barking","Saint Hildemar of Beauvais","Saint Hildemarca of Fécamp","Saint Marquard of Hildesheim","Blessed Hilsindis","Saint Hiltrude of Liessies","Saint Himelin","Saint Himerius of Amelia","Saint Himerius","Blessed Imana of Loss","Saint Hipparchus of Samosata","Saint Hippolytus of Antioch","Saint Hippolytus of Africa","Saint Hippolytus of Saint-Claude","Saint Hippolytus","Saint Hippolytus of Rome","Saint Hirenarchus of Sebaste","Saint Ioachim Hao","Blessed Sidney Hodgson","Blessed Ida of Hohenfels","Blessed Margaret of Hohenfels","Saint Vicelin of Oldenburg","Holy Innocents","Blessed John of Vallombrosa","Saint Homobonus of Cremona","Saint Honestus of Nimes","Saint Honorata of Pavia","Saint Honoratus of Milan","Saint Honorius of Amiens","Saint Honoratus of Potenza","Saint Honoratus of Vercelli","Saint Honoratus of Toulouse","Saint Honoratus of Ostia","Saint Honoratus of North Africa","Saint Honorina","Saint Honorius of Ostia","Saint Aaron of Brettany","Saint Honorius of Alexandria","Saint Hope","Pope Saint Hormisdas","Saint Hormisdas of Persia","Saint Horres of Nicaea","Saint Salvator of Horta","Saint Hortulanus of North Africa","Hosea the Prophet","Saint Julian the Hospitaller","Saint Hospitius of Cap-Saint-Hospice","Saint John Houghton","Saint Margaret of Hulme","Blessed William Howard","Blessed Rabanus Maurus","Blessed Hroznata of Bohemia","Saint Jonas of Hubaham","Saint Hubert of Liege","Blessed James Thompson","Saint Hugh dei Lippi-Uguccioni","Blessed Hugh More","Blessed Hugh Taylor","Saint Hugh of Cluny","Blessed Hugh of Fosse","Blessed Hugh of Macon","Blessed Hugh of Marchiennes","Blessed Hugh of Montaigu","Saint Hugh of Novara","Saint Hugh of Rouen","Blessed Hugh of Sassoferrato","Blessed Hugh of Vaucelles","Hugh the Little","Blessed Hugolinus Magalotti","Saint Hugolinus of Morocco","Blessed Hugolinus Zefferini","Blessed Humbeline of Jully","Blessed Humbert","Blessed Humbert of Clairvaux","Blessed Humbert III of Savoy","Saint Humbert of Pelagius","Blessed Humbert of Romans","Saint Humility","Saint Humphrey of Prüm","Blessed Humphrey Middlemore","Saint Hunegund of Homblieres","Blessed John of Hungary","Blessed Ladislas of Hungary","Saint Margaret of Hungary","Blessed Nicholas of Hungary","Saint Hunger of Utrecht","Saint Hunna of Alsace","Saint Huno","Blessed Ivetta of Huy","Saint Hyacinth the Soldier","Saint Hyacinth of Caesarea","Saint Hyacinth of Amastris","Saint Hyacinth","Saint Hyacinth","Saint Hyacinth","Saint Hyacinth of Lucania","Saint Hyacinth of Rome","Blessed Jacinto Orfanell-Prades","Blessed José Negro Maroto","Saint Hyacintha of Mariscotti","Saint Hydroc","Pope Hyginus","Saint Joseph the Hymnographer","Saint Hypatius","Saint Hypatius of Chalcedon","Saint Hypatius of Tripoli","Saint Hypatius of Constantinople","Saint Hypatius of Gangra","Saint Hyperechios","Saint Ia of Persia","Saint James the Greater","Saint Jarlath of Tuam","Saint Josaphat","Saint Ibar of Meath","Saint Louis Ibaraki","Saint Paul Ibaraki","Saint Ida of Boulogne","Saint Ida of Herzfeld","Blessed Ida of Louvain","Saint Ida of Nivelles","Blessed Ida of Toggenburg","Blessed Idesbald of Dunes","Saint Idus of Ath-Fadha","Saint Justinian","Blessed Ignatius Jorge-Fernandes","Saint Ignatius of Laconi","Saint Ignatius of Antioch","Saint Ignatius of Loyola","Blessed Ignazio Maloyan","Saint Ildephonsus of Toledo","Saint Illadan of Rathlihen","Saint Illuminata of Todi","Saint Lawrence the Illuminator","Saint Illuminatus of San Severino","Blessed Illuminatus","Saint Lawrence Mary Joseph Imbert","Blessed Imelda Lambertini","Saint Imina of Würzburg","Saint Ina of Wessex","Saint Raynerius Inclusus","Saint Indaletius of Urci","Saint Indes of Nicomedia","Saint Indract","Infant Jesus of Prague","Saint Ingen of Alexandria","Blessed John Ingram","Saint Ingrid of Sweden","Saint Inischolus of Corfu","Saint Senan","Saint Injuriosus of Auvergne","Saint Innocent of Le Mans","Saint Innocent of Sirmium","Pope Saint Innocent I","Pope Blessed Innocent V","Saint Innocent of Tortona","Saint Innocent of Agaunum","Saint Inocencio de la Immaculada","Saint Jodocus","Saint Machar of Aberdeen","Martyrs of Iona","Saint Ronan of Iona","Saint Iphigenia","Saint Irchard","Blessed John Ireland","Blessed Marianus Scotus","Saint Palladius of Ireland","Blessed William Ireland","Saint Irenaeus the Soldier","Saint Irenaeus of Sirmium","Saint Irenaeus of Pentapolis","Saint Irenaeus of Armenia","Saint Irenaeus of Thessalonica","Saint Irenaeus of Lyons","Saint Irenaeus of Chiusi","Saint Irenaeus of Rome","Saint Irenaeus of Rome","Saint Irene","Saint Irene of Rome","Saint Irene of Thessalonica","Saint Irene of Egypt","Saint Irene","Saint Irenion","Blessed Irmengard","Saint Irmina of Oehren","Blessed Jildo Irwa","Saint Isacius of Nicomedia","Saint Isaac of Constantinople","Saint Isaac of Gnesen","Saint Isaac of Beth-Seleucia","Saint Isaac Jogues","Saint Isaac of Cordoba","Saint Isaac of Monteluco","Blessed Isabella of France","Isaiah the Prophet","Saint Isaias the Martyr","Saint Isaias","Saint Isaias Boner","Saint Isarnus of Toulouse","Saint Isaurus of Apollonia","Saint Isberga","Saint Ischyrion","Saint Isidore of Alexandria","Saint Isidore of Seville","Saint Isidore of Cordoba","Saint Isidora of Egypt","Saint Isidore of Besançon","Saint Isidore of Alexandria","Saint Isidore of Nitria","Saint Isidore of Pelusium","Saint Maughold","Saint Ismael of Menevia","Saint Ismael of Chalcedon","Saint Rémi Isoré","Saint Israel of Limoges","Saint Isserninus of Ireland","Saint Ithamar of Rochester","Saint Ivan the Hermit","Blessed Ivan Merz","Saint Joseph of Volokolamsk","Blessed Ivan Ziatyk","Blessed Ioannes Iwanaga","Saint Ivo of Huntingdonshire","Saint Iwi","Saint Jacinta Marto","Blessed Jacques Désiré Laval","Saint Jacob of Nisibis","Blessed Jacoba de Settesoli","Saint James the Lesser","Saint Giustino de Jacobis","Blessed James of Voragine","Blessed Jacques Ledoyen","Saint Jacut","Saint Jader of Sigum","Blessed Yakym Senkivsky","Saint Jambert of Canterbury","Saint James of Persia","Blessed James Oldo","Saint James Intercisus","Saint Jacobus Lacops","Saint Giacôbê Ðo Mai Nam","Blessed Jacques Salès","Blessed James Walworth","Saint James of Numidia","Blessed James of Nocera","Blessed James of Padua","Saint James of Persia","Saint James of Samosata","Saint James of Sasseau","Blessed James of Strepar","Saint James of Tarentaise","Saint James of Toul","Saint James the Syrian","Blessed Roger James","Saint Jan Sarkander","Catholic Encyclopedia – Blessed John Ruysbroeck","Saint Stanislaw of Jesus and Mary","Saint Jeanne Jugan","Saint Jeanne de Lestonnac","Saint Januarius of Heraclea","Saint Januarius of Numidia","Saint Januarius of Africa","Saint Januarius of Corfu","Saint Januarius of Rome","Saint Januarius","Saint Januarius of Carthage","Saint Januarius the Deacon","Jared the Patriarch","Venerable Pauline-Marie Jaricot","Saint Jason of Tarsus","Saint Jason the Martyr","Blessed Jean-Baptiste Lego","Saint John Francis Regis","Saint John de la Lande","Saint John Maria Muzeyi","Blessed Jean-Martin Moÿe",
    "Blessed Jean Ménard","Saint Jean-Théophane Vénard","Blessed Rosalie Rendu","Saint Jenaro Sánchez DelGadillo","Jeremiah the Prophet","Blessed Ion Costist","Saint Jeremias of Cilicia","Saint Jeremias of Cordoba","Saint Jerome of Pavia","Blessed Jerome of Vallumbrosa","Saint Hieronymus van Weert","Saint Macarius of Jerusalem","Blessed Mariana of Jesus","Saint Mary Ann de Paredes","Blessed Joachim Piccolomini","Saint Joaquín Royo Pérez","Saint Joachim Sakakibara","Saint Joaquina Vedruna Vidal de Mas","Saint Joanna the Myrrhbearer","Blessed Juana Maria Condesa Lluch","Saint John the Almoner","Martyred Jesuit Missionaries of Brazil","Saint Joannicus of Mount Olympus","Saint Joavan of Brittany","Job","Saint Joel of Pulsano","Joel the Prophet","Blessed Johann Nepomuk von Tschiderer","Saint Johannes Ri","Saint John of Rome","Saint John the Physician","Saint John of Rome","Saint John of Rome","Saint John of Moyenmoutier","Saint Giovanni of Pavia","Saint John of Persia","Saint John of Tuscany","Blessed John Angelo Porro","Saint Ioannes Baptista Luo Tingyin","Blessed John Baptist Machado de Tavora","Blessed John Baptist Scalabrini","Blessed John Baptist Spagnuolo","Saint John Baptist de La Salle","Saint Gioan Baotixta Ðinh Van Thành","Blessed Giovanni Battista Zola","Saint Jón Helgi Ögmundarson","Pope Saint John I","Saint John I of Naples","Saint John Joseph of the Cross","Saint John Kemble","Saint John Kisaka","Blessed Ioannes Kisaku","Saint John Kukuzelis","Saint Josaphat Kuncevyc","Saint Giovanni of Triora","Blessed John Larke","Blessed John Lockwood","Saint John Leonardi","Blessed John Licci","Saint John Lloyd","Saint Juan Macías","Blessed John Maki","Blessed John Marinoni","Saint Mark the Evangelist","Saint John Maron","Blessed Jean-Marie du Lau d’Alleman","Blessed John Mason","Blessed Ioannes Motoyama","Blessed John Morosini","Blessed John Munden","Blessed Ioannes Onizuka Naizen","Blessed Ioannes Nagata Matashichi","Saint Jean-Pierre Néel","Blessed John Nelson","Saint John Nepomucene","Saint John Nepomucene Neumann","Blessed Juan Nepomuceno Zegrí y Moreno","Blessed John Henry Newman","Blessed John Nutter","Saint John Ogilvie","Saint Nicholas Owen","Saint John Payne","Blessed John Pelingotto","Blessed John Pibush","Saint John Plessington","Saint John Rigby","Saint John Roberts","Blessed John Robinson","Blessed John Roche","Blessed John Rochester","Blessed John Rugg","Blessed John Saziari","Saint John Sergius","Blessed John Shert","Blessed John Slade","Blessed Ivan Sleziuk","Blessed John Soreth","Saint John Southworth","Blessed John Speed","Saint John Stone","Blessed John Storey","Blessed Ioannes Tanaka","Blessed Giovanni Tavalli","Blessed John Thorne","Saint John Vincent","Blessed Ioannes Yoshida Shoun","Blessed Ioannes Yago","Blessed John Zakoly","Saint John Zedazneli","Saint John de Brebeuf","Saint John de Brito","Blessed Juan de Mayorga","Blessed John de Montmirail","Blessed John of Montfort","Saint Juan de Ortega","Saint Juan de Ribera","Blessed Juan de Zafra","Blessed John del Prado","Saint John of God","Saint Giovanni of Lodi","Saint John of Pulsano","Saint John of Matha","Saint John of Montemarano","Saint John of Nicomedia","Saint Joannes Lenaerts","Saint John the Syrian of Pinna","Blessed John of Penna","Blessed John of Perugia","Saint John of Ratzenburg","Saint John of Ravenna","Saint Jean of Réomay","Blessed John of Saint Martha","Blessed John of Salerno","Blessed Juan de San Martín","Blessed John the Spaniard","Saint John of Tuy","Blessed John of Valence","Saint John of Verona","Blessed John of Warneton","Saint John of the Cross","Saint John the Saxon","Saint John the Wonder Worker","Blessed Lawrence Richardson","Blessed Robert Johnson","Jonah the Prophet","Saint Jonas","Saint Jonas of Muchon","Saint Jonatus","Saint Jorandus of Kergrist","Blessed Jordan of Pisa","Blessed Jordan of Pulsano","Blessed Yosafat Kotsylovskyi","Blessed José Juan Perot y Juanmarti","Saint Josemaria Escriva","Saint José María Robles Hurtado","Saint Jose Maria de Yermo y Parres","Saint José María Díaz Sanjurjo","Saint José María Rubio y Peralta","Blessed Jose Tàpies y Sirvant","Blessed José Trinidad Rangel y Montaño","Venerable Josefa Segovia Morón","Saint Joseph the Martyr","Saint Raphael Kalinowski","Saint Giuse Nguyen Duy Khang","Blessed Jozef Kowalski","Saint Joseph Marchand","Saint Joseph Mary Tomasi","Blessed Joseph Moreau","Saint Joseph Moscati","Saint Giuse Nguyen Ðình Nghi","Saint Giuse Ðang Van Viên","Venerable Joseph-Amand Passerat","Saint Giuse Nguyen Ðình Uyen","Saint Joseph Sebastian Pelczar","Saint Giuse Lê Dang Thi","Saint Iosephus Zhang","Saint Joseph Zhang Dapeng","Saint Iosephus Yuan Zaide","Saint Joseph Vaz","Saint Joseph of Arimathea","Saint Joseph of Palestine","Saint Joseph of Persia","Saint Mary Joseph Rosello","Blessed Josephine Vannini","Joshua the Patriarch","Saint Jovinus the Martyr","Saint Jovinus of Rome","Saint Jovita","Blessed Juan of Tlaxcala","Saint Juan del Castillo Rodríguez","Venerable Juan Vicente of Jesus and Mary","Saint Jucunda of Reggio Aemilia","Saint Jucunda of Nicomedia","Saint Jucundian","Saint Jucundinus of Troyes","Saint Jucundus of Africa","Saint Jucundus of Rheims","Saint Judas Cyriacus","Saint Judith of Niederaltaich","Saint Jude Thaddeus","Saint Judicaël","Saint Judith of Prussia","Saint Ursula Ledochowska","Venerable Julia Navarrete y Guerrero","Blessed Julia of Arezzo","Saint Julia of Carthage","Saint Julia of Corsica","Saint Julia of Lisbon","Saint Julia of Merida","Saint Julia of Nicomedia","Saint Julia of Saragossa","Saint Julia of Troyes","Saint Julia the Martyr","Saint Julian of Ankyra","Saint Julian of North Africa","Saint Julian of Alexandria","Saint Julian the Martyr","Saint Julian of Carthage","Saint Julian of Alexandria","Saint Julian the Confessor","Saint Julian of Mesopotamia","Saint Julian of Syria","Saint Julian the Martyr","Saint Julian of Ankyra","Saint Julian of Africa","Blessed Julian Cesarello de Valle","Blessed Julian Majali","Blessed Julian of Saint Augustine","Blessed René-Julien Massey","Saint Julian Sabas the Elder","Saint Julian of Le Mans","Saint Julian of Lyon","Blessed Julian of Norwich",
    "Saint Julian of Sora","Saint Julian of Syria","Saint Julian of Toledo","Saint Juliana of Amisus","Saint Juliana of Ptolemais","Saint Juliana of Stobylum","Saint Juliana of Tarsus","Saint Juliana Falconieri","Blessed Juliana Puricelli","Saint Juliana of Nicomedia","Saint Juliana of Pavilly","Saint Julitta of Iconium","Saint Julius of Numidia","Saint Julius of Caerleon","Pope Saint Julius I","Saint Julius of Novara","Saint Philibert of Jumièges","Saint Juniper","Saint Junipero Serra","Saint Jurmin","Saint Justina of Seville","Saint Justa of Carthage","Saint Justin of Paris","Saint Justin of Rome","Saint Justin of Trier","Saint Justin Martyr","Saint Justina of Mainz","Saint Justina of Padua","Saint Justus the Potter","Saint Justus of Vienne","Saint Justus of Campania","Saint Justus of Alcala","Saint Justus of Spain","Saint Justus of Lyons","Saint Justus of Troyes","Saint Juthware","Saint Juvenal of Narni","Saint Juventius of Antioch","Blessed Khodianin Oghlou Kadir","Blessed Mary Restituta Kafka","Blessed Kamen Vitchev","Blessed Karl Leisner","Blessed Maria Karlowska","Saint Kessog","Saint Kea","Saint Stanislas Kazimierczyk","Saint Kebius","Saint Kenelm","Saint Kennera","Saint Kenneth of Wales","Saint Kevoca of Kyle","Saint Kieran","Saint Keverne","Saint Kilda","Saint Kilian of Inishcaltra","Saint Trien of Killelga","Blessed Leonardus Kimura","Saint Richard the King","Saint Kinnia","Saint Luke Kirby","Saint Mukasa Kiriwawanvu","Blessed Richard Kirkman","Saint Kizito","Saint Nicholas of Myra","Saint Maximilian Kolbe","Blessed Mykola Konrad","Saint Stanislaus Kostka","Blessed Kouradji Oghlou Tzeroum","Blessed Zenon Kovalyk","Blessed Michaël Kozal","Blessed Margareta Kratz","Blessed Wisinto of Kremsmünster","Saint Macedonius Kritophagos","Saint Michaël Kurobyoie","Saint Kyneburga of Castor","Saint Kyneswide of Castor","Saint Lazaro of Kyoto","Saint Lactali of Freshford","Saint Ladislas","Blessed Ladislas of Gielniów","Saint Lamalisse","Saint Lambert of Maastricht",
    "Saint Nerses Lambronazi","Saint Landericus of Paris","Saint Lanoald of Maastricht","Blessed Richard Langley","Saint Urban of Langres","Venerable Luis Lapuente","Saint Salvador Lara Puente","Saint Largus","Saint Pelagius of Laodicea","Saint Largus of Aquileia","Saint Laserian of Leighlin","Saint Paul of Latros","Saint Laura of Saint Catherine of Siena","Blessed Laura Vicuña","Saint Lawrence of Rome","Saint Lawrence O’Toole","Saint Laurentius Bai Xiaoman","Saint Lawrence of Novara","Saint Laurentinus of Carthage","Saint Laurentius of Carthage","Saint Laurian of Seville","Blessed Martyrs of Laval","Saint Lawdog","Saint Lawrence Wang","Saint Lorenzo Ruiz of Manila","Saint Robert Lawrence","Saint Lazarus","Saint Lazarus of Milan","Saint Lazarus Zographos","Saint Peregrine Laziosi","Saint Solomon le Clerq","Saint Romanus of Le Mans","Saint Lea of Rome","Saint Leander of Seville","Saint Livinus","Saint Mucius the Lector","Blessed Richard Sergeant","Blessed Roger Lefort","Saint Leodegarius of Autun","Blessed René Lego","Saint Lelia","Saint Philadelphus of Lentini","Saint Leo of Patera","Saint Leo of Africa","Pope Saint Leo the Great","Pope Saint Leo II","Pope Saint Leo III","Pope Saint Leo IV","Saint Leolucas of Corleone","Saint Leo of Rouen","Saint Leobard of Tours","Saint Leobinus of Chartres","Saint Leocadia of Toledo","Saint Martin of Leon","Saint Vincent of Léon","Venerable Leonard Lessius","Saint Leonard Murialdo",
    "Saint Leonard of Avranches","Saint Leonard of Noblac","Saint Leonard of Port Maurice","Saint Leonard of Reresby","Saint Leonidas of Alexandria","Saint Leonides the Martyr","Saint Leontia the Martyr","Saint Leontinus of Braga","Saint Leontius of Saintes","Saint Leontius of Perga","Saint Leopardinus of Vivaris","Blessed Leopoldina Naudet","Saint Leucius of Apollonia","Saint Leudomer","Saint Leutfridus of La-Croix","Venerable Librada Orozco Santa Cruz","Saint Liberata of Pavia","Saint Liberata of Como","Saint Wilgefortis","Saint Liberatus of Rome","Saint Oliver of Ancona","Saint Liberius of Ravenna","Saint Liborius of Le Mans","Martyrs of Lichfield","Saint Lidanus of Sezze","Blessed Stephen of Liège","Saint Peter Lieou","Saint Liephard of Cambrai","Saint Llewellyn","Saint Lillian of Cordoba","Blessed Salvatore Lilli","Saint Munchin of Limerick","Saint Martial of Limoges","Blessed Romeo of Limoges","Saint Lindru of Partois","Pope Saint Linus","Saint Szymon of Lipnica","Saint Maxima of Lisbon","Saint Verissimus of Lisbon","Venerable Marguerite Parigot","Saint Ludger of Utrecht","Saint Simeon the Logothete","Saint Lollian of Samosata","Saint Loman of Trim","Saint Lomer of Corbion","Saint Longinus the Centurian","Saint Longinus of Rome","Saint Vincenza María López y Vicuña","Blessed Lorenzo Salvi","Our Lady of Loreto","Blessed Margaret of Lorraine","Saint Louis IX","Saint Louis-Marie Grignion de Montfort","Saint Louis Martin","Saint Mutien Marie Wiaux","Blessed Louis-Zéphirin Moreau","Saint Louis of Toulouse","Blessed Louise Bessay de la Voûte","Saint Louise de Marillac","Blessed Louise of Savoy","Blessed Luchesius","Saint Lucy of Syracuse","Saint Lucian","Saint Lucian of Tripoli","Saint Lucidius of Verona","Saint Lucilla","Saint Lucilla of Rome","Saint Lucius the Soldier","Saint Lucius of North Africa","Saint Lucius of Carthage","Saint Lucius of Caesarea","Saint Lucius of Rome","Saint Lucius","Pope Saint Lucius I","Saint Lucy of Rome",
    "Blessed Lucia de Freitas","Saint Ludmila","Saint Ludovico Pavoni","Saint Ludre","Saint Lufthild","Blessed Luigi Beltrame Quattrocchi","Venerable Luigi Maria Olivares","Saint Luigi Orione","Blessed Luigi Talamoni","Blessed Luigi Variara","Our Lady of Luján","Blessed Symeon Lukach","Blessed Luke Belludi","Saint Luke the Evangelist","Saint Terence of Pesaro","Saint Lupercus","Saint Lupercus of Saragossa","Saint Lupus of Sens","Saint Lupus of Verona","Blessed Peter of Luxembourg","Saint Lybosus of North Africa","Saint Themistocles of Lycia","Saint Lydia Purpuraria","Saint Lydwina of Schiedam","Saint Stephen of Lyon","Blessed Roman Lysko","Saint Macaille","Saint Maccalin of Waulsort","Saint Maccallin of Lusk","Saint Macaria of Africa","Saint Macarius the Potter","Saint Macarius of Alexandria","Saint Macarius of Alexandria","Saint Macarius of Arabia","Saint Macarius of Syria","Saint Macarius the Wonder Worker","Saint Macarius the Younger","Saint Muirchu","Saint Macedonius of Nicomedia","Saint Macedonius","Saint Macedonius of Phrygia","Saint Machan","Saint Maches","Saint Machudd of Llanfechel","Saint Malo of Aleth","Blessed Tarsykia Matskiv","Saint Macra of Rheims","Saint Macrina the Elder","Saint Macrina the Younger","Saint Macrinus of Noviodunum","Saint Macrobius","Saint Madalberta","Blessed Maddalena Caterina Morano","Blessed Teresa Grillo Michel","Saint Vincent Madelgaire","Saint Malou of Hautvillers","Saint Madoes","Our Lady of Good Counsel","Saint Madron of Cornwall","Saint Mael","Saint Malachy O’More","Saint Maelmuire O’Gorman","Saint Maelrhys","Saint Maelruan","Blessed Magdalen Panattieri","Saint Magdalena of Nagasaki","Blessed Sebastian Maggi","Saint Magnericus of Trier","Saint Magnus the Martyr","Saint Magnus the Deacon","Saint Magnus of Anagni","Saint Mahanes the Persian","Saint Maimbod","Saint Mewan of Bretagne","Saint Mayeul","Saint Marius of Bodon","Saint Majoricus the Martyr","Saint Patrick of Málaga","Saint Mellon","Saint Malo the Martyr","Saint Publius of Malta","Blessed Mary Mamala","Saint Mamas","Saint Mamertus of Vienne","Saint Mammius of Corfu","Saint Manahen","Saint Manirus of Scotland","Blessed Mariam Thresia Chiramel Mankidiyan","Blessed Paolo Manna","Saint Mansuetus of North Africa","Saint Mansuetus of Alexandria","Blessed Manuel Medina Olmos","Saint Martin Manuel","Many Martyrs Who Suffered in Rome","Saint María Maravillas de Jesús","Saint Marcella","Saint Marcella of Alexandria","Saint Marcellian","Saint Marcellina","Saint Marcellinus of Tomi","Saint Marcellinus","Pope Saint Marcellinus","Pope Saint Marcellus I","Saint Marcellus of Spoleto","Blessed Marcelo Spínola y Maestre","Saint Marcia the Martyr","Saint Marcia of Campania","Saint Marcian of Africa","Saint Marcian of Constantinople","Saint Marcian of Rome","Saint Marcian the Senator","Saint Marcian","Saint Marcian of Tortona","Saint Marciana","Saint Marciana of Galatia","Saint Marcionilla of Antioch","Saint Marculf","Pope Saint Mark","Saint Mardonius of Rome","Saint Mardonius","Saint Mareas","Blessed Filippa Mareri","Saint Tressan of Mareuil","Blessed Margaret Pole","Saint Margaret Ward","Saint Margaret of Antioch","Saint Margaret of Cortona","Saint Margaret of Fontana","Saint Margaret of Louvain","Blessed Margaret of Ravenna","Saint Margaret of Scotland","Blessed Margaret of Vau-le-Duc","Saint Margaret the Barefooted","Blessed Margarita de Maturana","Saint Marguerite d’Youville","Saint Mari","Blessed Maria Adeodata Pisani","Blessed Maria Angeles of Saint Joseph","Blessed Maria Anna Sala","Blessed Maria Bagnesi","Blessed Maria Crucified Satellico","Blessed Maria del Carmen of the Child Jesus","Blessed María Dolores Rodríguez Sopeña","Saint Mary MacKillop","Blessed Maria Gabriella Sagheddu","Blessed María López de Rivas Martínez","Saint Mary Magdalen","Saint Maria Mazzarello","Blessed Maria Romero Meneses","Blessed Maria Mercedes Prat","Blessed Maria Louise Merkert","Blessed Marija Petkovic","Blessed Mary de Cerevellon","Saint Maria de Mattias","Blessed María de Los Ángeles Ginard Martí","Blessed Maria Pilar of Saint Francis Borgia","Blessed María Rafols-Bruna","Blessed Maria Repetto","Blessed Maria Teresa of Jesus","Saint Maria Soledad Torres Acosta","Blessed Vicenta Chávez Orozco","Blessed Mariano de Jesus Eues Hoyos","Blessed Mariano de la Mata Aparicio","Saint Marianus","Blessed Marie Rivier","Blessed Marie-Anne Vaillot","Blessed Mary of Oignies","Saint Marie of the Incarnation Guyart","Blessed Marie Leonie Paradis","Saint Marie-Madeleine Postel","Blessed Marie Poussepin","Blessed Teresa Maria Mastena","Saint Marina of Omura","Saint Marinus of Caesarea","Saint Marinus of Tomi","Saint Marinus of Africa","Saint Marinus of Maurienne","Saint Marinus of Rome","Saint Maris of Persia","Saint Maurus the Martyr","Saint Mark the Soldier","Saint Mark of Sorrento","Saint Mark of Rome","Saint Mark","Saint Mark of Mesia","Blessed Mark Scalabrini","Blessed Mark of Montegallo","Saint Maro","Saint Marsalius of Corfu","Blessed William Marsden","Saint Oderisius de Marsi","Saint Martha of Persia","Saint Martha of Astorga","Saint Martha of Bethany","Saint Martha Wang","Saint Martial of Saragossa","Saint Martialis of Rome","Saint Narcisa de Jesús Martillo-Morán","Saint Martinus Wu Xuesheng","Pope Saint Martin I","Saint Martin of Tongres","Saint Martin of Vienne","Blessed Richard Martin","Saint Martina of Rome","Blessed Mary Magdalene Martinengo","Saint Martinian of Milan","Saint Martinian of Rome","Saint Martinian the Hermit","Saint Martinus","Saint Peter Verona","Saint Romulus the Martyr","Saint Secundian the Martyr","Martyred Soldiers of Rome","Martyrs of China","Butler’s Lives of the Saints – Martyrs of Hadiab","Martyrs of Korea","Martyrs of Mesopotamia","Martyrs of Mount Sinai","Martyrs of Nagasaki","Martyrs of Nicomedia","Martyrs of Nowogrodek","Martyrs of September","Martyrs of Podlasie","Martyrs of Raithu","Martyrs of Samosata","Martyrs of Saragossa","Martyrs of Shanxi","Martyrs of the Spanish Civil War","Martyrs of the Theban Legion","Martyrs of Uganda","Martyrs of Vietnam","Saint Maruthas","Saint Maria of Lérida","Blessed Maria de la Cabeza","Saint Mary di Rosa","Saint Mary Euphrasia Pelletier","Blessed Mary Frances Schervier","Our Lady, Help of Christians","Saint Mary Magdalen of Pazzi","Blessed Suzanne-Agathe Deloye","Saint Mary Salome","Blessed Mary Teresa Bonzel","Saint Mary of Cordoba","Massa Candida","Blessed Peter Massalenus","Saint Odo of Massay","Saint Matilda of Saxony","Blessed Matilde del Sagrado Corazón Téllez Robles","Saint Matiya Mulumba","Saint Matrona of Amisus","Saint Matronianus of Milan","Venerable Matthew Talbot","Saint Matthew the Apostle","Blessed Matthia dei Nazzarei","Saint Matthias of Jerusalem","Saint Matthias of Miyako","Saint Matthias the Apostle","Saint Pierre Philibert Maubant","Saint Maur","Saint Maura","Saint Maura of Ravenna","Saint Maura of Constantinople","Saint Maura of Antinoe","Saint Maura of Troyes","Saint Maurice","Saint Maurontus","Saint Maurus","Saint Maurus of Rome","Saint Maurus of Verdun","Saint Victor Maurus","Saint Nowa Mawaggali","Saint Maxentiolus","Saint Maxentius of Trier","Saint Maxima of Africa","Saint Maxima","Saint Maximianus of Syrmium","Saint Maximilian of Thebeste","Saint Maximinus of Antioch","Saint Maximinus of Trier","Saint Maximinus of Micy","Saint Maximus of North Africa","Saint Maximus of Rome","Saint Maximus of Nola","Saint Maximus of Pavia","Saint Maximus of Turin","Saint Mbaga Tuzinde","Saint Mura McFeredach","Venerable Michael Joseph McGivney","Saint Mirin of Bangor","Saint Sanctinus of Meaux","Saint Medard of Noyon","Saint Medrald","Saint Medran","Saint Megingaud of Würzburg","Saint Megistus of Rome","Saint Meigan","Saint Meingold","Saint Melaine of Rennes","Saint Melania the Younger","Saint Melas of Rhinocolura","Saint Melchior","Saint José Melchór García-Sampedro Suárez","Saint Polyeuctus of Melitene","Saint Melito of Sardis","Saint Melitus of Pontus","Saint Mella of Doire-Melle","Saint Melosa","Saint Waltheof of Melrose","Saint Numerian of Treves","Blessed Mercedes de Jesús Molina y Ayala","Saint Werburgh of Bardney","Saint Mercuria of Alexandria","Saint Mercurius of Caesarea","Saint Meriadoc of Vannes","Saint Merewenna","Saint Merninus","Our Lady of Meritxell","Venerable Thecla Merlo","Saint Mirocles of Milan","Saint Merulus of Rome","Saint Mesrop the Teacher","Saint Messalina of Foligno","Saint Metellus","Saint Methodius","Saint Methodius of Olympus","Blessed Metoděj Dominik Trčka","Saint Metrobius of Tripoli","Saint Metrophanes of Byzantium","Saint Patiens of Metz","Saint Terence of Metz","Saint Meuris of Alexandria","Saint Mugagga","Blessed Michaël Nakashima Saburoemon","Blessed Michael Pini","Blessed Michelina of Pesaro","Saint Migdonius of Rome","Blessed Miguel Agustin Pro","Saint Miguel de la Mora","Saint Paul Miki","Father Enrico Beretta","Saint Natalis of Milan","Saint Protasius of Milan","Saint Savina of Milan","Saint Venerius of Milan","Saint Valeria of Milan","Saint Vitalis of Milan","Saint Milburga","Saint Mildgytha","Saint Mildred of Thanet","Saint Optatus of Milevis","Saint Milles of Persia","Pope Saint Miltiades","Saint Minervinus of Catania","Saint Philiphê Phan Van Minh","Saint Miniato of Florence","Saint Portianus of Miranda","Saint Sacer","Saint Mochoemoc","Saint Mochta","Saint Modan","Saint Modanic","Saint Peregrinus of Modena","Saint Moderan of Rennes","Saint Modesta of Nicomedia","Saint Modesta of Trier","Saint Modestus the Martyr","Saint Modestus of Carinthia","Saint Modestus of Carthage","Saint Modestus of Lucania","Saint Modoald of Trier","Blessed Pedro Martret y Molet","Saint Mummolus of Lagny","Saint Mummolinus of Noyon","Saint Mummolus of Fleury","Saint Monan","Saint Mundus","Saint Monegundis","Saint Monessa","Saint Monica","Blessed Sebastian Montanol","Saint Montanus of Carthage","Saint Petronax of Monte Cassino","Saint William of Vercelli","Saint Morand of Cluny","Saint Thomas More","Saint Moroc of Scotland","Blessed Pierina Morosini","Blessed Mosè Tovini","Saint Moses the Hermit","Saint Moses of Rome","Saint Moseus","Saint Seraphim of Sarov","Saint Mucian of Mesia","Saint Mucius","Saint Mun","Saint Trudpert of Münstertal","Saint Murtagh of Killala","Saint Walderic of Murrhardt","Saint Musa of Rome","Saint Musonius","Saint Mustiola of Chiusi","Saint Phaolô Nguyen Van My","Saint Mydwyn","Saint Myron of Cyzicus","Saint Myrope","Saint Natalis of Ulster","Saint Nabor the Martyr","Nahum the Prophet","Saint Patricia of Naples","Saint Severinus of Septempeda","Saint Paul of Narbonne","Saint Narcissus of Tomi","Saint Narcissus of Gerona","Saint Narcissus of Jerusalem","Blessed Paul Josef Nardini","Saint Proculus of Narni","Saint Narses of Subagord","Saint Natalia","Saint Natalia of Nicomedia","Blessed Noël Pinot","Saint Natalis of Casale","Saint Navalis of Ravenna","Blessed Peter Paul Navarro","Saint Nazarius of Rome","Saint Nemesius of Rome","Saint Nennius","Saint Neophytus of Nicaea","Saint Neopolus of Alexandria","Saint Neot","Saint Neoterius of Alexandria","Blessed Nera","Saint Nereus of Terracina","Saint Philip Neri","Blessed Otto Neururer","Blessed Sebastian Newdigate","Saint Robert of Newminster","Saint Ponsiano Ngondwe","Saint Nicanor the Deacon","Saint Nicasius of Rheims","Saint Nicephorus of Antioch","Saint Nicephorus of Africa","Saint Nicetas of Remesiana","Saint Nicetius of Trier","Saint Nicetas of Novgorod","Pope Saint Nicholas I","Saint Nicholas Peregrinus","Saint Nicolaas Pieck","Saint Nicholas Studites","Blessed Nicholas Woodfen","Saint Nicholas of Tolentino","Saint Nichole","Saint Nicodemus","Blessed Nicolas Roland","Saint Nicomedes of Rome","Saint Patricia of Nicomedia","Saint Peter of Nicomedia","Saint Zeno of Nicomedia","Saint Nicon of Sicily","Saint Nicostratus","Saint Salome of Niederaltaich","Saint Nilus of Phunon","Saint Nilus the Younger","Saint Nithard","Saint Nivard of Rheims","Venerable Pierre-Bienvenu Noailles","Saint Raynald of Nocera","Saint Paulinus of Nola","Saint Nominanda of Rome","Saint Non","Saint Nuño de Santa Maria Álvares Pereira","Saint Nonna","Saint Nonnosus","Saint Norbert of Xanten","Saint Severinus of Noricum","Saint Oswald of Northumbria","Our Lady of Prompt Succor","Saint Odo of Novara","Saint Nunilo of Huesca","Blessed Nuntius Sulprizio","Blessed Robert Nutter","Blessed Terrence Albert O’Brien","Obadiah the Prophet","Saint Stephen of Obazine","Saint Obdulia","Wernher of Oberwessel","Saint Obitius","Saint Oceanus the Martyr","Saint Octavian of Carthage","Blessed Octavian of Savona","Saint Octavius of Turin","Blessed Oda of Rivreulle","Saint Oda of Aquitaine","Saint Odilia of Alsace","Saint Odilo","Saint Odo of Urgell","Blessed Odoric of Pordenone","Saint Odran","Saint Odran","Saint Odrian of Waterford","Saint Odulph of Utrecht","Saint Oduvald of Melrose","Saint Conrad of Offida","Blessed Ogerius","Blessed Ollegarius of Tarragona","Saint Olaus of Sweden","Blessed Oleksa Zarytsky","Saint Oliva of Anagni","Saint Olivia of Palermo","Saint Oliva of Brescia","Saint Oliver Plunkett","Saint Olympiades","Saint Olympias of Constantinople","Saint Olympus of Aenos","Saint Onesimus","Saint Onesimus of Soissons","Saint Onuphrius","Saint Opportuna of Montreuil","Saint Optatian of Brescia","Saint Optatus of Saragossa","Saint Orestes of Cappadocia","Saint Prosper of Orléans","Blessed Orlando of Vallombrosa","Saint Theodoric of Orleans","Saint Orontius of Puigcerda","Saint Peter Orseolo","Saint Osith","Saint Romanus Ostiarius","Saint Oswald of Worcester","Saint Oswine of Deira","Saint Otto of Morocco","Saint Peter Ou","Saint Owen","Our Lady Aparecida","Our Lady of All Nations","Our Lady of Altagracia","Our Lady of Altotting","Our Lady of Carmel of the Maipú","Our Lady of Knock","Our Lady of La Salette","Our Lady of Liesse","Feast of Our Lady of Mercy","Our Lady of Montevergine","Our Lady of Perpetual Help","Our Lady of Pontmain","Our Lady of Solitude","Our Lady of the Snow","Our Lady of Walsingham","Our Lady of Washington","Saint Pabo","Saint Pacificus of Cerano","Saint Pacificus of San Severino","Saint Palaemon of Thebaid","Blessed Thomas Palaser","Saint Paldo","Saint Palladius of Rome","Saint Vincent Pallotti","Saint Palmatius of Rome","Blessed Palumbus of Subiaco","Saint Pambo of the Nitrian Desert","Saint Pammachius","Saint Pamphilus of Rome","Saint Pamphilus of Sulmona","Saint Theodulus of Pamphylia","Saint Zoe of Pamphylia","Saint Pancharius of Nicomedia","Saint Pancras of Rome","Saint Pantagapes the Martyr","Saint Panteleon","Venerable Teresa Valse Pantellini","Saint Papias of Rome","Saint Papias of Hierapolis","Saint Papinianus of North Africa","Saint Papius","Saint Peter of Pappacarbone","Blessed Valentin Paquay","Saint Paragnus of Samosata","Saint Servulus of Rome","Saint Parasceva of Rome","Saint Petca Parasceva","Saint Paregorius of Patara","Blessed Pietro Parenzo","Saint Parmenas the Deacon","Saint Parthenius of Rome","Pope Saint Paschal I","Blessed Peter Paschal","Saint Paschasius of Vienne","Saint Paschasius Radbertus","Saint Paschasius of Salamanca","Blessed Silvestre Arnau y Pascuet","Saint Pastor of Alcala","Saint Poemen","Saint Patapius of Constantinople","Blessed William Patenson","Saint Paterniano","Saint Patrick","Blessed Patrick Salmon","Saint Patroclus of Troyes","Saint Paul of Porto Romano","Saint Paul of Corinth","Saint Paul of Rome","Saint Paul of Ptolemais","Saint Paul of Nicaea","Saint Paul of Tripoli","Pope Saint Paul I","Saint Paul Chong Hasang","Saint Paul Suzuki","Saint Paul of Numidia","Saint Paul of Prusa","Saint Paul of Trois Châteaux","Saint Paul of the Cross","Saint Paul the Apostle","Saint Paul the Hermit","Saint Paul the Simple","Blessed Paula of Tuscany","Saint Paula of Rome","Saint Paulillus of Salamanca","Saint Paulina","Saint Paulina of Rome","Saint Paulinus of York","Blessed William de Paulo","Saint Pompeius of Pavia","Saint Theodore of Pavia","Saint Pega of Peakirk","Blessed Peter Thomas","Saint Pelagia of Caesarea","Saint Pelagia","Saint Pelagia the Penitent","Saint Pelagius of Oviedo","Saint Peleus of Phunon","Saint Pens","Saint Theophilus the Penitent","Saint Peregrinus of Rome","Saint Perfecto of Córdoba","Saint Perpetua","Saint Maharsapor the Persian","Saint William of Rochester","Saint Peter the Soldier","Saint Peter of Morocco","Saint Peter of Rome","Saint Peter the Exorcist","Saint Peter of Philadelphia","Saint Peter of Constantinople","Saint Peter Baptist","Saint Peter Canisius","Saint Peter Claver","Saint Peter Damian","Saint Peter Gonzales","Saint Peter Nolasco","Saint Peter of Alcantara","Saint Peter of Poitiers","Blessed Peter of Sassoferrato","Saint Peter of Sebaste","Blessed Peter of Subiaco","Saint Peter Regalatus","Saint Pere Sans Jordà","Saint Peter Sukejiroo","Saint Phêrô Truong Van Thi","Blessed Peter To Rot","Saint Phêrô Vu Van Truat","Blessed Peter Verhun","Blessed Pierre Vigne",
    "Blessed Peter Wright","Saint Victorinus of Pettau","Saint Petroc","Saint Petronius of Bologna","Saint Petronius of Die","Saint Pharaildis of Ghent","Saint Phiala of Hayle","Saint Philadelphus the Martyr","Saint Philemon of Antinoë","Saint Philemon","Saint Philip of Rome","Saint Philip the Martyr","Saint Philip Howard","Blessed Philip Rinaldi","Saint Philip of Vienne","Saint Philip of Zell","Saint Philip the Apostle","Saint Philippian of Africa","Saint Philomena of San Severino","Saint Philoterus","Saint Phocas of Antioch","Saint Phocas the Gardener","Saint Pia of Numidia","Saint Sabinus of Piacenza","Saint Victor of Piacenza","Saint Piaton of Tournai","Blessed Piedad de la Cruz Ortiz Real","Blessed Pierre Tessier","Venerable Pierre Toussaint","Venerable Pietro Riminucci","Saint Pigmenius of Rome","Saint Pionius","Saint Pior","Saint Piran","Saint Pirmin","Saint Torpes of Pisa","Saint Sulpicius of Bourges","Pope Pius I","Pope Blessed Pius IX","Pope Saint Pius X","Saint Placid","Saint Placid of Autun","Saint Placid of Ocre","Saint Placid of Dissentis","Saint Placid","Blessed Placide Viel","Saint Placidia","Saint Plato","Saint Plato of Ancyra","Blessed Thomas Plumtree","Saint William Tempier","Saint Pollio of Cybalae","Saint Polycarp of Antioch","Saint Polycarp of Smyrna","Saint Polychronius of Babylon","Saint Pompeius of Carthage","Saint Pomposa of Cordoba","Saint Ponziano of Spoleto","Saint Pontian of Rome","Saint Pontian","Saint Pontian of Rome","Saint Pontian of Catania","Pope Saint Pontian","Saint Pontius of Carthage","Saint Troadius of Pontus","Blessed Torello of Poppi","Saint Poppo","Saint Porcarius of Lérins","Saint Porfirio","Saint Thorlac Thorhallsson","Saint Porphyrius of Caesarea","Saint Porphyrius of Gaza","Saint Rafaela Porras y Ayllón","Saint Theresa of Portugal","Saint Potamiaena the Elder","Saint Potentian of Sens","Saint Potitus","Saint Practextatus of Rome","Saint Praepedigna of Rome","Saint Praetexatus of Rouen","Saint Praxides of Rome","Saint Prilidian","Saint Primian of North Africa","Saint Primitivus of Saragossa","Saint Primus of Cyzicus","Saint Primus of Antioch","Saint Priscilla the Tent Maker","Saint Prisca of Rome","Saint Priscillianus of Rome","Saint Priscus of Rome","Saint Priscus","Saint Probus of Salamanca","Saint Probus of Verona","Saint Processus of Rome","Saint Procopius","Saint Proculus of Pozzuoli","Saint Proculus of Verona","Saint Projectus","Saint Prosper of Reggio","Saint Protase","Saint Proterius of Alexandria","Saint Protogenes of Syria","Saint Protus of Rome","Saint Prudentius of Tarazona","Blessed Volodymyr Ivanovych Pryima","Saint Ptolomy of Alexandria","Saint Publia","Saint Publius of North Africa","Saint Publius of Saragossa","Saint Pudentiana of Rome","Saint Pulcheria","Saint Pupulus of Alexandria","Saint Quadragesimus of Policastro","Saint Quadratus of Anatolia","Saint Quadratus of Herbipolis","Saint Quadratus","Saint Quadratus the Apologist","Saint Quadratus of Utica","Saint Quartilla of Sorrento","Saint Quartus of Capua","Saint Quartus","Saint Quartus","Saint Quentin","Saint Quintius the Martyr","Saint Quieta of Dijon","Saint Quintian","Saint Quintilian of Saragossa","Saint Quintilis of Nicomedia","Saint Quintilla of Sorrento","Saint Quintius the Martyr","Saint Quintus of Africa","Saint Quintus of Capua","Saint Quintus","Blessed Stephana de Quinzanis","Saint Quirinus of Sescia","Saint Quodvultdeus","Rachel the Matriarch","Saint Rachildis of Saint-Gall","Saint Radbod of Utrecht","Saint Radegunde","Blessed Rafaela Ybarra Arambarri de Villalonga","Saint Reinhold of Cologne","Blessed Ranieri Scacceri","Saint Ralph Sherwin","Blessed Raoul of Vaucelles","Blessed Victoire Rasoamanarivo","Saint Raymond Nonnatus","Saint Raymond of Penyafort","Blessed Raynald de Bar","Saint Rebecca of Alexandria","Blessed Redemptorus of the Cross","Saint Stephen of Reggio","Saint Regina","Saint Regulus of Rheims","Saint Regulus of Scotland","Saint Regulus of Senlis","Saint Remigius of Rheims","Saint Remaclus","Saint Rembert of Hamburg","Saint Remedius of Gap","Saint Remigius of Rouen","Saint Renatus of Angers","Saint Rene Goupil","Saint Reparata","Saint Restituta of Carthage","Saint Revocatus of Smyrna","Saint Sabas Reyes Salazar","Saint Rigobert of Rheims","Saint Rhuddlad","Saint Ribert of Saint-Oyend","Blessed Richard Rolle de Hampole","Blessed Richard Thirkeld","Blessed Richard Whiting","Saint Richard of Chichester","Saint Richard of Vaucelles","Saint Richardis of Andlou","Saint Richarius of Celles","Saint Richimir","Saint Richrudis of Marchiennes","Saint Stephen of Rieti","Saint Riginos","Blessed Simon Rinalducci","Blessed Rita Amada de Jesus","Saint Rita of Cascia","Saint Robert of Chaise-Dieu","Blessed Robert Salt","Saint Robert Southwell","Saint Roch","Saint Rocco Gonzalez","Saint Ruadhán of Lorrha","Saint Roderick of Cordoba","Saint Tarsicia of Rodez","Saint Rodingus","Saint Rodron of Bèze","Saint William of Roskilde","Saint Rogation of Africa","Saint Rogatus of Rome","Saint Rogatus the Martyr","Saint Rogellus of Cordoba","Blessed Roger of Todi","Saint Romana","Blessed Vincent Romano","Saint Romanus of Samosata","Saint Romanus of Rouen","Saint Romanus the Wonder Worker","Saint Romaric of Remiremont","Saint Rumold","Saint Silvia of Rome","Saint Tatiana of Rome","Saint Theodulus of Rome","Saint Theopistes of Rome","Saint Theopistus of Rome","Saint Valentine of Rome","Saint Zoe of Rome","Saint Toribio Romo González","Saint Romuald","Saint Romula of Rome","Saint Romulus of North Africa","Saint Romulus of Berry","Saint Romulus","Saint Romulus of Fiesole","Blessed Teodor Jurij Romzha","Saint Ronald of Orkney","Saint Ronan of Lismore","Saint Rosa Chen Aijieh","Saint Rosalia","Blessed Rosalina of Villeneuve","Saint Rose of Lima","Saint Rose of Viterbo","Saint Rose Venerini","Saint Rudesind","Saint Roswinda","Blessed Scubilion Rousseau","Blessed Rudolph","Saint Ruellinus of Treguier","Saint Rufinus the Potter",
    "Saint Rufinus of Soissons","Saint Rufinus of Ancyra","Saint Rufus of Philadelphia","Saint Rufus","Saint Rufus of Rome","Saint Rumwold of Buckingham","Saint Rustica of Rome","Ruth the Matriarch","Saint Rutulus of North Africa","Saint Sabas the Lector","Saint Sabbas of Mar Saba","Saint Sebastia of Sirmium","Saint Sabbatius of Antioch","Saint Sabina of Rome","Saint Sabinian of Sens","Saint Sabinian of Troyes","Saint Sabinus the Martyr","Saint Sabinus","Saint Sabinus of Canosa","Saint Sabino of Spoleto","Saint Sacerdos of Saguntum","Sacred Heart of Jesus","Saint Theodore the Sacristan","Blessed Sadoc of Sandomierz","Saint Sadwen of Wales","Saint Sidonius of Saint-Saens","Saint Saethryth","Saint Ursicinus of Saint-Ursanne","Saint Servan of Culross","Blessed Sára Salkaházi","Saint Sallustia","Saint Salvinus of Verdun","Saint Salvius","Saint Samson of York","Samuel the Patriarch","Saint Sanctian of Sens","Saint Sapor","Saint Saturnius of Saragossa","Saint Successus of Saragossa","Saint Urban of Saragossa","Saint Valerius of Saragossa","Saint Sarbelius","Saint Saturninus of Toulouse","Saint Saturninus of Alexandria","Saint Saturninus the Martyr","Saint Saturninus of Corfu","Saint Saturninus of Alexandria","Saint Saturninus of Cagliari","Saint Saturninus of Rome","Saint Saturninus of Crete","Saint Saturninus","Saint Saturus of Africa","Saint Satyrus","Saint Satyrus of Milan","Blessed Giles of Saumur","Saint Sawl","Saint Schotin","Saint Scholastica","Saint Sedulius","Saint Sebaldus","Saint Sebastian","Saint Secundilla of Porto Romano","Saint Secundarius","Saint Secundel","Saint Secundina of Rome","Saint Secundinus of North Africa","Saint Secundinus of Cirta","Saint Secundinus of Sinuessa","Saint Secundus of North Africa","Saint Secundus of Africa","Saint Secundus of Nicomedia","Saint Secundus of North Africa","Saint Secundus the Theban","Saint Sedna of Ossory","Saint Sequanus","Saint Seiriol","Saint Seleucius of Caesarea","Saint Seleucus of Syria","Saint Senen","Saint Seraphina","Saint Serapion of Pentapolis","Saint Servilian","Saint Seraphia the Martyr","Blessed Seraphina Sforza","Saint Serapion the Scholastic","Saint Sergius of Caesarea","Pope Saint Sergius I","Saint Sergius the Martyr","Saint Serotina of Rome","Saint Servatus of Tongres","Saint Servus the Martyr","Seth the Patriarch","Seven Holy Brothers","Saint Severian","Saint Severian the Martyr","Saint Severinus of Trier","Saint Severus of Alexandria","Saint Sulpicius Severus","Saint Sithney","Saint Wulsin of Sherborne","Blessed Thomas Sherwood","Saint Vicente Shiwozuka de la Cruz","Saint Sifrard of Bèze","Saint Sigfrid of Sweden","Saint Theodore of Sykeon","Saint Sulian","Saint Silvanus of Emesa","Saint Silvanus of Rome","Saint Silvanus of Ancyra","Saint Silvanus","Pope Saint Silverio","Pope Saint Sylvester I","Saint Simon","Saint Simeon of Persia","Saint Simeon Stylites","Simon of Trent","Saint Simon the Apostle","Blessed Simon Yempo","Saint Simplician of Catania","Pope Saint Simplicius","Saint Simplicius of North Africa","Saint Simplicius of Rome","Saint Simpronian","Pope Saint Siricius","Saint Syrus of Pavia","Saint Sisinnius of Rome","Pope Saint Sixtus I","Pope Saint Sixtus II","Pope Saint Sixtus III","Saint Smaragdus","Saint Sophia","Saint Sola","Saint Ursus the Theban","Saint Solutor of Turin","Blessed Thomas Somers","Saint Sophronius of Jerusalem","Saint Valerius of Sorrento","Saint Sosius of Puzzuoli","Saint Sosthenes of Colophon","Pope Saint Soter","Saint Soteris the Martyr",
    "Blessed William Southerne","Saint Speciosus","Saint Spyridon of Cyprus","Saint Stanislaus of Cracow","Saint Stephen the Deacon","Saint Stephen of Catania","Pope Saint Stephen I","Saint Stephen of Hungary","Saint Stephen the Martyr","Saint Stephen the Younger","Saint Vincent Strambi","Saint Valentine of Strasbourg","Saint Theodore Stratelates","Saint Stratonicus","Saint Tysilio of Wales","Saint Sulpicius","Saint Susanna the Martyr","Saint Susanna","Saint Susanna of Rome","Saint Susanna of Eleutheropolis","Saint Swithun","Saint Symphorosa of Campania","Saint Syncletica","Saint Theonas","Saint Zosimus of Syracuse","Saint Zebinus of Syria","Saint Tobias of Syrmium","Saint Tancred of Thorney","Saint Tarcisius","Saint Tarskius of Alexandria","Saint Trasilla","Saint Taso","Saint Tatian of Aquileia","Saint Tatian of Phrygia","Saint Tato","Saint Teath","Saint Tudy","Pope Saint Telesphorus","Saint Terence of Rome","Saint Terence of Carthage","Saint Terence","Saint Terence of Todi","Saint Terentian of Todi","Saint Teresa de Gesu, Jornet y Ibars","Blessed Teresa Maria of the Cross","Saint Teresa of Avila","Blessed Teresa of the Child Jesus and of Saint John of the Cross","Saint Valentine of Terni","Saint Tertula of Cirta","Saint Tertus","Saint Thalus the Martyr","Saint Thamel","Saint Thea of Alexandria","Saint Thecla of Rome","Saint Thecla of Caesarea","Saint Thecla the Martyr","Saint Theobald","Saint Theodatus of Ancyra","Blessed Thiemo of Salzburg","Saint Theodore of Pentapolis","Saint Theodore of Syria","Saint Theodore of Rome","Saint Theodore Trichinas","Saint Theodore the Martyr","Saint Theodoric of Mont d’Or","Saint Theodoric of Ninden","Saint Theodosia of Amisus","Saint Theodosia the Martyr","Saint Theodosius","Saint Theodosius the Soldier","Saint Theodota of Thrace","Saint Theodotus of Africa","Saint Theodotus of Tomi","Saint Theodolus the Martyr","Saint Theodolus of Phrygia","Saint Theodulus of Crete","Saint Theogenes of Cyzicus","Saint Theophanes","Saint Theophila of Nicomedia","Saint Theophilis of Alexandria","Saint Theophilus the Martyr","Saint Theophilus of Caesarea","Saint Theophylact of Nicomedia","Saint Theophilus of Alexandria","Saint Theotimus of Laodicea","Saint Theotimus of Tripoli","Saint Theotimus of Tomi","Saint Thérèse of Lisieux","Saint Thomas a Becket","Book of Saints – Kybi","Saint Thomas of Villanova","Saint Thomas the Apostle","Blessed Thomas Tsuji",
    "Blessed Thomas Warcop","Blessed Thomas Whitaker","Blessed Thomas Woodhouse","Saint Thomasello","Saint Thorette","Saint Thyrsus","Saint Thyrsus of Apollonia","Saint Thrysus","Saint Tibba of Castor","Saint Tiburtius","Saint Tipasio of Tigava","Saint Tigides of Gap","Saint Tigrius","Saint Timothy of Syrmium","Saint Timothy","Saint Timothy of Rome","Saint Timothy of Gaza","Saint Timothy of Antinoe","Saint Titian of Oderzo","Saint Titus","Saint Turibius of Mogroveio","Saint Torthred of Thorney","Blessed Toscana of Verona","Saint Totnan of Thuringia","Saint Volusian of Tours","Saint Tova of Thorney","Saint Trason of Rome","Saint Vigilius of Trent","Saint Triduna","Saint Tryphon of Lampsacus","Saint Triphina of Sicily","Saint Triphina of Brittany","Saint Triverius","Saint Trojan","Saint Trophimus the Martyr","Saint Trophimus of Antioch","Saint Vincent of Saragossa","Saint Tryphon of Africa","Saint Tryphon of Alexandria","Saint Tudwal of Tréguier","Saint Tudclyd","Saint Tydfil","Saint Turiaf of Dol","Saint Tydecho","Saint Ulfrid of Sweden","Saint Ultan of Ardbraccan","Saint Urban the Martyr","Saint Urban of Campania","Saint Urban of North Africa","Saint Urciscenus of Pavia","Saint Urbicius","Saint Ursicinus II","Saint Ursicius of Nicomedia","Saint Ursinus of Bourges","Saint Ursula","Saint Ursus of Ravenna","Saint Vaise","Saint Valentin Faustino Berri Ochoa","Saint Valentine of Passau","Saint Valentine of Segovia","Saint Valentine of Ravenna","Saint Valeria","Saint Valeria of Limoges","Saint Valerian of Noviodunum","Saint Valerian of Abbenza","Saint Valerius of Soissons","Saint Valerius of Trier","Saint Vales","Saint Vitonus of Verdun","Saint Veep","Saint Venustian of Spoleto","Saint Venustus of Milan","Saint Veranus","Saint Veridiana","Saint Zeno of Verona","Saint Veronica","Saint Vincentian of Tulle","Saint Victalicus","Saint Victor of Alexandria","Saint Victor of North Africa","Saint Victor of Africa","Saint Victor of Africa","Saint Victor of North Africa","Pope Victor I","Saint Victor of Xanten","Saint Victor of Marseilles","Saint Victor of Puigcerda","Saint Victor the Theban","Saint Victoria of Albitina","Saint Victoria of Cordoba","Saint Victoria","Saint Victorian of Asana","Saint Victorian of Hadrumetum","Saint Victoricus","Saint Victorinus of Africa","Saint Victorius of Carthage","Saint Victurus of Africa","Blessed Villana de’Botti","Saint Vincent of North Africa","Saint Vincent of Rome","Saint Vincent of Rome","Blessed Vincent de Cunha","Saint Vincent de Paul","Saint Vincent Ferrer","Saint Virila",
    "Saint Vitalicus of Smyrna","Pope Saint Vitalian","Saint Vitalis of Africa","Saint Vitalis of Rome","Saint Vitalis of Calabria","Saint Vitus","Saint Vitus of Syrmium","Saint Viventius","Saint Wivina","Saint Vladimir I of Kiev","Saint Vodoaldus of Soissons","Saint Voloc","Saint Vulgis of Lobbes","Saint Walburga","Saint Walstan of Bawburgh","Blessed William Ward","Saint Wenceslaus of Bohemia","Saint Wendelin","Saint Wenog","Saint Wiomad of Trèves","Saint Werenfridus","Blessed Wittikund of Westphalia","Saint Wilfrid of York","Saint Wilfrida","Saint Willibald of Eichstätt","Saint Willeic","Blessed William Saultemouche","William of Norwich","Saint Willibrord of Echternach","Saint Willigis of Mainz","Saint Winebald of Heidenheim","Saint Wolfgang of Ratisbon","Saint Wulstan of Worcester","Saint Zosimus the Wonder Worker","Saint Wulfhilda","Saint Wulfram of Sens","Saint Wulfric of Haselbury","Blessed Gaius Xeymon","Saint Yolanda","Pope Saint Zachary","Saint Zeno of Alexandria","Saint Zenobius of Florence","Saint Zenobius of Tripoli","Saint Zeticus of Crete","Saint Zosimus of Cilicia","Saint Zosimus of Carthage","Saint Zosimus of Palestine","Saint Zosimus of Syria","Saint Zosimus of Rome","Saint Zosimus of Nicaea","Pope Saint Zosimus","Saint Zoticus the Martyr","Saint Zoticus of Alexandria"};

vector<string> demons = {"Abaddon","Abraxas","Adramelech","Agares","Ahriman","Aim","Alastor","Alloces","Amdusias","Amon","Amy","Andras","Andrealphus","Andromalius","Anzu","Asmoday","Astaroth","Astarte","Azazel","Bael","Balam","Barbatos","Bathin","Beelzebub","Behemoth","Beleth","Belial","Belphegor","Berith","Bhairava ","Bifrons","Botis","Buer","Bune","Cacus","Cali ","Callicantzaros","Camio","Cerberus","Chiang-shih ","Cimejes","Crocell","Daevas","Dagwanoenyent","Daityas","Dantalion","Decarabia","Deumus","Eligos","Ereshkigal","Erinyes","Eurynome","Satan","Lucifer","Fenrir","Flaga","Focalor","Fomorians","Foras","Forneus","Furcas","Furfur","Gaap","Ganga","Garuda","Glasya-Labolas","Gremory","Gusion","Haagenti","Halphas","Haures","Hecate","Hel","Hiisi","Hun Came and Vucub Came","Hurakan","Iblis","Ipos","Ishtar","Izanami","Jinn","Jormungand","Kappas","Lamia","Lechies","Leonard","Leraje","Leviathan","Lilith","Lix Tetrax","Loki","Malphas","Mammon","Mara","Marax","Marbas","Marchosias","Mastema","Melchiresa","Melchom","Mephistophiles","Mictlantecuhtli","Moloch","Murmur","Naberius","Nagas","Nickar","Nybbas","Onoskelis","Oriax","Ornias","Orobas","Ose","Paimon","Phenex","Picollus","Pretas","Pruflas","Purson","Rahovart","Rakshasas","Raum","Ribesal","Ronove","Sabnock","Sallos","Samigina","Seere","Seraphim","Shax","Shedim","Sitri","Stolas","Tezcatlipoca","Tlaltecuhtli","Torngarsuk","Tzitzimime","Ukobach","Uvall","Valefor","Vapula","Vassago","Vepar","Vine","Volac","Vrykolakas","Vucub Caquix","Xaphan","Yan-gant-y-tan","Yara-ma-yha-who","Yenaldooshi","Zagan","Zepar"};

vector<string> evil_adj = {"great", "cursed", "wicked", "wanton" };
vector<string> good_adj = {"humble", "virtuous", "pious", "chaste", "devout" };
vector<string> evil_verbs = {"loathes", "defies", "destroys" };
vector<string> good_verbs = {"labors for", "devotes thyself to" };
vector<string> good_nouns = {"peace", "hope", "love", "faith", "grace" };

//generates a random prayer to a random saint/demon
string genPrayer(){
    
    string prayer = "";
    
    //PRAYER
    prayer.append("Oh ");
    prayer.append(good_adj[rand()%good_adj.size()] + " "); //random
    prayer.append(saints[rand()%saints.size()] + ", "); //random
    prayer.append("Whom ever ");
    prayer.append(good_verbs[rand()%good_verbs.size()] + " "); //random
    prayer.append(good_nouns[rand()%good_nouns.size()] + ", "); //random
    prayer.append("Pray answer my ");
    prayer.append(good_adj[rand()%good_adj.size()] + " "); //random
    prayer.append("petition! ");
    
    return prayer;
    
}

////////////////////////
//MAGIC

vector<string> hexagrams = {
    "Force",
    "Field",
    "Sprouting",
    "Enveloping",
    "Attending",
    "Arguing",
    "Leading",
    "Grouping",
    "Small Accumulating",
    "Treading",
    "Pervading",
    "Obstruction",
    "Concording People",
    "Great Possessing",
    "Humbling",
    "Providing-For",
    "Correcting",
    "Nearing",
    "Viewing",
    "Gnawing Bite",
    "Adorning",
    "Stripping",
    "Returning",
    "Without Embroiling",
    "Great Accumulating",
    "Swallowing",
    "Great Exceeding",
    "Gorge",
    "Radiance",
    "Conjoining",
    "Persevering",
    "Retiring",
    "Great Invigorating",
    "Prospering",
    "Darkening of the Light",
    "Dwelling People",
    "Polarising",
    "Limping",
    "Taking-Apart",
    "Diminishing",
    "Augmenting",
    "Displacement",
    "Coupling",
    "Clustering",
    "Ascending",
    "Confining",
    "Welling",
    "Skinning",
    "Holding",
    "Shake",
    "Bound",
    "Infiltrating",
    "Converting the Maiden",
    "Abounding",
    "Sojourning",
    "Ground",
    "Open",
    "Dispersing",
    "Articulating",
    "Center Returning",
    "Small Exceeding",
    "Already Fording",
    "Not Yet Fording"
};

vector<string> fave_color_names = {"Cloudy blue","Dark pastel green","Dust","Electric lime","Fresh green","Light eggplant",
    "Nasty green","Really light blue",
    "Tea","Warm purple",
    "Yellowish tan","Cement","Dark grass green",
    "Dusty teal","Grey teal","Macaroni and cheese",
    "Pinkish tan","Spruce","Strong blue",
    "Toxic green","Windows blue","Blue blue","Blue with a hint of purple","Booger","Bright sea green","Dark green blue","Deep turquoise","Green teal","Strong pink","Bland","Deep aqua",
    "Lavender pink",
    "Light moss green",
    "Light seafoam green",
    "Olive yellow",
    "Pig pink",
    "Deep lilac",
    "Desert",
    "Dusty lavender",
    "Purpley grey",
    "Purply",
    "Candy pink",
    "Light pastel green",
    "Boring green",
    "Kiwi green",
    "Light grey green",
    "Orange pink",
    "Tea green",
    "Very light brown",
    "Egg shell",
    "Eggplant purple",
    "Powder pink",
    "Reddish grey",
    "Baby shit brown",
    "Liliac",
    "Stormy blue",
    "Ugly brown",
    "Custard",
    "Darkish pink",
    "Deep brown",
    "Greenish beige",
    "Manilla",
    "Off blue",
    "Battleship grey",
    "Browny green",
    "Bruise",
    "Kelley green",
    "Sickly yellow",
    "Sunny yellow",
    "Azul",
    "Darkgreen",
    "Green/yellow",
    "Lichen",
    "Light light green",
    "Pale gold",
    "Sun yellow",
    "Tan green",
    "Burple",
    "Butterscotch",
    "Toupe",
    "Dark cream",
    "Indian red",
    "Light lavendar",
    "Poison green",
    "Baby puke green",
    "Bright yellow green",
    "Charcoal grey",
    "Squash",
    "Cinnamon",
    "Light pea green",
    "Radioactive green",
    "Raw sienna",
    "Baby purple",
    "Cocoa",
    "Light royal blue",
    "Orangeish",
    "Rust brown",
    "Sand brown",
    "Swamp",
    "Tealish green",
    "Burnt siena",
    "Camo",
    "Dusk blue",
    "Fern",
    "Old rose",
    "Pale light green",
    "Peachy pink",
    "Rosy pink",
    "Light bluish green",
    "Light bright green",
    "Light neon green",
    "Light seafoam",
    "Tiffany blue",
    "Washed out green",
    "Browny orange",
    "Nice blue",
    "Sapphire",
    "Greyish teal",
    "Orangey yellow",
    "Parchment",
    "Straw",
    "Very dark brown",
    "Terracota",
    "Ugly blue",
    "Clear blue",
    "Creme",
    "Foam green",
    "Grey/green",
    "Light gold",
    "Seafoam blue",
    "Topaz",
    "Violet pink",
    "Wintergreen",
    "Yellow tan",
    "Dark fuchsia",
    "Indigo blue",
    "Light yellowish green",
    "Pale magenta",
    "Rich purple",
    "Sunflower yellow",
    "Green/blue",
    "Leather",
    "Racing green",
    "Vivid purple",
    "Dark royal blue",
    "Hazel",
    "Muted pink",
    "Booger green",
    "Canary",
    "Cool grey",
    "Dark taupe",
    "Darkish purple",
    "True green",
    "Coral pink",
    "Dark sage",
    "Dark slate blue",
    "Flat blue",
    "Mushroom",
    "Rich blue",
    "Dirty purple",
    "Greenblue",
    "Icky green",
    "Light khaki",
    "Warm blue",
    "Dark hot pink",
    "Deep sea blue",
    "Carmine","Dark yellow green",
    "Pale peach",
    "Plum purple",
    "Golden rod",
    "Neon red",
    "Old pink",
    "Very pale blue","Blood orange","Grapefruit",
    "Sand yellow","Clay brown",
    "Dark blue grey",
    "Flat green","Light green blue","Warm pink",
    "Dodger blue",    "Gross green",
    "Ice",    "Metallic blue","Pale salmon","Sap green",
    "Algae",    "Bluey grey","Greeny grey","Highlighter green",
    "Light light blue",
    "Light mint",
    "Raw umber",
    "Vivid blue",
    "Deep lavender","Dull teal","Light greenish blue",
    "Mud green","Pinky",
    "Red wine","Shit green",
    "Tan brown","Darkblue",
    "Rosa",
    "Lipstick","Pale mauve",
    "Claret",
    "Dandelion",
    "Orangered","Poop green",
    "Ruby",
    "Dark",
    "Greenish turquoise",
    "Pastel red",
    "Piss yellow",
    "Bright cyan",
    "Dark coral",
    "Algae green",
    "Darkish red",
    "Reddy brown",
    "Blush pink",
    "Camouflage green",
    "Lawn green",
    "Putty",
    "Vibrant blue",
    "Dark sand",
    "Purple/blue",
    "Saffron",
    "Twilight","Warm brown",
    "Bluegrey",
    "Bubble gum pink","Duck egg blue",
    "Greenish cyan",
    "Petrol",
    "Royal",
    "Butter","Dusty orange",
    "Off yellow",
    "Pale olive green",
    "Orangish","Leaf",
    "Light blue grey",
    "Dried blood",
    "Lightish purple",
    "Rusty red","Lavender blue",
    "Light grass green",
    "Light mint green",
    "Sunflower","Velvet",
    "Brick orange",
    "Lightish red","Pure blue","Twilight blue","Violet red","Yellowy brown","Carnation","Muddy yellow","Dark seafoam green","Deep rose","Dusty red","Grey/blue","Lemon lime","Purple/pink",
"Brown yellow",
    "Purple brown",
    "Wisteria","Banana yellow",
    "Lipstick red",
    "Water blue",
    "Brown grey",
    "Vibrant purple",
    "Baby green",
    "Barf green",
    "Eggshell blue",
    "Sandy yellow",
    "Cool green",
    "Pale",
    "Blue/grey",
    "Hot magenta",
    "Greyblue",
    "Purpley",
    "Baby shit green",
    "Brownish pink",
    "Dark aquamarine",
    "Diarrhea",
    "Light mustard",
    "Pale sky blue",
    "Turtle green",
    "Bright olive",
    "Dark grey blue",
    "Greeny brown",
    "Lemon green",
    "Light periwinkle",
    "Seaweed green",
    "Sunshine yellow",
    "Ugly purple",
    "Medium pink",
    "Puke brown",
    "Very light pink",
    "Viridian",
    "Bile",
    "Faded yellow",
    "Very pale green",
    "Vibrant green",
    "Bright lime",
    "Spearmint",
    "Light aquamarine",
    "Light sage",
    "Yellowgreen",
    "Baby poo",
    "Dark seafoam",
    "Deep teal",
    "Heather",
    "Rust orange",
    "Dirty blue",
    "Fern green",
    "Bright lilac",
    "Weird green",
    "Peacock blue",
    "Avocado green",
    "Faded orange",
    "Grape purple",
    "Hot green",
    "Lime yellow",
    "Mango",
    "Shamrock",
    "Bubblegum",
    "Purplish brown",
    "Vomit yellow",
    "Pale cyan",
    "Key lime",
    "Tomato red",
    "Lightgreen",
    "Merlot",
    "Night blue",
    "Purpleish pink",
    "Apple",
    "Baby poop green",
    "Green apple",
    "Heliotrope",
    "Yellow/green",
    "Almost black",
    "Cool blue",
    "Leafy green",
    "Mustard brown",
    "Dusk",
    "Dull brown",
    "Frog green",
    "Vivid green",
    "Bright light green",
    "Fluro green",
    "Kiwi",
    "Seaweed",
    "Navy green",
    "Ultramarine blue",
    "Iris",
    "Pastel orange",
    "Yellowish orange",
    "Perrywinkle",
    "Tealish",
    "Dark plum",
    "Pear",
    "Pinkish orange",
    "Midnight purple",
    "Light urple",
    "Dark mint",
    "Greenish tan",
    "Light burgundy",
    "Turquoise blue",
    "Ugly pink",
    "Sandy",
    "Electric pink",
    "Muted purple",
    "Mid green",
    "Greyish",
    "Neon yellow",
    "Banana",
    "Carnation pink",
    "Tomato",
    "Sea",
    "Muddy brown",
    "Turquoise green",
    "Buff",
    "Fawn",
    "Muted blue",
    "Pale rose",
    "Dark mint green",
    "Amethyst",
    "Blue/green",
    "Chestnut",
    "Sick green",
    "Pea",
    "Rusty orange",
    "Stone",
    "Rose red",
    "Pale aqua",
    "Deep orange",
    "Earth",
    "Mossy green",
    "Grassy green",
    "Pale lime green",
    "Light grey blue",
    "Pale grey",
    "Asparagus",
    "Blueberry",
    "Purple red",
    "Pale lime",
    "Greenish teal",
    "Caramel",
    "Deep magenta",
    "Light peach",
    "Milk chocolate",
    "Ocher",
    "Off green",
    "Purply pink",
    "Lightblue",
    "Dusky blue",
    "Golden",
    "Light beige",
    "Butter yellow",
    "Dusky purple",
    "French blue",
    "Ugly yellow",
    "Greeny yellow",
    "Orangish red",
    "Shamrock green",
    "Orangish brown",
    "Tree green",
    "Deep violet",
    "Gunmetal",
    "Blue/purple",
    "Cherry",
    "Sandy brown",
    "Warm grey",
    "Dark indigo",
    "Midnight",
    "Bluey green",
    "Grey pink",
    "Soft purple",
    "Blood",
    "Brown red",
    "Medium grey",
    "Berry",
    "Poo",
    "Purpley pink",
    "Light salmon",
    "Snot",
    "Easter purple",
    "Light yellow green",
    "Dark navy blue",
    "Drab",
    "Light rose",
    "Rouge",
    "Purplish red",
    "Slime green",
    "Baby poop",
    "Irish green",
    "Pink/purple",
    "Dark navy",
    "Greeny blue",
    "Light plum",
    "Pinkish grey",
    "Dirty orange",
    "Rust red",
    "Pale lilac",
    "Orangey red",
    "Primary blue",
    "Kermit green",
    "Brownish purple",
    "Murky green",
    "Wheat",
    "Very dark purple",
    "Bottle green",
    "Watermelon",
    "Deep sky blue",
    "Fire engine red",
    "Yellow ochre",
    "Pumpkin orange",
    "Pale olive",
    "Light lilac",
    "Lightish green",
    "Carolina blue",
    "Mulberry",
    "Shocking pink",
    "Auburn",
    "Bright lime green",
    "Celadon",
    "Pinkish brown",
    "Poo brown",
    "Bright sky blue",
    "Celery",
    "Dirt brown",
    "Strawberry",
    "Dark lime",
    "Copper",
    "Medium brown",
    "Muted green",
    "Robin's egg",
    "Bright aqua",
    "Bright lavender",
    "Ivory",
    "Very light purple",
    "Light navy",
    "Pink red",
    "Olive brown",
    "Poop brown",
    "Mustard green",
    "Ocean green",
    "Very dark blue",
    "Dusty green",
    "Light navy blue",
    "Minty green",
    "Adobe",
    "Barney",
    "Jade green",
    "Bright light blue",
    "Light lime",
    "Dark khaki",
    "Orange yellow",
    "Ocre",
    "Maize",
    "Faded pink",
    "British racing green",
    "Sandstone",
    "Mud brown",
    "Light sea green",
    "Robin egg blue",
    "Aqua marine",
    "Dark sea green",
    "Soft pink",
    "Orangey brown",
    "Cherry red",
    "Burnt yellow",
    "Brownish grey",
    "Camel",
    "Purplish grey",
    "Marine",
    "Greyish pink",
    "Pale turquoise",
    "Pastel yellow",
    "Bluey purple",
    "Canary yellow",
    "Faded red",
    "Sepia",
    "Coffee",
    "Bright magenta",
    "Mocha",
    "Ecru",
    "Purpleish",
    "Cranberry",
    "Darkish green",
    "Brown orange",
    "Dusky rose",
    "Melon",
    "Sickly green",
    "Silver",
    "Purply blue",
    "Purpleish blue",
    "Hospital green",
    "Shit brown",
    "Mid blue",
    "Amber",
    "Easter green",
    "Soft blue",
    "Cerulean blue",
    "Golden brown",
    "Bright turquoise",
    "Red pink",
    "Red purple",
    "Greyish brown",
    "Vermillion",
    "Russet",
    "Steel grey",
    "Lighter purple",
    "Bright violet",
    "Prussian blue",
    "Slate green",
    "Dirty pink",
    "Dark blue green",
    "Pine",
    "Yellowy green",
    "Dark gold",
    "Bluish",
    "Darkish blue",
    "Dull red",
    "Pinky red",
    "Bronze",
    "Pale teal",
    "Military green",
    "Barbie pink",
    "Bubblegum pink",
    "Pea soup green",
    "Dark mustard",
    "Shit",
    "Medium purple",
    "Very dark green",
    "Dirt",
    "Dusky pink",
    "Red violet",
    "Lemon yellow",
    "Pistachio",
    "Dull yellow",
    "Dark lime green",
    "Denim blue",
    "Teal blue",
    "Lightish blue",
    "Purpley blue",
    "Light indigo",
    "Swamp green",
    "Brown green",
    "Dark maroon",
    "Hot purple",
    "Dark forest green",
    "Faded blue",
    "Drab green",
    "Light lime green",
    "Snot green",
    "Yellowish",
    "Light blue green",
    "Bordeaux",
    "Light mauve",
    "Ocean",
    "Marigold",
    "Muddy green",
    "Dull orange",
    "Steel",
    "Electric purple",
    "Fluorescent green",
    "Yellowish brown",
    "Blush",
    "Soft green",
    "Bright orange",
    "Lemon",
    "Purple grey",
    "Acid green",
    "Pale lavender",
    "Violet blue",
    "Light forest green",
    "Burnt red",
    "Khaki green",
    "Cerise",
    "Faded purple",
    "Apricot",
    "Dark olive green",
    "Grey brown",
    "Green grey",
    "True blue",
    "Pale violet",
    "Periwinkle blue",
    "Light sky blue",
    "Blurple",
    "Green brown",
    "Bluegreen",
    "Bright teal",
    "Brownish yellow",
    "Pea soup",
    "Forest",
    "Barney purple",
    "Ultramarine",
    "Purplish",
    "Puke yellow",
    "Bluish grey",
    "Dark periwinkle",
    "Dark lilac",
    "Reddish",
    "Light maroon",
    "Dusty purple",
    "Terra cotta",
    "Avocado",
    "Marine blue",
    "Teal green",
    "Slate grey",
    "Lighter green",
    "Electric green",
    "Dusty blue",
    "Golden yellow",
    "Bright yellow",
    "Light lavender",
    "Umber",
    "Poop",
    "Dark peach",
    "Jungle green",
    "Eggshell",
    "Denim",
    "Yellow brown",
    "Dull purple",
    "Chocolate brown",
    "Wine red",
    "Neon blue",
    "Dirty green",
    "Light tan",
    "Ice blue",
    "Cadet blue",
    "Dark mauve",
    "Very light blue",
    "Grey purple",
    "Pastel pink",
    "Very light green",
    "Dark sky blue",
    "Evergreen",
    "Dull pink",
    "Aubergine",
    "Mahogany",
    "Reddish orange",
    "Deep green",
    "Vomit green",
    "Purple pink",
    "Dusty pink",
    "Faded green",
    "Camo green",
    "Pinky purple",
    "Pink purple",
    "Brownish red",
    "Dark rose",
    "Mud",
    "Brownish",
    "Emerald green",
    "Pale brown",
    "Dull blue",
    "Burnt umber",
    "Medium green",
    "Clay",
    "Light aqua",
    "Light olive green",
    "Brownish orange",
    "Dark aqua",
    "Purplish pink",
    "Dark salmon",
    "Greenish grey",
    "Jade",
    "Ugly green",
    "Dark beige",
    "Emerald",
    "Pale red",
    "Light magenta",
    "Sky",
    "Light cyan",
    "Yellow orange",
    "Reddish purple",
    "Reddish pink",
    "Orchid",
    "Dirty yellow",
    "Orange red",
    "Deep red",
    "Orange brown",
    "Cobalt blue",
    "Neon pink",
    "Rose pink",
    "Greyish purple",
    "Raspberry",
    "Aqua green",
    "Salmon pink",
    "Tangerine",
    "Brownish green",
    "Red brown",
    "Greenish brown",
    "Pumpkin",
    "Pine green",
    "Charcoal",
    "Baby pink",
    "Cornflower",
    "Blue violet",
    "Chocolate",
    "Greyish green",
    "Scarlet",
    "Green yellow",
    "Dark olive",
    "Sienna",
    "Pastel purple",
    "Terracotta",
    "Aqua blue",
    "Sage green",
    "Blood red",
    "Deep pink",
    "Grass",
    "Moss",
    "Pastel blue",
    "Bluish green",
    "Green blue",
    "Dark tan",
    "Greenish blue",
    "Pale orange",
    "Vomit",
    "Forrest green",
    "Dark lavender",
    "Dark violet",
    "Purple blue",
    "Dark cyan",
    "Olive drab",
    "Pinkish",
    "Cobalt",
    "Neon purple",
    "Light turquoise",
    "Apple green",
    "Dull green",
    "Wine",
    "Powder blue",
    "Off white",
    "Electric blue",
    "Dark turquoise",
    "Blue purple",
    "Azure",
    "Bright red",
    "Pinkish red",
    "Cornflower blue",
    "Light olive",
    "Grape",
    "Greyish blue",
    "Purplish blue",
    "Yellowish green",
    "Greenish yellow",
    "Medium blue",
    "Dusty rose",
    "Light violet",
    "Midnight blue",
    "Bluish purple",
    "Red orange",
    "Dark magenta",
    "Greenish",
    "Ocean blue",
    "Coral",
    "Cream",
    "Reddish brown",
    "Burnt sienna",
    "Brick",
    "Sage",
    "Grey green",
    "White",
    "Robin's egg blue",
    "Moss green",
    "Steel blue",
    "Eggplant",
    "Light yellow",
    "Leaf green",
    "Light grey",
    "Puke",
    "Pinkish purple",
    "Sea blue",
    "Pale purple",
    "Slate blue",
    "Blue grey",
    "Hunter green",
    "Fuchsia",
    "Crimson",
    "Pale yellow",
    "Ochre",
    "Mustard yellow",
    "Light red",
    "Cerulean",
    "Pale pink",
    "Deep blue",
    "Rust",
    "Light teal",
    "Slate",
    "Goldenrod",
    "Dark yellow",
    "Dark grey",
    "Army green",
    "Grey blue",
    "Seafoam",
    "Puce",
    "Spring green",
    "Dark orange",
    "Sand",
    "Pastel green",
    "Mint",
    "Light orange",
    "Bright pink",
    "Chartreuse",
    "Deep purple",
    "Dark brown",
    "Taupe",
    "Pea green",
    "Puke green",
    "Kelly green",
    "Seafoam green",
    "Blue green",
    "Khaki",
    "Burgundy",
    "Dark teal",
    "Brick red",
    "Royal purple",
    "Plum",
    "Mint green",
    "Gold",
    "Baby blue",
    "Yellow green",
    "Bright purple",
    "Dark red",
    "Pale blue",
    "Grass green",
    "Navy",
    "Aquamarine",
    "Burnt orange",
    "Neon green",
    "Bright blue",
    "Rose",
    "Light pink",
    "Mustard",
    "Indigo",
    "Lime",
    "Sea green",
    "Periwinkle",
    "Dark pink",
    "Olive green",
    "Peach",
    "Pale green",
    "Light brown",
    "Hot pink",
    "Black",
    "Lilac",
    "Navy blue",
    "Royal blue",
    "Beige",
    "Salmon",
    "Olive",
    "Maroon",
    "Bright green",
    "Dark purple",
    "Mauve",
    "Forest green",
    "Aqua",
    "Cyan",
    "Tan",
    "Dark blue",
    "Lavender",
    "Turquoise",
    "Dark green",
    "Violet",
    "Light purple",
    "Lime green",
    "Grey",
    "Sky blue",
    "Yellow",
    "Magenta",
    "Light green",
    "Orange",
    "Teal",
    "Light blue",
    "Red",
    "Brown",
    "Pink",
    "Blue",
    "Green",
    "Purple"
};

vector<string> state_names = {
    "Alabama",
    "Alaska",
    "Arizona",
    "Arkansas",
    "California",
    "Colorado",
    "Connecticut",
    "Delaware",
    "Florida",
    "Georgia",
    "Hawaii",
    "Idaho",
    "Illinois",
    "Indiana",
    "Iowa",
    "Kansas",
    "Kentucky",
    "Louisiana",
    "Maine",
    "Maryland",
    "Massachusetts",
    "Michigan",
    "Minnesota",
    "Mississippi",
    "Missouri",
    "Montana",
    "Nebraska",
    "Nevada",
    "New Hampshire",
    "New Jersey",
    "New Mexico",
    "New York",
    "North Carolina",
    "North Dakota",
    "Ohio",
    "Oklahoma",
    "Oregon",
    "Pennsylvania",
    "Rhode Island",
    "South Carolina",
    "South Dakota",
    "Tennessee",
    "Texas",
    "Utah",
    "Vermont",
    "Virginia",
    "Washington",
    "West Virginia",
    "Wisconsin",
    "Wyoming"
};

vector<vector<int>> fave_rgb_colors = {
    {172, 194, 217},{86, 174, 87},
    {178, 153, 110},
    {168, 255, 4},    {105, 216, 79},{137, 69, 133},
    {112, 178, 63},    {212, 255, 255},
    {101, 171, 124},    {149, 46, 143},
    {252, 252, 129},    {165, 163, 145},
    {56, 128, 4},    {76, 144, 133},
    {94, 155, 138},    {239, 180, 53},
    {217, 155, 130},
    {10, 95, 56},
    {12, 6, 247},
    {97, 222, 42},
    {55, 120, 191},
    {34, 66, 199},
    {83, 60, 198},
    {155, 181, 60},
    {5, 255, 166},
    {31, 99, 87},    {1, 115, 116},
    {12, 181, 119},    {255, 7, 137},
    {175, 168, 139},    {8, 120, 127},
    {221, 133, 215},    {166, 200, 117},{167, 255, 181},    {194, 183, 9},{231, 142, 165},    {150, 110, 189},{204, 173, 96},    {172, 134, 168},
    {148, 126, 148},{152, 63, 178},{255, 99, 233},{178, 251, 165},{99, 179, 101},{142, 229, 63},
    {183, 225, 161},{255, 111, 82},{189, 248, 163},{211, 182, 131},{255, 252, 196},{67, 5, 65},{255, 178, 208},{153, 117, 112},{173, 144, 13},{196, 142, 253},
{80, 123, 156},{125, 113, 3},{255, 253, 120},{218, 70, 125},{65, 2, 0},{201, 209, 121},
    {255, 250, 134},{86, 132, 174},{107, 124, 133},{111, 108, 10},
    {126, 64, 113},{0, 147, 55},{208, 228, 41},{255, 249, 23},{29, 93, 236},{5, 73, 7},
    {181, 206, 8},
    {143, 182, 123},
    {200, 255, 176},
    {253, 222, 108},{255, 223, 34},
    {169, 190, 112},
    {104, 50, 227},
    {253, 177, 71},
    {199, 172, 125},{255, 243, 154},
    {133, 14, 4},
    {239, 192, 254},
    {64, 253, 20},
    {182, 196, 6},
    {157, 255, 0},
    {60, 65, 66},
    {242, 171, 21},
    {172, 79, 6},
    {196, 254, 130},
    {44, 250, 31},
    {154, 98, 0},
    {202, 155, 247},
    {135, 95, 66},
    {58, 46, 254},
    {253, 141, 73},
    {139, 49, 3},
    {203, 165, 96},
    {105, 131, 57},
    {12, 220, 115},
    {183, 82, 3},
    {127, 143, 78},
    {38, 83, 141},
    {99, 169, 80},
    {200, 127, 137},
    {177, 252, 153},
    {255, 154, 138},
    {246, 104, 142},
    {118, 253, 168},
    {83, 254, 92},
    {78, 253, 84},
    {160, 254, 191},
    {123, 242, 218},
    {188, 245, 166},
    {202, 107, 2},
{16, 122, 176},
    {33, 56, 171},{113, 159, 145},
    {253, 185, 21},
    {254, 252, 175},
    {252, 246, 121},
    {29, 2, 0},
    {203, 104, 67},{49, 102, 138},
    {36, 122, 253},
    {255, 255, 182},
    {144, 253, 169},{134, 161, 125},
    {253, 220, 92},
    {120, 209, 182},{19, 187, 175},
    {251, 95, 252},{32, 249, 134},
    {255, 227, 110},
    {157, 7, 89},
    {58, 24, 177},
    {194, 255, 137},
    {215, 103, 173},
    {114, 0, 88},
    {255, 218, 3},
    {1, 192, 141},
    {172, 116, 52},
    {1, 70, 0},
    {153, 0, 250},
    {2, 6, 111},
    {142, 118, 24},
    {209, 118, 143},
    {150, 180, 3},
    {253, 255, 99},
    {149, 163, 166},
    {127, 104, 78},
    {117, 25, 115},
    {8, 148, 4},
    {255, 97, 99},
    {89, 133, 86},
    {33, 71, 97},
    {60, 115, 168},
    {186, 158, 136},
    {2, 27, 249},
    {115, 74, 101},
    {35, 196, 139},
    {143, 174, 34},
    {230, 242, 162},
    {75, 87, 219},
    {217, 1, 102},
    {1, 84, 130},
    {157, 2, 22},
    {114, 143, 2},
    {255, 229, 173},
    {78, 5, 80},
    {249, 188, 8},
    {255, 7, 58},
    {199, 121, 134},
    {214, 255, 254},
    {254, 75, 3},
    {253, 89, 86},
    {252, 225, 102},
    {178, 113, 61},
    {31, 59, 77},
    {105, 157, 76},
    {86, 252, 162},
    {251, 85, 129},
    {62, 130, 252},
    {160, 191, 22},
    {214, 255, 250},
    {79, 115, 142},
    {255, 177, 154},
    {92, 139, 21},
    {84, 172, 104},
    {137, 160, 176},
    {126, 160, 122},
    {27, 252, 6},
    {202, 255, 251},
    {182, 255, 187},
    {167, 94, 9},
    {21, 46, 255},
    {141, 94, 183},
    {95, 158, 143},
    {99, 247, 180},
    {96, 102, 2},
    {252, 134, 170},
    {140, 0, 52},
    {117, 128, 0},
    {171, 126, 76},
    {3, 7, 100},
    {254, 134, 164},
    {213, 23, 78},
    {254, 208, 252},
    {104, 0, 24},
    {254, 223, 8},
    {254, 66, 15},
    {111, 124, 0},
    {202, 1, 71},
    {27, 36, 49},
    {0, 251, 176},
    {219, 88, 86},
    {221, 214, 24},
    {65, 253, 254},
    {207, 82, 78},
    {33, 195, 111},
    {169, 3, 8},
    {110, 16, 5},
    {254, 130, 140},
    {75, 97, 19},
    {77, 164, 9},
    {190, 174, 138},
    {3, 57, 248},
    {168, 143, 89},
    {93, 33, 208},
    {254, 178, 9},
    {78, 81, 139},
    {150, 78, 2},
    {133, 163, 178},
    {255, 105, 175},
    {195, 251, 244},
    {42, 254, 183},
    {0, 95, 106},
    {12, 23, 147},
    {255, 255, 129},
    {240, 131, 58},
    {241, 243, 63},
    {177, 210, 123},
    {252, 130, 74},
    {113, 170, 52},
    {183, 201, 226},
    {75, 1, 1},
    {165, 82, 230},
    {175, 47, 13},
    {139, 136, 248},
    {154, 247, 100},
    {166, 251, 178},
    {255, 197, 18},
    {117, 8, 81},
    {193, 74, 9},
    {254, 47, 74},
    {2, 3, 226},
    {10, 67, 122},
    {165, 0, 85},
    {174, 139, 12},
    {253, 121, 143},
    {191, 172, 5},
    {62, 175, 118},
    {199, 71, 103},
    {185, 72, 78},
    {100, 125, 142},
    {191, 254, 40},
    {215, 37, 222},
    {178, 151, 5},
    {103, 58, 63},
    {168, 125, 194},
    {250, 254, 75},
    {192, 2, 47},
    {14, 135, 204},
    {141, 132, 104},
    {173, 3, 222},
    {140, 255, 158},
    {148, 172, 2},
    {196, 255, 247},
    {253, 238, 115},
    {51, 184, 100},
    {255, 249, 208},
    {117, 141, 163},
    {245, 4, 201},
    {119, 161, 181},
    {135, 86, 228},
    {136, 151, 23},
    {194, 126, 121},
    {1, 115, 113},
    {159, 131, 3},
    {247, 213, 96},
    {189, 246, 254},
    {117, 184, 79},
    {156, 187, 4},
    {41, 70, 91},
    {105, 96, 6},
    {173, 248, 2},
    {193, 198, 252},
    {53, 173, 107},
    {255, 253, 55},
    {164, 66, 160},
    {243, 97, 150},
    {148, 119, 6},
    {255, 244, 242},
    {30, 145, 103},
    {181, 195, 6},
    {254, 255, 127},
    {207, 253, 188},
    {10, 221, 8},
    {135, 253, 5},
    {30, 248, 118},
    {123, 253, 199},
    {188, 236, 172},
    {187, 249, 15},
    {171, 144, 4},
    {31, 181, 122},
    {0, 85, 90},
    {164, 132, 172},
    {196, 85, 8},
    {63, 130, 157},
    {84, 141, 68},
    {201, 94, 251},
    {58, 229, 127},
    {1, 103, 149},
    {135, 169, 34},
    {240, 148, 77},
    {93, 20, 81},
    {37, 255, 41},
    {208, 254, 29},
    {255, 166, 43},
    {1, 180, 76},
    {255, 108, 181},
    {107, 66, 71},
    {199, 193, 12},
    {183, 255, 250},
    {174, 255, 110},
    {236, 45, 1},
    {118, 255, 123},
    {115, 0, 57},
    {4, 3, 72},
    {223, 78, 200},
    {110, 203, 60},
    {143, 152, 5},
    {94, 220, 31},
    {217, 79, 245},
    {200, 253, 61},
    {7, 13, 13},
    {73, 132, 184},
    {81, 183, 59},
    {172, 126, 4},
    {78, 84, 129},
    {135, 110, 75},
    {88, 188, 8},
    {47, 239, 16},
    {45, 254, 84},
    {10, 255, 2},
    {156, 239, 67},
    {24, 209, 123},
    {53, 83, 10},
    {24, 5, 219},
    {98, 88, 196},
    {255, 150, 79},
    {255, 171, 15},
    {143, 140, 231},
    {36, 188, 168},
    {63, 1, 44},
    {203, 248, 95},
    {255, 114, 76},
    {40, 1, 55},
    {179, 111, 246},
    {72, 192, 114},
    {188, 203, 122},
    {168, 65, 91},
    {6, 177, 196},
    {205, 117, 132},
    {241, 218, 122},
    {255, 4, 144},
    {128, 91, 135},
    {80, 167, 71},
    {168, 164, 149},
    {207, 255, 4},
    {255, 255, 126},
    {255, 127, 167},
    {239, 64, 38},
    {60, 153, 146},
    {136, 104, 6},
    {4, 244, 137},
    {254, 246, 158},
    {207, 175, 123},
    {59, 113, 159},
    {253, 193, 197},
    {32, 192, 115},
    {155, 95, 192},
    {15, 155, 142},
    {116, 40, 2},
    {157, 185, 44},
    {164, 191, 32},
    {205, 89, 9},
    {173, 165, 135},
    {190, 1, 60},
    {184, 255, 235},
    {220, 77, 1},
    {162, 101, 62},
    {99, 139, 39},
    {65, 156, 3},
    {177, 255, 101},
    {157, 188, 212},
    {253, 253, 254},
    {119, 171, 86},
    {70, 65, 150},
    {153, 1, 71},
    {190, 253, 115},
    {50, 191, 132},
    {175, 111, 9},
    {160, 2, 92},
    {255, 216, 177},
    {127, 78, 30},
    {191, 155, 12},
    {107, 163, 83},
    {240, 117, 230},
    {123, 200, 246},
    {71, 95, 148},
    {245, 191, 3},
    {255, 254, 182},
    {255, 253, 116},
    {137, 91, 123},
    {67, 107, 173},
    {208, 193, 1},
    {198, 248, 8},
    {244, 54, 5},
    {2, 193, 77},
    {178, 95, 3},
    {42, 126, 25},
    {73, 6, 72},
    {83, 98, 103},
    {90, 6, 239},
    {207, 2, 52},
    {196, 166, 97},
    {151, 138, 132},
    {31, 9, 84},
    {3, 1, 45},
    {43, 177, 121},
    {195, 144, 155},
    {166, 111, 181},
    {119, 0, 1},
    {146, 43, 5},
    {125, 127, 124},
    {153, 15, 75},
    {143, 115, 3},
    {200, 60, 185},
    {254, 169, 147},
    {172, 187, 13},
    {192, 113, 254},
    {204, 253, 127},
    {0, 2, 46},
    {130, 131, 68},
    {255, 197, 203},
    {171, 18, 57},
    {176, 5, 75},
    {153, 204, 4},
    {147, 124, 0},
    {1, 149, 41},
    {239, 29, 231},
    {0, 4, 53},
    {66, 179, 149},
    {157, 87, 131},
    {200, 172, 169},
    {200, 118, 6},
    {170, 39, 4},
    {228, 203, 255},
    {250, 66, 36},
    {8, 4, 249},
    {92, 178, 0},
    {118, 66, 78},
    {108, 122, 14},
    {251, 221, 126},
    {42, 1, 52},
    {4, 74, 5},
    {253, 70, 89},
    {13, 117, 248},
    {254, 0, 2},
    {203, 157, 6},
    {251, 125, 7},
    {185, 204, 129},
    {237, 200, 255},
    {97, 225, 96},
    {138, 184, 254},
    {146, 10, 78},
    {254, 2, 162},
    {154, 48, 1},
    {101, 254, 8},
    {190, 253, 183},
    {177, 114, 97},
    {136, 95, 1},
    {2, 204, 254},
    {193, 253, 149},
    {131, 101, 57},
    {251, 41, 67},
    {132, 183, 1},
    {182, 99, 37},
    {127, 81, 18},
    {95, 160, 82},
    {109, 237, 253},
    {11, 249, 234},
    {199, 96, 255},
    {255, 255, 203},
    {246, 206, 252},
    {21, 80, 132},
    {245, 5, 79},
    {100, 84, 3},
    {122, 89, 1},
    {168, 181, 4},
    {61, 153, 115},
    {0, 1, 51},
    {118, 169, 115},
    {46, 90, 136},
    {11, 247, 125},
    {189, 108, 72},
    {172, 29, 184},
    {43, 175, 106},
    {38, 247, 253},
    {174, 253, 108},
    {155, 143, 85},
    {255, 173, 1},
    {198, 156, 4},
    {244, 208, 84},
    {222, 157, 172},
    {5, 72, 13},
    {201, 174, 116},
    {96, 70, 15},
    {152, 246, 176},
    {138, 241, 254},
    {46, 232, 187},
    {17, 135, 93},
    {253, 176, 192},
    {177, 96, 2},
    {247, 2, 42},
    {213, 171, 9},
    {134, 119, 95},
    {198, 159, 89},
    {122, 104, 127},
    {4, 46, 96},
    {200, 141, 148},
    {165, 251, 213},
    {255, 254, 113},
    {98, 65, 199},
    {255, 254, 64},
    {211, 73, 78},
    {152, 94, 43},
    {166, 129, 76},
    {255, 8, 232},
    {157, 118, 81},
    {254, 255, 202},
    {152, 86, 141},
    {158, 0, 58},
    {40, 124, 55},
    {185, 105, 2},
    {186, 104, 115},
    {255, 120, 85},
    {148, 178, 28},
    {197, 201, 199},
    {102, 26, 238},
    {97, 64, 239},
    {155, 229, 170},
    {123, 88, 4},
    {39, 106, 179},
    {254, 179, 8},
    {140, 253, 126},
    {100, 136, 234},
    {5, 110, 238},
    {178, 122, 1},
    {15, 254, 249},
    {250, 42, 85},
    {130, 7, 71},
    {122, 106, 79},
    {244, 50, 12},
    {161, 57, 5},
    {111, 130, 138},
    {165, 90, 244},
    {173, 10, 253},
    {0, 69, 119},
    {101, 141, 109},
    {202, 123, 128},
    {0, 82, 73},
    {43, 93, 52},
    {191, 241, 40},
    {181, 148, 16},
    {41, 118, 187},
    {1, 65, 130},
    {187, 63, 63},
    {252, 38, 71},
    {168, 121, 0},
    {130, 203, 178},
    {102, 124, 62},
    {254, 70, 165},
    {254, 131, 204},
    {148, 166, 23},
    {168, 137, 5},
    {127, 95, 0},
    {158, 67, 162},
    {6, 46, 3},
    {138, 110, 69},
    {204, 122, 139},
    {158, 1, 104},
    {253, 255, 56},
    {192, 250, 139},
    {238, 220, 91},
    {126, 189, 1},
    {59, 91, 146},
    {1, 136, 159},
    {61, 122, 253},
    {95, 52, 231},
    {109, 90, 207},
    {116, 133, 0},
    {112, 108, 17},
    {60, 0, 8},
    {203, 0, 245},
    {0, 45, 4},
    {101, 140, 187},
    {116, 149, 81},
    {185, 255, 102},
    {157, 193, 0},
    {250, 238, 102},
    {126, 251, 179},
    {123, 0, 44},
    {194, 146, 161},
    {1, 123, 146},
    {252, 192, 6},
    {101, 116, 50},
    {216, 134, 59},
    {115, 133, 149},
    {170, 35, 255},
    {8, 255, 8},
    {155, 122, 1},
    {242, 158, 142},
    {111, 194, 118},
    {255, 91, 0},
    {253, 255, 82},
    {134, 111, 133},
    {143, 254, 9},
    {238, 207, 254},
    {81, 10, 201},
    {79, 145, 83},
    {159, 35, 5},
    {114, 134, 57},
    {222, 12, 98},
    {145, 110, 153},
    {255, 177, 109},
    {60, 77, 3},
    {127, 112, 83},
    {119, 146, 111},
    {1, 15, 204},
    {206, 174, 250},
    {143, 153, 251},
    {198, 252, 255},
    {85, 57, 204},
    {84, 78, 3},
    {1, 122, 121},
    {1, 249, 198},
    {201, 176, 3},
    {146, 153, 1},
    {11, 85, 9},
    {160, 4, 152},
    {32, 0, 177},
    {148, 86, 140},
    {194, 190, 14},
    {116, 139, 151},
    {102, 95, 209},
    {156, 109, 165},
    {196, 66, 64},
    {162, 72, 87},
    {130, 95, 135},
    {201, 100, 59},
    {144, 177, 52},
    {1, 56, 106},
    {37, 163, 111},
    {89, 101, 109},
    {117, 253, 99},
    {33, 252, 13},
    {90, 134, 173},
    {254, 198, 21},
    {255, 253, 1},
    {223, 197, 254},
    {178, 100, 0},
    {127, 94, 0},
    {222, 126, 93},
    {4, 130, 67},
    {255, 255, 212},
    {59, 99, 140},
    {183, 148, 0},
    {132, 89, 126},
    {65, 25, 0},
    {123, 3, 35},
    {4, 217, 255},
    {102, 126, 44},
    {251, 238, 172},
    {215, 255, 254},
    {78, 116, 150},
    {135, 76, 98},
    {213, 255, 255},
    {130, 109, 140},
    {255, 186, 205},
    {209, 255, 189},
    {68, 142, 228},
    {5, 71, 42},
    {213, 134, 157},
    {61, 7, 52},
    {74, 1, 0},
    {248, 72, 28},
    {2, 89, 15},
    {137, 162, 3},
    {224, 63, 216},
    {213, 138, 148},
    {123, 178, 116},
    {82, 101, 37},
    {201, 76, 190},
    {219, 75, 218},
    {158, 54, 35},
    {181, 72, 93},
    {115, 92, 18},
    {156, 109, 87},
    {2, 143, 30},
    {177, 145, 110},
    {73, 117, 156},
    {160, 69, 14},
    {57, 173, 72},
    {182, 106, 80},
    {140, 255, 219},
    {164, 190, 92},
    {203, 119, 35},
    {5, 105, 107},
    {206, 93, 174},
    {200, 90, 83},
    {150, 174, 141},
    {31, 167, 116},
    {122, 151, 3},
    {172, 147, 98},
    {1, 160, 73},
    {217, 84, 77},
    {250, 95, 247},
    {130, 202, 252},
    {172, 255, 252},
    {252, 176, 1},
    {145, 9, 81},
    {254, 44, 84},
    {200, 117, 196},
    {205, 197, 10},
    {253, 65, 30},
    {154, 2, 0},
    {190, 100, 0},
    {3, 10, 167},
    {254, 1, 154},
    {247, 135, 154},
    {136, 113, 145},
    {176, 1, 73},
    {18, 225, 147},
    {254, 123, 124},
    {255, 148, 8},
    {106, 110, 9},
    {139, 46, 22},
    {105, 97, 18},
    {225, 119, 1},
    {10, 72, 30},
    {52, 56, 55},
    {255, 183, 206},
    {106, 121, 247},
    {93, 6, 233},
    {61, 28, 2},
    {130, 166, 125},
    {190, 1, 25},
    {201, 255, 39},
    {55, 62, 2},
    {169, 86, 30},
    {202, 160, 255},
    {202, 102, 65},
    {2, 216, 233},
    {136, 179, 120},
    {152, 0, 2},
    {203, 1, 98},
    {92, 172, 45},
    {118, 153, 88},
    {162, 191, 254},
    {16, 166, 116},
    {6, 180, 139},
    {175, 136, 74},
    {11, 139, 135},
    {255, 167, 86},
    {162, 164, 21},
    {21, 68, 6},
    {133, 103, 152},
    {52, 1, 63},
    {99, 45, 233},
    {10, 136, 138},
    {111, 118, 50},
    {212, 106, 126},
    {30, 72, 143},
    {188, 19, 254},
    {126, 244, 204},
    {118, 205, 38},
    {116, 166, 98},
    {128, 1, 63},
    {177, 209, 252},
    {255, 255, 228},
    {6, 82, 255},
    {4, 92, 90},
    {87, 41, 206},
    {6, 154, 243},
    {255, 0, 13},
    {241, 12, 69},
    {81, 112, 215},
    {172, 191, 105},
    {108, 52, 97},
    {94, 129, 157},
    {96, 30, 249},
    {176, 221, 22},
    {205, 253, 2},
    {44, 111, 187},
    {192, 115, 122},
    {214, 180, 252},
    {2, 0, 53},
    {112, 59, 231},
    {253, 60, 6},
    {150, 0, 86},
    {64, 163, 104},
    {3, 113, 156},
    {252, 90, 80},
    {255, 255, 194},
    {127, 43, 10},
    {176, 78, 15},
    {160, 54, 35},
    {135, 174, 115},
    {120, 155, 115},
    {255, 255, 255},
    {152, 239, 249},
    {101, 139, 56},
    {90, 125, 154},
    {56, 8, 53},
    {255, 254, 122},
    {92, 169, 4},
    {216, 220, 214},
    {165, 165, 2},
    {214, 72, 215},
    {4, 116, 149},
    {183, 144, 212},
    {91, 124, 153},
    {96, 124, 142},
    {11, 64, 8},
    {237, 13, 217},
    {140, 0, 15},
    {255, 255, 132},
    {191, 144, 5},
    {210, 189, 10},
    {255, 71, 76},
    {4, 133, 209},
    {255, 207, 220},
    {4, 2, 115},
    {168, 60, 9},
    {144, 228, 193},
    {81, 101, 114},
    {250, 194, 5},
    {213, 182, 10},
    {54, 55, 55},
    {75, 93, 22},
    {107, 139, 164},
    {128, 249, 173},
    {165, 126, 82},
    {169, 249, 113},
    {198, 81, 2},
    {226, 202, 118},
    {176, 255, 157},
    {159, 254, 176},
    {253, 170, 72},
    {254, 1, 177},
    {193, 248, 10},
    {54, 1, 63},
    {52, 28, 2},
    {185, 162, 129},
    {142, 171, 18},
    {154, 174, 7},
    {2, 171, 46},
    {122, 249, 171},
    {19, 126, 109},
    {170, 166, 98},
    {97, 0, 35},
    {1, 77, 78},
    {143, 20, 2},
    {75, 0, 110},
    {88, 15, 65},
    {143, 255, 159},
    {219, 180, 12},
    {162, 207, 254},
    {192, 251, 45},
    {190, 3, 253},
    {132, 0, 0},
    {208, 254, 254},
    {63, 155, 11},
    {1, 21, 62},
    {4, 216, 178},
    {192, 78, 1},
    {12, 255, 12},
    {1, 101, 252},
    {207, 98, 117},
    {255, 209, 223},
    {206, 179, 1},
    {56, 2, 130},
    {170, 255, 50},
    {83, 252, 161},
    {142, 130, 254},
    {203, 65, 107},
    {103, 122, 4},
    {255, 176, 124},
    {199, 253, 181},
    {173, 129, 80},
    {255, 2, 141},
    {0, 0, 0},
    {206, 162, 253},
    {0, 17, 70},
    {5, 4, 170},
    {230, 218, 166},
    {255, 121, 108},
    {110, 117, 14},
    {101, 0, 33},
    {1, 255, 7},
    {53, 6, 62},
    {174, 113, 129},
    {6, 71, 12},
    {19, 234, 201},
    {0, 255, 255},
    {209, 178, 111},
    {0, 3, 91},
    {199, 159, 239},
    {6, 194, 172},
    {3, 53, 0},
    {154, 14, 234},
    {191, 119, 246},
    {137, 254, 5},
    {146, 149, 145},
    {117, 187, 253},
    {255, 255, 20},
    {194, 0, 120},
    {150, 249, 123},
    {249, 115, 6},
    {2, 147, 134},
    {149, 208, 252},
    {229, 0, 0},
    {101, 55, 0},
    {255, 129, 192},
    {3, 67, 223},
    {21, 176, 26},
    {126, 30, 156}
};

//REturns a random favorite color rgb code
vector<int> return_fave_rgb(){
    
    vector<int> fave_rgb = fave_rgb_colors[rand()%fave_rgb_colors.size()];
    return fave_rgb;
    
}

//return the equivalent I ching trigram of the color
//NOT COMPLETE
string rgb_to_trigram(int r, int g, int b){
    
    string trigram;
    trigram = "fire";
    
    return trigram;
    
    
}

//POTIONS
//Generates the name of a potion using random state name + random color name
string genPotionName(){
    string potion_name = "";
    potion_name = potion_name + state_names[rand()%state_names.size()];
    potion_name = potion_name + " ";
    //potion_name = potion_name + fave_color_names[rand()%fave_color_names.size()];
    potion_name = potion_name + hexagrams[rand()%hexagrams.size()];
    return potion_name;
}

//Generates -ine name
//These can be the names of the active ingrediant in the above potions
string genOilName(){
    
    char consonants[] = "bcdfghjklmnpqrstvwxyz";
    char vowels[] = "aeiou";
    
    string chem_name = "";
    
    chem_name += consonants[rand()%21];
    chem_name += vowels[rand()%5];
    chem_name += consonants[rand()%21];
    //posibly add another syllable
    if(rand()%2==1){
        chem_name += vowels[rand()%5];
        chem_name += consonants[rand()%21];
    }
    chem_name += "ine";
    
    return chem_name;
    
}

//Generates -ium -ide name
//These can be the names of the active ingrediant in the above powders
string genSaltName(){

    char consonants[] = "bcdfghjklmnpqrstvwxyz";
    char vowels[] = "aeiou";
    
    string chem_name = "";
    
    chem_name += consonants[rand()%21];
    chem_name += vowels[rand()%5];
    chem_name += consonants[rand()%21];
    //posibly add another syllable
    if(rand()%2==1){
        chem_name += vowels[rand()%5];
        chem_name += consonants[rand()%21];
    }
    chem_name += "ium ";
    
    chem_name += consonants[rand()%21];
    chem_name += vowels[rand()%5];
    chem_name += consonants[rand()%21];
    //posibly add another syllable
    if(rand()%2==1){
        chem_name += vowels[rand()%5];
        chem_name += consonants[rand()%21];
    }
    chem_name += "ide";
    
    return chem_name;
    
}

//Generates a glass name
//either a (Pyr)-ex name
string genGlassName(){
    
    char consonants[] = "bcdfghjklmnpqrstvwxyz";
    char vowels[] = "aeiou";
    
    string glass_name = "";
        
    glass_name += toupper(consonants[rand()%21]);
    glass_name += vowels[rand()%5];
    glass_name += consonants[rand()%21];
    glass_name += "ex";
    
    return glass_name;
    
}

//Generates a cloth name
// consonant - vowel - consonant - consonant
//Ex: "hemp" or "gexj" - (kind of occult-sounding)
string genClothName(){
    char consonants[] = "bcdfghjklmnpqrstvwxyz";
    char vowels[] = "aeiou";
    
    string cloth_name = "";
    
    cloth_name += consonants[rand()%21];
    cloth_name += vowels[rand()%5];
    cloth_name += consonants[rand()%21];
    //cloth_name += "'";
    cloth_name += consonants[rand()%21];
    
    return cloth_name;
}

//ZODIAC
//Implementing a codified system of the Zodiac...
//Each Zodiac Sign assigned a code
//0 - Aries
//1 - Taurus
//2 - Gemini
//3 - Cancer
//4 - Leo
//5 - Virgo
//6 - Libra
//7 - Scorpio
//8 - Sagittarius
//9 - Capricorn
//10 - Aquarius
//11 - Pisces

//Zodiac compatibility chart
//Each row corresponds to one of the zodiac signs (according to above chart)
//Column refers to compatibility of the sign to another sign
//0 - compatible
//1 - less compatible
//2 - non-compatible
int zodiac_compat[12][12] = {
    
    {0,1,0,2,0,1,0,1,0,2,0,1}, //Aries
    {1,0,1,0,2,0,1,0,1,0,2,0}, //Taurus
    {0,1,0,1,0,2,0,1,0,1,0,2}, //Gemini
    {2,0,1,0,1,0,2,0,1,0,1,0}, //Cancer
    {0,2,0,1,0,1,0,2,0,1,0,1}, //Leo
    {1,0,2,0,1,0,1,0,2,0,1,0}, //Virgo
    {0,1,0,2,0,1,0,1,0,2,0,1}, //Libra
    {1,0,1,0,2,0,1,0,1,0,2,0}, //Scorpio
    {0,1,0,1,0,2,0,1,0,1,0,2}, //Sagittarius
    {2,0,1,0,1,0,2,0,1,0,1,0}, //Capricorn
    {0,2,0,1,0,1,0,2,0,1,0,1}, //Aquarius
    {1,0,2,0,1,0,1,0,2,0,1,0}  //Pisces
    
};
//queries the above table
int find_compatibility(int z1, int z2){
    if(z1 >= 12 || z2 >= 12){
        return 5;
    }
    return zodiac_compat[z1][z2];
}

//returns the element of the entered zodiac sign code: 1: Air, 2: Fire, 3: Water, 4:Earth
/*
 * 1: Air - Gemini, Libra, Aquarius
 * 2: Fire - Aries, Leo, Sagittarius
 * 3: Water - Cancer, Scorpio, Pisces
 * 4: Earth - Taurus, Virgo, Capricorn
 */
int find_zodiac_element(int z){
    
    //Refer to the table above of zodiac sign codes
    if(z == 2 || z == 6 || z == 10){
        return 1;
    }else if(z == 0 || z == 4 || z == 8){
        return 2;
    }else if(z == 3 || z == 7 || z == 11){
        return 3;
    }else if(z == 1 || z == 5 || z == 9){
        return 4;
    }
    
    return 0;
    
}


//////////////////
//HOTEL

vector<string> wine_aromas = {
    //Primary
    "iris", "peony", "elderflower", "acacia", "lilac", "jasmine", "honeysuckle", "violet", "lavender", "rose", "potpourri", "hibiscus",
    "lime", "lemon", "grapefruit", "orange", "marmalade",
    "quince", "apple", "pear", "nectarine", "peach", "apricot", "persimmon",
    "pineapple", "mango", "guava", "passion fruit", "lychee", "bubblegum",
    "cranberry", "red plum", "pomegranate", "sour cherry", "strawberry", "cherry", "raspbery",
    "boysenberry", "black currant", "black cherry", "plum", "blackberry", "blueberry", "olive",
    "raisin", "fig", "date", "fruitcake",
    "beeswax", "ginger", "honey",
    "white pepper", "red pepper", "black pepper", "cinnamon", "anise", "5-spice", "fennel", "eucalyptus", "mint", "thyme",
    "grass", "tomato leaf", "gooseberry", "bell pepper", "jalapeno", "bitter almond", "tomato", "sun-dried tomato", "black tea",
    "clay pot", "slate", "wet gravel", "potting soil", "red beet", "volcanic rocks", "petroleum",
    
    //Secondary
    "butter", "cream", "sourdough", "lager", "truffle", "mushroom",
    
    //Tertiary
    "vanilla", "coconut", "baking spices", "cigar box", "smoke", "dill",
    "dried fruit", "nuts", "tobacco", "coffee", "cocoa", "leather",
    
    //Faults & Other
    "musty cardboard", "wet dog",
    "cured meat", "boiled eggs", "burnt rubber", "lit match", "garlic", "onion", "cat pee",
    "black cardamon", "band-aid", "sweaty leather saddle", "horse manure",
    "toffee", "stewed fruit",
    "vinegar", "nail polish remover"
    
};
vector<string> wine_sweetness = {"sweet", "semi-sweet", "dry", "sparkling"};

//Generates a wine name with a saint, vintage year, sweetness level, and fruit/aroma taste
//Ex. Saint Calire's wine MCXLII. It's semi-sweet with a hints of ___, ___, and ___.
string genWine(){
    
    string wine_name = "";
    wine_name.append(saints[rand()%saints.size()]);
    wine_name.append("'s ");
    wine_name.append("wine ");
    wine_name.append(roman_numerals(rand()%2010));
    wine_name.append(". It's ");
    wine_name.append(wine_sweetness[rand()%wine_sweetness.size()]);
    wine_name.append(" with hints of ");
    wine_name.append(wine_aromas[rand()%wine_aromas.size()]);
    wine_name.append(", ");
    wine_name.append(wine_aromas[rand()%wine_aromas.size()]);
    wine_name.append(", and ");
    wine_name.append(wine_aromas[rand()%wine_aromas.size()]);
    wine_name.append(".");
    
    //As far as meta-data, every wine bottle should have a cork-and-lebel type
//    wine_name.append(" It has a ");
//    wine_name.append(genClothName());
//    wine_name.append(" cork and label.");
    
    return wine_name;
    
}



