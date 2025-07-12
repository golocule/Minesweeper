//#include "Textures.h"
#include "tile.h"
#include <vector>
#include <cstdlib>

class board {
    int mines;
    int rows;
    int cols;
    int currMines;
    bool lost;
    vector<vector<tile>> boardVector;
public:
    void setNeighbors(tile &T) {

    }
    board(int mines, int rows, int cols) {
        this->mines = mines;
        this->rows = rows;
        this->cols = cols;
        this->currMines = mines;
        this->lost = false;
        boardVector.resize(cols);
    }
    void setBoard() {
        // creating the 2d vector for the board with all the tile objects
        for (int x = 0; x < cols; x++) {
            for (int y = 0; y < rows; y++) {
                vector<float> tilePos = {x*32.0f, y*32.0f};
                //boardVector[x][y] = tile("tile_hidden", tilePos);
                boardVector[x].push_back(tile("tile_hidden", tilePos));
            }
        }
        // randomly set up mines
        int count = mines;
        while (count > 0) {
            bool valid = true;
            vector<vector<int>> chosenPos;
            int rowRand = rand()%(rows);
            int colRand = rand()%(cols);
            for (int i = 0; i < chosenPos.size(); i++) {
                if (chosenPos[i] == vector<int>{rowRand, colRand}) {
                    valid = false;
                }
            }
            if (valid == true) {
                chosenPos.push_back({rowRand, colRand});
                boardVector[colRand][rowRand].changeType("mine");
                count -= 1;
            }
        }
        // setting up neighbors for each tile
        for (int x = 0; x < cols; x++) {
            for (int y = 0; y < rows; y++) {
                vector<float> tilePos = {x*32.0f, y*32.0f};
                tile &currentTile = boardVector[x][y];
                if (x < cols - 1 && y < rows - 1) {
                    currentTile.setNeighbor(&boardVector[x+1][y+1]);
                }
                if (y < rows - 1) {
                    currentTile.setNeighbor(&boardVector[x][y+1]);
                }
                if (x < cols - 1) {
                    currentTile.setNeighbor(&boardVector[x+1][y]);
                }
                if (x > 0 && y < rows - 1 ) {
                    currentTile.setNeighbor(&boardVector[x-1][y+1]);
                }
                if (x > 0) {
                    currentTile.setNeighbor(&boardVector[x-1][y]);
                }
                if (x > 0 && y > 0) {
                    currentTile.setNeighbor(&boardVector[x-1][y-1]);
                }
                if (y > 0) {
                    currentTile.setNeighbor(&boardVector[x][y-1]);
                }
                if (x < cols - 1 && y > 0) {
                    currentTile.setNeighbor(&boardVector[x+1][y-1]);
                }
            }
        }
    };
    vector<vector<tile>>& getBoardVector() {
        return boardVector;
    }
    bool checkLose() {
        return lost;
    }
    bool checkWin() {
        bool win = false;
        for (int x = 0; x < cols; x++) {
            for (int y = 0; y < rows; y++) {
                vector<float> tilePos = {x*32.0f, y*32.0f};
                if (boardVector[x][y].getType() == "tile_hidden") {
                    return false;
                }
            }
        }
        // flag all the mine tiles when win is detected
        for (int x = 0; x < cols; x++) {
            for (int y = 0; y < rows; y++) {
                vector<float> tilePos = {x*32.0f, y*32.0f};
                if (boardVector[x][y].getType() == "mine" && boardVector[x][y].isflagged() == false) {
                    boardVector[x][y].flagTile();
                }
            }
        }
        return true;
    }
    void revealTiles(tile &T) {
        if (T.getType() == "mine") {
            lost = true;
        }
        if (T.getType() == "tile_hidden") {
            if (T.getMineNeighbors() == 0) {
                T.changeType("tile_revealed");
                //run recursive function here
                vector<tile*> Tneighbors = T.getNeighbors();
                for (int i = 0; i < Tneighbors.size(); i++) {
                    revealTiles(*Tneighbors[i]);
                }
            } else {
                T.changeType("number_" + to_string(T.getMineNeighbors()));
            }
        }
    }
    void updateBoard(sf::Vector2<int> mPos, string mType) {
        for (int x = 0; x < cols; x++) {
            for (int y = 0; y < rows; y++) {
                vector<float> tilePos = {x * 32.0f, y * 32.0f};
                if ((mPos.x < tilePos[0] + 32.0f && mPos.y < tilePos[1] + 32.0f) &&
                    (mPos.x > tilePos[0] && mPos.y > tilePos[1])) {
                    if (mType == "L" && boardVector[x][y].isflagged() == false) {
                        revealTiles(boardVector[x][y]);
                    }
                    if (mType == "R") {
                        if (boardVector[x][y].isflagged()) {
                            boardVector[x][y].flagTile();
                            currMines += 1;
                        } else if (boardVector[x][y].getType() == "tile_hidden" ||
                                   boardVector[x][y].getType() == "mine") {
                            boardVector[x][y].flagTile();
                            currMines -= 1;
                        }
                    }
                }
            }
        }
    }
    int getMineCount() {
        return currMines;
    }
    void resetBoard() {
        lost = false;
        currMines = mines;
        for (int x = 0; x < cols; x++) {
            for (int y = 0; y < rows; y++) {
                if (boardVector[x][y].isflagged()) {
                    boardVector[x][y].flagTile();
                }
                boardVector[x][y].changeType("tile_hidden");
                boardVector[x][y].clearNeighbors();
            }
        }
        // randomly set up mines
        int count = mines;
        while (count > 0) {
            bool valid = true;
            vector<vector<int>> chosenPos;
            int rowRand = rand()%(rows);
            int colRand = rand()%(cols);
            for (int i = 0; i < chosenPos.size(); i++) {
                if (chosenPos[i] == vector<int>{rowRand, colRand}) {
                    valid = false;
                }
            }
            if (valid == true) {
                chosenPos.push_back({rowRand, colRand});
                boardVector[colRand][rowRand].changeType("mine");
                count -= 1;
            }
        }
        // setting up neighbors for each tile
        for (int x = 0; x < cols; x++) {
            for (int y = 0; y < rows; y++) {
                vector<float> tilePos = {x*32.0f, y*32.0f};
                tile &currentTile = boardVector[x][y];
                if (x < cols - 1 && y < rows - 1) {
                    currentTile.setNeighbor(&boardVector[x+1][y+1]);
                }
                if (y < rows - 1) {
                    currentTile.setNeighbor(&boardVector[x][y+1]);
                }
                if (x < cols - 1) {
                    currentTile.setNeighbor(&boardVector[x+1][y]);
                }
                if (x > 0 && y < rows - 1 ) {
                    currentTile.setNeighbor(&boardVector[x-1][y+1]);
                }
                if (x > 0) {
                    currentTile.setNeighbor(&boardVector[x-1][y]);
                }
                if (x > 0 && y > 0) {
                    currentTile.setNeighbor(&boardVector[x-1][y-1]);
                }
                if (y > 0) {
                    currentTile.setNeighbor(&boardVector[x][y-1]);
                }
                if (x < cols - 1 && y > 0) {
                    currentTile.setNeighbor(&boardVector[x+1][y-1]);
                }
            }
        }
    }
};


#ifndef PROJECT3FILE_BOARD_H
#define PROJECT3FILE_BOARD_H

#endif //PROJECT3FILE_BOARD_H
