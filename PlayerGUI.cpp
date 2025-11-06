#include "PlayerGUI.h"
#include <taglib/tag.h>
#include "MainComponent.h" 
#include <taglib/fileref.h>

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
    for (auto* btn : { &loadButton, &restartButton, &pauseButton, &playButton,
                      &goToStartButton, &endButton, &loopButton, &muteButton, &tenForwardButton, &tenBackwardButton,&nextButton,&previousButton })
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

    loopButton.setButtonText("Loop OFF");
    muteButton.setButtonText("Mute OFF");

    volumeSlider.setRange(0.0f, 1.0f, 0.01f);
    volumeSlider.setValue(0.5f);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);


    mixerButton.addListener(this);
    addAndMakeVisible(mixerButton);

    mixerVolumeSlider.setRange(0.0f, 1.0f, 0.01f);
    mixerVolumeSlider.setValue(0.7f);
    mixerVolumeSlider.addListener(this);
    addAndMakeVisible(mixerVolumeSlider);

    mixerVolumeLabel.setText("Mix Vol:", juce::dontSendNotification);
    mixerVolumeLabel.attachToComponent(&mixerVolumeSlider, true);
    addAndMakeVisible(mixerVolumeLabel);

    mixerVolumeSlider.setVisible(false);
    mixerVolumeLabel.setVisible(false);


    speedSlider.setRange(0.25f, 4.0f, 0.05f);
    speedSlider.setValue(1.0f);
    speedSlider.addListener(this);
    addAndMakeVisible(speedSlider);

    speedLabel.setText("Speed:", juce::dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, true);
    addAndMakeVisible(speedLabel);

    positionSlider.setRange(0.0, 1.0, 0.001);
    positionSlider.addListener(this);
    positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(positionSlider);

    currentTimeLabel.setText("00:00", juce::dontSendNotification);
    totalTimeLabel.setText("00:00", juce::dontSendNotification);
    addAndMakeVisible(currentTimeLabel);
    addAndMakeVisible(totalTimeLabel);

    playerAudio.addChangeListener(this);

    startTimer(50);  
    
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
    int margin = 15;
    int buttonHeight = 35;
    int sliderHeight = 28;
    int labelHeight = 22;
    int currentY = margin;
    int buttonWidth = 80;
    int spacing = 6;
    int contentWidth = getWidth() - (2 * margin);

    int mixerY = currentY;
    mixerButton.setBounds(margin, mixerY, 90, 32);
    mixerVolumeSlider.setBounds(margin + 150, mixerY, contentWidth - 180, 28);
    currentY += 40;

    int buttonX = margin;
    loadButton.setBounds(buttonX, currentY, 100, buttonHeight);
    buttonX += 105;

    previousButton.setBounds(buttonX, currentY, buttonWidth, buttonHeight);
    buttonX += buttonWidth + spacing;

    goToStartButton.setBounds(buttonX, currentY, buttonWidth, buttonHeight);
    buttonX += buttonWidth + spacing;

    tenBackwardButton.setBounds(buttonX, currentY, buttonWidth, buttonHeight);
    buttonX += buttonWidth + spacing;

    restartButton.setBounds(buttonX, currentY, buttonWidth, buttonHeight);
    buttonX += buttonWidth + spacing;

    pauseButton.setBounds(buttonX, currentY, buttonWidth, buttonHeight);
    buttonX += buttonWidth + spacing;

    currentY += buttonHeight + spacing;

    buttonX = margin;

    playButton.setBounds(buttonX, currentY, buttonWidth, buttonHeight);
    buttonX += buttonWidth + spacing;

    nextButton.setBounds(buttonX, currentY, buttonWidth, buttonHeight);
    buttonX += buttonWidth + spacing;

    tenForwardButton.setBounds(buttonX, currentY, buttonWidth, buttonHeight);
    buttonX += buttonWidth + spacing;

    endButton.setBounds(buttonX, currentY, buttonWidth, buttonHeight);
    buttonX += buttonWidth + spacing;

    loopButton.setBounds(buttonX, currentY, buttonWidth, buttonHeight);
    buttonX += buttonWidth + spacing;

    muteButton.setBounds(buttonX, currentY, buttonWidth, buttonHeight);

    currentY += buttonHeight + 15;

    volumeSlider.setBounds(margin + 55, currentY, contentWidth - 55, sliderHeight);
    currentY += sliderHeight + 12;

    int waveformHeight = 80;
    currentY += waveformHeight + 8;

    speedSlider.setBounds(margin + 55, currentY, contentWidth - 55, sliderHeight);
    currentY += sliderHeight + 12;

    currentTimeLabel.setBounds(margin, currentY, 60, labelHeight);
    totalTimeLabel.setBounds(getWidth() - margin - 60, currentY, 60, labelHeight);
    positionSlider.setBounds(margin + 65, currentY, contentWidth - 130, sliderHeight);
    currentY += sliderHeight + 15;

    int abButtonWidth = 75;
    int abButtonX = margin;

    setAButton.setBounds(abButtonX, currentY, abButtonWidth, 32);
    abButtonX += abButtonWidth + spacing;

    setBButton.setBounds(abButtonX, currentY, abButtonWidth, 32);
    abButtonX += abButtonWidth + spacing;

    enableABLoopButton.setBounds(abButtonX, currentY, 90, 32);
    abButtonX += 95;

    clearABLoopButton.setBounds(abButtonX, currentY, abButtonWidth, 32);
    abButtonX += abButtonWidth + 12;

    pointALabel.setBounds(abButtonX, currentY, 70, 32);
    abButtonX += 75;

    pointBLabel.setBounds(abButtonX, currentY, 70, 32);

    currentY += 40;

    titleLabel.setBounds(margin, currentY, contentWidth, labelHeight);
    currentY += labelHeight + 3;

    authorLabel.setBounds(margin, currentY, contentWidth, labelHeight);
    currentY += labelHeight + 3;

    durationLabel.setBounds(margin, currentY, contentWidth, labelHeight);
    currentY += labelHeight + 12;

    int remainingHeight = getHeight() - currentY - margin;
    if (remainingHeight > 60)
    {
        playlistBox.setBounds(margin, currentY, contentWidth, remainingHeight);
    }
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

