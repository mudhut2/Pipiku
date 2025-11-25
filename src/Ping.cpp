//
// Created by hgrov on 11/6/2025.
//
#include "../include/Ping.h"
#include "../include/Player.h"
#include <cmath>

const Color Ping::PING_COLOR = { 100, 200, 255, 255 }; // Light Blue/Cyan

Ping::Ping(Vector2 startPosition)
    : center(startPosition),
      radius(0.0f),
      active(true)
{
}

void Ping::update(float dt) {
    if (!active) return;

    radius += PING_SPEED * dt;

    if (radius >= PING_MAX_RADIUS) {
        active = false;
    }
}

void Ping::draw()
{
    if (!active) return;

    float ratio = radius / PING_MAX_RADIUS;

    int alpha = (int)(255 * (1.0f - ratio));
    // Clamp alpha to 0-255 range
    if (alpha < 0) alpha = 0;

    Color drawColor = PING_COLOR;
    drawColor.a = (unsigned char)alpha;

    DrawCircleLines(
        (int)center.x,
        (int)center.y,
        radius,
        drawColor
    );
}

void Ping::reset(Vector2 newPosition)
{
    center = newPosition;
    radius = 0.0f;
    active = true;
}