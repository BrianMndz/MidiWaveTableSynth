/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class WolfTableSynthAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                            private juce::Slider::Listener
{
public:
    WolfTableSynthAudioProcessorEditor (WolfTableSynthAudioProcessor&);
    ~WolfTableSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged (juce::Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    WolfTableSynthAudioProcessor& audioProcessor;
    
    juce::Slider gainSlider;
    
    /** Memory space to allocate the attachment for the slider and AudioProcessorValue Tree State */
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WolfTableSynthAudioProcessorEditor)
};
