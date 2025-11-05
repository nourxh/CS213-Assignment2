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

    // loop button
    void setLoopEnabled(bool shouldLoop) noexcept;
    bool isLoopEnabled() const noexcept { return loopEnabled; }
    // --------------
private:
	juce::AudioFormatManager formatManager;
	std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
	juce::AudioTransportSource transportSource;

    bool loopEnabled = false;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)

};
