#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
    : state(STOPPED),
    thumbnailCache(5),
    thumbnailComp(512, formatManager, thumbnailCache),
    positionOverlay (transportSource) {
    addAndMakeVisible(&openButton);
    openButton.setButtonText("Open...");
    openButton.onClick = [this] {openButtonClicked();};

    addAndMakeVisible(&playButton);
    playButton.setButtonText("Play");
    playButton.onClick = [this] {playButtonClicked();};
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    playButton.setEnabled(false);

    addAndMakeVisible(&stopButton);
    stopButton.setButtonText("Stop");
    stopButton.onClick = [this] {stopButtonClicked();};
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    stopButton.setEnabled(false);

    addAndMakeVisible(&thumbnailComp);
    addAndMakeVisible(&positionOverlay);

    // Make sure you set the size of the component after
    // you add any child components.
    setSize (600, 400);

    formatManager.registerBasicFormats();
    transportSource.addChangeListener(this);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio) && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio)) {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio, [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    } else {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent() {
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate) {
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) {
    if (readerSource.get() == nullptr) {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    transportSource.getNextAudioBlock(bufferToFill);    
}

void MainComponent::releaseResources() {
    transportSource.releaseResources();
}

//==============================================================================

void MainComponent::resized() {
    openButton.setBounds(10, 10, getWidth() - 20, 20);
    playButton.setBounds(10, 40, getWidth() - 20, 20);
    stopButton.setBounds(10, 70, getWidth() - 20, 20);

    juce::Rectangle<int> thumbnailBounds(10, 100, getWidth() - 20, getHeight() - 120);
    thumbnailComp.setBounds(thumbnailBounds);
    positionOverlay.setBounds(thumbnailBounds);
}

void MainComponent::changeListenerCallback(juce::ChangeBroadcaster* source) {
    if (source == &transportSource) { 
        transportSourceChanged();
    }
}

void MainComponent::changeState(TransportState newState) {
    if (state != newState) {
        state = newState;

        switch (state) {
        case STOPPED:
            stopButton.setEnabled(false);
            playButton.setEnabled(true);
            transportSource.setPosition(0.0);
            break;

        case STARTING:
            playButton.setEnabled(false);
            transportSource.start();
            break;

        case PLAYING:
            stopButton.setEnabled(true);
            break;

        case STOPPING:
            transportSource.stop();
            break;

        default:
            jassertfalse;
            break;
        }
    }
}

void MainComponent::transportSourceChanged() {
    changeState(transportSource.isPlaying() ? PLAYING : STOPPED);
}

void MainComponent::openButtonClicked() {
    chooser = std::make_unique<juce::FileChooser>("Select a Wave file to play...", juce::File{}, "*.wav");
    auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

    chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc) {
        auto file = fc.getResult();

        if (file != juce::File{}) {
            auto* reader = formatManager.createReaderFor(file);

            if (reader != nullptr) {
                auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
                transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
                playButton.setEnabled(true);
                thumbnailComp.setFile(file);
                readerSource.reset(newSource.release());
            }
        }
    });
}

void MainComponent::playButtonClicked() {
    changeState(STARTING);
}

void MainComponent::stopButtonClicked() {
    changeState(STOPPING);
}
