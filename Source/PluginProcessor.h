/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


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
    const float* currentSamples;
    

private:
    //==============================================================================
    //declare all parameters here
    AudioParameterFloat* makeupGain;
    AudioParameterFloat* threshold;
    AudioParameterFloat* attackTime;
    AudioParameterFloat* releaseTime;
    AudioParameterChoice* ratio;
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
    const String ratios[4] = {String("2"),String("4"),String("6"),String("8")};

    float gainStage(float inputSample);
    
    //metering
    OwnedArray<Value>* currentLevel;  //Pointer to value to be used as an array of values with set number of channels
    
    dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> lowPassFilter;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Compressor2AudioProcessor)
};
