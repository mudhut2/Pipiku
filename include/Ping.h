//
// Created by hgrov on 11/6/2025.
//

#ifndef PING_H
#define PING_H

#include "raylib.h"

class Ping {
public:
    // --- Configuration ---
    static constexpr float PING_MAX_RADIUS = 300.0f;
    static constexpr float PING_SPEED = 300.0f;
    static constexpr float PING_THICKNESS = 4.0f;
    static const Color PING_COLOR;

    Ping(Vector2 startPosition);
    void update(float dt);
    void draw();
    bool isActive() const { return active; }
    float getRadius() const { return radius; }
    Vector2 getCenter() const { return center; }
    void reset(Vector2 newPosition);
private:
    Vector2 center;
    float radius;
    bool active;
};

#endif // PING_H