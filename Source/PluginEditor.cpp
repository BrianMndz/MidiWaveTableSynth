/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WolfTableSynthAudioProcessorEditor::WolfTableSynthAudioProcessorEditor (WolfTableSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 75, 37);
    gainSlider.addListener(this);
    addAndMakeVisible(gainSlider);
    
    /** Allocate the memory for the attachment between the slider and apvts parameters.
     * &audioprocessor usage, reference in the pluginEditor, an object to reference the PlugInProcessor.
     * Usage of STRING as key */
    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GAIN", gainSlider);
    
    setSize (400, 300);
}

WolfTableSynthAudioProcessorEditor::~WolfTableSynthAudioProcessorEditor()
{
}

//==============================================================================
void WolfTableSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void WolfTableSynthAudioProcessorEditor::resized()
{
    /**Slider subcomponent on screen*/
    gainSlider.setBounds(getWidth()/2 - 25, getHeight() / 2 - 75, 200, 150);
}

void WolfTableSynthAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    audioProcessor.volumeProcessor = gainSlider.getValue();
}
