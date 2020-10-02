/*/*
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
class A1StarterAudioProcessorEditor : public juce::AudioProcessorEditor, private juce::Slider::Listener
{
public:
    A1StarterAudioProcessorEditor(A1StarterAudioProcessor&);
    ~A1StarterAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

    enum RadioButtonIds
    {
        OrderButtons = 1001
    };
    enum FirstButtonIds
    {
        FirstButtons = 1002
    };

    enum LastButtonIds
    {
        LastButtons = 1003
    };
    

private:
    void sliderValueChanged(juce::Slider* slider) override;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    A1StarterAudioProcessor& audioProcessor;

    juce::Slider arpSlider; // [1]
    juce::Slider octave;
    void updateToggleState(juce::Button* button, juce::String name)
    {
        auto state = button->getToggleState();

        juce::String stateString = state ? "ON" : "OFF";
        juce::String selectedString = state ? " (selected)" : "";

        juce::Logger::outputDebugString(name + " Button changed to " + stateString);
        button->setButtonText(name + selectedString);
        audioProcessor.ascOn = ascButton.getToggleState();
        audioProcessor.descOn = descButton.getToggleState();
        audioProcessor.outInOn = outInButton.getToggleState();
        audioProcessor.sweeping = sweeping.getToggleState();
        audioProcessor.ascDur = ascSpeed.getToggleState();
        audioProcessor.doubleFirst = doubleFirst.getToggleState();
        audioProcessor.doubleLast = doubleLast.getToggleState();
        audioProcessor.halfFirst = halfFirst.getToggleState();
        audioProcessor.halfLast = halfLast.getToggleState();
    }


    

    
    juce::Label patternLabel{ {}, "Select your note ordering (Ascending by Default)" };
    juce::Label durationLabel{ {}, "Change duration of notes (Can select multiple options)" };
    juce::Label sliderLabel{ {}, "Select your Note Speeds" };
    juce::Label octaveLabel{ {}, "How many octaves up or down?" };
    juce::TextButton ascButton{ "Ascending" },
        descButton{ "Descending" }, outInButton{ "Outside to Inside Notes" }, sweeping{ "Sweeping Up and Down" }, ascSpeed {"Ascending (First note doubled, last note halved)"};
    juce::TextButton doubleFirst{ "Double duration of first note" },
        doubleLast{ "Double duration of last note" }, halfFirst{ "Halve duration of first note" }, halfLast{ "Halve duration of last note" };


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(A1StarterAudioProcessorEditor)
};
