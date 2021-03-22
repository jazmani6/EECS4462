/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <windows.h>
#include <stdlib.h>

//==============================================================================
A2StarterAudioProcessor::A2StarterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
#endif
{
}

A2StarterAudioProcessor::~A2StarterAudioProcessor()
{
}

//==============================================================================
const juce::String A2StarterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool A2StarterAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool A2StarterAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool A2StarterAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double A2StarterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int A2StarterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int A2StarterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void A2StarterAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String A2StarterAudioProcessor::getProgramName(int index)
{
    return {};
}

void A2StarterAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void A2StarterAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    rate = static_cast<float> (sampleRate);
    volumeBoost = 1.0;
    delayLength = 3.0;
    pingPong = false;

    // Need to change this value to a number that corresponds to 3 seconds
    delayBufferLength = 3 * (rate);

    delayBuffer.setSize(2, delayBufferLength);
    delayBuffer.clear();

    // Initialize the delay length in seconds and set the write pointer to 0
    delayWritePos = 0;
     delayReadPos = 0;
   
   
    float delaySamples = delayLength * rate; // converting delay from seconds to samples

    delayReadPos = static_cast<int>(delayWritePos - delaySamples + delayBufferLength) % delayBufferLength;
    

}

void A2StarterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool A2StarterAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void A2StarterAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    int delayWrite = 0;
    int delayRead = 0;
    delayBufferLength = delayLength * (rate);
  

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
   

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* data = buffer.getWritePointer(channel, 0);
        float* dataL = buffer.getWritePointer(0);
        float* dataR = buffer.getWritePointer(1);
        
        int numSamples = buffer.getNumSamples();


        // Get a pointer to write in the delayBuffer
        float* delayData = delayBuffer.getWritePointer(channel);
        auto delayDataReader = delayBuffer.getReadPointer(channel);
        double outR = 0.0;
        double outL = 0.0;
        
       
        

        // Write something to the delayBuffer
        delayRead = delayReadPos;
        delayWrite = delayWritePos;
        if (pingPong == true)
        {
            int channelPong = 0;
            while (channelPong < totalNumInputChannels) {
                int oddChannel = (channelPong+1) % totalNumInputChannels;
                float* dataOdd = buffer.getWritePointer(oddChannel, 0);
                float* dataOddOut = buffer.getWritePointer(oddChannel, 0);
                float* dataOut = buffer.getWritePointer(channel, 0);
                float* delayDataOdd = delayBuffer.getWritePointer(oddChannel);
                for (int i = 0; i < numSamples; ++i)
                {
                    const float inL = data[i];
                    const float inR = dataOdd[i];
                    delayData[delayWrite] = inL + (feedback * delayDataOdd[delayRead]);
                    delayDataOdd[delayWrite] = inR + (feedback * delayData[delayRead]);


                    float delaySamples = delayLength * rate;

                  
                        outL = (inL * dryVal) + (wetVal * delayData[delayRead]);
                        outR = (inR * dryVal) + (wetVal * delayDataOdd[delayRead]);
                        
                        
                        if (channel == 0) {

                            data[i] = outL;
                            channelPong = 1;
                           
                        }
                        else {
                            dataOdd[i] = outR;
                        
                        }
                      
                        delayRead++;
                        delayWrite++;
                        delayRead %= delayBufferLength;
                        delayWrite %= delayBufferLength;
                }
                channelPong += 2;
                DBG(channel);

            }
            
        }
        else{

            for (int i = 0; i < numSamples; ++i)
            {
                const float in = data[i];
                double out = 0.0;

              
                const float inL = dataL[i];
                const float inR = dataR[i];
                float delaySamples = delayLength * rate;
                out = (in * dryVal) + (wetVal * delayDataReader[delayRead]);
                delayData[delayWrite] = in + (feedback * delayDataReader[delayRead]);
              
                delayRead++;
                delayWrite++;

                delayRead %= delayBufferLength;
                delayWrite %= delayBufferLength;

                data[i] = out;
            }
               
            
        }

        // Must also write to the output buffer
    }
    delayReadPos = delayRead;
    delayWritePos = delayWrite;


}

//==============================================================================
bool A2StarterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* A2StarterAudioProcessor::createEditor()
{
    return new A2StarterAudioProcessorEditor(*this);
}

//==============================================================================
void A2StarterAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void A2StarterAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new A2StarterAudioProcessor();
}
