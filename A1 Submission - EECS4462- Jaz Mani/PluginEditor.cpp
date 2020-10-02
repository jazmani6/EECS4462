/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
A1StarterAudioProcessorEditor::A1StarterAudioProcessorEditor(A1StarterAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(600, 600);

    // these define the parameters of our slider object
    arpSlider.setSliderStyle(juce::Slider::LinearBar);
    arpSlider.setRange(0.0, 1.0, 0.05);
    arpSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    arpSlider.setPopupDisplayEnabled(true, false, this);
    arpSlider.setTextValueSuffix("Speed");
    arpSlider.setValue(0.5);

    // this function adds the slider to the editor
    addAndMakeVisible(&arpSlider);
    arpSlider.addListener(this);

    //slider for the octave feature
    octave.setSliderStyle(juce::Slider::LinearBar);
    octave.setRange(0, 3, 1);
    octave.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 90);
    octave.setPopupDisplayEnabled(true, false, this);
    octave.setTextValueSuffix(" Octaves +/-");
    octave.setValue(0);

    // this function adds the slider to the editor
    addAndMakeVisible(&octave);
    octave.addListener(this);

    addAndMakeVisible (ascButton);
        addAndMakeVisible (descButton);
        addAndMakeVisible(outInButton);
        addAndMakeVisible(sweeping);
        

        addAndMakeVisible(doubleFirst);
        addAndMakeVisible(doubleLast);
        addAndMakeVisible(halfFirst);
        addAndMakeVisible(halfLast);
        doubleFirst.onClick = [this] { updateToggleState(&doubleFirst, "Double duration of first note"); };
        doubleLast.onClick = [this] { updateToggleState(&doubleLast, "Double duration of last note"); };
        halfFirst.onClick = [this] { updateToggleState(&halfFirst, "Halve duration of first note"); };
        halfLast.onClick = [this] { updateToggleState(&halfLast, "Halve duration of last note"); };
        

        addAndMakeVisible(octaveLabel);
        addAndMakeVisible(sliderLabel);
        addAndMakeVisible(durationLabel);
        addAndMakeVisible(patternLabel);
        patternLabel.setFont(juce::Font(16.0f, juce::Font::bold));
        patternLabel.setColour(juce::Label::textColourId, juce::Colours::blue);
        patternLabel.setJustificationType(juce::Justification::centred);
        sliderLabel.setFont(juce::Font(16.0f, juce::Font::bold));
        sliderLabel.setColour(juce::Label::textColourId, juce::Colours::blue);
        sliderLabel.setJustificationType(juce::Justification::centred);
        octaveLabel.setFont(juce::Font(16.0f, juce::Font::bold));
        octaveLabel.setColour(juce::Label::textColourId, juce::Colours::blue);
        octaveLabel.setJustificationType(juce::Justification::centred);
        durationLabel.setFont(juce::Font(16.0f, juce::Font::bold));
        durationLabel.setColour(juce::Label::textColourId, juce::Colours::blue);
        durationLabel.setJustificationType(juce::Justification::centred);
        ascButton.onClick = [this] { updateToggleState (&ascButton,   "Ascending");   };
        descButton.onClick = [this] { updateToggleState (&descButton, "Descending"); };
        sweeping.onClick = [this] { updateToggleState(&sweeping, "Sweeping Up and Down"); };
        outInButton.onClick = [this] { updateToggleState(&outInButton, "Outside notes to Inside"); };
        ascSpeed.onClick = [this] { updateToggleState(&ascSpeed, "Ascending (First note doubled, last note halved)"); };
        ascButton.setRadioGroupId(OrderButtons);
        descButton.setRadioGroupId(OrderButtons);
        sweeping.setRadioGroupId(OrderButtons);
        outInButton.setRadioGroupId(OrderButtons);
        ascSpeed.setRadioGroupId(OrderButtons);
       
        
        ascButton.setClickingTogglesState(true);
        descButton.setClickingTogglesState(true);
        outInButton.setClickingTogglesState(true);
        sweeping.setClickingTogglesState(true);
        ascSpeed.setClickingTogglesState(true);
        doubleFirst.setClickingTogglesState(true);
        doubleLast.setClickingTogglesState(true);
        halfFirst.setClickingTogglesState(true);
        halfLast.setClickingTogglesState(true);
 

}

A1StarterAudioProcessorEditor::~A1StarterAudioProcessorEditor()
{
}

//==============================================================================
void A1StarterAudioProcessorEditor::paint(juce::Graphics& g)
{
    // fill the whole window white
    g.fillAll(juce::Colours::white);

    // set the current drawing colour to black
    g.setColour(juce::Colours::black);

    // set the font size and draw text to the screen
    g.setFont(17.0f);

    g.drawFittedText("Arpeggiator - By Jaz Mani", 0, 0, getWidth(), 30, juce::Justification::centred, 1);
}

void A1StarterAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    // sets the position and size of the slider with arguments (x, y, width, height)
    arpSlider.setBounds(40, getHeight() - 170, getWidth() - 60, 20);
    octave.setBounds(40, getHeight() - 110, getWidth() - 60, 20);
    octaveLabel.setBounds(40, getHeight() - 140, getWidth() - 60, 20);
    sliderLabel.setBounds(40, getHeight() - 200, getWidth() - 60, 20);

    patternLabel.setBounds(100, 60, getWidth() - 200, 20);
    ascButton.setBounds(110, 90, getWidth() - 200, 20);
    descButton.setBounds(110, 120, getWidth() - 200, 20);
    outInButton.setBounds(110, 150, getWidth() - 200, 20);
    sweeping.setBounds(110, 180, getWidth() - 200, 20);
    ascSpeed.setBounds(110, 210, getWidth() - 200, 20);

    durationLabel.setBounds(100, 270, getWidth() - 200, 20);
    doubleFirst.setBounds(100, 300, getWidth() - 400, 20);
    doubleLast.setBounds(100, 330, getWidth() - 400, 20);
    halfFirst.setBounds(getWidth() - 280, 300, getWidth() - 400, 20);
    halfLast.setBounds(getWidth() -280, 330, getWidth() - 400, 20);
    
}

void A1StarterAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.arpSpeed = arpSlider.getValue();
    audioProcessor.octave = octave.getValue();
}

