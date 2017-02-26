//  StOversampler2x.hpp

//  Created by Fred Anton Corvest (FAC) on 26-Feb-2017

//  Code ported from the st-oversampler.jsfx-inc code by SAULT
//  http://stash.reaper.fm/v/29680/sault-4xlimiter.zip#file_info

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


/*!
 @class StOversampler2x
 @brief Quarterband filter (31 tap)
 */
class StOversampler2x {
public:
    
    /*
     
     18th order FIR halfband filter
     -0.05 dB at 15 kHz
     -60.5 dB at 31 kHz
     -67 dB at 40 kHz
     
     */
    
    // coefficients from Matlab
    // 0.0028         0        -0.0118         0    0.0334         0   -0.0849
    //      0         0.3106    0.5000    0.3106         0   -0.0849         0
    // 0.0334         0   -0.0118         0    0.0028
    
    StOversampler2x()  {
        reset();
    }
    
    /*!
     @class StOversampler2x
     @brief reset states
     */
    void reset() {
            x9 = x8 = x7 = x6 = x5 = x4 = x3 = x2 = x1 = x0 = y18 = y17 = y16 = y15 = y14 = y13 = y12 = y11 = y10 = y9 = y8 = y7 = y6 = y5 = y4 = y3 = y2 = y1 = y0 = 0;
    }
    
public:
    // TEMP!!!
    double x9, x8, x7, x6, x5, x4, x3, x2, x1, x0, y18, y17, y16, y15, y14, y13, y12, y11, y10, y9, y8, y7, y6, y5, y4, y3, y2, y1, y0;
    
    void up2(double x) {
        x9 = x8; x8 = x7; x7 = x6; x6 = x5; x5 = x4; x4 = x3; x3 = x2; x2 = x1; x1 = x0;
        x0 = x * 2;
        
        this->os2();
        y1 = 0.0028*(x0+x9) - 0.0118*(x1+x8) + 0.0334*(x2+x7) - 0.0849*(x3+x6) + 0.3106*(x4+x5);
        y0 = 0.5*x4;
    }
    
    double down2() {
          return 0.0028*(y0+y18) - 0.0118*(y2+y16) + 0.0334*(y4+y14) - 0.0849*(y6+y12) + 0.3106*(y8+y10)+0.5*y9;
    }
    
    void os2() {
        y18 = y16; y17 = y15; y16 = y14; y15 = y13; y14 = y12; y13 = y11; y12 = y10; y11 = y9; y10 = y8; y9 = y7; y8 = y6; y7 = y5;
        y6 = y4; y5 = y3; y4 = y2; y3 = y1; y2 = y0;
    }
};
