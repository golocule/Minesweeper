#pragma once
//#include "Textures.h"
#include <vector>

class tile {
    string tileType;
    Textures textures;
    vector<tile*> neighbors;
    vector<float> position;
    bool flagged;
    int mineNeighbors;
public:
    tile(string tileType, vector<float> position) {
        this->tileType = tileType;
        this->position = position;
        this->mineNeighbors = 0;
        this->flagged = false;
    }
    void changeType(string tileType) {
        this->tileType = tileType;
    }
    vector<sf::Sprite> getTileSprites() {
        vector<sf::Sprite> sprites;

        if (flagged) {
            sf::Sprite flagSprite(textures.getTexture("flag"));
            sf::Sprite hiddenSprite(textures.getTexture("tile_hidden"));
            sprites.push_back(hiddenSprite);
            sprites.push_back(flagSprite);
        } else if (tileType == "tile_revealed") {
            sf::Sprite revealedTileSprite(textures.getTexture("tile_revealed"));
            sprites.push_back(revealedTileSprite);
        } else {
            sf::Sprite revealedTileSprite(textures.getTexture("tile_revealed"));
            sf::Sprite tileSprite(textures.getTexture(tileType));
            sprites.push_back(revealedTileSprite);
            sprites.push_back(tileSprite);
        }
        return sprites;
    }
    vector<float> getPosition() {
        return position;
    }
    void setNeighbor(tile* neighbor) {
        neighbors.push_back(neighbor);
        if (neighbor->getType() == "mine") {
            mineNeighbors += 1;
        }
    }
    void clearNeighbors() {
        neighbors.clear();
        mineNeighbors = 0;
    }
    vector<tile*> getNeighbors() {
        return neighbors;
    }
    int getMineNeighbors() {
        return mineNeighbors;
    }
    string getType() {
        return tileType;
    }
    bool isflagged() {
        return flagged;
    }
    void flagTile() {
        if (flagged) {
            flagged = false;
        } else {
            flagged = true;
        }
    }
};

#ifndef PROJECT3FILE_TILE_H
#define PROJECT3FILE_TILE_H

#endif //PROJECT3FILE_TILE_H
