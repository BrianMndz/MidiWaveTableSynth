/*
  ==============================================================================

    wtOsc.cpp
    Created: 20 Oct 2022 11:27:22pm
    Author:  Brian Mendoza

  ==============================================================================
*/

#include "wtOsc.h"

wtOsc::wtOsc(std::vector<float> waveTable, double sampleRate)
    : waveTable{ std::move(waveTable) },
        sampleRate{ sampleRate }
{}

void wtOsc::setFrequency(float freq)
{
    indexIncrement = freq * static_cast<float>(waveTable.size()) / static_cast<float>(sampleRate);
}

float wtOsc::getSample()
{
    const float sample = interpolateLinearly();
    /** fmod function to wrap over the table when looping it */
    index += indexIncrement;
    index = std::fmod( index, static_cast<float>(waveTable.size()) );
    return sample;
                      
}

/** a value between two integer indices, we need to make a calculation to avoid noises.
We return the weigthed sum, wich means, what index weights more, the nearer. */
float wtOsc::interpolateLinearly()
{
    const auto truncatedIndex = static_cast<int>(index);
    const auto nextIndex = (truncatedIndex + 1) % static_cast<int>(waveTable.size());

    const auto nextIndexWeight = index - static_cast<float>(truncatedIndex);
    const auto truncatedIndexWeight = 1.f - nextIndexWeight;

    return truncatedIndexWeight * waveTable[truncatedIndex] + nextIndexWeight * waveTable[nextIndex];
}

/** Reset index and indexIncrement to 0 */
void wtOsc::stop()
{
    index = 0.f;
    indexIncrement = 0.f;
}

/** If the index increment is 0, wavetable is not looping */
bool wtOsc::isPlaying()
{
    return indexIncrement != 0.f;
}
