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
                                        private Slider::Listener,
                                            private Button::Listener
{
public:
    Compressor2AudioProcessorEditor (Compressor2AudioProcessor&);
    ~Compressor2AudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    Compressor2AudioProcessor& processor;
    const int width = 560;
    const int height = 390;
    
    Image myBackgroundImage;
    
    MeterComponent* myVolume;
    
    void sliderValueChanged (Slider* slider) override;
    void sliderDragStarted (Slider* slider) override;
    void sliderDragEnded (Slider* slider) override;
    void buttonClicked (Button* button) override;
    void buttonStateChanged (Button* button) override;
    AudioParameterFloat* getParameterForSlider(Slider* slider);
    
    void createControl(const AudioProcessorParameterWithID* parameter, int parameterNumber);
    void createWaveform();
    
    OwnedArray<Slider> parameterSliders;
    OwnedArray<Button> ratioButtons;

    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Compressor2AudioProcessorEditor)
};
