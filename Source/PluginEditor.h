/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "MeterComponent.h"


//==============================================================================
/**
*/
class Compressor2AudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Compressor2AudioProcessorEditor (Compressor2AudioProcessor&);
    ~Compressor2AudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    Image myBackgroundImage;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Compressor2AudioProcessor& processor;
    MeterComponent* myVolume;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Compressor2AudioProcessorEditor)
};
