/*
  ==============================================================================

    ThumbnailComponent.cpp
    Created: 30 Apr 2024 1:03:08pm
    Author:  Zubin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ThumbnailComponent.h"

//==============================================================================
ThumbnailComponent::ThumbnailComponent(int sourceSamplesPerThumbnailSample, juce::AudioFormatManager& formatManager, juce::AudioThumbnailCache& cache)
    : thumbnail(sourceSamplesPerThumbnailSample, formatManager, cache)
{
    thumbnail.addChangeListener(this);
}

ThumbnailComponent::~ThumbnailComponent()
{
}

void ThumbnailComponent::setFile(const juce::File& file) {
    thumbnail.setSource(new juce::FileInputSource(file));
}

void ThumbnailComponent::paint (juce::Graphics& g)
{
    if (thumbnail.getNumChannels() == 0) {
        paintIfNoFileLoaded(g);
    } else {
        paintIfFileLoaded(g);
    }
}

void ThumbnailComponent::paintIfNoFileLoaded(juce::Graphics& g) {
    g.fillAll(juce::Colours::white);
    g.setColour(juce::Colours::darkgrey);
    g.drawFittedText("No File Loaded", getLocalBounds(), juce::Justification::centred, 1);
}

void ThumbnailComponent::paintIfFileLoaded(juce::Graphics& g) {
    g.fillAll(juce::Colours::white);

    g.setColour(juce::Colours::red);
    thumbnail.drawChannels(g, getLocalBounds(), 0.0, thumbnail.getTotalLength(), 1.0f);
}

void ThumbnailComponent::changeListenerCallback(juce::ChangeBroadcaster* source) {
    if (source == &thumbnail) {
        thumbnailChanged();
    }
}

void ThumbnailComponent::thumbnailChanged() {
    repaint();
}