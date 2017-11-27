//
//  music.cpp
//  Civ2
//
//  Created by Medz on 11/7/17.
//  Copyright © 2017 Medz. All rights reserved.
//

#include "music.hpp"
#include <SDL2_mixer/SDL_mixer.h>

#include <vector>
#include <math.h>
#include <string>

#define Pi32 3.14159265358979f

using std::vector;
using std::string;

////////////////////
//TODO: SCAT: doo WOP ba WOP
// ---------> generate strings
// ---------> generate audio

// NOTE: Sound test
int SamplesPerSecond = 48000; //This should be constant
int ToneVolume = 3000;
int BytesPerSample = sizeof(int) * 2; //because each sample takes two values for each channel (2)?
// Open our audio device:
//initAudio(48000, SamplesPerSecond * BytesPerSample / 60);
bool SoundIsPlaying = false;

//Just a default audio callback to play silence (0)
void SDLAudioCallback(void *UserData, Uint8 *AudioData, int Length)
{
    // Clear our audio buffer to silence.
    memset(AudioData, 0, Length);
    
    printf("you never call me back");
}

//a call to initialize sdl audio
//opens audio device with the correct settings (sampling freq, buffer size, etc).
void initAudio(int SamplesPerSecond, int BufferSize)
{
    SDL_AudioSpec AudioSettings = {0};
    
    AudioSettings.freq = SamplesPerSecond;
    AudioSettings.format = AUDIO_S16LSB;
    AudioSettings.channels = 2;
    AudioSettings.samples = BufferSize;
    
    SDL_OpenAudio(&AudioSettings, 0);
    
    if (AudioSettings.format != AUDIO_S16LSB)
    {
        printf("Oops! We didn't get AUDIO_S16LSB as our sample format!\n");
        SDL_CloseAudio();
    }
    
}

//Play sound continously
//Needs all those other input parameters to calculate continously
//(No need for it when we move to playToneOnce)
void playToneCont(int ToneHz){
    int SquareWavePeriod = SamplesPerSecond / ToneHz;
    int HalfSquareWavePeriod = SquareWavePeriod / 2;
    uint RunningSampleIndex = 0;
    // Sound output test
    while(true){
        int TargetQueueBytes = SamplesPerSecond * BytesPerSample;
        int BytesToWrite = TargetQueueBytes - SDL_GetQueuedAudioSize(1);
        if (BytesToWrite)
        {
            void *SoundBuffer = malloc(BytesToWrite);
            int *SampleOut = (int *)SoundBuffer;
            int SampleCount = BytesToWrite/BytesPerSample;
            for(int SampleIndex = 0;
                SampleIndex < SampleCount;
                ++SampleIndex)
            {
                int SampleValue = ((RunningSampleIndex++ / HalfSquareWavePeriod) % 2) ? ToneVolume : -ToneVolume;
                //We fill in same value twice because we use two channels
                *SampleOut++ = SampleValue;
                *SampleOut++ = SampleValue;
            }
            SDL_QueueAudio(1, SoundBuffer, BytesToWrite);
            free(SoundBuffer);
        }

        if(!SoundIsPlaying)
        {
            SDL_PauseAudio(0);
            SoundIsPlaying = true;
        }
    }
} //end playToneCont

//plays tone once for duration
void playToneOnce(int ToneHz, int ms){
    
    //First we figure how much buffer we need
    // ms * (1 s / 1000 ms) * ( samples / s )
    int samples_needed = ( ms / 1000 ) * SamplesPerSecond;
    //int BytesToWrite = BytesPerSample * samples_needed;  //Calculate How Many bytes we need
    int BytesToWrite = samples_needed; //One byte per sample?
    
    //Allocate Some Memory for these suckers
    void *SoundBuffer = malloc(BytesToWrite);
    int *SampleOut = (int *)SoundBuffer;
    
    //Calculate waveform params
    int SquareWavePeriod = SamplesPerSecond / ToneHz;
    int HalfSquareWavePeriod = SquareWavePeriod / 2; //This is the only value we really need
    
    //Go through each sample and determine if it should be high or low
    for(int i = 0; i < samples_needed; i++){
        //create a sample
        int SampleValue  = ( (i/HalfSquareWavePeriod) % 2 ) ? ToneVolume : -ToneVolume;
        *SampleOut++ = SampleValue;
    }
    SDL_QueueAudio(1, SoundBuffer, 4*BytesToWrite);
    free(SoundBuffer);
    
//    if(!SoundIsPlaying)
//    {
//        SDL_PauseAudio(0);
//        SoundIsPlaying = true;
//    }
    
    
}

