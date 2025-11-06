#include "PlayerGUI.h"
#include "MainComponent.h" 

void PlayerGUI::setParentComponent(MainComponent* parent)
{
    parentComponent = parent;
}

void PlayerGUI::pausePlayback()
{
    playerAudio.pause();
}

PlayerGUI::PlayerGUI()
{
    for (auto* btn : { &loadButton, &restartButton , &pauseButton , &playButton , &goToStartButton , &endButton , &loopButton, &muteButton, , &tenForwardButton, &tenBackwardButton,  &nextButton, &previousButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    addAndMakeVisible(titleLabel);
    addAndMakeVisible(authorLabel);
    addAndMakeVisible(durationLabel);
    addAndMakeVisible(playlistBox);

    titleLabel.setText("Title: -", juce::dontSendNotification);
    authorLabel.setText("Author: -", juce::dontSendNotification);
    durationLabel.setText("Duration: -", juce::dontSendNotification);
    
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

    // ------------
    loopButton.setButtonText("Loop OFF");
    muteButton.setButtonText("Mute OFF");

    // -----------

    //speed
    speedSlider.setRange(0.25f, 4.0f, 0.05f);
    speedSlider.setValue(1.0f);
    speedSlider.addListener(this);
    addAndMakeVisible(speedSlider);
    
    speedLabel.setText("Speed:", juce::dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, true);
    addAndMakeVisible(speedLabel);
    //-----------------------


    //waveform t9 - 
    startTimer(50);  // Update every 50ms

    
    // position
    positionSlider.setRange(0.0, 1.0, 0.001);
    positionSlider.addListener(this);
    positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(positionSlider);

    currentTimeLabel.setText("00:00", juce::dontSendNotification);
    totalTimeLabel.setText("00:00", juce::dontSendNotification);
    addAndMakeVisible(currentTimeLabel);
    addAndMakeVisible(totalTimeLabel);

    playerAudio.addChangeListener(this);

    setAButton.addListener(this);
    setBButton.addListener(this);
    enableABLoopButton.addListener(this);
    clearABLoopButton.addListener(this);

    addAndMakeVisible(setAButton);
    addAndMakeVisible(setBButton);
    addAndMakeVisible(enableABLoopButton);
    addAndMakeVisible(clearABLoopButton);

    pointALabel.setText("A: --:--", juce::dontSendNotification);
    pointBLabel.setText("B: --:--", juce::dontSendNotification);
    addAndMakeVisible(pointALabel);
    addAndMakeVisible(pointBLabel);
    
    playlistBox.setModel(&playlistModel);

}


PlayerGUI::~PlayerGUI() {}

void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio.getNextAudioBlock(bufferToFill);
}

void PlayerGUI::releaseResources()
{
    playerAudio.releaseResources();
}

void PlayerGUI::resized()
{
    int y = 20;
    loadButton.setBounds(20, y, 100, 40);
    restartButton.setBounds(140, y, 80, 40);
    pauseButton.setBounds(240, y, 80, 40);
    playButton.setBounds(340, y, 80, 40);
    goToStartButton.setBounds(440, y, 80, 40);
    endButton.setBounds(540, y, 80, 40);
    loopButton.setBounds(450, getHeight() - 150, 80, 40);
    volumeSlider.setBounds(20, 100, getWidth() - 40, 30);
    muteButton.setBounds(640, 20, 80, 40);
    speedSlider.setBounds(20, 150, getWidth() - 40, 30);

    tenBackwardButton.setBounds(740, y, 80, 40);
    tenForwardButton.setBounds(840, y, 80, 40);

    
    titleLabel.setBounds(20, 375, getWidth() - 40, 25);
    authorLabel.setBounds(20, 405, getWidth() - 40, 25);
    durationLabel.setBounds(20, 435, getWidth() - 40, 25);
    
    playlistBox.setBounds(20, 475 , getWidth() - 40, getHeight() - 260);

}
void PlayerGUI::loadTrack(int index)
{
    if (index >= 0 && index < playlist.size())
    {
        currentTrackIndex = index;
        auto& item = playlist[index];

        if (playerAudio.loadFile(item.file))
        {
            updateTrackDisplay();
            playlistBox.repaint();

        }
    }
}
void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
        "Select audio files...", juce::File{}, "*.wav;*.mp3");
    
        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles | juce::FileBrowserComponent::canSelectMultipleItems,
            [this](const juce::FileChooser& fc)
            {
                auto files = fc.getResults();
        
                for (const auto& file : files)
                {
                    if (file.existsAsFile())
                    {
                        PlaylistItem item;
                        item.file = file;
                        item.title = file.getFileNameWithoutExtension();
        
        
                        std::string filePath = file.getFullPathName().toStdString();
                        TagLib::FileRef f(filePath.c_str());
                        item.author = "Unknown";
        
                        if (!f.isNull() && f.tag())
                        {
                            auto* tag = f.tag();
                            if (!tag->title().isEmpty()) {
                                item.title = juce::String::fromUTF8(tag->title().toCString(true));
                            }
                            if (!tag->artist().isEmpty()) {
                                item.author = juce::String::fromUTF8(tag->artist().toCString(true));
                            }
                        }
        
                        juce::AudioFormatManager audioformats;
                        audioformats.registerBasicFormats();
                        std::unique_ptr<juce::AudioFormatReader> reader(audioformats.createReaderFor(file));
        
                        if (reader != nullptr && reader->sampleRate > 0)
                        {
                            double durationSec = reader->lengthInSamples / reader->sampleRate;
                            int min = (int)durationSec / 60;
                            int sec = (int)durationSec % 60;
                            item.duration = juce::String(min) + ":" + juce::String(sec).paddedLeft('0', 2);
                        }
        
                        playlist.push_back(item);
                    }
                }
        
                playlistBox.updateContent();
        
                if (currentTrackIndex == -1 && !playlist.empty())
                {
                    loadTrack(0);
                }
            });
    }

    else if (button == &restartButton)
    {
        playerAudio.restart();
    }

    else if (button == &pauseButton)
    {
        playerAudio.pause();
    }
    else if (button == &playButton)
     {
     if (parentComponent != nullptr)
     {
         parentComponent->setActivePlayer(this);
     }
     playerAudio.play();
     }   

  

    else if (button == &goToStartButton)
    {
        playerAudio.goToStart();
    }

    else if (button == &endButton)
    {
        playerAudio.end();
    }
    else if (button == &tenForwardButton)
    {
        playerAudio.tenForward();
    }
    else if (button == &tenBackwardButton)
    {
        playerAudio.tenBackward();
    }
    else if (button == &nextButton)
    {
        if (!playlist.empty())
        {
            int nextIndex = (currentTrackIndex + 1) % playlist.size();
            loadTrack(nextIndex);
        }
    }
    else if (button == &previousButton)
    {
        if (!playlist.empty())
        {
            int prevIndex = (currentTrackIndex - 1 + playlist.size()) % playlist.size();
            loadTrack(prevIndex);
        }
    }
        // loop button
    else if (button == &loopButton)
    {
      bool newLoopState = !playerAudio.isLoopEnabled();
      playerAudio.setLoopEnabled(newLoopState);
      loopButton.setButtonText(newLoopState ? "Loop ON" : "Loop OFF");
      loopButton.setColour(juce::TextButton::buttonColourId,
          newLoopState ? juce::Colours::green : juce::Colours::lightgrey);
    }

        
     else if (button == &muteButton)
     {
        static float previousVolume = volumeSlider.getValue();

        if (muteButton.getButtonText() == "Mute OFF")
        {
             previousVolume = volumeSlider.getValue();
             volumeSlider.setValue(0.0);
             muteButton.setButtonText("Mute ON");
             muteButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
        }
        else
        {
             volumeSlider.setValue(previousVolume);
             muteButton.setButtonText("Mute OFF");
             muteButton.setColour(juce::TextButton::buttonColourId, juce::Colours::lightgrey);
        }
     }
      else if (button == &setAButton)
  {
      double currentTime = playerAudio.getPosition();
      playerAudio.setLoopPointA(currentTime);
      pointALabel.setText("A: " + formatTime(currentTime), juce::dontSendNotification);
  }
  else if (button == &setBButton)
  {
      double currentTime = playerAudio.getPosition();
      playerAudio.setLoopPointB(currentTime);
      pointBLabel.setText("B: " + formatTime(currentTime), juce::dontSendNotification);
  }
  else if (button == &enableABLoopButton)
  {
      playerAudio.enableABLoop(true);
      enableABLoopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
  }

  else if (button == &clearABLoopButton)
  {
      playerAudio.clearABLoop();
      pointALabel.setText("A: --:--", juce::dontSendNotification);
      pointBLabel.setText("B: --:--", juce::dontSendNotification);
      enableABLoopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::lightgrey);

      playerAudio.enableABLoop(false);
  }


}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        playerAudio.setGain((float)slider->getValue());
    // sppeedd 
    else if (slider == &speedSlider)
        playerAudio.speedAudio((float)slider->getValue());
    // position
     else if (slider == &positionSlider)
    {
     double newPosition = positionSlider.getValue() * playerAudio.getLength();
     playerAudio.setPosition(newPosition);
    }

}


