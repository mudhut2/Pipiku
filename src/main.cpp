#include <iostream>
#include <vector>
#include <memory>
#include "raylib.h"
#include "raymath.h" // Needed for Vector2Distance, Vector2Add

#include "../include/Player.h"
#include "../include/Ping.h"
#include "../include/SoundObject.h"
#include "../include/Level.h"
#include "../include/PingHitQueue.h"
#include "../include/PlayTargetMelodyObject.h"

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 960;
    InitWindow(screenWidth, screenHeight, "Pipiku");

    InitAudioDevice();
    if (!IsAudioDeviceReady())
        std::cerr << "Audio device not ready!\n";

    // --- WORLD SETUP ---------------------------------------------------------
    Level level;
    level.loadFromCSV("../assets/levels/level1.csv");

    Player player;
    player.setTilemap(&level.tiles);

    std::unique_ptr<Ping> activePing;

    // --- SOUND OBJECTS --------------------------------------------------------
    std::vector<std::unique_ptr<SoundObject>> soundObjects;

    auto addSoundObject = [&](Vector2 pos, const std::string& path, Color color, bool moveable) {
        soundObjects.push_back(std::make_unique<SoundObject>(pos, path, color, moveable));
    };

    // Note: Use indices 0-3 for C, D, E, F sounds
    addSoundObject({180.0f, 250.0f}, "../assets/sounds/c_note.wav", RED, false); // Index 0 (Fixed)
    addSoundObject({930.0f, 415.0f}, "../assets/sounds/d_note.wav", BLUE, true);  // Index 1 (Moveable)
    addSoundObject({500.0f, 100.0f}, "../assets/sounds/e_note.wav", GREEN, false); // Index 2 (Fixed)
    addSoundObject({990.0f, 355.0f}, "../assets/sounds/f_note.wav", PURPLE, true); // Index 3 (Moveable)

    // --- PLAY TARGET MELODY OBJECT SETUP (MODIFIED) --------------------------

    // 1. Extract the individual sounds for easy reference
    Sound cNote = soundObjects[0]->getNoteSound();
    Sound dNote = soundObjects[1]->getNoteSound();
    Sound eNote = soundObjects[2]->getNoteSound();
    Sound fNote = soundObjects[3]->getNoteSound();

    // 2. Define the melody sequence using the MelodyStep structure
    //    Format: {{Sound1, Sound2, ...}, Duration to next step}
    std::vector<MelodyStep> ptmoMelodySequence = {
        // Step 1: Single note (C)
        {{cNote}, 0.5f},

        // Step 2: Chord (D and F together)
        {{dNote, fNote}, 0.7f},

        // Step 3: Single note (E)
        {{eNote}, 0.4f},

        // Step 4: Final Chord (C and E together)
        {{cNote, eNote}, 1.0f}
    };

    // 3. Initialize PTMO with the new sequence structure
    PlayTargetMelodyObject ptmo({600.0f, 300.0f}, ptmoMelodySequence, WHITE);

    SetTargetFPS(60);

    // --- GAME LOOP ------------------------------------------------------------
    const float PICKUP_RADIUS = 30.0f; // Max distance for the player to grab
    const float OBJECT_HOLD_OFFSET = 35.0f; // Distance object sits in front

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // PLAYER
        player.update(dt);

        if (IsKeyPressed(KEY_E))
        {
            float ptmoDistance = Vector2Distance(player.position, ptmo.getPosition());

            // 1. CHECK FOR PTMO PLAY
            if (ptmoDistance <= PICKUP_RADIUS)
            {
                // Player is over the fixed PTMO. Start the melody sequence.
                ptmo.play(); // This now STARTS the timed sequence
            }
            else if (player.heldObject != nullptr)
            {
                // 2. DROP THE OBJECT
                player.heldObject->setHeld(false);
                player.heldObject = nullptr;
            }
            else
            {
                // 3. TRY TO PICK UP A MOVEABLE OBJECT
                for (const auto& objPtr : soundObjects)
                {
                    SoundObject* object = objPtr.get();

                    if (!object->getIsMoveable()) continue;
                    float distance = Vector2Distance(player.position, object->getPosition());

                    if (distance <= PICKUP_RADIUS)
                    {
                        player.heldObject = object;
                        player.heldObject->setHeld(true);
                        break;
                    }
                }
            }
        }

        // 3. MOVE HELD OBJECT
        if (player.heldObject != nullptr)
        {
            Vector2 offset = {
                cosf(player.angle) * OBJECT_HOLD_OFFSET,
                sinf(player.angle) * OBJECT_HOLD_OFFSET
            };

            Vector2 newPos = Vector2Add(player.position, offset);
            player.heldObject->setPosition(newPos);
        }

        // SOUND OBJECTS
        for (auto& obj : soundObjects)
            obj->update(dt);

        // CREATE NEW PING
        if (IsKeyPressed(KEY_SPACE) && !activePing)
            activePing = std::make_unique<Ping>(player.getPosition());

        // UPDATE ACTIVE PING + CHECK INTERACTIONS
        if (activePing) {
            activePing->update(dt);

            // SoundObjects react to ping
            for (auto& obj : soundObjects)
                obj->checkPingHit(*activePing);

            // PTMO reacts to ping (arms itself if ping overlaps)
            ptmo.checkPingHit(*activePing);

            if (!activePing->isActive())
                activePing.reset();
        }

        // PROCESS PLAYER NOTE INPUT
        gHitQueue.update(dt);
        gHitQueue.tryPlayerInput(KEY_D); // normal notes

        // PTMO UPDATE (MODIFIED)
        ptmo.update(dt);        // Handles flash timer
        ptmo.updateMelody(dt);  // NEW: Handles the sequential, timed playback

        // --- DRAW -------------------------------------------------------------
        BeginDrawing();
        ClearBackground(DARKBLUE);

        level.draw();
        ptmo.draw(); // draw PTMO

        for (const auto& obj : soundObjects)
            obj->draw();

        if (activePing)
            activePing->draw();

        player.draw();

        DrawText("Controls: Right-Click to Move | SPACE = Ping | D = Note | E = Play PTMO / Pickup/Drop",
                 10, 10, 20, RAYWHITE);

        EndDrawing();
    }

    // --- SHUTDOWN -------------------------------------------------------------
    CloseAudioDevice();
    CloseWindow();
    return 0;
}