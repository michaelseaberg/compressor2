/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
Compressor2AudioProcessor::Compressor2AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    currentLevel = new Value(var(0));
    
}

Compressor2AudioProcessor::~Compressor2AudioProcessor()
{
}

//==============================================================================
const String Compressor2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Compressor2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Compressor2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double Compressor2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Compressor2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Compressor2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Compressor2AudioProcessor::setCurrentProgram (int index)
{
}

const String Compressor2AudioProcessor::getProgramName (int index)
{
    return {};
}

void Compressor2AudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Compressor2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
}

void Compressor2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Compressor2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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


float linTodB(float& sample){
    float temp = fabsf(sample);
    if(temp < 0.000001)
        return -120;
    else
        return 20*log10(fabsf(sample));
}



void Compressor2AudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);
        float bufferMaxValue = -121;
        float bufferTempValue = -121;
        for(int i=0; i<buffer.getNumSamples(); i++){
            if((bufferTempValue = linTodB(channelData[i])) > bufferMaxValue)
                bufferMaxValue = bufferTempValue;
        }
        currentLevel->setValue(bufferMaxValue);

    }
}

Value* Compressor2AudioProcessor::getVolumeLevel(){
    return currentLevel;
}

//==============================================================================
bool Compressor2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Compressor2AudioProcessor::createEditor()
{
    return new Compressor2AudioProcessorEditor (*this);
}

//==============================================================================
void Compressor2AudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Compressor2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Compressor2AudioProcessor();
}
