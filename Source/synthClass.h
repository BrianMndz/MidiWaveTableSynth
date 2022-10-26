/*
  ==============================================================================

    synthClass.h
    Created: 18 Oct 2022 9:52:53pm
    Author:  Brian Mendoza

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "wtOsc.h"

//==============================================================================
/*
*/
class synthClass  : public juce::Component
{
public:
    synthClass();
    ~synthClass() override;
    /** Function to call from plugin processor */
    void prepareToPlay(double sampleRate);
    /** Actual processing: */
    void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages, float gainValue);

    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (synthClass)
    
    void initializeOscillators();
    std::vector<float> generateSineWaveTable();
    void handleMidiEvent(const juce::MidiMessage& midiEvent);
    float midiNoteToFrequency(int midiNote);
    void render(juce::AudioBuffer<float> &buffer, int startSample, int endSample, float gainValue);
    
    double sampleRate;
    double ampValue = 0.25f;
    std::vector<wtOsc> oscillators;
};
