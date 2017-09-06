/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//Constructors
//==============================================================================
Compressor2AudioProcessor::Compressor2AudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                       .withOutput ("Output", AudioChannelSet::stereo(), true))

{
    
    //Create Parameters
    addParameter (ratio = new AudioParameterFloat ("ratio", // parameterID
                                                   "Ratio", // parameter name
                                                   2,   // mininum value
                                                   8,   // maximum value
                                                   2)); // default value
    addParameter (threshold = new AudioParameterFloat ("threshold", // parameterID
                                                       "Threshold", // parameter name
                                                       -60,   // mininum value
                                                       0,   // maximum value
                                                       0)); // default value
    addParameter (attackTime = new AudioParameterFloat ("attack", // parameterID
                                                        "Attack Time (ms)", // parameter name
                                                        5,   // mininum value
                                                        100,   // maximum value
                                                        5)); // default value
    addParameter (releaseTime = new AudioParameterFloat ("release", // parameterID
                                                         "Release Time (ms)", // parameter name
                                                         10,   // mininum value
                                                         1000,   // maximum value
                                                         10)); // default value
    addParameter (tone = new AudioParameterFloat("tone","Tone",0,2,0));
    addParameter (makeupGain = new AudioParameterFloat ("gain", // parameterID
                                                        "Makeup Gain", // parameter name
                                                        -10,   // mininum value
                                                        30,   // maximum value
                                                        0)); // default value
    

    //metering initialization
    currentLevel = new OwnedArray<Value>;
    for(int i=0; i<getTotalNumInputChannels(); i++){
        currentLevel->add(new Value(var(0)));
    }
    
    
}

Compressor2AudioProcessor::~Compressor2AudioProcessor()
{

}

//Class Methods
//==============================================================================
const String Compressor2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Compressor2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Compressor2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double Compressor2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Compressor2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Compressor2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Compressor2AudioProcessor::setCurrentProgram (int index)
{
}

const String Compressor2AudioProcessor::getProgramName (int index)
{
    return {};
}

void Compressor2AudioProcessor::changeProgramName (int index, const String& newName)
{
}

//Helper Methods
//==============================================================================
float linTodB2(float& sample){
    float temp = fabsf(sample);
    if(temp < 0.000001)
        return -120;
    else
        return 20*log10(fabsf(sample));
}

float computeConstant(float time, double sampleRate){
    return exp(-1/((time*.001)*sampleRate));
}

void linTodB(float& sample){
    float temp = fabsf(sample);
    if(temp < 0.000001)
        sample = -120;
    else
        sample = 20*log10(fabsf(sample));
}

void dBToLin(float& sample){
    sample = powf(10,(sample/20));
}

//why can I not access threshold or values from inside helper methods-private objects
float computeGainCorrection(float sample, float thresholdValue, float ratioValue){
    if(sample <= thresholdValue)
        return sample;
    else
        return thresholdValue+((sample-thresholdValue)/ratioValue);
}

float computeLevelDetection(float sample, float previousSample, float attackConstant, float releaseConstant){
    if (sample>previousSample)
        return (attackConstant*previousSample)+((1-attackConstant)*sample);
    else
        return (releaseConstant*previousSample)+((1-releaseConstant)*sample);
}

//Input Gain Stage - Distortion
//==============================================================================
float Compressor2AudioProcessor::gainStage(float inputSample){
    
    //process through distortion
    float outputSample;
    float threshold1 = 1.0f/3.0f;
    float threshold2 = 2.0f/3.0f;
    
    if(inputSample > threshold2)
        outputSample = 1.0f;
    else if(inputSample > threshold1)
        outputSample = (3.0f - (2.0f - 3.0f*inputSample) *
                        (2.0f - 3.0f*inputSample))/3.0f;
    else if(inputSample < -threshold2)
        outputSample = -1.0f;
    else if(inputSample < -threshold1)
        outputSample = -(3.0f - (2.0f + 3.0f*inputSample) *
                         (2.0f + 3.0f*inputSample))/3.0f;
    else
        outputSample = 2.0f* inputSample;
    
    return outputSample;
}



