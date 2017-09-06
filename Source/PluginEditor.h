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
class Compressor2AudioProcessorEditor  : public AudioProcessorEditor,
                                        private Slider::Listener
{
public:
    Compressor2AudioProcessorEditor (Compressor2AudioProcessor&);
    ~Compressor2AudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    Compressor2AudioProcessor& processor;
    const int width = 270;
    const int height = 585;
    const int parameterOffset = 30;
    const int parameterSize = 80;
    const int labelWidth = 120;
    const int labelHeight = 29;
    const int labelOffset = 10;
    const int parameterPadding = 5;
    const int toneSliderOffset = 20;
    const int columnSpacing = 20;
    const int meterHeight = 208;
    const int meterWidth = 80;
    const int dBRange = 20;
    
    Image myBackgroundImage;
    
    MeterComponent* myReductionLevel;
    MeterComponent* myOutputLevel;
    
    
    void sliderValueChanged (Slider* slider) override;
    void sliderDragStarted (Slider* slider) override;
    void sliderDragEnded (Slider* slider) override;
    AudioParameterFloat* getParameterForSlider(Slider* slider);
    
    void createControl(const AudioProcessorParameterWithID* parameter, int parameterNumber);
    
    OwnedArray<Slider> parameterSliders;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Compressor2AudioProcessorEditor)
};
