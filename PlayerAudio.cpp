#include "PlayerAudio.h"

PlayerAudio::PlayerAudio() 
	: audioThumbnail(512, formatManager, thumbnailCache)
{
	formatManager.registerBasicFormats();

}


PlayerAudio::~PlayerAudio()
{
	transportSource.setSource(nullptr);
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	transportSource.getNextAudioBlock(bufferToFill);
}

void PlayerAudio::releaseResources()
{
	transportSource.releaseResources();
}

bool PlayerAudio::loadFile(const juce::File& file)
{
    if (auto* reader = formatManager.createReaderFor(file))
    {
        transportSource.stop();
        transportSource.setSource(nullptr);
        readerSource.reset();

        readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

		transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate * playbackSpeed);
        transportSource.start();

        return true;
    }
    return false;
}

void PlayerAudio::restart()
{
    transportSource.setPosition(0.0);
    transportSource.start();
}

void PlayerAudio::play()
{
    transportSource.start();
}


void PlayerAudio::pause()
{
    transportSource.stop();
}

void PlayerAudio::goToStart()
{
    transportSource.stop();
    transportSource.setPosition(0.0);

}

void PlayerAudio::end()
{
    transportSource.setPosition(transportSource.getLengthInSeconds());
}

void PlayerAudio::tenBackward()
{
    double newPos = juce::jmax(0.0, transportSource.getCurrentPosition() - 10.0);
    transportSource.setPosition(newPos);
}

void PlayerAudio::tenForward()
{
    double newPos = juce::jmin(getLength(), transportSource.getCurrentPosition() + 10.0);
    transportSource.setPosition(newPos);
}

void PlayerAudio::setGain(float gain)
{
    transportSource.setGain(gain);
}

void PlayerAudio::setPosition(double pos)
{
    transportSource.setPosition(pos);
}

double PlayerAudio::getPosition() const
{
    return transportSource.getCurrentPosition();
}

double PlayerAudio::getLength() const
{
    return transportSource.getLengthInSeconds();
}
// loop button
void PlayerAudio::setLoopEnabled(bool shouldLoop) noexcept
{
    loopEnabled = shouldLoop;
    if (readerSource)
        readerSource->setLooping(loopEnabled);
}
// -------------

//speed
void PlayerAudio::speedAudio(float speed)
{

    playbackSpeed = speed;
    if (readerSource && readerSource->getAudioFormatReader())
    {
        // Update the source with new speed
        transportSource.setSource(readerSource.get(), 0, nullptr,
            readerSource->getAudioFormatReader()->sampleRate * playbackSpeed);
    }
}
// position
void PlayerAudio::addChangeListener(juce::ChangeListener* listener)
     {
    changeBroadcaster.addChangeListener(listener);
     }

void PlayerAudio::removeChangeListener(juce::ChangeListener* listener)
     {
    changeBroadcaster.removeChangeListener(listener);
     }
