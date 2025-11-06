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

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    player1.getNextAudioBlock(bufferToFill);
}

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
