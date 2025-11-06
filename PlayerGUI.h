#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlayerGUI : public juce::Component,
	public juce::Button::Listener,
	public juce::Slider::Listener
{
public:
	PlayerGUI();
	~PlayerGUI() override;

	void resized() override;

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
	void releaseResources();


private:

    struct PlaylistItem
    {
        juce::File file;
        juce::String title;
        juce::String author;
        juce::String duration;
    };

	PlayerAudio playerAudio;

    juce::Label titleLabel, authorLabel, durationLabel;
	juce::TextButton loadButton{ "Load Files" };
	juce::TextButton restartButton{ "Restart" };
	juce::TextButton playButton{ "Play" };
	juce::TextButton pauseButton{ "Pause" };
	juce::TextButton goToStartButton{ "Go To Start" };
	juce::TextButton endButton{ "End" };
    juce::TextButton loopButton{ "Loop" };
    juce::TextButton muteButton{ "Mute" };
    juce::ListBox playlistBox;

	juce::Slider volumeSlider;

	juce::Slider speedSlider;
    juce::Label speedLabel;

	std::unique_ptr<juce::FileChooser> fileChooser;

 	std::vector<PlaylistItem> playlist;
 	int currentTrackIndex = -1;

	// Event handlers

	void buttonClicked(juce::Button* button) override;
	void sliderValueChanged(juce::Slider* slider) override;

    void loadTrack(int index);
    void updateTrackDisplay();

	class PlaylistModel : public juce::ListBoxModel
	{
	private:
	    PlayerGUI& plManager;
	
	public:
	    PlaylistModel(PlayerGUI& plManager) : plManager(plManager) {}
	
	    int getNumRows() override
	    {
	        return (int)plManager.playlist.size();
	    }
	
	    void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override
	    {
	        if (rowIsSelected) {
	            g.fillAll(juce::Colours::steelblue);
	        }
	        else if (rowNumber == plManager.currentTrackIndex) {
	            g.fillAll(juce::Colours::steelblue);
	        }
	        else {
	            g.fillAll(juce::Colours::transparentBlack);
	        }
	
	        g.setColour(juce::Colours::white);
	
	        if (rowNumber < plManager.playlist.size())
	        {
	            auto& item = plManager.playlist[rowNumber];
	
	            juce::String displayText = juce::String(rowNumber + 1) + ". " + item.title + " - " + item.author;
	
	            g.drawText(displayText, 5, 0, width - 10, height, juce::Justification::centredLeft, true);
	        }
	    }
	
	    void listBoxItemClicked(int row, const juce::MouseEvent&) override
	    {
	        plManager.loadTrack(row);
	    }
	
	
	};
	
	PlaylistModel playlistModel{ *this };

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)

};

