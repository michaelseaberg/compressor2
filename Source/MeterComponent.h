/*
  ==============================================================================

    MeterComponent.h
    Created: 16 Aug 2017 5:20:38pm
    Author:  Michael Seaberg

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class MeterComponent    : public Component,
                            public Value::Listener
{
public:
    MeterComponent(Value* levelToShow)
    {
        myLevel = levelToShow;
        this->setSize(100, 240);
        myLevel->addListener(this);

    }

    ~MeterComponent()
    {
    }
    
    

    void paint (Graphics& g) override
    {
        //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the backgroun
        g.setColour (Colour(94,94,94));
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
        g.setFont (14.0f);
        drawMeters(20,20,60,200,60,2,1,g);
        
    }

    void resized() override
    {

    }
    
    void valueChanged(Value &value) override{
        repaint();
    }

private:
    Value* myLevel;
    
    void drawMeters(int x, int y, int width, int height, int range, int channels, int orientation, Graphics& graphics){
        graphics.setColour (Colour(132, 132, 132));
        graphics.fillRect(x, y, width, height);
        graphics.setColour (Colour(137,244,66));
        int loudness;
        for(int i=0; i<channels; i++){
            loudness=(int) myLevel->getValue();
            graphics.drawText("Level: "+String(loudness)+" dB",2,222,96,20,Justification(36),true);
            if(loudness>(-range))
                graphics.fillRect(((x+7)+(i)*(((width-14-(3*(channels-1)))/channels)+3)), y+height, ((width-14-(3*(channels-1)))/channels), -height-(loudness*(height/range)));
        }
    }
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeterComponent)
};
