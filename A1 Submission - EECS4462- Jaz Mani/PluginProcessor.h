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
class A1StarterAudioProcessor : public juce::AudioProcessor
{
public:
    float arpSpeed;
    float octave;
    int first, last;
    bool ascOn, descOn, outInOn,sweeping,ascDur;
    bool doubleFirst, doubleLast, halfFirst, halfLast;
    //==============================================================================
    A1StarterAudioProcessor();
    ~A1StarterAudioProcessor() override;

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
    //==============================================================================
    int currentNote, lastNoteValue, firstNoteValue, currentNoteOdd;
    int time;
    float rate;
    juce::SortedSet<int> notes;
    juce::SortedSet<int> notesCustom;
    int customIndex = 0;
    bool sweepUp, sweepDown;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(A1StarterAudioProcessor)
};
