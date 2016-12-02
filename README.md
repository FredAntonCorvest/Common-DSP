# Common-DSP
Utilities related to DSP

### SvfLinearTrapOptimised2 (State Variable Filter)

A ready to use C++ port of Andy Simper's implementation of State Variable Filters described in the technical paper provided at http://www.cytomic.com/files/dsp/SvfLinearTrapOptimised2.pdf. Thank you Andy for the time you spent on making those technical papers.

### SvfLinearTrapOptimised2Demo

Demonstrates how to use the class SvfLinearTrapOptimised2.
 
The signal path is very simple: a saw oscillator (aliasing-free) runnning through a filter having its frequency modulated. 
The resulted signal is saved into wave files, there is a wave file per filter type + a dry version.
 
This example is for educational purposes only, I've tried to gather various DSP technics around the demonstration of the filter itself.

* :sound: [SvfLinearTrapOptimised2_Dry.wav](https://github.com/FredAntonCorvest/Common-DSP/blob/master/Filter/Data/SvfLinearTrapOptimised2_Dry.wav)
* :sound: [SvfLinearTrapOptimised2_Lpf.wav](https://github.com/FredAntonCorvest/Common-DSP/blob/master/Filter/Data/SvfLinearTrapOptimised2_Lpf.wav)
* :sound: [SvfLinearTrapOptimised2_Bpf.wav](https://github.com/FredAntonCorvest/Common-DSP/blob/master/Filter/Data/SvfLinearTrapOptimised2_Bpf.wav)
* :sound: [SvfLinearTrapOptimised2_Hpf.wav](https://github.com/FredAntonCorvest/Common-DSP/blob/master/Filter/Data/SvfLinearTrapOptimised2_Hpf.wav)
* :sound: [SvfLinearTrapOptimised2_Notch.wav](https://github.com/FredAntonCorvest/Common-DSP/blob/master/Filter/Data/SvfLinearTrapOptimised2_Notch.wav)
* :sound: [SvfLinearTrapOptimised2_Peak.wav](https://github.com/FredAntonCorvest/Common-DSP/blob/master/Filter/Data/SvfLinearTrapOptimised2_Peak.wav)
* :sound: [SvfLinearTrapOptimised2_All.wav](https://github.com/FredAntonCorvest/Common-DSP/blob/master/Filter/Data/SvfLinearTrapOptimised2_All.wav)
* :sound: [SvfLinearTrapOptimised2_Bell.wav](https://github.com/FredAntonCorvest/Common-DSP/blob/master/Filter/Data/SvfLinearTrapOptimised2_Bell.wav)
* :sound: [SvfLinearTrapOptimised2_Lsf.wav](https://github.com/FredAntonCorvest/Common-DSP/blob/master/Filter/Data/SvfLinearTrapOptimised2_Lsf.wav)
* :sound: [SvfLinearTrapOptimised2_Hsf.wav](https://github.com/FredAntonCorvest/Common-DSP/blob/master/Filter/Data/SvfLinearTrapOptimised2_Hsf.wav)

#### Build/Execution instruction:
    g++ SvfLinearTrapOptimised2Demo.cpp -o SvfLinearTrapOptimised2Demo
    ./SvfLinearTrapOptimised2Demo

### WaveFileWriter

Quick & dirty wave file - This code snippet comes from an example on wavetable oscillator written by Nigel Redmon on 4/31/12
Check his blog! An amazing source of information/example about DSP. Thanks Nigel!
