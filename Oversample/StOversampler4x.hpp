//  StOversampler4x.hpp

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
 @class StOversampler4x
 @brief Quarterband filter (31 tap). 0.1814 ms delay at SR 44100hz
 */
class StOversampler4x {

	static constexpr double C01 = -0.0024807893101506097929820349179408367490;
	static constexpr double C02 = -0.0045702441762163143260111475285611959407;
	static constexpr double C03 = -0.0043553562081467848618454574705083359731;
	static constexpr double C05 = 0.0079571481654585607401486413436941802502;
	static constexpr double C06 = 0.0147776235269474112005161359206795168575;
	static constexpr double C07 = 0.0135783695693199746995505350355415430386;
	static constexpr double C09 = -0.0224567156277435953093402076774509623647;
	static constexpr double C10 = -0.0409147793849926541542849633970035938546;
	static constexpr double C11 = -0.0376915164496308477404262760046549374238;
	static constexpr double C13 = 0.0706496448477983984837180742033524438739;
	static constexpr double C14 = 0.1553800745710906539009954485663911327720;
	static constexpr double C15 = 0.2245335588462797071773735524402582086623;
	static constexpr double C16 = 0.2511859632599723890500342804443789646029;
	static constexpr double C17 = 0.2245335588462797071773735524402582086623;
	static constexpr double C18 = 0.1553800745710906539009954485663911327720;
	static constexpr double C19 = 0.0706496448477983984837180742033524438739;
	static constexpr double C21 = -0.0376915164496308477404262760046549374238;
	static constexpr double C22 = -0.0409147793849926541542849633970035938546;
	static constexpr double C23 = -0.0224567156277435953093402076774509623647;
	static constexpr double C25 = 0.0135783695693199746995505350355415430386;
	static constexpr double C26 = 0.0147776235269474112005161359206795168575;
	static constexpr double C27 = 0.0079571481654585607401486413436941802502;
	static constexpr double C29 = -0.0043553562081467848618454574705083359731;
	static constexpr double C30 = -0.0045702441762163143260111475285611959407;
	static constexpr double C31 = -0.0024807893101506097929820349179408367490;

public:
	StOversampler4x() {
		reset();
	}

	void setY3(double x) {this->y3 = x;};
	void setY2(double x) {this->y2 = x;};
	void setY1(double x) {this->y1 = x;};
	void setY0(double x) {this->y0 = x;};

	double getY3() {return this->y3;};
	double getY2() {return this->y2;};
	double getY1() {return this->y1;};
	double getY0() {return this->y0;};

	void reset() {
		y30 = y29 = y28 = y27 = y26 = y25 = y24 = y23 = y22 = y21 = y20 = y19 = y18 = y17 = y16 = y15 = y14 = y13 = y12 = y11 = y10 = y9 = y8 = y7 = y6 = y5 = y4 = y3 = y2 = y1 = y0 = x7 = x6 = x5 = x4 = x3 = x2 = x1 = x0 = 0;
	}

	void up4(double x) {
		x7 = x6; x6 = x5; x5 = x4; x4 = x3; x3 = x2; x2 = x1; x1 = x0; 
		x0 = x * 4.000; // dc gain (4/sum(C))

		this->os4();

		y3 = C01*x0 + C05*x1 + C09*x2 + C13*x3 + C17*x4 + C21*x5 + C25*x6 + C29*x7;
		y2 = C02*(x0+x7) + C06*(x1+x6) + C10*(x2+x5) + C14*(x3+x4);
		y1 = C03*x0 + C07*x1 + C11*x2 + C15*x3 + C19*x4 + C23*x5 + C27*x6 + C31*x7;
		y0 = C16*x3;
	}

	double down4() {
		return C01*(y0+y30) + C02*(y1+y29) + C03*(y2+y28) + C05*(y4+y26) + C06*(y5+y25) + C07*(y6+y24) + C09*(y8+y22) + C10*(y9+y21) + C11*(y10+y20) + C13*(y12+y18) + C14*(y13+y17) + C15*(y14+y16) + C16*(y15);
	}

private:
	double y30, y29, y28, y27, y26, y25, y24, y23, y22, y21, y20, y19, y18, y17, y16, y15, y14, y13, y12, y11, y10, y9, y8, y7, y6, y5, y4, y3, y2, y1, y0, x7, x6, x5, x4, x3, x2, x1, x0;

	void os4() {
		y30 = y26; y29 = y25; y28 = y24; y27 = y23; y26 = y22; y25 = y21; y24 = y20; y23 = y19; y22 = y18; y21 = y17; y20 = y16; y19 = y15; y18 = y14; y17 = y13; y16 = y12; y15 = y11; y14 = y10; y13 = y9; y12 = y8; y11 = y7; y10 = y6; y9 = y5; y8 = y4; y7 = y3; y6 = y2; y5 = y1; y4 = y0; 
	}
};
