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
class A2StarterAudioProcessorEditor : public juce::AudioProcessorEditor, private juce::Slider::Listener
{
public:
    A2StarterAudioProcessorEditor(A2StarterAudioProcessor&);
    ~A2StarterAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void sliderValueChanged(juce::Slider* slider) override;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    A2StarterAudioProcessor& audioProcessor;

    juce::Slider volumeSlider; // [1]
    juce::Slider drySlider;
    juce::Slider wetSlider;
    juce::Slider feedbackSlider;

    juce::Label delayLabel{ {}, "Time Delay" };
    juce::Label dryLabel{ {}, "Dry" };
    juce::Label wetLabel{ {}, "Wet" };
    juce::Label feedLabel{ {}, "Feedback" };

    juce::TextButton pingPong{ "Ping Pong" };

    void updateToggleState(juce::Button* button, juce::String name)
    {
        auto state = button->getToggleState();

        juce::String stateString = state ? "ON" : "OFF";
        juce::String selectedString = state ? " (selected)" : "";

        juce::Logger::outputDebugString(name + " Button changed to " + stateString);
        button->setButtonText(name + selectedString);
        audioProcessor.pingPong = pingPong.getToggleState();
    }



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(A2StarterAudioProcessorEditor)
};
