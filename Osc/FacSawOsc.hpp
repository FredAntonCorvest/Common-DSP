//  FacSawOsc.hpp
//
//  Created by Fred Anton Corvest (FAC) on 03/12/2016.
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
#define PI2 (M_PI*2)

/*!
 @class FacSawOsc
 @brief Saw oscillator (aliasing-free)
 */
class FacSawOsc {
public:
    FacSawOsc() : _phase(0), _phaseIncr(0), _normalisedFreq(0) {
    }
    
    /*!
     @class FacSawOsc
     @param freq
        Frequency of the oscillator
     @param sr
        Sample rate
     */
    void setFrequency(double freq, int sr) {
        _normalisedFreq = freq / (double)sr;
        _phaseIncr = _normalisedFreq * PI2;
        // Normalisation of the pitch + setting of the increment
    }
    
    /*!
     @class FacSawOsc
     @brief Resets osc phase
     */
    void reset() {
        _phase = 0;
    }
    
    /*!
     @class FacSawOsc
     @brief Tick method
     */
    inline double tick() {
        double t = _phase / PI2;
        double oscSmp = (2.f * t) - 1.f;
        // Creates an aliased saw waverform
        
        double polyBlep = 0.f;
        if (t < _normalisedFreq) {
            t /= _normalisedFreq;
            polyBlep = t+t - t*t - 1.f;
        } else if (t > 1.f - _normalisedFreq) {
            t = (t - 1.f) / _normalisedFreq;
            polyBlep = t*t + t+t + 1.f;
        }
        // Computation of the polyblep
        
        oscSmp -= polyBlep;
        // Layer of the poly blep on top of the aliased waveforme (Remove the aliasing)
        
        _phase += _phaseIncr;
        _phase = _phase >= PI2 ? _phase-PI2 : _phase;
        // phase increment + wrapping
        
        return oscSmp;
    }
    
private:
    double _normalisedFreq;
    double _phaseIncr;
    double _phase;
};
