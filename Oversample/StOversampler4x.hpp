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

#define C1 -0.0024690763746848414829371254342049724073
#define C2 -0.0045486659733113788892100437522003630875
#define C3 -0.0043347925891454759661503004508631420322
#define C4 0.0000000000000000000000000000000000000000
#define C5 0.0079195788472692964232724222028991789557
#define C6 0.0147078516402336448298804327805555658415
#define C7 0.0135142599063812311743770777638928848319
#define C8 0.0000000000000000000000000000000000000000
#define C9 -0.0223506872520791988190325128016411326826
#define C10 -0.0407216020891329466557806426862953230739
#define C11 -0.0375135576451587904323048405785812065005
#define C12 0.0000000000000000000000000000000000000000
#define C13 0.0703160757182492712757948538637720048428
#define C14 0.1546464545177186344915298832347616553307
#define C15 0.2234734337184001162768254289403557777405
#define C16 0.2500000000000000000000000000000000000000
#define C17 0.2234734337184001162768254289403557777405
#define C18 0.1546464545177186344915298832347616553307
#define C19 0.0703160757182492712757948538637720048428
#define C20 0.0000000000000000000000000000000000000000
#define C21 -0.0375135576451587904323048405785812065005
#define C22 -0.0407216020891329466557806426862953230739
#define C23 -0.0223506872520791988190325128016411326826
#define C24 0.0000000000000000000000000000000000000000
#define C25 0.0135142599063812311743770777638928848319
#define C26 0.0147078516402336448298804327805555658415
#define C27 0.0079195788472692964232724222028991789557
#define C28 0.0000000000000000000000000000000000000000
#define C29 -0.0043347925891454759661503004508631420322
#define C30 -0.0045486659733113788892100437522003630875
#define C31 -0.0024690763746848414829371254342049724073

/*!
 @class StOversampler4x
 @brief Quarterband filter (31 tap)
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
     @brief Experimental - add aliasing by itself
     */
    double process8x(StOversampler4x& other, double x, processFuncDef processFunc) {
        other.up4(x);
        other.y3 = process(other.y3, processFunc);
        other.y2 = process(other.y2, processFunc);
        other.y1 = process(other.y1, processFunc);
        other.y0 = process(other.y0, processFunc);
        return other.down4();
    }
    
    /*!
     @class StOversampler4x
     @brief Experimental - add aliasing by itself
     */
    double process16x(StOversampler4x& other, StOversampler4x& other2, double x, processFuncDef processFunc) {
        other.up4(x);
        other.y3 = process8x(other2, other.y3, processFunc);
        other.y2 = process8x(other2, other.y2, processFunc);
        other.y1 = process8x(other2, other.y1, processFunc);
        other.y0 = process8x(other2, other.y0, processFunc);
        return other.down4();
    }
    
    /*!
     @class StOversampler4x
     @brief reset states
     */
    
    
    void reset() {
        y30 = y29 = y28 = y27 = y26 = y25 = y24 = y23 = y22 = y21 = y20 = y19 = y18 = y17 = y16 = y15 = y14 = y13 = y12 = y11 = y10 = y9 = y8 = y7 = y6 = y5 = y4 = y3 = y2 = y1 = y0 = x0 = x1 = x2 = x3 = x4 = x5 = x6 = x7 = 0;
    }
    
private:
    double y30, y29, y28, y27, y26, y25, y24, y23, y22, y21, y20, y19, y18, y17, y16, y15, y14, y13, y12, y11, y10, y9, y8, y7, y6, y5, y4, y3, y2, y1, y0, x0, x1, x2, x3, x4, x5, x6, x7;
    
    void up4(double x) {
        x7 = x6; x6 = x5; x5 = x4; x4 = x3; x3 = x2; x2 = x1; x1 = x0;
        x0 = x * 4; // gain compensation
        
        this->os4();
        
        y0 = C1*x0 + C5*x1 + C9*x2 + C13*x3 + C17*x4 + C21*x5 + C25*x6 + C29*x7;
        y1 = C2*(x0 + x7) + C6*(x1 + x6) + C10*(x2 + x5) + C14*(x3 + x4);
        y2 = C3*x0 + C7*x1 + C11*x2 + C15*x3 + C19*x4 + C23*x5 + C27*x6 + C31*x7;
        y3 = x3*C16;
    }
    
    double down4() {
        return C1*(y0+y30) + C2*(y1+y29) + C3*(y2+y28) + C5*(y4+y26) + C6*(y5+y25) + C7*(y6+y24) + C9*(y8+y22) + C10*(y9+y21) + C11*(y10+y20) + C13*(y12+y18) + C14*(y13+y17) + C15*(y14+y16) + C16*(y15+0);
    }
    
    void os4() {
        y30 = y26; y29 = y25; y28 = y24; y27 = y23; y26 = y22; y25 = y21; y24 = y20; y23 = y19; y22 = y18; y21 = y17; y20 = y16; y19 = y15; y18 = y14; y17 = y13; y16 = y12; y15 = y11; y14 = y10; y13 = y9; y12 = y8; y11 = y7; y10 = y6; y9 = y5; y8 = y4; y7 = y3; y6 = y2; y5 = y1; y4 = y0; 
    }
};
