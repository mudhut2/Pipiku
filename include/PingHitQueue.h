#ifndef PINGHITQUEUE_H
#define PINGHITQUEUE_H

#include <deque>
#include "raylib.h"

class SoundObject;

struct QueuedHit {
    SoundObject* obj;
    float timeRemaining;
};

class PingHitQueue {
private:
    std::deque<QueuedHit> queue;

public:
    void add(SoundObject* obj);
    void update(float dt);
    void tryPlayerInput(int key);
};

extern PingHitQueue gHitQueue;

#endif
