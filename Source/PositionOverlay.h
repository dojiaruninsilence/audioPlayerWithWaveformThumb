/*
  ==============================================================================

    PositionOverlay.h
    Created: 30 Apr 2024 1:01:45pm
    Author:  Zubin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PositionOverlay  : public juce::Component,
    private juce::Timer
{
public:
    PositionOverlay(const juce::AudioTransportSource& transportSourceToUse);
    ~PositionOverlay() override;

    void paint (juce::Graphics&) override;

private:
    void timerCallback() override;
    
    const juce::AudioTransportSource& transportSource;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PositionOverlay)
};
