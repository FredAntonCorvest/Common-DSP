//  StOversampler4x.hpp
//
//  Created by Fred Anton Corvest (FAC) on 05/01/2017.
//
//  Code ported from the st-oversampler.jsfx-inc code by SAULT
//  http://stash.reaper.fm/v/29680/sault-4xlimiter.zip#file_info
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

/*!
 @class StOversampler4x
 @brief Quarterband filter (31 tap) - Fixed coefs
 */
class StOversampler4x {
public:
    typedef double (*processFuncDef)(double);
    
    StOversampler4x()  {
        reset();
    }
    
    /*!
     @class StOversampler4x
     @param x
     The sample to process
     @param processFunc
     Function that will process the given sample x at 4x its original sample rate
     @brief runs the given function at 4x its original sample rate and returns the computed value
     */
    double process(double x, processFuncDef processFunc) {
        up4(x);
        y3 = (*processFunc)(y3);
        y2 = (*processFunc)(y2);
        y1 = (*processFunc)(y1);
        y0 = (*processFunc)(y0);
        return down4();
    }
    
    /*!
     @class StOversampler4x
     @brief reset states
     */
    void reset() {
        y30 = y29 = y28 = y27 = y26 = y25 = y24 = y23 = y22 = y21 = y20 = y19 = y17 = y16 = y15 = y14 = y13 = y12 = y11 = y10 = y9 = y8 = y7 = y6 = y5 = y4 = y3 = y2 = y1 = y0 = x0 = x1 = x2 = x3 = x4 = x5 = x6 = x7 = x8 = x9 = 0;
    }
    
private:
    double y30, y29, y28, y27, y26, y25, y24, y23, y22, y21, y20, y19, y18, y17, y16, y15, y14, y13, y12, y11, y10, y9,  y8, y7, y6, y5, y4, y3, y2, y1, y0, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9;
    
    void up4(double x) {
        x7 = x6; x6 = x5; x5 = x4; x4 = x3; x3 = x2; x2 = x1; x1 = x0;
        //  x0 = x * 4.0189;
        x0 = x * 4;
        
        this->os4();
        
        y0 = -0.0025*x0 + 0.0079*x1 - 0.0224*x2 + 0.0703*x3 + 0.2235*x4 - 0.0375*x5 + 0.0135*x6 - 0.0043*x7;
        y1 = -0.0045*(x0 + x7) + 0.0147*(x1 + x6) - 0.0407*(x2 + x5) + 0.1546*(x3 + x4);
        y2 = -0.0043*x0 + 0.0135*x1 - 0.0375*x2 + 0.2235*x3 + 0.0703*x4 - 0.0224*x5 + 0.0079*x6 - 0.0025*x7;
        y3 = x3*0.25;
    }
    
    double down4() {
        return -0.0025*( y0+y30) - 0.0045*( y1+y29) - 0.0043*( y2+y28) + 0.0079*( y4+y26) + 0.0147*( y5+y25) + 0.0135*( y6+y24) + -0.0224*( y8+y22) - 0.0407*( y9+y21) - 0.0375*(y10+y20) + 0.0703*(y12+y18) + 0.1546*(y13+y17) + 0.2235*(y14+y16) + 0.25*y15;
    }
    
    void os4() {
        y30 = y26; y29 = y25; y28 = y24; y27 = y23;
        y26 = y22; y25 = y21; y24 = y20; y23 = y19;
        y22 = y18; y21 = y17; y20 = y16; y19 = y15;
        y18 = y14; y17 = y13; y16 = y12; y15 = y11;
        y14 = y10; y13 =  y9; y12 =  y8; y11 =  y7;
        y10 =  y6;  y9 =  y5;  y8 =  y4;  y7 =  y3;
        y6 =  y2;  y5 =  y1;  y4 =  y0;
    };
};