//plays two tones for duration
//sums them together
//divide by two/?
void playTwoChord(int tone1, int tone2, int ms){
    
    //First we figure how much buffer we need
    // ms * (1 s / 1000 ms) * ( samples / s )
    int samples_needed = ( ms / 1000 ) * SamplesPerSecond;
    //int BytesToWrite = BytesPerSample * samples_needed;  //Calculate How Many bytes we need
    int BytesToWrite = samples_needed; //One byte per sample?
    
    //Allocate Some Memory for these suckers
    void *SoundBuffer = malloc(BytesToWrite);
    int *SampleOut = (int *)SoundBuffer;
    
    //Calculate waveform params
    int SquareWavePeriod_1 = SamplesPerSecond / tone1;
    int HalfSquareWavePeriod_1 = SquareWavePeriod_1 / 2; //This is the only value we really need
    int SquareWavePeriod_2 = SamplesPerSecond / tone2;
    int HalfSquareWavePeriod_2 = SquareWavePeriod_2 / 2; //This is the only value we really need
    
    //Go through each sample and determine if it should be high or low
    for(int i = 0; i < samples_needed; i++){
        //create a sample
        int SampleValue = 0; //initialize
        SampleValue  = ( ( ( (i/HalfSquareWavePeriod_1) % 2 ) ? ToneVolume : -ToneVolume) + ( ( (i/HalfSquareWavePeriod_2) % 2 ) ? ToneVolume : -ToneVolume) ) ; //two iff statemes (combining the two waves and normalizing the amplitudes
        *SampleOut++ = SampleValue;
    }
    SDL_QueueAudio(1, SoundBuffer, 4*BytesToWrite);
    free(SoundBuffer);
    
}

//plays the sum of all input tones
void playToneList(vector<int> tones, int ms){
    
    //First we figure how much buffer we need
    // ms * (1 s / 1000 ms) * ( samples / s )
    int samples_needed = ( ms / 1000 ) * SamplesPerSecond;
    //int BytesToWrite = BytesPerSample * samples_needed;  //Calculate How Many bytes we need
    int BytesToWrite = samples_needed; //One byte per sample?
    
    //Allocate Some Memory for these suckers
    void *SoundBuffer = malloc(BytesToWrite);
    int *SampleOut = (int *)SoundBuffer;
    
    //Calculate waveform params
    vector<int> half_wave_periods; //holds all the halfwave tones
    for(int i = 0 ; i<tones.size(); i++){
        half_wave_periods.push_back( (SamplesPerSecond/tones[i]) / 2 );
    }
    
    ////Go through each sample and calculate waveform
    for(int i = 0; i < samples_needed; i++){
        
        int SampleValue = 0; //iniitialize
        for(int j = 0; j<tones.size(); j++){ //go through the calculations of each tone
            SampleValue = SampleValue + (( (i/half_wave_periods[j]) % 2 ) ? ToneVolume : -ToneVolume); //add to the sum total
        }
        *SampleOut++ = SampleValue; //finally add sample
        
    }
    SDL_QueueAudio(1, SoundBuffer, 4*BytesToWrite);
    free(SoundBuffer);
}

