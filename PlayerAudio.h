#pragma once
#include <JuceHeader.h>

class PlayerAudio : public juce::AudioSource
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
	void setGain(float gain);
	void setPosition(double pos);
	double getPosition() const;
	double getLength() const;

    void tenForward();
    void tenBackward();

    // loop button
    void setLoopEnabled(bool shouldLoop) noexcept;
    bool isLoopEnabled() const noexcept { return loopEnabled; }
    // --------------

    // t 10
    void addChangeListener(juce::ChangeListener* listener);
    void removeChangeListener(juce::ChangeListener* listener);

    void setLoopPointA(double timeInSeconds);
    void setLoopPointB(double timeInSeconds);
    void enableABLoop(bool shouldEnable);
    void clearABLoop();

    double getLoopPointA() const { return loopPointA; }
    double getLoopPointB() const { return loopPointB; }
    bool isABLoopEnabled() const { return abLoopEnabled; }


private:
	juce::AudioFormatManager formatManager;
	std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
	juce::AudioTransportSource transportSource;

    juce::ChangeBroadcaster changeBroadcaster;


    bool loopEnabled = false;

     double loopPointA = 0.0;
     double loopPointB = 0.0;
     bool abLoopEnabled = false;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)

};
