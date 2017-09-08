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
    addParameter (drive = new AudioParameterFloat ("drive", // parameterID
                                                   "Drive", // parameter name
                                                   0,   // mininum value
                                                   20,   // maximum value
                                                   0)); // default value
    addParameter (tone = new AudioParameterFloat("tone","Tone",0,2,0));
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

static float tone0Function(float inputSample) {
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
static float tone1Function(float inputSample) {
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
static float tone2Function(float inputSample) {
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
    
    //Setup post processing filter
    auto channels = static_cast<uint32> (jmin (getMainBusNumInputChannels(), getMainBusNumOutputChannels()));
    globalSpec.operator=({sampleRate, static_cast<uint32> (samplesPerBlock), channels});
    //globalSpec.operator=({sampleRate*4, static_cast<uint32> (samplesPerBlock*4), channels});
    
    
    
    
    //---SETUP DRIVE (tone) SPECS--//
    
    //input gain
    auto& gainUp = toneStage.get<0>();
    gainUp.setGainDecibels (drive->get());//refactor to use drive control
    
    //anti-imaging filter
    auto& antiImagingFilter = toneStage.get<1>();
    antiImagingFilter.state = FilterDesign<float>::designFIRLowpassWindowMethod (20000.0f, globalSpec.sampleRate, 21, WindowingFunction<float>::blackman);
    
    //tone selected waveshaper curve
    auto& wavShaper = toneStage.get<2>();
    switch ((int) tone->get()) {
        case 0:
        {
            wavShaper.functionToUse = tone0Function;
            break;
        }
        case 1:
        {
            wavShaper.functionToUse = tone1Function;
            break;
        }
        case 2:
        {
            wavShaper.functionToUse = tone2Function;
            break;
        }
        default:
        {break;}
    }
    
    //anti-aliasing filter
    auto& antiAliasingFilter = toneStage.get<3>();
    antiAliasingFilter.state = FilterDesign<float>::designFIRLowpassWindowMethod (20000.0f, globalSpec.sampleRate, 21, WindowingFunction<float>::blackman);
    
    //output gain
    auto& gainDown = toneStage.get<4>();
    gainDown.setGainDecibels (-(drive->get()));//refactor to use drive control
    
    //Process buffer through tone distortion before compressing
    toneStage.prepare (globalSpec);
    
}

void Compressor2AudioProcessor::reset()
{
    toneStage.reset();
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
    
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    
    
    //---DUPLICATE DRIVE SPECS FROM prepareToPlay() for real-time modification--//
    
    //input gain
    auto& gainUp = toneStage.get<0>();
    gainUp.setGainDecibels (drive->get());//refactor to use drive control
    
    //tone selected waveshaper curve
    auto& wavShaper = toneStage.get<2>();
    switch ((int) tone->get()) {
        case 0:
        {
            wavShaper.functionToUse = tone0Function;
            break;
        }
        case 1:
        {
            wavShaper.functionToUse = tone1Function;
            break;
        }
        case 2:
        {
            wavShaper.functionToUse = tone2Function;
            break;
        }
        default:
        {break;}
    }
    
    //output gain
    auto& gainDown = toneStage.get<4>();
    gainDown.setGainDecibels (-(drive->get()));//refactor to use drive control
    

    
    
    
    //Initialize buffer for oversampling
    AudioSampleBuffer distortionBuffer = AudioSampleBuffer();
    distortionBuffer.setSize(totalNumInputChannels, buffer.getNumSamples()*4);
    
    //Upsample for better resolution
//    for(int channel=0; channel<totalNumInputChannels;channel++){
//        float* tempWrite = distortionBuffer.getWritePointer(channel);
//        const float* tempRead = buffer.getReadPointer(channel);
//        for(int sample=0; sample<buffer.getNumSamples(); sample++){
//            tempWrite[sample*4] = tempRead[sample];
//        }
//        
//    }
    
    AudioBlock<float> block (buffer);
    toneStage.process((const ProcessContextReplacing<float>&) block);

    //Decimate for Compression
//    for(int channel=0; channel<totalNumInputChannels;channel++){
//        float* tempWrite = buffer.getWritePointer(channel);
//        const float* tempRead = distortionBuffer.getReadPointer(channel);
//        for(int sample=0; sample<buffer.getNumSamples(); sample++){
//            tempWrite[sample] = tempRead[sample*4];
//        }
//        
//    }
    
    
    
    
    
    //Run resultant sound through compression
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);
        //float* channgeData = block.getChannelPointer(channel);
        
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
