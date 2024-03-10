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

    // パラメータ定義
    enum Parameters
    {
        MasterBypass = 0,  // バイパスのON/OFF切り替え
        InputVolume,       // 入力ボリューム調整
        Gain,              // 歪み量(クリッピング閾値)
        OutputVolume,      // 出力ボリューム調整
        Special,           // スペシャル
        TotalParameterNum, // パラメータの合計数
    };

    // パラメータ項目数
    int getNumParameters() override;

    // パラメータの値
    float getParameter(int index) override;

    // パラメータのID
    juce::String getParameterID(int index) override;

    // パラメータの表示名
    const juce::String getParameterName(int index) override;

    // パラメータの表示内容
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
