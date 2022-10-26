/*
  ==============================================================================

    wtOsc.h
    Created: 20 Oct 2022 11:27:22pm
    Author:  Brian Mendoza

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class wtOsc
{
public:
    wtOsc(std::vector<float> waveTable, double sampleRate);
    void setFrequency(float freq);
    float getSample();
    void stop();
    bool isPlaying();
private:
    float interpolateLinearly();
    
    std::vector<float> waveTable;
    double sampleRate;
    float indexIncrement = 0.0f;
    float index = 0.0f;
};
