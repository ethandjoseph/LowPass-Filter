#include "PluginProcessor.h"
#include "PluginEditor.h"

LowPassFilterAudioProcessorEditor::LowPassFilterAudioProcessorEditor (LowPassFilterAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    frequencySlider.setSliderStyle(juce::Slider::LinearHorizontal);
    frequencySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    qKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    qKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    
    frequencyLabel.setText("Frequency", juce::dontSendNotification);
    qLabel.setText("Resonance", juce::dontSendNotification);

    frequencyLabel.attachToComponent(&frequencySlider, false);
    qLabel.attachToComponent(&qKnob, false);

    addAndMakeVisible(frequencyLabel);
    addAndMakeVisible(qLabel);

    addAndMakeVisible(frequencySlider);
    addAndMakeVisible(qKnob);

    frequencySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FREQUENCY", frequencySlider);
    qKnobAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Q", qKnob);
    setSize (400, 120);
}

LowPassFilterAudioProcessorEditor::~LowPassFilterAudioProcessorEditor()
{
}

void LowPassFilterAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkslateblue);
}

void LowPassFilterAudioProcessorEditor::resized()
{
    const int padding = 10;
    const int labelHeight = 20;

    auto bounds = getLocalBounds().reduced(10);

    const int sliderWidth = bounds.getWidth() * 0.75f;
    const int knobWidth = bounds.getWidth() - sliderWidth - padding;

    auto sliderArea = bounds.removeFromLeft(sliderWidth);
    auto sliderLabelArea = sliderArea.removeFromTop(labelHeight);
    frequencyLabel.setBounds(sliderLabelArea);
    frequencySlider.setBounds(sliderArea);

    bounds.removeFromLeft(padding);

    auto knobArea = bounds.removeFromLeft(knobWidth);
    auto knobLabelArea = knobArea.removeFromTop(labelHeight);
    qLabel.setBounds(knobLabelArea);
    qKnob.setBounds(knobArea);
}