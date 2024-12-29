#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"


class LowPassFilterAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    LowPassFilterAudioProcessorEditor (LowPassFilterAudioProcessor&);
    ~LowPassFilterAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider frequencySlider;
    juce::Slider qKnob;
    juce::Label frequencyLabel;
    juce::Label qLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> frequencySliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> qKnobAttachment;
    LowPassFilterAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LowPassFilterAudioProcessorEditor)
};