void PlayerGUI::timerCallback()
{
    repaint();  // Trigger repaint for waveform updates
}

void PlayerGUI::paint(juce::Graphics& g)
{
    if (playerAudio.getAudioThumbnail().getTotalLength() > 0.0)
    {
        int margin = 10;
        int waveX = margin;
        int waveY = 150;  // Position after Volume and Speed sliders
        int waveWidth = getWidth() - 2 * margin;
        int waveHeight = 75;

        // Background for waveform
        g.setColour(juce::Colours::black);
        g.fillRect(waveX, waveY, waveWidth, waveHeight);

        // Draw the Waveform
        g.setColour(juce::Colours::cyan);
        juce::Rectangle<int> waveArea(waveX, waveY, waveWidth, waveHeight);
        playerAudio.getAudioThumbnail().drawChannel(g, waveArea,
            0.0, playerAudio.getAudioThumbnail().getTotalLength(), 0, 1.0f);

        // Progress overlay (played portion)
        double progress = playerAudio.getPosition() / playerAudio.getLength();
        if (progress > 0.0 && progress <= 1.0)
        {
            g.setColour(juce::Colours::green.withAlpha(0.35f));
            g.fillRect(waveX, waveY, static_cast<int>(waveWidth * progress), waveHeight);
        }

        // Current position line
        float pointerX = static_cast<float>(waveX + (waveWidth * progress));
        g.setColour(juce::Colours::red);
        g.drawLine(pointerX, static_cast<float>(waveY),
            pointerX, static_cast<float>(waveY + waveHeight), 2.5f);

        // Time display on the waveform
        g.setColour(juce::Colours::white);
        int currentSecs = static_cast<int>(playerAudio.getPosition());
        int totalSecs = static_cast<int>(playerAudio.getLength());
        juce::String timeText = juce::String::formatted("%02d:%02d / %02d:%02d",
            currentSecs / 60, currentSecs % 60, totalSecs / 60, totalSecs % 60);

        // Background for text to ensure visibility
        g.setColour(juce::Colours::black.withAlpha(0.7f));
        g.fillRect(waveX + 5, waveY + 5, 80, 18);

        g.setColour(juce::Colours::white);
        g.drawText(timeText, waveX + 5, waveY + 5, 80, 18,
            juce::Justification::centred);
    }
}