//Processing Methods
//==============================================================================
void Compressor2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSample = 0;
    previousSamples[0] = 0;
    previousSamples[1] = 0;
    gainComputerOut = 0;
    levelDetectorOut = 0;
    controlSignal = 0;
    computedSample = 0;
    attackConstant = 0;
    releaseConstant = 0;
    
    
    
    //Setup lowpass filter 
    auto channels = static_cast<uint32> (jmin (getMainBusNumInputChannels(), getMainBusNumOutputChannels()));
    dsp::ProcessSpec spec { sampleRate, static_cast<uint32> (samplesPerBlock), channels};
    fir.state = dsp::FilterDesign<float>::designFIRLowpassWindowMethod (20000.0f, spec.sampleRate, 21, dsp::WindowingFunction<float>::blackman);
    fir.prepare(spec);
    
}

void Compressor2AudioProcessor::reset()
{
    fir.reset();
}

void Compressor2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Compressor2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Compressor2AudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    
    attackConstant = computeConstant(attackTime->get(),this->getSampleRate());
    releaseConstant = computeConstant(releaseTime->get(),this->getSampleRate());
    //inputGainLinear = powf(10.0f, ((inputGain->get())/20.0f));
    float currentThreshold = threshold->get();
    float currentRatio = ratio->get();
    float currentGain = makeupGain->get();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    switch ((int) tone->get()) {
        case 0:
        {
            for (int channel = 0; channel < totalNumInputChannels; ++channel)
            {
                float* channelData = buffer.getWritePointer (channel);
                
                for(int sample=0; sample < buffer.getNumSamples(); ++sample){
                    currentSample = channelData[sample];
                    //take channelData and treat as "data"-one sample. Increment the pointer when time to process next sample
                    controlSignal = currentSample;
                    //linear to DB conversion
                    linTodB(controlSignal);
                    //gain computing
                    gainComputerOut = computeGainCorrection(controlSignal,currentThreshold,currentRatio);
                    controlSignal = controlSignal-gainComputerOut;
                    //dBToLin(controlSignal);
                    
                    //level detection
                    levelDetectorOut = computeLevelDetection(controlSignal,previousSamples[channel],attackConstant,releaseConstant);
                    previousSamples[channel] = levelDetectorOut;
                    //send control signal reduction to meter
                    currentLevel->operator[](channel)->setValue(levelDetectorOut);
                    //Compute control signal from applied gain and level detector signal
                    controlSignal = (currentGain)-levelDetectorOut;
                    //db to linear conversion
                    dBToLin(controlSignal);
                    //set sample to equal new sample
                    computedSample = currentSample*controlSignal;
                    channelData[sample] = computedSample;
                }
            }
            break;
        }
            
        case 1:
        {
            AudioSampleBuffer resampleBuffer;
            resampleBuffer.setSize (getTotalNumInputChannels(), buffer.getNumSamples() * 4);
            for (int channel = 0; channel < totalNumInputChannels; ++channel)
            {
                LagrangeInterpolator* overSampler = new LagrangeInterpolator();
                float* resampleWriter = resampleBuffer.getWritePointer(channel);
                float* channelData = buffer.getWritePointer (channel);
                
                overSampler->process(0.25, channelData, resampleWriter, buffer.getNumSamples()*4);
                
                for(int sample=0; sample < buffer.getNumSamples(); ++sample){
                    channelData[sample] = gainStage(channelData[sample]);
                    
                }
            }
            break;
        }
            
        case 2:
            break;
        default:
            break;
    }
    
    //Post Processing Low Pass Filter
    dsp::AudioBlock<float> block (buffer);
    //fir.process (dsp::ProcessContextReplacing<float> (block));
}

//Getters and Setters
//==============================================================================
OwnedArray<Value>* Compressor2AudioProcessor::getVolumeLevel(){
    //initialize metering object to zero
    return currentLevel;
}

//Editor Methods
//==============================================================================
bool Compressor2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Compressor2AudioProcessor::createEditor()
{
    return new Compressor2AudioProcessorEditor (*this);
}

//State Methods
//==============================================================================
void Compressor2AudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Compressor2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================


// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Compressor2AudioProcessor();
}
