// PlayerAudio.h
#pragma once
#include <JuceHeader.h>

class PlayerAudio
{
public:
    PlayerAudio();
    ~PlayerAudio();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    bool loadFile(const juce::File& file);

    void restart();
    void play();
    void pause();
    void goToStart();
    void end();
    void tenForward();
    void tenBackward();
    void setGain(float gain);

    //task 7
    void setPosition(double pos);
    double getPosition() const;
    double getLength() const;

    bool isPlaying() const { return transportSource.isPlaying(); }
    double getCurrentPosition() const { return transportSource.getCurrentPosition(); }
    //-------

    //task 10
    void setLoopEnabled(bool shouldLoop) noexcept;
    bool isLoopEnabled() const noexcept { return loopEnabled; }
    //-----------

    void speedAudio(float speed);

   
    juce::AudioThumbnail& getAudioThumbnail() { return audioThumbnail; }

    void addChangeListener(juce::ChangeListener* listener);
    void removeChangeListener(juce::ChangeListener* listener);

    //task 10
    void setLoopPointA(double timeInSeconds);
    void setLoopPointB(double timeInSeconds);
    void enableABLoop(bool shouldEnable);
    void clearABLoop();

    double getLoopPointA() const { return loopPointA; }
    double getLoopPointB() const { return loopPointB; }
    bool isABLoopEnabled() const { return abLoopEnabled; }

private:
    juce::AudioFormatManager formatManager;
    juce::AudioTransportSource transportSource;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;

    juce::AudioThumbnailCache thumbnailCache{ 1 };
    juce::AudioThumbnail audioThumbnail{ 512, formatManager, thumbnailCache };

    float playbackSpeed = 1.0f;

    bool loopEnabled = false;

    juce::ChangeBroadcaster changeBroadcaster;

    //task 10
    double loopPointA = 0.0;
    double loopPointB = 0.0;
    bool abLoopEnabled = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};
