#include <iostream>
#include <vector>
#include <memory>
#include "raylib.h"
#include "../include/Player.h"
#include "../include/Ping.h"
#include "../include/SoundObject.h"
#include "../include/Level.h"
#include "../include/PingHitQueue.h" // include the queue

int main() {
    // Initialization
    const int screenWidth = 1280;
    const int screenHeight = 960;
    InitWindow(screenWidth, screenHeight, "Resonance");
    InitAudioDevice();
    if (!IsAudioDeviceReady()) std::cerr << "Audio device not ready!\n";

    Level level;
    level.loadFromCSV("../assets/levels/level1.csv");
    Player player;
    player.setTilemap(&level.tiles);

    std::unique_ptr<Ping> activePing;

    // Sound objects
    std::vector<std::unique_ptr<SoundObject>> soundObjects;
    auto addSoundObject = [&](Vector2 pos, const std::string& path, Color color) {
        soundObjects.push_back(std::make_unique<SoundObject>(pos, path, color));
    };

    addSoundObject({150.0f, 250.0f}, "../assets/sounds/c_note.wav", RED);
    addSoundObject({500.0f, 100.0f}, "../assets/sounds/d_note.wav", BLUE);
    addSoundObject({800.0f, 500.0f}, "../assets/sounds/e_note.wav", GREEN);
    addSoundObject({300.0f, 700.0f}, "../assets/sounds/f_note.wav", PURPLE);

    SetTargetFPS(60);

    // Main loop
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        player.update(dt);

        // Update sound objects (flash, timers, etc.)
        for (auto& obj : soundObjects)
            obj->update(dt);

        // Spawn ping
        if (IsKeyPressed(KEY_SPACE) && !activePing)
            activePing = std::make_unique<Ping>(player.getPosition());

        // Update active ping and arm notes
        if (activePing) {
            activePing->update(dt);
            for (auto& obj : soundObjects)
                obj->checkPingHit(*activePing);

            if (!activePing->isActive())
                activePing.reset();
        }

        // Update queue timers and handle player input
        gHitQueue.update(dt);
        gHitQueue.tryPlayerInput(KEY_J); // this ensures 1 note per key

        // Draw
        BeginDrawing();
        ClearBackground(DARKBLUE);
        level.draw();
        for (const auto& obj : soundObjects)
            obj->draw();
        if (activePing)
            activePing->draw();
        player.draw();
        DrawText("Controls: WASD to Move | SPACE to PING | J/K/L to play notes", 10, 10, 20, RAYWHITE);
        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
