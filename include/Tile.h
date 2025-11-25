#pragma once
#include <raylib.h>
#include <string>
#include <map>

enum class TileType {
    FLOOR,
    WALL,
    START,
    NOTE
};

const int TILE_SIZE = 64;

struct TileDef {
    TileType    type;
    std::string texturePath;
    bool        walkable;
    std::string label;   // optional metadata (kept for your note tiles)
};

inline std::map<int, TileDef> tileRegistry = {
    {0,  {TileType::FLOOR, "../assets/textures/cave_floor_2.png", true,  ""}},
    {1,  {TileType::FLOOR,  "../assets/textures/moss_stone_1.png", false, ""}},
    {2,  {TileType::WALL,  "../assets/textures/stone_1.png", false, ""}},
    {3,  {TileType::WALL,  "../assets/textures/moon_floor_2.png", false, ""}},
};

class Tile {
public:
    int id = 0;
    TileType type;
    Vector2 position = {0, 0};
    bool walkable = true;

    Texture2D texture{0};
    std::string label = "";

    Tile() = default;

    Tile(int id, Vector2 pos)
        : id(id), position(pos)
    {
        const TileDef& def = tileRegistry[id];

        type = def.type;
        walkable = def.walkable;
        label = def.label;

        texture = LoadTexture(def.texturePath.c_str());
    }

    // Non-copyable (protect texture ownership)
    Tile(const Tile&) = delete;
    Tile& operator=(const Tile&) = delete;

    // Movable
    Tile(Tile&& other) noexcept {
        *this = std::move(other);
    }

    Tile& operator=(Tile&& other) noexcept {
        if (this != &other) {
            if (texture.id != 0) UnloadTexture(texture);

            id = other.id;
            type = other.type;
            position = other.position;
            walkable = other.walkable;
            label = std::move(other.label);

            texture = other.texture;
            other.texture = {0};
        }
        return *this;
    }

    ~Tile() {
        if (texture.id != 0)
            UnloadTexture(texture);
    }

    void draw() const {
        Rectangle src = {0, 0, (float)texture.width, (float)texture.height};
        Rectangle dest = {position.x, position.y, (float)TILE_SIZE, (float)TILE_SIZE};
        DrawTexturePro(texture, src, dest, {0,0}, 0.0f, WHITE);
    }

    bool isWalkable() const { return walkable; }
};
