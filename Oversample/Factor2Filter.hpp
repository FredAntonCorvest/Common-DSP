#ifndef FACTOR2FILTER_H_
#define FACTOR2FILTER_H_

/*
 
 FIR filter designed with
 http://t-filter.appspot.com
 
 sampling frequency: 88200 Hz
 
 * 0 Hz - 15000 Hz
 gain = 1
 desired ripple = 0.1 dB
 actual ripple = 0.07051496757581141 dB
 
 * 22050 Hz - 44100 Hz
 gain = 0
 desired attenuation = -100 dB
 actual attenuation = -100.38753223360678 dB
 
 */

#define FACTOR2FILTER_TAP_NUM 53

static double filter_taps2x[FACTOR2FILTER_TAP_NUM] = {
    -0.00010407262888197978,
    -0.0003209570017633415,
    -0.00025717784145249084,
    0.0006327364085023385,
    0.001920144150230849,
    0.0018065555158670713,
    -0.0007370770429829215,
    -0.0032558726009788742,
    -0.0015540425708096685,
    0.003924578524221338,
    0.005876153859127602,
    -0.0010333059396801314,
    -0.00967709968167278,
    -0.006239869859525132,
    0.009291903269969856,
    0.016366529116012163,
    -0.0008139370625978141,
    -0.024346167121712326,
    -0.01747196659624832,
    0.022367154190265463,
    0.043450452206570435,
    -0.0004309188245310287,
    -0.07107038131149002,
    -0.05989294696685504,
    0.09228236241268722,
    0.30143966185495363,
    0.39975426862092917,
    0.30143966185495363,
    0.09228236241268722,
    -0.05989294696685504,
    -0.07107038131149002,
    -0.0004309188245310287,
    0.043450452206570435,
    0.022367154190265463,
    -0.01747196659624832,
    -0.024346167121712326,
    -0.0008139370625978141,
    0.016366529116012163,
    0.009291903269969856,
    -0.006239869859525132,
    -0.00967709968167278,
    -0.0010333059396801314,
    0.005876153859127602,
    0.003924578524221338,
    -0.0015540425708096685,
    -0.0032558726009788742,
    -0.0007370770429829215,
    0.0018065555158670713,
    0.001920144150230849,
    0.0006327364085023385,
    -0.00025717784145249084,
    -0.0003209570017633415,
    -0.00010407262888197978
};

typedef struct {
    double history[FACTOR2FILTER_TAP_NUM];
    unsigned int last_index;
} factor2Filter;

void factor2Filter_init(factor2Filter* f) {
    int i;
    for(i = 0; i < FACTOR2FILTER_TAP_NUM; ++i)
        f->history[i] = 0;
    f->last_index = 0;
}

void factor2Filter_put(factor2Filter* f, double input) {
    f->history[f->last_index++] = input;
    if(f->last_index == FACTOR2FILTER_TAP_NUM)
        f->last_index = 0;
}

double factor2Filter_get(factor2Filter* f) {
    double acc = 0;
    int index = f->last_index, i;
    for(i = 0; i < FACTOR2FILTER_TAP_NUM; ++i) {
        index = index != 0 ? index-1 : FACTOR2FILTER_TAP_NUM-1;
        acc += f->history[index] * filter_taps2x[i];
    };
    return acc;
}

#endif
