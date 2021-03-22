/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class A2StarterAudioProcessor : public juce::AudioProcessor
{
public:
    float volumeBoost;
    double delayLength;
    double dryVal = 1.0;   // meant to hold values for rotary Parameters Dry and Wet
    double wetVal = 1.0;
    double feedback = 0.5;
    bool pingPong;

    //==============================================================================
    A2StarterAudioProcessor();
    ~A2StarterAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

private:
    float rate;
    int delayWritePos;
    int delayReadPos;
    juce::AudioBuffer<float> delayBuffer;
    juce::AudioBuffer<float> feedbackBuffer;
    int delayBufferLength;
    int dp = 0;
    int delaySamples = (int)(44100);

  
   
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(A2StarterAudioProcessor)
};
