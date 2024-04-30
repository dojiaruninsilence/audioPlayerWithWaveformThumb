/*
  ==============================================================================

    ThumbnailComponent.h
    Created: 30 Apr 2024 1:03:08pm
    Author:  Zubin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ThumbnailComponent  : public juce::Component,
    private juce::ChangeListener
{
public:
    ThumbnailComponent(int sourceSamplesPerThumbnailSample, juce::AudioFormatManager& formatManager, juce::AudioThumbnailCache& cache);
    ~ThumbnailComponent() override;

    void setFile(const juce::File& file);

    void paint (juce::Graphics&) override;

    void paintIfNoFileLoaded(juce::Graphics& g);
    void paintIfFileLoaded(juce::Graphics& g);

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

private:
    void thumbnailChanged();

    juce::AudioThumbnail thumbnail;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThumbnailComponent)
};
