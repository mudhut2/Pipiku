#include "../include/SoundObject.h"
#include <iostream>
#include "raymath.h"
#include "../include/PingHitQueue.h"

SoundObject::SoundObject(Vector2 pos, const std::string& soundPath, Color color, bool moveable)
    : position(pos), baseColor(color), currentColor(color), isMoveable(moveable)
{
    noteSound = LoadSound(soundPath.c_str());
    if (noteSound.frameCount == 0)
        std::cerr << "Warning: Failed to load sound: " << soundPath << std::endl;
}

SoundObject::~SoundObject()
{
    if (noteSound.frameCount != 0)
        UnloadSound(noteSound);
}

void SoundObject::update(float dt)
{
    // Flash effect when the note is played
    if (hitTimer > 0.0f)
    {
        hitTimer -= dt;
        float ratio = hitTimer / HIT_DURATION;
        currentColor.r = (unsigned char)Lerp((float)baseColor.r, 255.0f, ratio);
        currentColor.g = (unsigned char)Lerp((float)baseColor.g, 255.0f, ratio);
        currentColor.b = (unsigned char)Lerp((float)baseColor.b, 255.0f, ratio);
        currentColor.a = baseColor.a;

        if (hitTimer <= 0.0f)
        {
            currentColor = baseColor;
            hitTimer = 0.0f;
        }
    }

    if (armed)
    {
        confirmTimer -= dt;
        if (confirmTimer <= 0.0f)
            disarm();
    }
}

void SoundObject::checkPingHit(const Ping& ping)
{
    if (!ping.isActive() || noteSound.frameCount == 0)
    {
        lastPingHit = false;
        return;
    }

    float distance = Vector2Distance(ping.getCenter(), position);
    float radius = ping.getRadius();
    const float PING_TOLERANCE = 35.0f;

    // Only arm if not already armed and ping just hit
    if (!lastPingHit && !armed && radius >= distance - PING_TOLERANCE)
    {
        lastPingHit = true;
        arm(); // sets armed = true and confirmTimer
        gHitQueue.add(this);
    }

    if (!ping.isActive()) lastPingHit = false;
}

void SoundObject::play()
{
    PlaySound(noteSound);
    hitTimer = HIT_DURATION;
    currentColor = WHITE;

    disarm();
    lastPingHit = true;
}

void SoundObject::draw() const
{
    DrawCircleV(position, 15.0f, currentColor);
    DrawCircleLinesV(position, 17.0f, DARKGRAY);

    if (armed)
        DrawCircleLinesV(position, 20.0f, GRAY);
}
