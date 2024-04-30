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
    PositionOverlay(juce::AudioTransportSource& transportSourceToUse);
    ~PositionOverlay() override;

    void paint (juce::Graphics&) override;

    void mouseDown(const juce::MouseEvent& event) override;

private:
    void timerCallback() override;
    
    juce::AudioTransportSource& transportSource;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PositionOverlay)
};
