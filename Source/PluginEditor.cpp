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
    
    //Create gain meter
    myVolume = new MeterComponent(p.getVolumeLevel(),0,0,100,180,20);
    myVolume->setCentrePosition(200, 100);
    addAndMakeVisible(myVolume);
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
    
    if(parameter->paramID == "r"){
        //choice parameter-want buttons
        AudioParameterChoice* choiceParam = (AudioParameterChoice*) parameter;
        for(int i=0; i< choiceParam->choices.size(); i++){
            Button* newButton;
            ratioButtons.add(newButton = new TextButton (choiceParam->choices[i]));
            //newButton->setButtonText(choiceParam->name);
            newButton->setClickingTogglesState(true);
            newButton->setRadioGroupId(1);
            newButton->addListener(this);
            if(i==0){
                newButton->setConnectedEdges(2);
                Label* ratioLabel;
                ratioLabel = new Label(String("ratioLabel"), String("Ratio: "));
                //ratioLabel->setText(String("Ratio Setting"), dontSendNotification);
                ratioLabel->setBounds(0, 330, 50, 30);
                ratioLabel->attachToComponent(newButton, true);
                addAndMakeVisible(ratioLabel);
            }
            else if (i==3)
                newButton->setConnectedEdges(1);
            else
                newButton->setConnectedEdges(3);
            newButton->setBounds(70+(i*112.5), 331, 112.5, 40);
            addAndMakeVisible(newButton);
        }
        
        
    }
    else{
        //float parameter-want knob
        AudioParameterFloat* floatParam = (AudioParameterFloat*) parameter;
        Slider* newSlider;
        parameterSliders.add (newSlider = new Slider (floatParam->name));
        newSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        newSlider->setRange (floatParam->range.start, floatParam->range.end,1);
        //TODO: Create new Slider Label that gives Parameter: Value Unit
        newSlider->setTextBoxStyle(Slider::NoTextBox, true, 100,0);
        
        
        //newSlider->setTextValueSuffix(" :"+ floatParam->name);
        newSlider->setPopupDisplayEnabled (true, this);
        newSlider->setValue (*floatParam);
        newSlider->addListener (this);
        newSlider->setBounds(30+(parameterNumber*100), 220, 100, 80);
        addAndMakeVisible (newSlider);
        
        Label* sliderLabel;
        sliderLabel = new Label(floatParam->paramID, floatParam->name);
        sliderLabel->setBounds(30+(parameterNumber*100), 301, 100, 29);
        sliderLabel->setJustificationType(Justification::centred);
        addAndMakeVisible(sliderLabel);
        
    }
    
}
void Compressor2AudioProcessorEditor::buttonClicked (Button* button){
    //listener callback for button click
    
    //button->setState(juce::ArrowButton::buttonDown);
    
    
}
void Compressor2AudioProcessorEditor::buttonStateChanged (Button* button){
    //if button is clicked and was not previously selected then pass new ratio value-ratio parameter is added 6th, array index 5
    const OwnedArray<AudioProcessorParameter>& params = getAudioProcessor()->getParameters();
    
    if(AudioParameterChoice* param = dynamic_cast<AudioParameterChoice*> (params[5]))
        if(button->getState() == juce::TextButton::buttonDown)
            *param = ratioButtons.indexOf(button);
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
