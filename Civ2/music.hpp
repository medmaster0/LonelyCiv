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

void SDLAudioCallback(void *UserData, Uint8 *AudioData, int Length); //Just a default audio callback to play silence (0)
void initAudio(int SamplesPerSecond, int BufferSize); //a call to initialize sdl audio


void playToneCont(int ToneHz); //plays tone continously
//These functions currently don't queue properly (possibly queuing up too much memory qise)
void playToneOnce(int ToneHz, int ms); //plays tone once for duration
void playTwoChord(int tone1, int tone2, int ms); //plays two tones for duration
void playToneList(vector<int> tones, int ms); //plays the sum of all input tones
void playToneList_Noise(vector<int> tones, int ms); //plays the sum of all input tones, adds random noise
void playToneList_Phase(vector<int> tones, int ms); //plays the sum of all input tones But with random phase between them

#endif /* music_hpp */
