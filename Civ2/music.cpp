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

using std::vector;



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
    
    printf("bytes: fuck : %d",BytesToWrite);
    
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





