#include "PluginProcessor.h"
#include "PluginEditor.h"

LowPassFilterAudioProcessor::LowPassFilterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    apvts(*this, nullptr, "Parameters", createParameterLayout()),
    lowPassFilter(juce::dsp::IIR::Coefficients<float>::makeLowPass(44100, 20000.0f, 0.1f))
#endif
{
}

LowPassFilterAudioProcessor::~LowPassFilterAudioProcessor()
{
}

//==============================================================================
const juce::String LowPassFilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LowPassFilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LowPassFilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LowPassFilterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LowPassFilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LowPassFilterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LowPassFilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LowPassFilterAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String LowPassFilterAudioProcessor::getProgramName (int index)
{
    return {};
}

void LowPassFilterAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void LowPassFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    lowPassFilter.prepare(spec);
    lowPassFilter.reset();
}

void LowPassFilterAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LowPassFilterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void LowPassFilterAudioProcessor::updateFilter()
{
    float frequencyValue = apvts.getRawParameterValue("FREQUENCY")->load();
    float qValue = apvts.getRawParameterValue("Q")->load();
    *lowPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(44100, frequencyValue, qValue);
}

// Process Block =================================================================================================/
void LowPassFilterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    juce::dsp::AudioBlock<float> block(buffer);
    updateFilter();
    lowPassFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
}
//================================================================================================================/

bool LowPassFilterAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* LowPassFilterAudioProcessor::createEditor()
{
    return new LowPassFilterAudioProcessorEditor (*this);
}

void LowPassFilterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
}

void LowPassFilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

juce::AudioProcessorValueTreeState::ParameterLayout LowPassFilterAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "FREQUENCY",               // Parameter ID
        "Cut off Frequency",       // Parameter name
        juce::NormalisableRange{
                    20.f,          // rangeStart
                    20000.f,       // rangeEnd
                    0.1f,          // intervalValue
                    0.2f,          // skewFactor
                    false },       // useSymmetricSkew
        -12.0f                     // Default value in dB
    ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "Q",                       // Parameter ID
        "Quality Factor",          // Parameter name
        juce::NormalisableRange{
                    0.1f,          // rangeStart
                    1.0f,          // rangeEnd
                    0.01f,         // intervalValue
                    0.9f,          // skewFactor
                    false },       // useSymmetricSkew
        0.1f                       // Default value in dB
    ));

    return { params.begin(), params.end() };
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LowPassFilterAudioProcessor();
}
