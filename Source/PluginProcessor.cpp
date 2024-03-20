/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Juce_plugin_distortionAudioProcessor::Juce_plugin_distortionAudioProcessor() : 
#ifndef JucePlugin_PreferredChannelConfigurations
     AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    _parameters (*this, nullptr, juce::Identifier("WatanabeDistotion"),
        {
            // define parameters.
            // * volume slider: 0.0 ~ 1.5 => -100dB ~  7dB
            // * gain slider:   1.0 ~ 2.0 =>    0dB ~ 12dB
            std::make_unique<juce::AudioParameterFloat>(getParameterID(MasterBypass), getParameterName(MasterBypass), 0.0f, 1.0f, 0.0f),
            std::make_unique<juce::AudioParameterFloat>(getParameterID(InputVolume),  getParameterName(InputVolume),  0.0f, 1.5f, 1.0f),
            std::make_unique<juce::AudioParameterFloat>(getParameterID(Gain),         getParameterName(Gain),         1.0f, 2.0f, 1.0f),
            std::make_unique<juce::AudioParameterFloat>(getParameterID(OutputVolume), getParameterName(OutputVolume), 0.0f, 1.5f, 1.0f),
            std::make_unique<juce::AudioParameterFloat>(getParameterID(Special),      getParameterName(Special),      0.0f, 1.0f, 0.0f),
        })
{ 
    // set default values.
    _masterBypassParameter = _parameters.getRawParameterValue(getParameterID(MasterBypass));
    _inputVolumeParameter  = _parameters.getRawParameterValue(getParameterID(InputVolume));
    _gainParameter         = _parameters.getRawParameterValue(getParameterID(Gain));
    _outputVolumeParameter = _parameters.getRawParameterValue(getParameterID(OutputVolume));
    _specialParameter      = _parameters.getRawParameterValue(getParameterID(Special));
}

Juce_plugin_distortionAudioProcessor::~Juce_plugin_distortionAudioProcessor()
{
}

//==============================================================================
const juce::String Juce_plugin_distortionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Juce_plugin_distortionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Juce_plugin_distortionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Juce_plugin_distortionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Juce_plugin_distortionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Juce_plugin_distortionAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Juce_plugin_distortionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Juce_plugin_distortionAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Juce_plugin_distortionAudioProcessor::getProgramName (int index)
{
    return {};
}

void Juce_plugin_distortionAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Juce_plugin_distortionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Juce_plugin_distortionAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Juce_plugin_distortionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Juce_plugin_distortionAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // get IN/OUT chennels.
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // clear buffer.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // check bypass.
    if (getParameter(MasterBypass) == 1.0f)
    {
        return;
    }

    // apply input volume.
    buffer.applyGain(pow(getParameter(InputVolume), 2));

    // gain to threshold.
    auto gainDecibel = juce::Decibels::gainToDecibels(pow(getParameter(Gain), 2));
    auto threshold = juce::Decibels::decibelsToGain(gainDecibel * -1.0);

    // apply distortion.
    for (auto channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        for (auto buffNum = 0; buffNum < buffer.getNumSamples(); buffNum++)
        {
            // special effect.
            if (getParameter(Special) == 1.0f)
            {
                channelData[buffNum] *= (gainDecibel/2.0);
                channelData[buffNum] = tanh(5.0 * channelData[buffNum] / 2);
                continue;
            }

            // clipping by threshold.
            if (channelData[buffNum] >= threshold)
            {
                channelData[buffNum] = threshold;
            }
            else if (channelData[buffNum] <= -threshold)
            {
                channelData[buffNum] = -threshold;
            }
            channelData[buffNum] *= (1 / threshold);
        }
    }

    // apply output volume.
    buffer.applyGain(pow(getParameter(OutputVolume), 2) * 2.0f);
}

//==============================================================================
bool Juce_plugin_distortionAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* Juce_plugin_distortionAudioProcessor::createEditor()
{
    return new Juce_plugin_distortionAudioProcessorEditor (*this, _parameters);
}

//==============================================================================
void Juce_plugin_distortionAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // load parameter values.
    auto state = _parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void Juce_plugin_distortionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // save parameter values.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(_parameters.state.getType()))
            _parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Juce_plugin_distortionAudioProcessor();
}

int Juce_plugin_distortionAudioProcessor::getNumParameters()
{
    return TotalParameterNum;
}


float Juce_plugin_distortionAudioProcessor::getParameter(int index)
{
    switch (index)
    {
    case MasterBypass:
        return (float)*_masterBypassParameter;
    case InputVolume:
        return (float)*_inputVolumeParameter;
    case Gain:
        return (float)*_gainParameter;
    case OutputVolume:
        return (float)*_outputVolumeParameter;
    case Special:
        return (float)*_specialParameter;
    default:
        return -1.0f;
    }
}

juce::String Juce_plugin_distortionAudioProcessor::getParameterID(int index)
{
    switch (index)
    {
    case MasterBypass:
        return std::to_string(MasterBypass);
    case InputVolume:
        return std::to_string(InputVolume);
    case Gain:
        return std::to_string(Gain);
    case OutputVolume:
        return std::to_string(OutputVolume);
    case Special:
        return std::to_string(Special);
    default:
        return "";
    }
}

const juce::String Juce_plugin_distortionAudioProcessor::getParameterName(int index)
{
    switch (index)
    {
    case MasterBypass:
        return "BYPASS";
    case InputVolume:
        return "In";
    case Gain:
        return "Gain";
    case OutputVolume:
        return "Out";
    case Special:
        return "Special";
    default:
        return "";
    }
}

const juce::String Juce_plugin_distortionAudioProcessor::getParameterText(int index)
{
    switch (index)
    {
    case MasterBypass:
    case Special:
        return getParameterName(index);
    case InputVolume:
    case OutputVolume:
        return getParameterName(index) + "\n" + juce::String(juce::Decibels::gainToDecibels(pow(getParameter(index), 2)), 1) + "\ndB";
    case Gain:
        return getParameterName(index) + "\n" + juce::String(juce::Decibels::gainToDecibels(pow(getParameter(index), 2)), 1) + " dB";
    default:
        return "";
    }
}
