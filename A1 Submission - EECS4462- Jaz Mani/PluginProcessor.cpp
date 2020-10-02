/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
A1StarterAudioProcessor::A1StarterAudioProcessor()
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

A1StarterAudioProcessor::~A1StarterAudioProcessor()
{
}

//==============================================================================
const juce::String A1StarterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool A1StarterAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool A1StarterAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool A1StarterAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double A1StarterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int A1StarterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int A1StarterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void A1StarterAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String A1StarterAudioProcessor::getProgramName(int index)
{
    return {};
}

void A1StarterAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void A1StarterAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    notes.clear();                          // [1]
    currentNote = -1; 
    currentNoteOdd = -1;// [2]
    lastNoteValue = -1;                     // [3]
    time = 0;                               // [4]
    rate = static_cast<float> (sampleRate); // [5]
    arpSpeed = 0.5;
    octave = 0;
    ascOn = true;
    descOn = false;
    outInOn = false;
    sweepUp = true;
    first = 0;
    doubleFirst = false;
    doubleLast = false;
    halfFirst = false;
    halfLast = false;
}

void A1StarterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool A1StarterAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
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

void A1StarterAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    // the audio buffer in a midi effect will have zero channels!
                                                          // [6]

    // however we use the buffer to get timing information
    auto numSamples = buffer.getNumSamples();                
    juce::MidiBuffer processedMidi;// [7]

    // get note duration
    auto noteDuration = static_cast<int> (std::ceil(rate * 0.25f * (0.1f + (1.0f - (arpSpeed)))));   // [8]
    auto noteDurationMult = static_cast<int> (2 *(std::ceil(rate * 0.25f * (0.1f + (1.0f - (arpSpeed))))));
    auto noteDurationHalf = static_cast<int> ((std::ceil(rate * 0.25f * (0.1f + (1.0f - (arpSpeed)))))/2);

    for (const auto metadata : midi)                                                                // [9]
    {
        const auto msg = metadata.getMessage();
        

        if (msg.isNoteOn()) {
            if (ascOn == true || outInOn == true) {
                for (int i = 0; i <= octave; i++) {
                    notes.add(msg.getNoteNumber() + 12 * i);
                   
                }
            }
            else if (ascDur == true) {
                for (int i = 0; i <= octave; i++) {
                    notes.add(msg.getNoteNumber() + 12 * i);

                }
            }
            else if (descOn == true){
                for (int i = 0; i <= octave; i++) {
                notes.add(msg.getNoteNumber() - 12 * i);
               
                }
            }
            else if (outInOn == true) {
                for (int i = 0; i <= octave; i++) {
                    notes.add(msg.getNoteNumber() + 12 * i);
                    
                }
            }
            else if (sweeping == true) {
                for (int i = 0; i <= octave; i++) {
                    notes.add(msg.getNoteNumber() - 12 * i);
                    notes.add(msg.getNoteNumber() + 12 * i);

                }
            }

        }
        else if (msg.isNoteOff()) {
            if (ascOn == true || outInOn == true) {
                for (int i = 0; i <= octave; i++) {
                    notes.removeValue(msg.getNoteNumber() + 12 * i);
                   
                }
            }
            else if (descOn == true) {
                for (int i = 0; i <= octave; i++) {
                    notes.removeValue(msg.getNoteNumber() - 12 * i);
                    
                }
            }
            else if (outInOn == true) {
                for (int i = 0; i <= octave; i++) {
                    notes.removeValue(msg.getNoteNumber() + 12 * i);
                   
                }
            }
            else if (sweeping == true) {
                for (int i = 0; i <= octave; i++) {
                    notes.removeValue(msg.getNoteNumber() - 12 * i);
                    notes.removeValue(msg.getNoteNumber() + 12 * i);

                }
            }
            else if (ascDur == true) {
                for (int i = 0; i <= octave; i++) {
                    notes.removeValue(msg.getNoteNumber() + 12 * i);

                }
            }
        }
        
       

    }
    midi.clear();                                                                                   // [10]
    processedMidi.clear();
    noteDurationMult = noteDuration * 2;
    noteDurationHalf = noteDuration / 2;
    if (doubleFirst == true && currentNote == 0) {
        noteDuration = noteDurationMult;
    }
    if (doubleLast == true && currentNote == notes.size() - 1) {
        noteDuration = noteDurationMult;
    }
    if (halfLast == true && currentNote == notes.size() -1) {
        noteDuration = noteDurationHalf;
    }
    if (halfFirst == true && currentNote == 0) {
        noteDuration = noteDurationHalf;
    }

    if ((time + numSamples) >= noteDuration)                                                        // [11]
    {
        auto offset = juce::jmax(0, juce::jmin((int)(noteDuration - time), numSamples - 1));     // [12]
        auto offsetMult = juce::jmax(0, juce::jmin((int)(noteDurationMult - time), numSamples - 1));
        if (lastNoteValue > 0)                                                                      // [13]
        {
            
            midi.addEvent(juce::MidiMessage::noteOff(1, lastNoteValue), offset);
            lastNoteValue = -1;
           

            

         
          
        }

        if (notes.size() > 0)                                                                       // [14]
        {
          
            if (ascOn == true) {

                currentNote = (currentNote + 1) % notes.size();
                lastNoteValue = notes[currentNote];
                midi.addEvent(juce::MidiMessage::noteOn(1, lastNoteValue, (juce::uint8) 127), offset);
            }
            if (ascDur == true) {

                currentNote = (currentNote + 1) % notes.size();
                lastNoteValue = notes[currentNote]; 
                midi.addEvent(juce::MidiMessage::noteOn(1, lastNoteValue, (juce::uint8) 127), offset);
            }
          
           else if (descOn == true)
           {
               currentNote = (currentNote - 1) % notes.size();

               if (currentNote == -1) {
                   currentNote = notes.size() - 1;
               }
               lastNoteValue = notes[currentNote];
               midi.addEvent(juce::MidiMessage::noteOn(1, lastNoteValue, (juce::uint8) 127), offset);
           }
            if (outInOn == true) {
                if (currentNote == -1) {
                    currentNote = 0;
              
                    lastNoteValue = notes[currentNote];
                    
                    midi.addEvent(juce::MidiMessage::noteOn(1, lastNoteValue, (juce::uint8) 127), offset);
                }
                else if (currentNote == 0) {
                    currentNote = notes.size() -1;
                    lastNoteValue = notes[currentNote];
                    midi.addEvent(juce::MidiMessage::noteOn(1, lastNoteValue, (juce::uint8) 127), offset);
                }
                else if (currentNote == notes.size() -1) {
                    
                    currentNote = 1 ;
                    lastNoteValue = notes[currentNote];
                    customIndex++;
                    midi.addEvent(juce::MidiMessage::noteOn(1, lastNoteValue, (juce::uint8) 127), offset);
                }
                else {
                    currentNote = 2;
                    lastNoteValue = notes[currentNote];
                    
                    currentNote = -1;
                    midi.addEvent(juce::MidiMessage::noteOn(1, lastNoteValue, (juce::uint8) 127), offset);

                }
                              
            }
            if (sweeping == true) {
                if (sweepUp == true) {
                    currentNote = (currentNote + 1) % notes.size();
                    lastNoteValue = notes[currentNote];
                    midi.addEvent(juce::MidiMessage::noteOn(1, lastNoteValue, (juce::uint8) 127), offset);
                    if (currentNote == notes.size() - 1) {
                        sweepUp = false;
                    }
                }
                else if (sweepUp == false) {
                    currentNote = (currentNote - 1) % notes.size();

                    if (currentNote == -1) {
                        currentNote = 0;
                        sweepUp = true;
                    }
                    lastNoteValue = notes[currentNote];
                    midi.addEvent(juce::MidiMessage::noteOn(1, lastNoteValue, (juce::uint8) 127), offset);
                }
             
                

                
            }
          
                DBG(noteDuration);
            
        }

    }

    time = (time + numSamples) % noteDuration;                                                      // [15]
}

//==============================================================================
bool A1StarterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* A1StarterAudioProcessor::createEditor()
{
    return new A1StarterAudioProcessorEditor(*this);
}

//==============================================================================
void A1StarterAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void A1StarterAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new A1StarterAudioProcessor();
}

