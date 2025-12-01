// D:\game-dev\Pipiku\src\PlayTargetMelodyObject.cpp

#include "../include/PlayTargetMelodyObject.h"
#include "../include/PingHitQueue.h"
#include <iostream>
#include <raymath.h>

// FIX HERE: The second argument must be MelodyStep
PlayTargetMelodyObject::PlayTargetMelodyObject(Vector2 pos,
                                               const std::vector<MelodyStep>& melody,
                                               Color color
                                               )
    : SoundObject(pos, "", color, false), // Correctly calling the base class constructor
      melodySequence(melody) // Initialize the new member variable
{
}

void PlayTargetMelodyObject::play()
{
    // Implementation to START the melody sequence (only the first step)
    if (currentStep == -1 && !melodySequence.empty())
    {
        currentStep = 0;
        // Play the sounds for the first step
        for (auto& sound : melodySequence[currentStep].sounds)
        {
            PlaySound(sound);
        }
        stepTimer = melodySequence[currentStep].duration;

        // Flash and disarm logic
        hitTimer = HIT_DURATION;
        currentColor = WHITE;
        disarm();
    }
}

// FIX HERE: Implement the missing updateMelody method
void PlayTargetMelodyObject::updateMelody(float dt)
{
    if (currentStep != -1)
    {
        stepTimer -= dt;

        if (stepTimer <= 0.0f)
        {
            currentStep++;

            if (currentStep < melodySequence.size())
            {
                // Play the sounds for the new step
                for (auto& sound : melodySequence[currentStep].sounds)
                {
                    PlaySound(sound);
                }
                stepTimer = melodySequence[currentStep].duration;
            }
            else
            {
                // Sequence finished
                currentStep = -1;
            }
        }
    }
}