//plays the sum of all input tones
//Doesn't sound so great
//adds some random noise
void playToneList_Noise(vector<int> tones, int ms){
    
    //First we figure how much buffer we need
    // ms * (1 s / 1000 ms) * ( samples / s )
    int samples_needed = ( ms / 1000 ) * SamplesPerSecond;
    //int BytesToWrite = BytesPerSample * samples_needed;  //Calculate How Many bytes we need
    int BytesToWrite = samples_needed; //One byte per sample?
    
    //Allocate Some Memory for these suckers
    void *SoundBuffer = malloc(BytesToWrite);
    int *SampleOut = (int *)SoundBuffer;
    
    //Calculate waveform params
    vector<int> half_wave_periods; //holds all the halfwave tones
    for(int i = 0 ; i<tones.size(); i++){
        half_wave_periods.push_back( (SamplesPerSecond/tones[i]) / 2 );
    }
    
    ////Go through each sample and calculate waveform
    for(int i = 0; i < samples_needed; i++){
        
        int SampleValue = 0; //iniitialize
        for(int j = 0; j<tones.size(); j++){ //go through the calculations of each tone
            SampleValue = SampleValue + (( (i/half_wave_periods[j]) % 2 ) ? ToneVolume : -ToneVolume); //add to the sum total
            SampleValue = SampleValue + (rand() % (ToneVolume/2)); //add some random noise
        }
        *SampleOut++ = SampleValue; //finally add sample
        
    }
    SDL_QueueAudio(1, SoundBuffer, 4*BytesToWrite);
    free(SoundBuffer);
}

//plays the sum of all input tones
//But with random phase between them
void playToneList_Phase(vector<int> tones, int ms){
    
    //First we figure how much buffer we need
    // ms * (1 s / 1000 ms) * ( samples / s )
    int samples_needed = ( ms / 1000 ) * SamplesPerSecond;
    //int BytesToWrite = BytesPerSample * samples_needed;  //Calculate How Many bytes we need
    int BytesToWrite = samples_needed; //One byte per sample?
    
    //Allocate Some Memory for these suckers
    void *SoundBuffer = malloc(BytesToWrite);
    int *SampleOut = (int *)SoundBuffer;
    
    //Calculate waveform params
    vector<int> half_wave_periods; //holds all the halfwave tones
    vector<int> phase_offsets; //contains random phases to offset each tone by
    for(int i = 0 ; i<tones.size(); i++){
        half_wave_periods.push_back( (SamplesPerSecond/tones[i]) / 2 );
        phase_offsets.push_back( (rand()%(2*half_wave_periods[i])) );
        //phase_offsets.push_back(0);
    }
    
    ////Go through each sample and calculate waveform
    for(int i = 0; i < samples_needed; i++){
        
        int SampleValue = 0; //iniitialize
        for(int j = 0; j<tones.size(); j++){ //go through the calculations of each tone
            SampleValue = SampleValue + (( ( (i+ phase_offsets[j] ) /half_wave_periods[j]) % 2 ) ? ToneVolume : -ToneVolume); //add to the sum total
        }
        *SampleOut++ = SampleValue; //finally add sample
        
    }
    SDL_QueueAudio(1, SoundBuffer, 4*BytesToWrite);
    free(SoundBuffer);
}

//plays the sine wave at frequency, for duration ms
void playSine(float ToneHz, int ms){
    //First we figure how much buffer we need
    // ms * (1 s / 1000 ms) * ( samples / s )
    int samples_needed = ( ms / 1000 ) * SamplesPerSecond;
    int BytesToWrite = samples_needed; //One byte per sample?
    
    //Allocate Some Memory for these suckers
    void *SoundBuffer = malloc(BytesToWrite);
    int *SampleOut = (int *)SoundBuffer;
    
    //calculate waveform paramters
    float WavePeriod = (float)SamplesPerSecond / ToneHz;
    float HalfWavePeriod = WavePeriod / 2.0; //This is the only value we really need
    
    //Go through each sample and determine the value (sine)
    for(int i = 0 ; i < samples_needed; i++){
        float t = 2.0f*Pi32*i/WavePeriod;
        float SineValue = sinf(t);
        int SampleValue = (int)(SineValue*ToneVolume);
        *SampleOut++ = SampleValue;
    }
    
    SDL_QueueAudio(1, SoundBuffer, 4*BytesToWrite);
    free(SoundBuffer);
    
}

////Go through each sample and determine if it should be high or low
//for(int i = 0; i < samples_needed; i++){
//    //create a sample
//    int SampleValue  = ( (i/HalfSquareWavePeriod) % 2 ) ? ToneVolume : -ToneVolume;
//    printf("%d,,",SampleValue );
//    *SampleOut++ = SampleValue;
//}

