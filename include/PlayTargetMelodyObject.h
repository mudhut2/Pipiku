#ifndef PLAYTARGETMELODYOBJECT_H
#define PLAYTARGETMELODYOBJECT_H

#include "SoundObject.h"
#include <vector>
#include "raylib.h"

// The new MelodyStep structure MUST be defined here if it's not defined elsewhere
typedef struct {
    std::vector<Sound> sounds;
    float duration;
} MelodyStep;


class PlayTargetMelodyObject : public SoundObject
{
public:
    // FIX HERE: The second argument MUST be MelodyStep
    PlayTargetMelodyObject(Vector2 pos,
                           const std::vector<MelodyStep>& melody,
                           Color color);

    void play() override;             // E key triggers this

    // NEW: Add the updateMelody declaration so main.cpp can use it
    void updateMelody(float dt);

private:
    std::vector<MelodyStep> melodySequence;

    // Sequencer state variables
    int currentStep = -1;
    float stepTimer = 0.0f;
};

#endif