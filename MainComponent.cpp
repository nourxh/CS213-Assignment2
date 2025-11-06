#include "MainComponent.h"

MainComponent::MainComponent()
{
    addAndMakeVisible(player1);
    addAndMakeVisible(player2);

     player1.setParentComponent(this);
     player2.setParentComponent(this);

    setSize(100, 800);
    setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
//mixer
void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();

    //========== MIXER ADDITIONS START ==========
    // Mixer mode - play both tracks simultaneously
    if (mixerEnabled)
    {
        // Create temporary buffers for each player
        juce::AudioBuffer<float> tempBuffer1(bufferToFill.buffer->getNumChannels(), bufferToFill.buffer->getNumSamples());
        juce::AudioBuffer<float> tempBuffer2(bufferToFill.buffer->getNumChannels(), bufferToFill.buffer->getNumSamples());

        juce::AudioSourceChannelInfo info1(&tempBuffer1, 0, bufferToFill.numSamples);
        juce::AudioSourceChannelInfo info2(&tempBuffer2, 0, bufferToFill.numSamples);

        // Get audio from both players
        player1.getNextAudioBlock(info1);
        player2.getNextAudioBlock(info2);

        // Mix both buffers with individual volume control
        for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
        {
            auto* output = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
            auto* input1 = tempBuffer1.getReadPointer(channel);
            auto* input2 = tempBuffer2.getReadPointer(channel);

            for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
            {
                // Mix with volume control and prevent clipping
                float mixedSample = (input1[sample] * player1MixerVolume) +
                    (input2[sample] * player2MixerVolume);

                // Simple limiter to prevent clipping
                output[sample] = juce::jlimit(-1.0f, 1.0f, mixedSample);
            }
        }
    }
    else
    {
        // Original single-player mode
        if (activePlayer == &player1)
            player1.getNextAudioBlock(bufferToFill);
        else if (activePlayer == &player2)
            player2.getNextAudioBlock(bufferToFill);
    }
    //========== MIXER ADDITIONS END ==========
}

// Mixer implementation
void MainComponent::setMixerEnabled(bool shouldEnable)
{
    mixerEnabled = shouldEnable;
}

void MainComponent::setPlayer1MixerVolume(float volume)
{
    player1MixerVolume = juce::jlimit(0.0f, 1.0f, volume);
}

void MainComponent::setPlayer2MixerVolume(float volume)
{
    player2MixerVolume = juce::jlimit(0.0f, 1.0f, volume);
}

void MainComponent::switchToPlayer(PlayerGUI* player)
{
    setActivePlayer(player);
    if (!mixerEnabled && activePlayer != nullptr)
    {
        if (player == &player1)
            player2.pausePlayback();
        else if (player == &player2)
            player1.pausePlayback();
    }
}

void MainComponent::setActivePlayer(PlayerGUI* player)
{
    if (activePlayer != nullptr && activePlayer != player)
    {
        activePlayer->pausePlayback();
    }

    activePlayer = player;
}

//-----------------

void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
}
void MainComponent::resized()
{
    int spacing = 20;
    int separatorWidth = 2;
    int halfWidth = (getWidth() - (2 * spacing) - separatorWidth) / 2;

    player1.setBounds(spacing, spacing, halfWidth, getHeight() - (2 * spacing));
    player2.setBounds(halfWidth + spacing + separatorWidth, spacing, halfWidth, getHeight() - (2 * spacing));
}
void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff2a2a2a));

    int spacing = 20;
    int separatorWidth = 2;
    int halfWidth = (getWidth() - (2 * spacing) - separatorWidth) / 2;
    int separatorX = halfWidth + spacing;

    g.setColour(juce::Colour(0xff505050));
    g.fillRect(separatorX, spacing, separatorWidth, getHeight() - (2 * spacing));
}
void MainComponent::setActivePlayer(PlayerGUI* player)
{
    if (activePlayer != nullptr && activePlayer != player)
    {
        activePlayer->pausePlayback();
    }

    activePlayer = player;
}

void MainComponent::resized()
{
    player1.setBounds(20, 20, getWidth() - 40, getHeight() - 40);
}