//plays the sine wave at frequency, for duration ms
void playSquare(float ToneHz, int ms){
    //First we figure how much buffer we need
    // ms * (1 s / 1000 ms) * ( samples / s )
    int samples_needed = ( ms / 1000 ) * SamplesPerSecond;
    int BytesToWrite = samples_needed; //One byte per sample?
    
    //Allocate Some Memory for these suckers
    void *SoundBuffer = malloc(BytesToWrite);
    int *SampleOut = (int *)SoundBuffer;
    
    //calculate waveform paramters
    float WavePeriod = (float)SamplesPerSecond / ToneHz;
    float HalfWavePeriod = WavePeriod / 2.0; //This is the only value we really need
    
    //Go through each sample and determine if it should be high or low
    for(int i = 0; i < samples_needed; i++){
        //create a sample
        int SampleValue  = ( (int)(i/HalfWavePeriod) % 2 ) ? ToneVolume : -ToneVolume;
        //printf("%d,%d,%d\n",i,BytesToWrite,BytesPerSample);
        *SampleOut++ = SampleValue;
    }

    
    SDL_QueueAudio(1, SoundBuffer, 4*BytesToWrite);
    free(SoundBuffer);
}

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
//# 22 Hi A#
//# 23 Hi B

//# 99 Special Symbol - REST

//generates Ionian Scale as list of ints (according to above table)
vector<int> genIonian(int tonic){
    vector<int> scale;
    scale.push_back(tonic + 0);
    scale.push_back(tonic + 2);
    scale.push_back(tonic + 4);
    scale.push_back(tonic + 5);
    scale.push_back(tonic + 7);
    scale.push_back(tonic + 9);
    scale.push_back(tonic + 11);

    return scale; 

}

//generates Dorian Scale as list of ints (according to above table)
vector<int> genDorian(int tonic){
    vector<int> scale;
    scale.push_back(tonic + 0);
    scale.push_back(tonic + 2);
    scale.push_back(tonic + 3);
    scale.push_back(tonic + 5);
    scale.push_back(tonic + 7);
    scale.push_back(tonic + 9);
    scale.push_back(tonic + 10);
    
    return scale;
    
}

//generates Phrygian Scale as list of ints (according to above table)
vector<int> genPhrygian(int tonic){
    vector<int> scale;
    scale.push_back(tonic + 0);
    scale.push_back(tonic + 1);
    scale.push_back(tonic + 3);
    scale.push_back(tonic + 5);
    scale.push_back(tonic + 7);
    scale.push_back(tonic + 8);
    scale.push_back(tonic + 10);
    
    return scale;
    
}

//generates Lydian Scale as list of ints (according to above table)
vector<int> genLydian(int tonic){
    vector<int> scale;
    scale.push_back(tonic + 0);
    scale.push_back(tonic + 2);
    scale.push_back(tonic + 4);
    scale.push_back(tonic + 6);
    scale.push_back(tonic + 7);
    scale.push_back(tonic + 9);
    scale.push_back(tonic + 11);
    
    return scale;
    
}

//generates Mixolydian Scale as list of ints (according to above table)
vector<int> genMixolydian(int tonic){
    vector<int> scale;
    scale.push_back(tonic + 0);
    scale.push_back(tonic + 2);
    scale.push_back(tonic + 4);
    scale.push_back(tonic + 5);
    scale.push_back(tonic + 7);
    scale.push_back(tonic + 9);
    scale.push_back(tonic + 10);
    
    return scale;
    
}

//generates Aeolian Scale as list of ints (according to above table)
vector<int> genAeolian(int tonic){
    vector<int> scale;
    scale.push_back(tonic + 0);
    scale.push_back(tonic + 2);
    scale.push_back(tonic + 3);
    scale.push_back(tonic + 5);
    scale.push_back(tonic + 7);
    scale.push_back(tonic + 8);
    scale.push_back(tonic + 10);
    
    return scale;
    
}

