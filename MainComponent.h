#pragma once
#include <JuceHeader.h>
#include "PlayerGUI.h"

class MainComponent : public juce::AudioAppComponent
{
public:
    MainComponent();
    ~MainComponent() override;

    void resized() override;
    void paint(juce::Graphics& g) override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
     //---------------- Mixer functionality
     void setMixerEnabled(bool shouldEnable);
     bool isMixerEnabled() const { return mixerEnabled; }
     void setPlayer1MixerVolume(float volume);
     void setPlayer2MixerVolume(float volume);
     float getPlayer1MixerVolume() const { return player1MixerVolume; }
     float getPlayer2MixerVolume() const { return player2MixerVolume; }
    
     PlayerGUI* getPlayer1() { return &player1; }
     PlayerGUI* getPlayer2() { return &player2; }
    
     PlayerGUI* getActivePlayer() const { return activePlayer; }
     void switchToPlayer(PlayerGUI* player);
     //----------------
    void setActivePlayer(PlayerGUI* player);


private:
    PlayerGUI player1;
    PlayerGUI player2;


    //---------------- Mixer variables
    bool mixerEnabled = false;
    float player1MixerVolume = 0.7f;
    float player2MixerVolume = 0.7f;
    //----------------
    PlayerGUI* activePlayer = nullptr;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

