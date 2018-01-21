//
//  music.hpp
//  Civ2
//
//  Created by Medz on 11/7/17.
//  Copyright © 2017 Medz. All rights reserved.
//

#ifndef music_hpp
#define music_hpp

#include <stdio.h>
#include <SDL2_mixer/SDL_mixer.h>

#include <vector>
using std::vector;

#define REST 0.0
#define A2 110.00
#define Bb2 116.54
#define B2 123.47
#define C3 130.81
#define Db3 138.59
#define D3 146.83
#define Eb3 155.56
#define E3 164.81
#define F3 174.61
#define Gb3 185.00
#define G3 196.00
#define Ab3 207.65
#define A3 220.00
#define Bb3 233.08
#define B3 246.94
#define C4 261.63
#define Db4 277.18
#define D4 293.66
#define Eb4 311.13
#define E4 329.63
#define F4 349.23
#define Gb4 369.99
#define G4 392.00
#define Ab4 415.30
#define A4 440.00
#define Bb4 466.16
#define B4 493.88
#define C5 523.25



void SDLAudioCallback(void *UserData, Uint8 *AudioData, int Length); //Just a default audio callback to play silence (0)
void initAudio(int SamplesPerSecond, int BufferSize); //a call to initialize sdl audio

void playToneCont(int ToneHz); //plays tone continously
//These functions currently don't queue properly (possibly queuing up too much memory qise)
void playToneOnce(int ToneHz, int ms); //plays tone once for duration
void playTwoChord(int tone1, int tone2, int ms); //plays two tones for duration
void playToneList(vector<int> tones, int ms); //plays the sum of all input tones
void playToneList_Noise(vector<int> tones, int ms); //plays the sum of all input tones, adds random noise
void playToneList_Phase(vector<int> tones, int ms); //plays the sum of all input tones But with random phase between them

//glitchy sound atm
void playSine(float ToneHz, int ms); //plays the sine wave at frequency, for duration ms
void playSquare(float ToneHz, int ms); //plays the squaree wave at frequency, for duration ms

//composition
//# Notes Coding
//# 0 C
//# 1 C#
//# 2 D
//# 3 D#
//# 4 E
//# 5 F
//# 6 F#
//# 7 G
//# 8 G#
//# 9 A
//# 10 A#
//# 11 B
//# 12 Hi C
//# 13 Hi C#
//# 14 Hi D
//# 15 Hi D#
//# 16 Hi E
//# 17 Hi F
//# 18 Hi F#
//# 19 Hi G
//# 20 Hi G#
//# 21 Hi A
//# 21 Hi A#
//# 23 Hi B

vector<int> genIonian(int tonic); //generates Ionian Scale as list of ints (according to above table)
vector<int> genDorian(int tonic); //generates Dorian Scale as list of ints (according to above table)
vector<int> genPhrygian(int tonic); //generates Phrygian Scale as list of ints (according to above table)
vector<int> genLydian(int tonic); //generates Lydian Scale as list of ints (according to above table)
vector<int> genMixolydian(int tonic); //generates Mixolydian Scale as list of ints (according to above table)
vector<int> genAeolian(int tonic); //generates Aeolian Scale as list of ints (according to above table)
vector<int> genLocrian(int tonic); //generates Locrian Scale as list of ints (according to above table)

vector<int> genMajorChord(int tonic); //generates a major chord based off the tonic
vector<int> genMinorChord(int tonic); //generates a minor chord based off the tonic

void printScale(vector<int> scale); //prints out a list of integers (scale)
void playScaleOnce(vector<int> scale); //plays out the list of integers (scale) with playToneOnce() function

void compose_music_thread();//A thread for keeping the music playing/composing



#endif /* music_hpp */
