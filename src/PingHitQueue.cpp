#include "../include/PingHitQueue.h"
#include "../include/SoundObject.h"

PingHitQueue gHitQueue;

// Add a SoundObject to the queue (arm it)
void PingHitQueue::add(SoundObject* obj)
{
    queue.push_back({ obj, SoundObject::CONFIRM_WINDOW });
    obj->arm();
}

// Countdown timer
void PingHitQueue::update(float dt)
{
    for (auto it = queue.begin(); it != queue.end(); )
    {
        it->timeRemaining -= dt;

        if (it->timeRemaining <= 0.0f)
        {
            it->obj->disarm();
            it = queue.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

// Trigger all queued notes once with a single key
void PingHitQueue::tryPlayerInput(int key)
{
    if (!IsKeyPressed(key)) return;

    for (auto it = queue.begin(); it != queue.end(); )
    {
        it->obj->play();  // play note once
        it->obj->disarm();
        it = queue.erase(it);
    }
}
