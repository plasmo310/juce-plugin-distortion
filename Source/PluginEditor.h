/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class Juce_plugin_distortionAudioProcessorEditor  : public juce::AudioProcessorEditor,
    private juce::Timer
{
public:
    Juce_plugin_distortionAudioProcessorEditor (Juce_plugin_distortionAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~Juce_plugin_distortionAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    Juce_plugin_distortionAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& valueTreeState;

    // UIコンポーネント
    juce::Slider _inputVolumeSlider;
    juce::Slider _gainSlider;
    juce::Slider _outputVolumeSlider;
    juce::Label _inputVolumeLabel;
    juce::Label _gainLabel;
    juce::Label _outputVolumeLabel;
    juce::ToggleButton _specialToggle;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> _inputVolumeSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> _gainSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> _outputVolumeSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> _specialToggleAttachment;

    // UIコンポーネント初期化処理
    void initSliderComponent(juce::Slider* slider, juce::Slider::SliderStyle style);
    void initLabelComponent(juce::Label* label, juce::String text);
    void initToggleButtonComponent(juce::ToggleButton* toggleButton, juce::String text);

    // タイマーによる変更監視：Processor->Editor
    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Juce_plugin_distortionAudioProcessorEditor)
};
