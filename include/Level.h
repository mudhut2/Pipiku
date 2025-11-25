#pragma once

#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "../include/Tile.h"
#include "../include/SoundObject.h"

class Level {
public:
    int width = 0;
    int height = 0;

    std::vector<std::vector<Tile>> tiles;
    std::vector<SoundObject*> notePattern;   // tiles in a melody order

    Level() = default;

    void loadFromCSV(const std::string& filename)
    {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Failed to open CSV: " << filename << std::endl;
            return;
        }
        tiles.clear();
        std::string line;
        int y = 0;

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string cell;
            std::vector<Tile> row;
            int x = 0;

            while (std::getline(ss, cell, ',')) {
                int id = std::stoi(cell);
                row.emplace_back(id, Vector2{ (float)x * TILE_SIZE, (float)y * TILE_SIZE });
                x++;
            }

            tiles.push_back(std::move(row));
            y++;
        }
        height = tiles.size();
        width = tiles.empty() ? 0 : tiles[0].size();
        std::cout << "Loaded Level: " << width << " x " << height << std::endl;
    }

    void draw() const
    {
        for (const auto& row : tiles) {
            for (const auto& tile : row) {
                tile.draw();
            }
        }
    }

    Tile& getTileAt(int x, int y)
    {
        return tiles[y][x];
    }

    void setTile(int x, int y, int registryId)
    {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return;

        tiles[y][x] = Tile(registryId, Vector2{ (float)x * TILE_SIZE,
                                                (float)y * TILE_SIZE });
    }

};
