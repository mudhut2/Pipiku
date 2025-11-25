#ifndef SOUNDOBJECT_H
#define SOUNDOBJECT_H

#include "raylib.h"
#include "Ping.h"
#include <string>

class PingHitQueue;
extern PingHitQueue gHitQueue;

class SoundObject
{
public:
    SoundObject(Vector2 pos, const std::string& soundPath, Color color);
    ~SoundObject();

    void update(float dt);
    void checkPingHit(const Ping& ping);
    void draw() const;

    // Mark the object as “armed” for queue
    void arm() { armed = true; confirmTimer = CONFIRM_WINDOW; }
    void disarm() { armed = false; confirmTimer = 0.0f; }

    // Called by queue when key pressed
    void play();

    float confirmTimer = 0.0f;
    static constexpr float CONFIRM_WINDOW = 0.3f; // 300 ms window

    Vector2 getPosition() const { return position; }
    Sound getNoteSound() const { return noteSound; }

    // Crash-safe
    SoundObject(const SoundObject&) = delete;
    SoundObject& operator=(const SoundObject&) = delete;
    SoundObject(SoundObject&&) noexcept = default;
    SoundObject& operator=(SoundObject&&) noexcept = default;

private:
    Vector2 position;
    Color baseColor;
    Color currentColor;
    Sound noteSound;

    float hitTimer = 0.0f;
    bool lastPingHit = false;
    bool armed = false;

    static constexpr float HIT_DURATION = 0.15f;
};

#endif
