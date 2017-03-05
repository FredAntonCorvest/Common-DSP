//  FacOversampleTest.hpp
//
//  Created by Fred Anton Corvest (FAC) on 01/01/2017
//
//  THIS IS FOR TEST PURPOSE ONLY!
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
#include "FacPolyphase.hpp"
#include "StOversampler2x.hpp"
#include "StOversampler4x.hpp"
#include "BandLimit.h"

enum FX_TYPE {NONE, HARD_CLIP, WAVESHAPER};

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

float fxChain(FX_TYPE fxType, float in) {
    switch (fxType) {
        case HARD_CLIP:
            return fabs(in) > .5 ? sgn(in) * .5 : in;
        case WAVESHAPER:
            return atan(in) * .5 * M_PI;
        default:
            return in;
    }
}

void fxChain(FX_TYPE fxType, float* input, int inSamples) {
    for(int i =0; i < inSamples; ++i) {
        input[i] = fxChain(fxType, input[i]) ;
    }
}

std::string buildOutputFile(int pos, const std::string& text, int sr, int factor) {
    char buff[255];
    snprintf(buff, sizeof(buff), "Data/%02d-%s[x%d].wav", pos, text.c_str(), factor);
    return buff;
}

int main(int argc,char *argv[]) {
    int FACTOR = 2; // Default up sample factor
    
    if (argc > 1) {
        int val = atoi(argv[1]);
        FACTOR = val == 2 || val == 4 ? val : FACTOR;
    }
    
    const int SR_IN = 44100; // Input sample rate
    const int SR_OUT = SR_IN * FACTOR; // Output sample rate
    FX_TYPE fxType = HARD_CLIP;
    
    const float DURATION_SEC = 4; // Duration of the signal
    const int NB_SAMPLES_IN = SR_IN * DURATION_SEC; // Computes nb of samples (SR_IN)
    const int NB_SAMPLES_OUT = SR_OUT * DURATION_SEC; // Computes nb of samples (SR_OUT)
    
    float endCutoff = SR_IN * .49; // End cutoff (Just below NYQUIST)
    float cutoff = 1000; // Start cutoff
    float cutoffRatio = 1.0 + (log(endCutoff) -  log(cutoff)) / (float)(NB_SAMPLES_IN * .5); // Cutoff ratio for sweep
    
    printf("] FAC oversampling test\n"); //===========================================================================
    printf("] In SR %dhz -> %dx -> Out SR %dhz\n", SR_IN, FACTOR, SR_OUT);
    printf("] Osc range %.2fhz to %.2fhz \n", cutoff, endCutoff);
    
    FacOsc osc;
    osc.setFrequency(cutoff, SR_IN);
    osc.setType(FacOsc::SIN);
    
    FacOsc lfo;
    lfo.setType(FacOsc::SIN);
    lfo.setFrequency(0.1, SR_OUT);
    
    float* bufferOsc = new float[NB_SAMPLES_IN];
    float* bufferUpSamp = new float[NB_SAMPLES_OUT];
    float* bufferFilter = new float[NB_SAMPLES_OUT];
    float* bufferFilterDown = new float[NB_SAMPLES_IN];
    
    StOversampler2x stOver2x;
    StOversampler4x stOver4x;
    
    CHalfBandFilter* upFilter = new CHalfBandFilter(12, true);
    
    printf("] Feed signal into oversampler (up only)..."); //=========================================================
    
    for (int i = 0, idx = 0; i < NB_SAMPLES_OUT; i+=FACTOR, idx++) {
        if (cutoff <= endCutoff) {
           osc.setFrequency(cutoff, SR_IN);
           cutoff *= cutoffRatio;
        }
        
        float x = osc.tick();
        
        if (FACTOR == 2) {
            stOver2x.up2(x);
            bufferUpSamp[i] = stOver2x.getY1();
            bufferUpSamp[i+1] = stOver2x.getY0();
        } else if (FACTOR == 4) {
            stOver4x.up4(x);
            bufferUpSamp[i]   = stOver4x.getY3();
            bufferUpSamp[i+1] = stOver4x.getY2();
            bufferUpSamp[i+2] = stOver4x.getY1();
            bufferUpSamp[i+3] = stOver4x.getY0();
        }
        
        bufferFilter[i] = upFilter->process(x * FACTOR) ;
        // TODO: cascade for FACTOR 4
        for(int a=1; a < FACTOR; ++a) {
            bufferFilter[i+a] = upFilter->process(0);
        }
        
        bufferOsc[idx] = x;
    }
    
    delete upFilter;
    
    writeFloatSound(NB_SAMPLES_IN, bufferOsc, buildOutputFile(0, "dry", SR_IN, FACTOR), SR_IN);
    writeFloatSound(NB_SAMPLES_OUT, bufferUpSamp, buildOutputFile(2, "xUp", SR_OUT, FACTOR), SR_OUT);
    writeFloatSound(NB_SAMPLES_OUT, bufferFilter, buildOutputFile(3, "hFltUp", SR_OUT, FACTOR), SR_OUT);
    
    printf("done\n] Apply fx on up sampled signal..."); //============================================================
    
    CHalfBandFilter* downFilter = new CHalfBandFilter(12, true);
    
    for (int i = 0, idx = 0; i < NB_SAMPLES_OUT; i++) {
        bufferFilter[i] = fxChain(fxType, bufferFilter[i]);
        bufferUpSamp[i] = fxChain(fxType, bufferUpSamp[i]);
        
        float y = downFilter->process(fxChain(fxType, bufferFilter[i]));
        // TODO: cascade for FACTOR 4
        if (i == 0 || i % FACTOR == 0) {
            bufferFilterDown[idx++] = y;
        }
    }
    
    delete downFilter;
    
    writeFloatSound(NB_SAMPLES_OUT, bufferUpSamp, buildOutputFile(4, "xUp-fx", SR_OUT, FACTOR), SR_OUT);
    writeFloatSound(NB_SAMPLES_OUT, bufferFilter, buildOutputFile(5, "hFltUp-fx", SR_OUT, FACTOR), SR_OUT);
    writeFloatSound(NB_SAMPLES_OUT, bufferFilterDown, buildOutputFile(7, "hFltFxDw", SR_IN, FACTOR), SR_IN);
    
    printf("done\n] Feed signal into oversampler (up/down)..."); //===================================================
    
    float* bufferOvSamp = new float[NB_SAMPLES_IN];
    float* bufferOvSampFx = new float[NB_SAMPLES_IN];
    
    stOver2x.reset();
    stOver4x.reset();
    
    for (int i = 0; i < NB_SAMPLES_IN; i++) {
        float in = bufferOsc[i];
        
        if (FACTOR == 2) {
            stOver2x.up2(in);
            bufferOvSamp[i] = stOver2x.down2();
        } else if (FACTOR == 4) {
            stOver4x.up4(in);
            bufferOvSamp[i] = stOver4x.down4();
        }
    }
    
    printf("done\n] Feed signal + fx into oversampler (up/down)..."); //==============================================
    
    stOver2x.reset();
    stOver4x.reset();
    
    for (int i = 0; i < NB_SAMPLES_IN; i++) {
        float in = bufferOsc[i];
        
        if (FACTOR == 2) {
            stOver2x.up2(in);
            stOver2x.setY1(fxChain(fxType, stOver2x.getY1()));
            stOver2x.setY0(fxChain(fxType, stOver2x.getY0()));
            bufferOvSampFx[i] = stOver2x.down2();
        } else if (FACTOR == 4) {
            stOver4x.up4(in);
            stOver4x.setY3(fxChain(fxType, stOver4x.getY3()));
            stOver4x.setY2(fxChain(fxType, stOver4x.getY2()));
            stOver4x.setY1(fxChain(fxType, stOver4x.getY1()));
            stOver4x.setY0(fxChain(fxType, stOver4x.getY0()));
            bufferOvSampFx[i] = stOver4x.down4();
        }
    }
    
    writeFloatSound(NB_SAMPLES_IN, bufferOvSamp, buildOutputFile(1, "xUpDw", SR_IN, FACTOR), SR_IN);
    writeFloatSound(NB_SAMPLES_IN, bufferOvSampFx, buildOutputFile(6, "xUpFxDw", SR_IN, FACTOR), SR_IN);
    
    delete [] bufferOsc;
    delete [] bufferFilter;
    delete [] bufferUpSamp;
    delete [] bufferOvSamp;
    delete [] bufferOvSampFx;
    delete [] bufferFilterDown;
    
    float* outputUp = new float[NB_SAMPLES_OUT];
    float* outputDown = new float[NB_SAMPLES_IN];
    
    bool testActive = FACTOR == mNumBranches;
    
    if (testActive) {
        printf("done\n] Feed signal into oversampler (polyphase) (up only)..."); //===================================
        interpolate(bufferOsc, outputUp, NB_SAMPLES_IN);
        writeFloatSound(NB_SAMPLES_OUT, outputUp, buildOutputFile(10, "xUp(poly)", SR_OUT, FACTOR), SR_OUT);
        printf("done\n");
    } else {
        printf("done\n] Test on oversampler (polyphase) ignored. Factor is %d, NBand is %d\n", FACTOR, mNumBranches);
    }
    
    if (testActive) {
        printf("] Apply fx on up sampled signal..."); //========================================================
        fxChain(fxType, outputUp, NB_SAMPLES_OUT);
        writeFloatSound(NB_SAMPLES_OUT, outputUp, buildOutputFile(11, "xUpFx(poly)", SR_OUT, FACTOR), SR_OUT);
        printf("done\n");
    }
    
    if (testActive) {
        printf("] Downsample signal..."); //====================================================================
        decimate(outputUp, outputDown, NB_SAMPLES_IN);
        writeFloatSound(NB_SAMPLES_IN, outputDown, buildOutputFile(12, "xUpFxDw(poly)", SR_IN, FACTOR), SR_IN);
        printf("done\n");
    }
    
    printf("] Feed 10khz + waveshaper into oversampler (up/down)..."); //=======================================
    
    delete [] outputUp;
    delete [] outputDown;
    
    float* outputWaveShaper = new float[NB_SAMPLES_IN];
    float* outputWaveShaperCascade = new float[NB_SAMPLES_IN];
    
    stOver2x.reset();
    stOver4x.reset();
    
    osc.setFrequency(10000, SR_IN);
    float gain = pow(10.0, -6 / 20.0); // in -6db
    fxType = WAVESHAPER;
    
    StOversampler2x stOver2xCascade;
    
    for (int i = 0; i < NB_SAMPLES_IN; i++) {
        float in = osc.tick() * gain;
        
        if (FACTOR == 2) {
            stOver2x.up2(in);
            stOver2x.setY1(fxChain(fxType, stOver2x.getY1()));
            stOver2x.setY0(fxChain(fxType, stOver2x.getY0()));
            outputWaveShaper[i] = stOver2x.down2();
        } else if (FACTOR == 4) {
            stOver4x.up4(in);
            stOver4x.setY3(fxChain(fxType, stOver4x.getY3()));
            stOver4x.setY2(fxChain(fxType, stOver4x.getY2()));
            stOver4x.setY1(fxChain(fxType, stOver4x.getY1()));
            stOver4x.setY0(fxChain(fxType, stOver4x.getY0()));
            outputWaveShaper[i] = stOver4x.down4();
            
            stOver2xCascade.up2(in);
            
            stOver2x.up2(stOver2xCascade.getY1());
            stOver2x.setY1(fxChain(fxType, stOver2x.getY1()));
            stOver2x.setY0(fxChain(fxType, stOver2x.getY0()));
            stOver2xCascade.setY1(stOver2x.down2());
            
            stOver2x.up2(stOver2xCascade.getY0());
            stOver2x.setY1(fxChain(fxType, stOver2x.getY1()));
            stOver2x.setY0(fxChain(fxType, stOver2x.getY0()));
            stOver2xCascade.setY0(stOver2x.down2());
            
            outputWaveShaperCascade[i] = stOver2xCascade.down2();
        }
    }
    
    writeFloatSound(NB_SAMPLES_IN, outputWaveShaper, buildOutputFile(20, "xUpWshpDw", SR_IN, FACTOR), SR_IN);
    
    if (FACTOR == 4) {
        writeFloatSound(NB_SAMPLES_IN, outputWaveShaperCascade, buildOutputFile(21, "xUpWshpDw-Cascade-", SR_IN, FACTOR), SR_IN);
    }
    
    printf("done\n");

    delete [] outputWaveShaper;
    delete [] outputWaveShaperCascade;
}
