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
                        private Value::Listener
{
public:
    MeterComponent(Value* levelToShow,int channels, int xLoc, int yLoc, int width, int height, int dBrange)
    {
        myLevel = levelToShow;
        myNumChannels = channels;
        myX = xLoc;
        myY = yLoc;
        myWidth = width;
        myHeight = height;
        myRange = dBrange;
        this->setSize(myWidth, myHeight);
        myLevel->addListener(this);

    }

    ~MeterComponent()
    {
    }
    
    void paint (Graphics& g) override
    {
        g.setFont (14.0f);
        drawMeters(g);
        
    }

    void resized() override
    {

    }
    
    void valueChanged(Value &value) override{
        repaint();
    }

private:
    Value* myLevel;
    int myNumChannels;
    int myX;
    int myY;
    int myWidth;
    int myHeight;
    int myRange;
    
    void drawMeters(Graphics& graphics){
        graphics.setColour (Colour(132, 132, 132));
        graphics.fillRect(myX, myY, myWidth, myHeight);
        graphics.setColour (Colour(137,244,66));
        
        int loudness;
        for(int i=0; i<myNumChannels; i++){
            loudness=(int) myLevel[i].getValue();
            //graphics.drawText("Level: "+String(loudness)+" dB",2,222,96,20,Justification(36),true);
            if(loudness>(-myRange))
                graphics.fillRect(((myX+7)+(i)*(((myWidth-14-(3*(myNumChannels-1)))/myNumChannels)+3)), myY+myHeight, ((myWidth-14-(3*(myNumChannels-1)))/myNumChannels), -myHeight-(loudness*(myHeight/myRange)));
        }
    }
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeterComponent)
};
