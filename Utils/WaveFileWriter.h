// WaveFileWriter.h

// This code snippet comes from an example on wavetable oscillator written by Nigel Redmon on 4/31/12
// Available here www.earlevel.com/main/2012/05/03/a-wavetable-oscillator—introduction/
// Check his blog! An amazing source of information/example about DSP. Thanks Nigel!

// Modification 30/11/2016: (Fred Anton Corvest) Add filename and sampleRate parameters to writeFloatSound method
// Modification 01/12/2016: (Fred Anton Corvest) Remove warning: multi-character character constant
// Modification 18/12/2016: (Fred Anton Corvest) Add stereo support

//
// quick & dirty wave file
//

#include <string>
#include <stdint.h>

enum {
    /* keep sorted for wav_w64_format_str() */
    WAVE_FORMAT_UNKNOWN = 0x0000,       /* Microsoft Corporation */
    WAVE_FORMAT_PCM = 0x0001,           /* Microsoft PCM format */
    WAVE_FORMAT_MS_ADPCM = 0x0002,      /* Microsoft ADPCM */
    WAVE_FORMAT_IEEE_FLOAT = 0x0003,    /* Micrososft 32 bit float format */
};

inline void writeFourCC(uint32_t val, uint8_t **bytePtr) {
    *(*bytePtr)++ = val >> 24;
    *(*bytePtr)++ = val >> 16;
    *(*bytePtr)++ = val >> 8;
    *(*bytePtr)++ = val;
}

inline void write4Bytes(uint32_t val, uint8_t **bytePtr) {
    *(*bytePtr)++ = val;
    *(*bytePtr)++ = val >> 8;
    *(*bytePtr)++ = val >> 16;
    *(*bytePtr)++ = val >> 24;
}

inline void write2Bytes(uint32_t val, uint8_t **bytePtr) {
    *(*bytePtr)++ = val;
    *(*bytePtr)++ = val >> 8;
}

void writeFloatSound(int len, float *wave, const std::string& filenName, int sampleRate, int numChannels = 1) {
    // build file
    const int bytesPersample = 4;
    const int soundChunkLen = len * bytesPersample;
    
    uint8_t bytes[58];
    uint8_t *bytePtr = bytes;
    
    writeFourCC(0x52494646/*'RIFF'*/, &bytePtr);
    write4Bytes(4 + 26 + 12 + 8 + soundChunkLen, &bytePtr); // chunk size
    writeFourCC(0x57415645/*'WAVE'*/, &bytePtr);
    writeFourCC(0x666d7420/*'fmt '*/, &bytePtr);
    write4Bytes(18, &bytePtr);                              // size of subchunk that follows
    write2Bytes(WAVE_FORMAT_IEEE_FLOAT, &bytePtr);          // format
    write2Bytes(numChannels, &bytePtr);                     // number of channels
    write4Bytes(sampleRate, &bytePtr);                      // sample rate
    write4Bytes(sampleRate * numChannels * bytesPersample, &bytePtr);  // byte rate
    write2Bytes(numChannels * bytesPersample, &bytePtr);    // block align
    write2Bytes(bytesPersample * 8, &bytePtr);              // bits per sample
    write2Bytes(0, &bytePtr);                               // cbSize
    writeFourCC(0x66616374/*'fact'*/, &bytePtr);
    write4Bytes(4, &bytePtr);                               // size of subchunk that follows
    write4Bytes(numChannels * len, &bytePtr);               // size of subchunk that follows
    writeFourCC(0x64617461/*'data'*/, &bytePtr);
    write4Bytes(soundChunkLen, &bytePtr);                   // subchunk size
    
    // write array to file
    FILE *pFile;
    pFile = fopen(filenName.c_str(), "wb");
    if (pFile) {
        fwrite(bytes, 1, sizeof(bytes), pFile);
        fwrite(wave, sizeof(*wave), len, pFile);            // note: little endian
        fclose(pFile);
    }
    
    return;
}
