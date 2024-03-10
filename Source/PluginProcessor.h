/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class Juce_plugin_distortionAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    Juce_plugin_distortionAudioProcessor();
    ~Juce_plugin_distortionAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // �p�����[�^��`
    enum Parameters
    {
        MasterBypass = 0,  // �o�C�p�X��ON/OFF�؂�ւ�
        InputVolume,       // ���̓{�����[������
        Gain,              // �c�ݗ�(�N���b�s���O臒l)
        OutputVolume,      // �o�̓{�����[������
        Special,           // �X�y�V����
        TotalParameterNum, // �p�����[�^�̍��v��
    };

    // �p�����[�^���ڐ�
    int getNumParameters() override;

    // �p�����[�^�̒l
    float getParameter(int index) override;

    // �p�����[�^��ID
    juce::String getParameterID(int index) override;

    // �p�����[�^�̕\����
    const juce::String getParameterName(int index) override;

    // �p�����[�^�̕\�����e
    const juce::String getParameterText(int index) override;

private:
    juce::AudioProcessorValueTreeState _parameters;
    std::atomic<float>* _masterBypassParameter = nullptr;
    std::atomic<float>* _inputVolumeParameter = nullptr;
    std::atomic<float>* _gainParameter = nullptr;
    std::atomic<float>* _outputVolumeParameter = nullptr;
    std::atomic<float>* _specialParameter = nullptr;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Juce_plugin_distortionAudioProcessor)
};