//generates Locrian Scale as list of ints (according to above table)
vector<int> genLocrian(int tonic){
    vector<int> scale;
    scale.push_back(tonic + 0);
    scale.push_back(tonic + 1);
    scale.push_back(tonic + 3);
    scale.push_back(tonic + 5);
    scale.push_back(tonic + 6);
    scale.push_back(tonic + 8);
    scale.push_back(tonic + 10);
    
    return scale;
    
}

//generates a major chord based off the tonic
vector<int> genMajorChord(int tonic){
    vector<int> chord;
    chord.push_back(tonic + 0);
    chord.push_back(tonic + 4);
    chord.push_back(tonic + 7);
    chord.push_back(99); //REST
    
    return chord; 
}

//generates a minor chord based off the tonic
vector<int> genMinorChord(int tonic){
    vector<int> chord;
    chord.push_back(tonic + 0);
    chord.push_back(tonic + 3);
    chord.push_back(tonic + 7);
    chord.push_back(99); //REST
    
    return chord;
}

//prints out a list of integers (scale)
void printScale(vector<int> scale){
    
    for(int i = 0; i < scale.size() ; i++){
        switch(scale[i]){
                
            case 0 : printf("C"); break;
            case 1 : printf("C#"); break;
            case 2 : printf("D"); break;
            case 3 : printf("D#"); break;
            case 4 : printf("E"); break;
            case 5 : printf("F"); break;
            case 6 : printf("F#"); break;
            case 7 : printf("G"); break;
            case 8 : printf("G#"); break;
            case 9 : printf("A"); break;
            case 10 : printf("A#"); break;
            case 11 : printf("B"); break;
            case 12 : printf("Hi C"); break;
            case 13 : printf("Hi C#"); break;
            case 14 : printf("Hi D"); break;
            case 15 : printf("Hi D#"); break;
            case 16 : printf("Hi E"); break;
            case 17 : printf("Hi F"); break;
            case 18 : printf("Hi F#"); break;
            case 19 : printf("Hi G"); break;
            case 20 : printf("Hi G#"); break;
            case 21 : printf("Hi A"); break;
            case 22 : printf("Hi A#"); break;
            case 23 : printf("Hi B"); break;
                
            case 99 : printf("REST"); break;
                
        }
        
    }
    
}

//plays out the list of integers (scale) with playToneOnce() function
void playScaleOnce(vector<int> scale){
    
    for(int i = 0; i < scale.size() ; i++){
        switch(scale[i]){
                
            case 0 : playSquare(C3, 1000); break;
            case 1 : playSquare(Db3, 1000); break;
            case 2 : playSquare(D3, 1000); break;
            case 3 : playSquare(Eb3, 1000); break;
            case 4 : playSquare(E3, 1000); break;
            case 5 : playSquare(F3, 1000); break;
            case 6 : playSquare(Gb3, 1000); break;
            case 7 : playSquare(G3, 1000); break;
            case 8 : playSquare(Ab3, 1000); break;
            case 9 : playSquare(A3, 1000); break;
            case 10 : playSquare(Bb3, 1000); break;
            case 11 : playSquare(B3, 1000); break;
            case 12 : playSquare(C4, 1000); break;
            case 13 : playSquare(Db4, 1000); break;
            case 14 : playSquare(D4, 1000); break;
            case 15 : playSquare(Eb4, 1000); break;
            case 16 : playSquare(E4, 1000); break;
            case 17 : playSquare(F4, 1000); break;
            case 18 : playSquare(Gb4, 1000); break;
            case 19 : playSquare(G4, 1000); break;
            case 20 : playSquare(Ab4, 1000); break;
            case 21 : playSquare(A4, 1000); break;
            case 22 : playSquare(Bb4, 1000); break;
            case 23 : playSquare(B4, 1000); break;
                
            case 99 : playSquare(REST, 1000); break;
                
        }
        
    }
    
}

///////////////
//SCAT
//doo-WOP boo-WOP
vector<string> scat_syls = {"oo", "op", "a", "ok"};
vector<string> scat_consonants = {"d", "w", "b", "r"};
//maybe??: ukka-ukka, ugga-ugga





