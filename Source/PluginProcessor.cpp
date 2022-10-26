/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WolfTableSynthAudioProcessor::WolfTableSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
apvts(*this, nullptr, "Parameters", paramLayout())
#endif
{
}

WolfTableSynthAudioProcessor::~WolfTableSynthAudioProcessor()
{
}

//==============================================================================
const juce::String WolfTableSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WolfTableSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WolfTableSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WolfTableSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WolfTableSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WolfTableSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WolfTableSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WolfTableSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String WolfTableSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void WolfTableSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void WolfTableSynthAudioProcessor::prepareToPlay (double sampleRate, int )
{
    /** In the class synthClass is now the prepareToPlay functionality, so now we can call it */
    wtSynth.prepareToPlay(sampleRate);
}

void WolfTableSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WolfTableSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void WolfTableSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    /** Clear the buffer, garbage values */
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();
    
    wtSynth.processBlock(buffer, midiMessages, volumeProcessor);
}

//==============================================================================
bool WolfTableSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* WolfTableSynthAudioProcessor::createEditor()
{
    return new WolfTableSynthAudioProcessorEditor (*this);
}

//==============================================================================
void WolfTableSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void WolfTableSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WolfTableSynthAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout WolfTableSynthAudioProcessor::paramLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN", "Gain", 0.0f, 0.9f, 0.25f));
    
    return {params.begin(), params.end() };
}
