/*
  ==============================================================================

    synthClass.cpp
    Created: 18 Oct 2022 9:52:53pm
    Author:  Brian Mendoza

  ==============================================================================
*/

#include <JuceHeader.h>
#include "synthClass.h"

//==============================================================================
synthClass::synthClass()
{

}

synthClass::~synthClass()
{
}

void synthClass::paint (juce::Graphics& g)
{

}

void synthClass::resized()
{

}

//==============================================================================
std::vector<float> synthClass::generateSineWaveTable()
{
    /** TODO: Check for different values what is the CPU usage */
    constexpr auto WAVETABLE_LEN = 128;
    /** Loop over the wavetable length and assign a value of a sin func (a point).
     * increase the argument of the sin by the 2pi / len of wt. */
    const auto PI = std::atanf(1.f) * 4;
    std::vector<float> sineWaveTable(WAVETABLE_LEN);
    
    for(size_t i = 0; i < WAVETABLE_LEN; i++)
    {
        sineWaveTable[i] = std::sinf(2 * PI * static_cast<float>(i) / static_cast<float>(WAVETABLE_LEN));
    }
    
    return sineWaveTable;
}

void synthClass::initializeOscillators()
{
    constexpr auto OSCILLATORS_COUNT = 128; //MIDI note numbers
    const auto waveTable = generateSineWaveTable();
    
    oscillators.clear();
    
    for(size_t i = 0; i < OSCILLATORS_COUNT; i++)
    {
        oscillators.emplace_back(waveTable, sampleRate);
    }
}

void synthClass::prepareToPlay (double sampleRate)
{
    //Plugin processor has given us the sampleRate
    this->sampleRate = sampleRate;
    
    initializeOscillators();
}

void synthClass::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages, float gainValue)
{
    /** Read the MIDI messages */
    auto currentSample = 0;
    
    /** Get single message in the MIDI Buffer */
    for (const auto midiMessage : midiMessages)
    {
        /** TODO: Check documentation of MIDI messages, buffer and other info in JUCE */
        const auto midiEvent = midiMessage.getMessage();    //Metadata of midi message
        //When this midi event happen? getting an index...
        const auto midiEventSample = static_cast<int> (midiEvent.getTimeStamp());
        
        /** TODO: generate sound in the interval sound: */
        render(buffer, currentSample, midiEventSample, gainValue);
        handleMidiEvent(midiEvent);     //Function to analize what happens in the midi message
        
        currentSample = midiEventSample;    /** Update current sample to be at the INDEX of the midi event */
    }
    
    render(buffer, currentSample, buffer.getNumSamples(), gainValue);
    
}

void synthClass::handleMidiEvent(const juce::MidiMessage& midiEvent)
{
    if (midiEvent.isNoteOn())
    {
        const int oscillatorID = midiEvent.getNoteNumber();
        const float frequency = midiNoteToFrequency(oscillatorID);
        oscillators[oscillatorID].setFrequency(frequency);          //This will start playing...
    }
    else if (midiEvent.isNoteOff())
    {
        const auto oscillatorID = midiEvent.getNoteNumber();
        oscillators[oscillatorID].stop();
    }
    else if( midiEvent.isAllNotesOff())
    {
        for(auto &oscillator : oscillators)
        {
            oscillator.stop();
        }
    }
}

float synthClass::midiNoteToFrequency(int midiNote)
{
    constexpr auto A4_FREQUENCY = 440.f;    //Constant expressions
    constexpr auto A4_NOTE_NUMB = 69.f;
    constexpr auto SEMITN_OCTAV = 12.f;
    /** Formula to convert midi note to frequency returned */
    return A4_FREQUENCY * std::powf (2.f, ((midiNote - A4_NOTE_NUMB) / SEMITN_OCTAV)); 
}

/** Here's happening the output sound, as in processBloc() original function  */
void synthClass::render(juce::AudioBuffer<float> &buffer, int startSample, int endSample, float gainValue)
{
    auto *firstChannel = buffer.getWritePointer(0);
    auto *secondChannl = buffer.getWritePointer(1);
    /** Iterate over our oscillators and if active, give us samples in a specific range */
    for (auto &oscillator : oscillators)
    {
        if (oscillator.isPlaying())
        {
            for(auto sample = startSample; sample < endSample; sample++)
            {
                firstChannel[sample] += oscillator.getSample() * gainValue;
                secondChannl[sample] += oscillator.getSample() * gainValue;
            }
        }
    }
    
}
