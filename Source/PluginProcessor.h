/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

using namespace dsp;


//==============================================================================
/**
*/
class Compressor2AudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Compressor2AudioProcessor();
    ~Compressor2AudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;
    void reset() override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    OwnedArray<Value>* getVolumeLevel();
    

private:
    //==============================================================================
    //declare all parameters here
    AudioParameterFloat* makeupGain;
    AudioParameterFloat* threshold;
    AudioParameterFloat* attackTime;
    AudioParameterFloat* releaseTime;
    AudioParameterFloat* ratio;
    AudioParameterFloat* tone;
    AudioParameterFloat* drive;
    //intermediate values
    float currentSample;
    float gainComputerOut;
    float levelDetectorOut;
    float controlSignal;
    float computedSample;
    //
    float attackConstant;
    float releaseConstant;
    float inputGainLinear;
    float previousSamples[2]; //need to be able to store previous samples for two channels //TODO:REFACTOR TO ACCOMODATE MORE THAN STEREO
    //const String tones[3] = {String("Off"),String("1"),String("2")};

    float gainStage(float inputSample);
    
    //metering
    OwnedArray<Value>* currentLevel;  //Pointer to value to be used as an array of values with set number of channels
    
    ProcessorChain<Gain<float>,ProcessorDuplicator<FIR::Filter<float>, FIR::Coefficients<float>>, WaveShaper<float>, ProcessorDuplicator<FIR::Filter<float>, FIR::Coefficients<float>>, Gain<float>> toneStage;
    
    ProcessSpec globalSpec;
    OwnedArray<LagrangeInterpolator>* resamplingInterpolators;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Compressor2AudioProcessor)
};
