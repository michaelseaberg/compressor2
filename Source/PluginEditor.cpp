/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
Compressor2AudioProcessorEditor::Compressor2AudioProcessorEditor (Compressor2AudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    //set Editor Size
    setSize (width, height);
    
    //Use to set background of whole plugin
    File f = File("/Users/michaelseaberg/Documents/development/plugins/compressor2/Source/gkbluepropic.png");
    myBackgroundImage = ImageFileFormat::loadFrom(f);
    
    //Import processor parameters for automatic slider creation.
    const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();
    for (int i = 0; i < params.size(); ++i)
    {
        if (const AudioProcessorParameterWithID* parameter = dynamic_cast<AudioProcessorParameterWithID*> (params[i]))
        {
            createControl(parameter,i);
            
        }
    }
    
    //Create reduction meter
    myReductionLevel = new MeterComponent(p.getVolumeLevel(),80,208,20);
    myReductionLevel->setBounds((width/2)+columnSpacing,parameterOffset,meterWidth,meterHeight);
    addAndMakeVisible(myReductionLevel);
    
    //Create output meter
    //TODO:IMPLEMENT OUTPUT
    myOutputLevel = new MeterComponent(p.getVolumeLevel(),80,208,20);
    myOutputLevel->setBounds((width/2)+columnSpacing,parameterOffset+meterHeight+parameterPadding,meterWidth,meterHeight);
    addAndMakeVisible(myOutputLevel);
    
    
}

Compressor2AudioProcessorEditor::~Compressor2AudioProcessorEditor()
{
}

//==============================================================================
void Compressor2AudioProcessorEditor::paint (Graphics& g)
{
    g.drawImageAt(myBackgroundImage, 0, 0);
}

void Compressor2AudioProcessorEditor::resized()
{
    //plugin currently cannot be resized
}

void Compressor2AudioProcessorEditor::createControl(const AudioProcessorParameterWithID* parameter, int parameterNumber){
    
    //set size variables, can be customized per parameter in switch statement
    int parameterX = parameterOffset;
    int parameterY = parameterOffset+(parameterNumber*(parameterSize+labelHeight+parameterPadding));
    int parameterWidth = parameterSize;
    int parameterHeight = parameterSize;
    
    //generic initialization
    Slider* newSlider;
    AudioParameterFloat* floatParam = (AudioParameterFloat*) parameter;
    parameterSliders.add (newSlider = new Slider (parameter->name));
    newSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    newSlider->setTextBoxStyle(Slider::NoTextBox, true, 100,0);
    newSlider->setPopupDisplayEnabled (true, this);
    newSlider->setValue (*floatParam);
    newSlider->setDoubleClickReturnValue(true, parameter->getDefaultValue());
    
    switch (parameterNumber) {
        case 0:
        {
           //ratio parameter
            newSlider->setRange (floatParam->range.start, floatParam->range.end,2);
            break;
        }
        case 1:
        {
            //threshold parameter
            newSlider->setRange (floatParam->range.start, floatParam->range.end,1);
            break;
        }
        case 2:
        {
            //attack parameter
            newSlider->setRange (floatParam->range.start, floatParam->range.end,1);
            newSlider->setSkewFactor(0.7);
            break;
        }
        case 3:
        {
            //release parameter
            newSlider->setRange (floatParam->range.start, floatParam->range.end,1);
            newSlider->setSkewFactor(0.7);
            break;
        }
        case 4:
        {
            //tone parameter
            newSlider->setSliderStyle(Slider::LinearHorizontal);
            newSlider->setRange(floatParam->range.start,floatParam->range.end,1);
            parameterX = parameterX-(toneSliderOffset/2);
            parameterWidth = parameterWidth+toneSliderOffset;
            parameterHeight = parameterHeight-toneSliderOffset;
            break;
        }
        case 5:
        {
            //makeupGain parameter
            newSlider->setRange (floatParam->range.start, floatParam->range.end,1);
            parameterX = (width/2)+columnSpacing;
            parameterY = parameterY-(parameterSize+labelHeight+parameterPadding)-toneSliderOffset;
            break;
        }
        default:
            break;
    }
    
    newSlider->setBounds(parameterX, parameterY, parameterWidth, parameterHeight);
    newSlider->addListener (this);
    addAndMakeVisible (newSlider);
    
    Label* sliderLabel;
    sliderLabel = new Label(parameter->paramID, parameter->name);
    sliderLabel->setBounds(labelOffset+parameterX-parameterOffset,parameterY+parameterHeight,labelWidth, labelHeight);
    sliderLabel->setJustificationType(Justification::centred);
    addAndMakeVisible(sliderLabel);
    
}

void Compressor2AudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    if (AudioParameterFloat* param = getParameterForSlider (slider))
        *param = (float) slider->getValue();
}

void Compressor2AudioProcessorEditor::sliderDragStarted (Slider* slider)
{
    if (AudioParameterFloat* param = getParameterForSlider (slider))
        param->beginChangeGesture();
}

void Compressor2AudioProcessorEditor::sliderDragEnded (Slider* slider)
{
    if (AudioParameterFloat* param = getParameterForSlider (slider))
        param->endChangeGesture();
}

AudioParameterFloat* Compressor2AudioProcessorEditor::getParameterForSlider (Slider* slider)
{
    const OwnedArray<AudioProcessorParameter>& params = getAudioProcessor()->getParameters();
    return dynamic_cast<AudioParameterFloat*> (params[parameterSliders.indexOf (slider)]);
}
