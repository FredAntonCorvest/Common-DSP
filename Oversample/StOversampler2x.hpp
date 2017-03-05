//  StOversampler2x.hpp

//  Created by Fred Anton Corvest (FAC) on 05-Mar-2017

//  Code ported from the st-oversampler.jsfx-inc code by SAULT
//  http://stash.reaper.fm/v/29680/sault-4xlimiter.zip#file_info

//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:

//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.

//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.


/*!
 @class StOversampler2x
 @brief Halfband filter (19 tap). 0.2268 ms delay at SR 44100hz
 */
class StOversampler2x {

	static constexpr double C01 = 0.0027968484349636551535012163327564849169;
	static constexpr double C03 = -0.0117544339124022474002417837368739128578;
	static constexpr double C05 = 0.0334028934203564817351228555253328522667;
	static constexpr double C07 = -0.0848322749393570219034188539808383211493;
	static constexpr double C09 = 0.3105018573185503449884947713144356384873;
	static constexpr double C10 = 0.4997702193557774230647794411197537556291;
	static constexpr double C11 = 0.3105018573185503449884947713144356384873;
	static constexpr double C13 = -0.0848322749393570219034188539808383211493;
	static constexpr double C15 = 0.0334028934203564817351228555253328522667;
	static constexpr double C17 = -0.0117544339124022474002417837368739128578;
	static constexpr double C19 = 0.0027968484349636551535012163327564849169;

public:
	StOversampler2x() {
		reset();
	}

	void setY1(double x) {this->y1 = x;};
	void setY0(double x) {this->y0 = x;};

	double getY1() {return this->y1;};
	double getY0() {return this->y0;};

	void reset() {
		y18 = y17 = y16 = y15 = y14 = y13 = y12 = y11 = y10 = y9 = y8 = y7 = y6 = y5 = y4 = y3 = y2 = y1 = y0 = x9 = x8 = x7 = x6 = x5 = x4 = x3 = x2 = x1 = x0 = 0;
	}

	void up2(double x) {
		x9 = x8; x8 = x7; x7 = x6; x6 = x5; x5 = x4; x4 = x3; x3 = x2; x2 = x1; x1 = x0; 
		x0 = x * 2.000; // dc gain (2/sum(C))

		this->os2();

		y1 = C01*(x0+x9) + C03*(x1+x8) + C05*(x2+x7) + C07*(x3+x6) + C09*(x4+x5);
		y0 = C10*x4;
	}

	double down2() {
		return C01*(y0+y18) + C03*(y2+y16) + C05*(y4+y14) + C07*(y6+y12) + C09*(y8+y10) + C10*(y9);
	}

private:
	double y18, y17, y16, y15, y14, y13, y12, y11, y10, y9, y8, y7, y6, y5, y4, y3, y2, y1, y0, x9, x8, x7, x6, x5, x4, x3, x2, x1, x0;

	void os2() {
		y18 = y16; y17 = y15; y16 = y14; y15 = y13; y14 = y12; y13 = y11; y12 = y10; y11 = y9; y10 = y8; y9 = y7; y8 = y6; y7 = y5; y6 = y4; y5 = y3; y4 = y2; y3 = y1; y2 = y0; 
	}
};
