//  SvfLinearTrapOptimised2Demo.cpp
//
//  Created by Fred Anton Corvest (FAC) on 30/11/2016.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.

#include <math.h>
#include <assert.h>
#include "../Utils/WaveFileWriter.h"
#include "../Osc/FacSawOsc.hpp"
#include "SvfLinearTrapOptimised2.hpp"

#define PI2 (M_PI*2)
#define sampleRate 44100

/*!
 @brief Converts a given filter type to a file name
 @param type
 Type of the filter
 */
std::string toFileName(SvfLinearTrapOptimised2::FLT_TYPE type) {
    switch (type) {
        case SvfLinearTrapOptimised2::LOW_PASS_FILTER:
            return "Data/SvfLinearTrapOptimised2_Lpf.wav";
        case SvfLinearTrapOptimised2::BAND_PASS_FILTER:
            return "Data/SvfLinearTrapOptimised2_Bpf.wav";
        case SvfLinearTrapOptimised2::HIGH_PASS_FILTER:
            return "Data/SvfLinearTrapOptimised2_Hpf.wav";
        case SvfLinearTrapOptimised2::NOTCH_FILTER:
            return "Data/SvfLinearTrapOptimised2_Notch.wav";
        case SvfLinearTrapOptimised2::PEAK_FILTER:
            return "Data/SvfLinearTrapOptimised2_Peak.wav";
        case SvfLinearTrapOptimised2::ALL_PASS_FILTER:
            return "Data/SvfLinearTrapOptimised2_All.wav";
        case SvfLinearTrapOptimised2::BELL_FILTER:
            return "Data/SvfLinearTrapOptimised2_Bell.wav";
        case SvfLinearTrapOptimised2::LOW_SHELF_FILTER:
            return "Data/SvfLinearTrapOptimised2_Lsf.wav";
        case SvfLinearTrapOptimised2::HIGH_SHELF_FILTER:
            return "Data/SvfLinearTrapOptimised2_Hsf.wav";
        case SvfLinearTrapOptimised2::NO_FLT_TYPE:
            return "Data/SvfLinearTrapOptimised2_Dry.wav";
        default:
            assert(false);
    };
}

/*!
 @brief Runs a saw oscillator (aliasing-free) at a given frequency for a given duration and level through a filter having a given state, Q and gain.
 The filter frequency is modulated from a start frequency to an end frequency.
 @param nbSamples
    Duration in sample
 @param pitchHz
    Pitch of the oscillator in hertz
 @param leveldB
    Level of the signal in decibel
 @param filterType
    Type (state) of the svf filter
 @param startCutoffHz
    Start frequency of the cutoff in hertz
 @param stopCutoffHz
    End frequency of the cutoff in hertz
 @param Q
    Q factor of the filter
 @param gainDb
    Gain in dB to boost or cut the cutoff point (only for some types)
 @return buffer
    Buffer of the signal generated
 */
float* createSaw(int nbSamples, float pitchHz, float leveldB, SvfLinearTrapOptimised2::FLT_TYPE filterType, float startCutoffHz, float stopCutoffHz, float Q, float gaindB) {
    float* buffer = new float[nbSamples];
    
    float cutoff = startCutoffHz;
    float cutoffRatio = 1.0 + (log(stopCutoffHz) -  log(cutoff)) / (float)nbSamples;
    // Exponential cutoff sweep from start to end
    
    FacSawOsc sawOsc;
    sawOsc.setFrequency(pitchHz, sampleRate);
    
    SvfLinearTrapOptimised2 filter;
    filter.setGain(gaindB);
    // Creation of the filter and setting of the gain
    
    bool filterByPass = filterType == SvfLinearTrapOptimised2::NO_FLT_TYPE;
    // Bypass the filter if the state equals NO_FLT_TYPE -> creates the dry version
    
    float linearLevel = pow(10.0, leveldB / 20.0);
    // Conversion of the level from decibel to linear
    
    for (int i = 0; i < nbSamples; i ++) {
        float oscSmp = sawOsc.tick();
        
        filter.updateCoefficients(cutoff, Q, filterType, sampleRate);
        // Updates the coefficients of the filter for the given cutoff, q, type and sample rate
        
        cutoff *= cutoffRatio;
        // cutoff increment (sweep)
        
        float currentSmp = !filterByPass ? filter.tick(oscSmp) : oscSmp;
        // Runs the oscillator through the filter (no bypas) or keeps the oscillator dry
        
        currentSmp *= linearLevel;
        // Changes the level
        
        buffer[i] = currentSmp;
        // Stores the sample to the buffer
    }
    
    return buffer;
}

/*!
 Demonstrates how to use the class SvfLinearTrapOptimised2.
 
 The signal path is very simple: a saw oscillator (aliasing-free) runnning through a filter having its frequency modulated. 
 The resulted signal is saved into wave files, there is a wave file per filter type + a dry version.
 
 This example is for educational purposes only, I've tried to gather various DSP technics around the demonstration of the filter itself.
 
 https://github.com/FredAntonCorvest
 
 NOTE: WaveFileWriter.h This code snippet comes from an example on wavetable oscillator written by Nigel Redmon on 4/31/12
 Available here www.earlevel.com/main/2012/05/03/a-wavetable-oscillator—introduction/
 Check his blog! An amazing source of information/example about DSP. Thanks Nigel!
 
 Build/Execution instruction:
    g++ SvfLinearTrapOptimised2Demo.cpp -o SvfLinearTrapOptimised2Demo
    ./SvfLinearTrapOptimised2Demo
 */
int main(void) {
    float pitchHz = 100; // Pitch
    float leveldB = -6;  // Level
    
    float filterGaindB = 3; // Gain to boost or cut the cutoff point
    float cutoffStartHz = sampleRate * .5; // Start cutoff frequency (NYQUIST)
    float cutoffEndHz = 16.35; // lowest "audible" C
    float Q = 0.5; // Q
    
    float durationSec = 2; // Duration of the signal recorded
    int nbSamples = sampleRate * durationSec; // Computes nb of samples
    
    for(int i=0; i <= SvfLinearTrapOptimised2::NO_FLT_TYPE; i++) {
        // Iterates over each type, generates and records the signal to a wave file
        SvfLinearTrapOptimised2::FLT_TYPE type = (SvfLinearTrapOptimised2::FLT_TYPE) i;
        
        float* buffer = createSaw(nbSamples, pitchHz, leveldB, type, cutoffStartHz, cutoffEndHz, Q, filterGaindB);
        
        std::string fileName = toFileName(type);
        writeFloatSound(nbSamples, buffer, fileName, sampleRate);
        
        delete [] buffer;
    }
}
