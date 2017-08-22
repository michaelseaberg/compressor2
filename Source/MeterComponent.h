/*
 ==============================================================================
 
 MeterComponent.h
 Created: 16 Aug 2017 5:20:38pm
 Author:  Michael Seaberg
 
 Operates as a gain reduction meter, telling user how many dBs are being reduced from signal
 
 
 ///EXAMPLE VOLUME METERING CODE FOR EXTENSION INTO OTHER PLUGINS
 //        float bufferMaxValue = -121;
 //        float bufferTempValue = -121;
 //        for(int i=0; i<buffer.getNumSamples(); i++){
 //            if((bufferTempValue = linTodB2(channelData[i])) > bufferMaxValue)
 //                bufferMaxValue = bufferTempValue;
 //        }
 //currentLevel[channel].setValue(bufferMaxValue);
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
    MeterComponent(OwnedArray<Value>* levelToShow, int xLoc, int yLoc, int width, int height, int dBrange)
    {
        myLevel = levelToShow;
        myNumChannels = levelToShow->size();
        myX = xLoc;
        myY = yLoc;
        myWidth = width;
        myHeight = height;
        myRange = dBrange;
        this->setSize(myWidth, myHeight);
        for(int i=0;i<myNumChannels;i++){
            myLevel->operator[](i)->addListener(this);
        }
        
        
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
    OwnedArray<Value>* myLevel;
    int myNumChannels;
    int myX;
    int myY;
    int myWidth;
    int myHeight;
    int myRange;
    const int meterSpacing = 3;
    const int meterMargin = 7;
    
    void drawMeters(Graphics& graphics){
        graphics.setColour (Colour(132, 132, 132));
        graphics.fillRect(myX, myY, myWidth, myHeight);
        graphics.setColour (Colour(137,244,66));
        
        int loudness;
        for(int i=0; i<myNumChannels; i++){
            loudness=(int) myLevel->operator[](i)->getValue();
            graphics.drawText("Level: "+String(loudness)+" dB",2,60,96,20,Justification(36),true);
            if(loudness>(-myRange))
                graphics.fillRect(getMeterX(i), getMeterY(),getMeterWidth(),getMeterHeight(loudness));
            else
                graphics.fillRect(getMeterX(i), getMeterY(),getMeterWidth(),getMeterHeight(myHeight));
            
        }
    }
    
    int getMeterX(int channel){
        //start location+offset+determined width based on number of channels and set margins and spacing
        return ((myX+meterMargin)+(channel)*(getMeterWidth()+meterSpacing));
    }
    
    int getMeterY(){
        //left corner start location
        return myY;
    }
    
    int getMeterWidth(){
        //set width-margins and spacing over total number of channels to determine individual channel width
        return ((myWidth-(meterMargin*2)-(meterSpacing*(myNumChannels-1)))/myNumChannels);
    }
    
    int getMeterHeight(int value){
        //equal to proportion of input dB value to meters total dB range
        return (value*(myHeight/myRange));
    }
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeterComponent)
};
