/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
A2StarterAudioProcessorEditor::A2StarterAudioProcessorEditor(A2StarterAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 300);

    // these define the parameters of our slider object
    volumeSlider.setSliderStyle(juce::Slider::Rotary);
    volumeSlider.setRange(0.0, 3.0, 0.1);
    volumeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    volumeSlider.setPopupDisplayEnabled(true, false, this);
    volumeSlider.setTextValueSuffix(" Second Time Delay");
    volumeSlider.setValue(3.0);

   
    delayLabel.setText("Time Delay", juce::dontSendNotification);
    delayLabel.setFont(juce::Font(15.0));
    delayLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    delayLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(delayLabel);

    dryLabel.setText("Dry", juce::dontSendNotification);
    dryLabel.setFont(juce::Font(15.0));
    dryLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    dryLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(dryLabel);

    wetLabel.setText("Wet", juce::dontSendNotification);
    wetLabel.setFont(juce::Font(15.0));
    wetLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    wetLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(wetLabel);

    feedLabel.setText("Feedback", juce::dontSendNotification);
    feedLabel.setFont(juce::Font(15.0));
    feedLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    feedLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(feedLabel);
    

    drySlider.setSliderStyle(juce::Slider::Rotary);
    drySlider.setRange(0.0, 100.0, 1.0);
    drySlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    drySlider.setPopupDisplayEnabled(true, false, this);
    drySlider.setTextValueSuffix(" % Dry Volume");
    drySlider.setValue(100.0);

    wetSlider.setSliderStyle(juce::Slider::Rotary);
    wetSlider.setRange(0.0, 100.0, 1.0);
    wetSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    wetSlider.setPopupDisplayEnabled(true, false, this);
    wetSlider.setTextValueSuffix(" % Wet Volume");
    wetSlider.setValue(100.0);

    feedbackSlider.setSliderStyle(juce::Slider::Rotary);
    feedbackSlider.setRange(0.0, 100.0, 1.0);
    feedbackSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    feedbackSlider.setPopupDisplayEnabled(true, false, this);
    feedbackSlider.setTextValueSuffix(" % Feedback");
    feedbackSlider.setValue(50.0);

    // this function adds the slider to the editor
    addAndMakeVisible(&volumeSlider);
    volumeSlider.addListener(this);

    addAndMakeVisible(&drySlider);
    drySlider.addListener(this);

    addAndMakeVisible(&wetSlider);
    wetSlider.addListener(this);

    addAndMakeVisible(&feedbackSlider);
    feedbackSlider.addListener(this);
   
    addAndMakeVisible(pingPong);
    pingPong.onClick = [this] { updateToggleState(&pingPong, "Ping Pong"); };
    pingPong.setClickingTogglesState(true);

}

A2StarterAudioProcessorEditor::~A2StarterAudioProcessorEditor()
{
}

//==============================================================================
void A2StarterAudioProcessorEditor::paint(juce::Graphics& g)
{
    // fill the whole window white
    g.fillAll(juce::Colours::white);

    // set the current drawing colour to black
    g.setColour(juce::Colours::black);

    // set the font size and draw text to the screen
    g.setFont(15.0f);

    g.drawFittedText("A2 Delay", 0, 0, getWidth(), 30, juce::Justification::centred, 1);
}

void A2StarterAudioProcessorEditor::resized()
{
    
    int x = getWidth();
    int y = getHeight();
    volumeSlider.setBounds(30, 20, 80, y - 100);
    drySlider.setBounds(120, 20, 80, y - 100);
    wetSlider.setBounds(210, 20, 80, y - 100);
    feedbackSlider.setBounds(300, 20, 80, y - 100);

    delayLabel.setBounds(30, 60, 80, y - 100);
    dryLabel.setBounds(120, 60, 80, y - 100);
    wetLabel.setBounds(210, 60, 80, y - 100);
    feedLabel.setBounds(300, 60, 80, y - 100);
    pingPong.setBounds(130, 200, 150, 40);

  
}

void A2StarterAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.delayLength = (double)volumeSlider.getValue();
    audioProcessor.dryVal = (double)drySlider.getValue() / 100.0;
    audioProcessor.wetVal = (double)wetSlider.getValue() / 100.0;
    audioProcessor.feedback = (double)feedbackSlider.getValue() / 100.0;
}
