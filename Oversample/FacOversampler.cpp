//  FacOversampleTest.hpp
//
//  Created by Fred Anton Corvest (FAC) on 01/01/2017.
//
//  THIS IS FOR DEMO PURPOSE ONLY!
//
//  Code originaly written by Christian Floisand from
//  https://christianfloisand.wordpress.com/2013/01/28/audio-resampling-part-2/
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
#include <iostream>
#include <assert.h>
#include "../Utils/WaveFileWriter.h"
#include "../Osc/FacOsc.hpp"
#include "FacPolyphaseFilters.h"

float mPFilterBranch[mNumBranches];

inline float processUpsample(float* in, int pos) {
    const float* mKernel = KERNEL[pos];
    
    float acc = 0;
    for(int i = 0; i < LENGTH; ++i) {
        acc += in[i] * mKernel[i];
    }
    return acc;
}

inline float processDownsample(float in, int pos) {
    const float* mKernel = KERNEL[pos];
    
    float acc = 0;
    for(int i = 0; i < LENGTH; ++i) {
        acc += in * mKernel[i];
    }
    return acc;
}

void interpolate(float *input, float *output, const int inSamples) {
    assert(input != output);
    int i,j,n = 0;
    memset(&mPFilterBranch, 0, sizeof(float)*mNumBranches);
    
    float z[mBranchOrder];
    memset(&z, 0, sizeof(float)*mBranchOrder);
    
    for(i =0; i <  inSamples; ++i) {
        z[0] = input[i];
        
        for(j = 0; j < mNumBranches; ++j) {
            mPFilterBranch[j] = processUpsample(z, j) * mNumBranches;
        }
        
        for(n = 0; n < mNumBranches; ++n) {
            output[i*mNumBranches+n] = mPFilterBranch[n];
        }
        
        for(n = mBranchOrder-1; n > 0; --n) {
            z[n] = z[n-1];
        }
    }
}

void decimate(float *input, float *output, const int inSamples) {
    assert(input != output);
    int i,j,n,M;
    
    memset(&mPFilterBranch, 0, sizeof(float)*mNumBranches);
    
    for(i = 0, M = 0; i <  inSamples; ++i, M+=mNumBranches) {
        for(j = 0; j < mNumBranches; ++j) {
            mPFilterBranch[j] = processDownsample(input[M], j);
        }
        
        output[i] = mPFilterBranch[0];
        for(n = 1; n < mNumBranches; ++n) {
            output[i] += mPFilterBranch[n];
        }
    }
}

void fx(float* input, int inSamples) {
    for(int i =0; i < inSamples; ++i) {
        input[i] = atan(input[i]) * .5 * M_PI; // Wave Shaper
    }
}

int main(void) {
    float durationSec = 2; // Duration of the signal
    int nbSamples = SR * durationSec; // Computes nb of samples (SR)
    int nbUpSamples = TARGET_SR * durationSec; // Computes nb of samples (TARGET_SR)
    
    printf("FACOversampler TEST: SR %d to %d (%dx)\n", SR, TARGET_SR, mNumBranches);
    
    float* input = new float[nbSamples];
    float* outputUp = new float[nbUpSamples];
    float* output = new float[nbSamples];
    
    FacOsc osc;
    osc.setFrequency(10000, SR);
    
    for (int i = 0; i < nbSamples; i++) {
        input[i] = osc.tick();
    }
    
    std::string fileNameInDry = "Data/in-out.wav";
    writeFloatSound(nbSamples, input, fileNameInDry, SR);
    
    interpolate(input, outputUp, nbSamples); // SR -> TARGET_SR
    
    std::string fileNameOutDry = "Data/up-out.wav";
    writeFloatSound(nbUpSamples, outputUp, fileNameOutDry, TARGET_SR);
    
    fx(outputUp, nbUpSamples);  // FX ON TARGET_SR
    
    std::string fileNameOut = "Data/up-fx-out.wav";
    writeFloatSound(nbUpSamples, outputUp, fileNameOut, TARGET_SR);
    
    decimate(outputUp, output, nbSamples);  // TARGET_SR -> SR
    
    std::string fileNameIn = "Data/up-fx-down.wav";
    writeFloatSound(nbSamples, output, fileNameIn, SR);
    
    delete [] input;
    delete [] outputUp;
    delete [] output;
}
