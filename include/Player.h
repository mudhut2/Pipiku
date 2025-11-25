//
// Created by hgrov on 10/30/2025.
//

#ifndef PLAYER_H
#define PLAYER_H
#include "raylib.h"
#include "../include/Tile.h"
#include<vector>

class Player {
public:
    Player();
    void draw();
    void update(float deltaTime);
    Vector2 position;
    Vector2 velocity;
    float angle = 0.0f;
    float thrustPower = 200.0f;  // forward/backward force
    float turnSpeed = 2.5f;
    float drag = 0.3f;          // negative y is up in raylib
    float flapTimer = 0.0f;

    Texture2D playerTexture;
    int frameWidth;
    int frameHeight;

    int currentFrame;
    float frameTimer;
    float frameSpeed;

    Vector2 getPosition() const{return position;}
    bool canMoveTo(Vector2 newPos);
    const std::vector<std::vector<Tile>>* tilemap = nullptr;
    void setTilemap(const std::vector<std::vector<Tile>>* map);
private:
    float inputCooldown;
};


#endif //PLAYER_H
