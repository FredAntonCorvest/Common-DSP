#ifndef FACTOR4FILTER_H_
#define FACTOR4FILTER_H_

/*
 
 FIR filter designed with
 http://t-filter.appspot.com
 
 sampling frequency: 176400 Hz
 
 * 0 Hz - 15000 Hz
 gain = 1
 desired ripple = 0.1 dB
 actual ripple = 0.009030632920609797 dB
 
 * 22050 Hz - 88200 Hz
 gain = 0
 desired attenuation = -100 dB
 actual attenuation = -118.23874773524926 dB
 
 */

#define FACTOR4FILTER_TAP_NUM 137

static double filter_taps4x[FACTOR4FILTER_TAP_NUM] = {
    0.0000037795017714714303,
    0.000011979348190063462,
    0.000026314232567997693,
    0.00004477784188982222,
    0.00006102749371897937,
    0.00006464555456985219,
    0.00004424798013429687,
    -0.000006843447555783268,
    -0.00008419336096084502,
    -0.0001683344185835631,
    -0.0002267704610767369,
    -0.00022368720877558767,
    -0.00013579206882786827,
    0.00003123211070595373,
    0.00023376202377667972,
    0.00039619744093652876,
    0.0004345237777363072,
    0.0002922830856881802,
    -0.000024196356519272793,
    -0.00042849553842755697,
    -0.0007687297939796259,
    -0.0008770548027362802,
    -0.0006411779040022286,
    -0.00007077449303665025,
    0.000673537014795685,
    0.0013122613828739564,
    0.0015414887083462028,
    0.001162438931208194,
    0.0001956863508002471,
    -0.0010756308975783044,
    -0.0021700352462981714,
    -0.002575241290144861,
    -0.001966992720038297,
    -0.00039246126024693184,
    0.0016713429096034617,
    0.003436548026879506,
    0.0040846938641133,
    0.003117082999189277,
    0.0006347289609818765,
    -0.002588848340080823,
    -0.005313616210290388,
    -0.006280524392712307,
    -0.004750684865476828,
    -0.0009191942503714803,
    0.003997932470415999,
    0.008099548831778883,
    0.009488487947168634,
    0.00708127362750909,
    0.0012147032574588922,
    -0.006242702048689195,
    -0.012406172882142885,
    -0.014407866070816563,
    -0.010605220529241918,
    -0.0014943322182477754,
    0.010096274922793972,
    0.0197317394641482,
    0.022880208402352428,
    0.016739413783067807,
    0.0017233670720870585,
    -0.01800938828517921,
    -0.035325475000174654,
    -0.042085159660169964,
    -0.031762017749417545,
    -0.0018743620698635477,
    0.044625510127120044,
    0.09956944499365979,
    0.15145636975316595,
    0.18849976582979194,
    0.201926866665578,
    0.18849976582979194,
    0.15145636975316595,
    0.09956944499365979,
    0.044625510127120044,
    -0.0018743620698635477,
    -0.031762017749417545,
    -0.042085159660169964,
    -0.035325475000174654,
    -0.01800938828517921,
    0.0017233670720870585,
    0.016739413783067807,
    0.022880208402352428,
    0.0197317394641482,
    0.010096274922793972,
    -0.0014943322182477754,
    -0.010605220529241918,
    -0.014407866070816563,
    -0.012406172882142885,
    -0.006242702048689195,
    0.0012147032574588922,
    0.00708127362750909,
    0.009488487947168634,
    0.008099548831778883,
    0.003997932470415999,
    -0.0009191942503714803,
    -0.004750684865476828,
    -0.006280524392712307,
    -0.005313616210290388,
    -0.002588848340080823,
    0.0006347289609818765,
    0.003117082999189277,
    0.0040846938641133,
    0.003436548026879506,
    0.0016713429096034617,
    -0.00039246126024693184,
    -0.001966992720038297,
    -0.002575241290144861,
    -0.0021700352462981714,
    -0.0010756308975783044,
    0.0001956863508002471,
    0.001162438931208194,
    0.0015414887083462028,
    0.0013122613828739564,
    0.000673537014795685,
    -0.00007077449303665025,
    -0.0006411779040022286,
    -0.0008770548027362802,
    -0.0007687297939796259,
    -0.00042849553842755697,
    -0.000024196356519272793,
    0.0002922830856881802,
    0.0004345237777363072,
    0.00039619744093652876,
    0.00023376202377667972,
    0.00003123211070595373,
    -0.00013579206882786827,
    -0.00022368720877558767,
    -0.0002267704610767369,
    -0.0001683344185835631,
    -0.00008419336096084502,
    -0.000006843447555783268,
    0.00004424798013429687,
    0.00006464555456985219,
    0.00006102749371897937,
    0.00004477784188982222,
    0.000026314232567997693,
    0.000011979348190063462,
    0.0000037795017714714303
};

typedef struct {
    double history[FACTOR4FILTER_TAP_NUM];
    unsigned int last_index;
} factor4Filter;

void factor4Filter_init(factor4Filter* f) {
    int i;
    for(i = 0; i < FACTOR4FILTER_TAP_NUM; ++i)
        f->history[i] = 0;
    f->last_index = 0;
}

void factor4Filter_put(factor4Filter* f, double input) {
    f->history[f->last_index++] = input;
    if(f->last_index == FACTOR4FILTER_TAP_NUM)
        f->last_index = 0;
}

double factor4Filter_get(factor4Filter* f) {
    double acc = 0;
    int index = f->last_index, i;
    for(i = 0; i < FACTOR4FILTER_TAP_NUM; ++i) {
        index = index != 0 ? index-1 : FACTOR4FILTER_TAP_NUM-1;
        acc += f->history[index] * filter_taps4x[i];
    };
    return acc;
}

#endif