void PlayerGUI::updateTrackDisplay()
{
    if (currentTrackIndex >= 0 && currentTrackIndex < playlist.size())
    {
        auto& item = playlist[currentTrackIndex];
        titleLabel.setText("Title: " + item.title, juce::dontSendNotification);
        authorLabel.setText("Author: " + item.author, juce::dontSendNotification);
        durationLabel.setText("Duration: " + item.duration, juce::dontSendNotification);
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
                            item.duration = juce::String(min) + ":" +
                                juce::String(sec).paddedLeft('0', 2);
                        }

                        playlist.push_back(item);
                    }
                }

                playlistBox.updateContent();

                if (currentTrackIndex == -1 && !playlist.empty())
                {
                    if (playerAudio.loadFile(playlist[0].file))
                    {
                        currentTrackIndex = 0;
                        updateTrackDisplay();
                    }
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

    else if (button == &mixerButton)
    {
        if (parentComponent != nullptr)
        {
            bool newMixerState = !parentComponent->isMixerEnabled();
            parentComponent->setMixerEnabled(newMixerState);

            mixerButton.setButtonText(newMixerState ? "Mixer ON" : "Mixer OFF");
            mixerButton.setColour(juce::TextButton::buttonColourId,
                newMixerState ? juce::Colours::orange : juce::Colours::lightgrey);

            mixerVolumeSlider.setVisible(newMixerState);
            mixerVolumeLabel.setVisible(newMixerState);
        }

    }
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{

    if (slider == &volumeSlider)
        playerAudio.setGain((float)slider->getValue());

        else if (slider == &speedSlider)
        playerAudio.speedAudio((float)slider->getValue());

    else if (slider == &positionSlider)
    {
        double newPosition = positionSlider.getValue() * playerAudio.getLength();
        playerAudio.setPosition(newPosition);
    }

    else if (slider == &mixerVolumeSlider)
    {
        if (parentComponent != nullptr)
        {
            if (this == parentComponent->getPlayer1())
                parentComponent->setPlayer1MixerVolume((float)slider->getValue());
            else if (this == parentComponent->getPlayer2())
                parentComponent->setPlayer2MixerVolume((float)slider->getValue());
        }
    }
}

void PlayerGUI::timerCallback()
{
    repaint();
}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff353535));

    if (playerAudio.getAudioThumbnail().getTotalLength() > 0.0)
    {
        int margin = 15;
        int waveX = margin;
        int waveY = 172;  
        int waveWidth = getWidth() - (2 * margin);
        int waveHeight = 80;

        g.setColour(juce::Colour(0xff1a1a1a));
        g.fillRoundedRectangle(waveX, waveY, waveWidth, waveHeight, 4.0f);

        g.setColour(juce::Colour(0xff505050));
        g.drawRoundedRectangle(waveX, waveY, waveWidth, waveHeight, 4.0f, 1.0f);

        g.setColour(juce::Colour(0xff00d4ff));
        juce::Rectangle<int> waveArea(waveX + 2, waveY + 2, waveWidth - 4, waveHeight - 4);
        playerAudio.getAudioThumbnail().drawChannel(g, waveArea,
            0.0, playerAudio.getAudioThumbnail().getTotalLength(), 0, 1.0f);

        double progress = playerAudio.getPosition() / playerAudio.getLength();
        if (progress > 0.0 && progress <= 1.0)
        {
            g.setColour(juce::Colour(0xff00ff88).withAlpha(0.25f));
            g.fillRoundedRectangle(waveX + 2, waveY + 2,
                static_cast<float>((waveWidth - 4) * progress),
                waveHeight - 4, 3.0f);
        }

        float pointerX = static_cast<float>(waveX + 2 + ((waveWidth - 4) * progress));
        g.setColour(juce::Colour(0xffff3366));
        g.drawLine(pointerX, static_cast<float>(waveY + 2),
            pointerX, static_cast<float>(waveY + waveHeight - 2), 2.5f);

        int currentSecs = static_cast<int>(playerAudio.getPosition());
        int totalSecs = static_cast<int>(playerAudio.getLength());
        juce::String timeText = juce::String::formatted("%02d:%02d / %02d:%02d",
            currentSecs / 60, currentSecs % 60, totalSecs / 60, totalSecs % 60);

        g.setColour(juce::Colour(0xff000000).withAlpha(0.8f));
        g.fillRoundedRectangle(waveX + 8, waveY + 8, 90, 22, 3.0f);

        g.setColour(juce::Colours::white);
        g.setFont(13.0f);
        g.drawText(timeText, waveX + 8, waveY + 8, 90, 22,
            juce::Justification::centred);
    }
}

void PlayerGUI::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    (void)source;
    double currentPos = playerAudio.getPosition();
    double totalLength = playerAudio.getLength();

    if (totalLength > 0)
    {
        double progress = currentPos / totalLength;
        positionSlider.setValue(progress, juce::dontSendNotification);
    }

    currentTimeLabel.setText(formatTime(currentPos), juce::dontSendNotification);
    totalTimeLabel.setText(formatTime(totalLength), juce::dontSendNotification);
}

juce::String PlayerGUI::formatTime(double seconds)
{
    int mins = (int)seconds / 60;
    int secs = (int)seconds % 60;
    return juce::String::formatted("%02d:%02d", mins, secs);
}





    
