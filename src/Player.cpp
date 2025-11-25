#include "raylib.h"
#include "../include/Player.h"
#include <cmath>
#include <iostream>
#include "../include/Tile.h"

Player::Player()
    : position{400.0f, 300.0f},
      velocity{0.0f, 0.0f},
      angle{0.0f},
      thrustPower{200.0f},
      drag{0.96},
      turnSpeed{PI / 4.0f},  // 45 degrees per step (8 directions)
      inputCooldown{0.0f}
{
    playerTexture = LoadTexture("../assets/textures/bat-sheet.png");
    if (playerTexture.id == 0) {
        std::cout << "Texture failed to load!" << std::endl;
    }

    frameWidth = playerTexture.width / 2;
    frameHeight = playerTexture.height;

    currentFrame = 0;
    frameTimer = 0.0f;
    frameSpeed = 0.15f;
}
void Player::update(float dt) {
    flapTimer += dt;

    // --- Get flap direction from held keys ---
    Vector2 flapDir = {0, 0};
    if (IsKeyDown(KEY_W)) flapDir.y -= 1.0f;
    if (IsKeyDown(KEY_S)) flapDir.y += 1.0f;
    if (IsKeyDown(KEY_A)) flapDir.x -= 1.0f;
    if (IsKeyDown(KEY_D)) flapDir.x += 1.0f;

    // Normalize direction
    if (flapDir.x != 0 || flapDir.y != 0)
    {
        float len = sqrtf(flapDir.x * flapDir.x + flapDir.y * flapDir.y);
        flapDir.x /= len;
        flapDir.y /= len;

        if (flapTimer >= 0.2f)
        {
            velocity.x += flapDir.x * thrustPower;   // no dt (impulse)
            velocity.y += flapDir.y * thrustPower;
            flapTimer = 0.0f;
        }
    }
    else
    {
        // If no key held, allow next press to flap immediately
        flapTimer = 0.6f;
    }

    velocity.x *= drag;
    velocity.y *= drag;

    Vector2 newPos;
    // X axis first
    newPos = { position.x + velocity.x * dt, position.y };
    if (canMoveTo(newPos)) {
        position.x = newPos.x;
    } else {
        velocity.x = 0;
    }
    // Y axis
    newPos = { position.x, position.y + velocity.y * dt };
    if (canMoveTo(newPos)) {
        position.y = newPos.y;
    } else {
        velocity.y = 0;
    }

    // auto-rotate to movement direction
    if (velocity.x != 0 || velocity.y != 0)
    {
        angle = atan2(velocity.y, velocity.x);
    }

    // Animation
    frameTimer += dt;
    if (frameTimer >= frameSpeed)
    {
        frameTimer = 0.0f;
        currentFrame++;
        if (currentFrame > 1)
            currentFrame = 0;
    }
}


void Player::draw() {
    // FIX 1: Increased size for better visibility
    float size = 80.0f; // diameter of submarine (Increased from 40.0f)

    // Source rectangle: entire texture
    Rectangle source = {
        (float)(currentFrame * frameWidth), // x position of frame
        0.0f,
        (float)frameWidth,
        (float)frameHeight
    };

    // Destination rectangle: square centered on player position
    Rectangle dest = {
        position.x,
        position.y,
        size,
        size
    };
    // Origin: The center point for rotation
    Vector2 origin = {
        size / 2.0f, // Center X of the destination
        size / 2.0f  // Center Y of the destination
    };
    // Convert angle (radians) to degrees for raylib
    float angleDegrees = angle * (180.0f / PI);

    // Original correction: +90.0f (to correct for asset pointing up/down vs atan2 pointing right)
    // FIX 2: Added +180.0f to flip the asset around
    float rotationDegrees = angleDegrees + 90.0f;
    // Draw the texture
    DrawTexturePro(playerTexture, source, dest, origin, rotationDegrees, WHITE);
}

bool Player::canMoveTo(Vector2 newPos) {
    const auto& tiles = *tilemap;

    int tileX = newPos.x / TILE_SIZE;
    int tileY = newPos.y / TILE_SIZE;

    if (tileY < 0 || tileX < 0 ||
        tileY >= tiles.size() || tileX >= tiles[0].size())
    {
        return false;
    }

    return tiles[tileY][tileX].isWalkable();
}

void Player::setTilemap(const std::vector<std::vector<Tile>>* map) {
    tilemap = map;
}