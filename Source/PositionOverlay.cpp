/*
  ==============================================================================

    PositionOverlay.cpp
    Created: 30 Apr 2024 1:01:45pm
    Author:  Zubin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PositionOverlay.h"

//==============================================================================
PositionOverlay::PositionOverlay(juce::AudioTransportSource& transportSourceToUse)
    : transportSource(transportSourceToUse)
{
    startTimer(40);

}

PositionOverlay::~PositionOverlay()
{
}

void PositionOverlay::paint (juce::Graphics& g)
{
    auto duration = (float)transportSource.getLengthInSeconds();

    if (duration > 0.0) {
        auto audioPosition = (float)transportSource.getCurrentPosition();
        auto drawPosition = (audioPosition / duration) * (float)getWidth();

        g.setColour(juce::Colours::darkgreen);
        g.drawLine(drawPosition, 0.0f, drawPosition, (float)getHeight(), 2.0f);
    }
}

void PositionOverlay::mouseDown(const juce::MouseEvent& event) {
    auto duration = transportSource.getLengthInSeconds();

    if (duration > 0.0) {
        auto clickPosition = event.position.x;
        auto audioPosition = (clickPosition / (float)getWidth()) * duration;

        transportSource.setPosition(audioPosition);
    }
}

void PositionOverlay::timerCallback() {
    repaint();
}
