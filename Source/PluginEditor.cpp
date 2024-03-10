/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Juce_plugin_distortionAudioProcessorEditor::Juce_plugin_distortionAudioProcessorEditor (Juce_plugin_distortionAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState (vts)
{
    setSize (420, 300);

    // init ui components.
    initSliderComponent(&_inputVolumeSlider, juce::Slider::LinearVertical);
    initSliderComponent(&_gainSlider, juce::Slider::RotaryHorizontalVerticalDrag);
    initSliderComponent(&_outputVolumeSlider, juce::Slider::LinearVertical);
    initLabelComponent(&_inputVolumeLabel, processor.getParameterText(Juce_plugin_distortionAudioProcessor::InputVolume));
    initLabelComponent(&_gainLabel, processor.getParameterText(Juce_plugin_distortionAudioProcessor::Gain));
    initLabelComponent(&_outputVolumeLabel, processor.getParameterText(Juce_plugin_distortionAudioProcessor::OutputVolume));
    initToggleButtonComponent(&_specialToggle, processor.getParameterText(Juce_plugin_distortionAudioProcessor::Special));

    // linking ui components and parameters.
    _inputVolumeSliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        valueTreeState,
        audioProcessor.getParameterID(Juce_plugin_distortionAudioProcessor::Parameters::InputVolume),
        _inputVolumeSlider));
    _gainSliderAttachment .reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        valueTreeState,
        audioProcessor.getParameterID(Juce_plugin_distortionAudioProcessor::Parameters::Gain),
        _gainSlider));
    _outputVolumeSliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        valueTreeState,
        audioProcessor.getParameterID(Juce_plugin_distortionAudioProcessor::Parameters::OutputVolume),
        _outputVolumeSlider));
    _specialToggleAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(
        valueTreeState,
        audioProcessor.getParameterID(Juce_plugin_distortionAudioProcessor::Parameters::Special),
        _specialToggle));

    // display window.
    addAndMakeVisible(&_inputVolumeSlider);
    addAndMakeVisible(&_gainSlider);
    addAndMakeVisible(&_outputVolumeSlider);
    addAndMakeVisible(&_inputVolumeLabel);
    addAndMakeVisible(&_gainLabel);
    addAndMakeVisible(&_outputVolumeLabel);
    addAndMakeVisible(&_specialToggle);

    // start timer monitoring.
    startTimer(30);
}

Juce_plugin_distortionAudioProcessorEditor::~Juce_plugin_distortionAudioProcessorEditor()
{
}

//==============================================================================
void Juce_plugin_distortionAudioProcessorEditor::paint (juce::Graphics& g)
{
    auto backgroundImage = juce::ImageCache::getFromMemory(BinaryData::bg_plugin_distortion_png, BinaryData::bg_plugin_distortion_pngSize);
    g.drawImageAt(backgroundImage, 0, 0);
}

void Juce_plugin_distortionAudioProcessorEditor::resized()
{
    auto contentHeight = 160;
    auto contentPosY = 48;
    auto labelHight = 60;
    auto labelPosY = contentHeight + contentPosY;

    _inputVolumeSlider
        .setBounds(300, contentPosY, 60, contentHeight);
    _gainSlider
        .setBounds(76, contentPosY, 140, contentHeight);
    _outputVolumeSlider
        .setBounds(348, contentPosY, 60, contentHeight);
    _inputVolumeLabel
        .setBounds(300, labelPosY, 60, labelHight);
    _gainLabel
        .setBounds(76, labelPosY, 140, labelHight);
    _outputVolumeLabel
        .setBounds(348, labelPosY, 60, labelHight);
    _specialToggle
        .setBounds(12, 8, 140, 30);
}

void Juce_plugin_distortionAudioProcessorEditor::initSliderComponent(juce::Slider* slider, juce::Slider::SliderStyle style)
{
    (*slider).setSliderStyle(style);
    (*slider).setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    (*slider).setColour(juce::Slider::backgroundColourId, juce::Colours::lightgrey);
    (*slider).setColour(juce::Slider::trackColourId, juce::Colours::darkgrey);
    (*slider).setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::lightgrey);
    (*slider).setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::darkgrey);
}

void Juce_plugin_distortionAudioProcessorEditor::initLabelComponent(juce::Label* label, juce::String text)
{
    (*label).setFont(juce::Font(16.0f, juce::Font::plain));
    (*label).setJustificationType(juce::Justification::centredTop);
    (*label).setEditable(false, false, false);
    (*label).setColour(juce::Label::textColourId, juce::Colours::white);
    (*label).setText(text, juce::dontSendNotification);
}

void Juce_plugin_distortionAudioProcessorEditor::initToggleButtonComponent(juce::ToggleButton* toggleButton, juce::String text)
{
    (*toggleButton).setButtonText(text);
    (*toggleButton).setColour(juce::ToggleButton::textColourId, juce::Colours::white);
}

void Juce_plugin_distortionAudioProcessorEditor::timerCallback()
{
    _inputVolumeSlider
        .setValue(processor.getParameter(Juce_plugin_distortionAudioProcessor::InputVolume), juce::dontSendNotification);
    _gainSlider
        .setValue(processor.getParameter(Juce_plugin_distortionAudioProcessor::Gain), juce::dontSendNotification);
    _outputVolumeSlider
        .setValue(processor.getParameter(Juce_plugin_distortionAudioProcessor::OutputVolume), juce::dontSendNotification);
    _specialToggle
        .setToggleState(processor.getParameter(Juce_plugin_distortionAudioProcessor::Special) == 1.0f, juce::dontSendNotification);

    _inputVolumeLabel
        .setText(processor.getParameterText(Juce_plugin_distortionAudioProcessor::InputVolume), juce::dontSendNotification);
    _gainLabel
        .setText(processor.getParameterText(Juce_plugin_distortionAudioProcessor::Gain), juce::dontSendNotification);
    _outputVolumeLabel
        .setText(processor.getParameterText(Juce_plugin_distortionAudioProcessor::OutputVolume), juce::dontSendNotification);
}
