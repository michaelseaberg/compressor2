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
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    myVolume = new MeterComponent(p.getVolumeLevel());
    //myVolume->centreWithSize(100, 240);
    myVolume->setCentrePosition(200, 150);
    addAndMakeVisible(myVolume);
    
    //Use to set background of whole plugin
    File f = File("/Users/michaelseaberg/Documents/development/plugins/compressor2/Source/gkbluepropic.png");
    myBackgroundImage = ImageFileFormat::loadFrom(f);
}

Compressor2AudioProcessorEditor::~Compressor2AudioProcessorEditor()
{
}

//==============================================================================
void Compressor2AudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.drawImageAt(myBackgroundImage, 0, 0);

}

void Compressor2AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
