#ifndef FACPOLYPHASE_H_
#define FACPOLYPHASE_H_

//  FacPolyphase.hpp
//
//  Created by Fred Anton Corvest (FAC) on 26/02/2017
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
            mPFilterBranch[j] = processUpsample(z, j) * DC_GAIN;
        }
        
        // shuffle
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

#endif
