#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <cctype>
#include "Textures.h"
#include "board.h"
#include "tile.h"
#include <cmath>
#include <chrono>
#include <sstream>
#include <algorithm>
using namespace std;


void setText(sf::Text &text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

int main() {

    Textures textures;

    ifstream leaderFile("files/leaderboard.txt");
    ifstream configFile("files/config.cfg");
    string line;
    map<int, string> leaderMap;
    vector<int> scores;
    while (getline(leaderFile, line)) {
        string person;
        string time;
        stringstream ss(line);
        getline(ss, time, ',');
        getline(ss, person, ',');

        time.erase(remove(time.begin(), time.end(), ':'), time.end());
        person.erase(remove(person.begin(), person.end(), ' '), time.end());
        int intTime = stoi(time.substr(0, 2))*60;
        intTime += (stoi(time.substr(2,2)));
        leaderMap[intTime] = person;
        scores.push_back(intTime);
    }
    string data;

    getline(configFile, data);
    int colCount = stoi(data);
    getline(configFile, data);
    int rowCount = stoi(data);
    getline(configFile, data);
    int mines = stoi(data);
    int currentMines = mines;


    float width = colCount*32;
    float height = (rowCount*32) + 100;

    sf::RenderWindow welcomeWind(sf::VideoMode((colCount*32), (rowCount*32) + 100), "Minesweeper", sf::Style::Close);
    bool startGameWind = false;
    string username;

    while(welcomeWind.isOpen()) {
        sf::Event event;

        //welcome text info
        sf::Font welcomeFont;
        welcomeFont.loadFromFile("files/font.ttf");
        sf::Text welcomeText("WELCOME TO MINESWEEPER!", welcomeFont, 24);
        welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
        welcomeText.setFillColor(sf::Color::White);
        setText(welcomeText, width/2, (height/2)-150);

        //prompt text info
        sf::Text promptText("Enter your name:", welcomeFont, 20);
        promptText.setStyle(sf::Text::Bold);
        promptText.setFillColor(sf::Color::White);
        setText(promptText, width/2, (height/2)-75);

        //username text info
        sf::Text nameText("", welcomeFont, 18);
        nameText.setStyle(sf::Text::Bold);
        nameText.setFillColor(sf::Color::Yellow);



        while(welcomeWind.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                welcomeWind.close();
            }
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 8) {
                    if (!username.empty()) {
                        username.pop_back();
                    }
                } else if (event.text.unicode < 128) {
                    if (event.text.unicode == 10 && !username.empty()) {
                        startGameWind = true;
                        welcomeWind.close();
                    }
                    if (username.length() < 10 && isalpha(static_cast<char>(event.text.unicode))) {
                        username += static_cast<char>(event.text.unicode);
                    }
                }
            }
        }

        welcomeWind.clear(sf::Color::Blue);

        username[0] = toupper(username[0]);
        nameText.setString(username + "|");
        setText(nameText, width/2, (height/2)-45);

        welcomeWind.draw(welcomeText);
        welcomeWind.draw(promptText);
        welcomeWind.draw(nameText);

        welcomeWind.display();
    }

    bool debugEnabled = false;
    bool playing = true;
    bool won = false;
    bool lost = false;
    bool paused = false;
    double timer = 0;
    int mins = 0;
    int seconds = 0;
    bool displayleaders = false;



    if (startGameWind) {
        //button sprites
        sf::RenderWindow Minesweeper(sf::VideoMode((colCount*32), (rowCount*32) + 100), "Minesweeper", sf::Style::Close);
        board gameBoard(mines, rowCount, colCount);
        sf::Sprite happyFace(textures.getTexture("face_happy"));
        happyFace.setPosition(((colCount/2.0f)*32.0f) - 32.0f, 32.0f*(rowCount+0.5f));
        sf::Sprite loseFace(textures.getTexture("face_lose"));
        loseFace.setPosition(((colCount/2.0f)*32.0f) - 32.0f, 32.0f*(rowCount+0.5f));
        sf::Sprite winFace(textures.getTexture("face_win"));
        winFace.setPosition(((colCount/2.0f)*32.0f) - 32.0f, 32.0f*(rowCount+0.5f));
        sf::Sprite pauseB(textures.getTexture("pause"));
        pauseB.setPosition((colCount*32.0f)-240.0f, 32.0f*(rowCount+0.5f));
        sf::Sprite playB(textures.getTexture("play"));
        playB.setPosition((colCount*32.0f)-240.0f, 32.0f*(rowCount+0.5f));
        sf::Sprite leaderboard(textures.getTexture("leaderboard"));
        leaderboard.setPosition((colCount*32.0f)-176.0f, 32.0f*(rowCount+0.5f));


        //digit sprites:
        sf::Sprite digit0(textures.getTexture("digits"));
        digit0.setTextureRect(sf::IntRect(0, 0, 21, 32));

        sf::Sprite digit1(textures.getTexture("digits"));
        digit1.setTextureRect(sf::IntRect(21*1, 0, 21, 32));


        sf::Sprite digit2(textures.getTexture("digits"));
        digit2.setTextureRect(sf::IntRect(21*2, 0, 21, 32));

        sf::Sprite digit3(textures.getTexture("digits"));
        digit3.setTextureRect(sf::IntRect(21*3, 0, 21, 32));

        sf::Sprite digit4(textures.getTexture("digits"));
        digit4.setTextureRect(sf::IntRect(21*4, 0, 21, 32));

        sf::Sprite digit5(textures.getTexture("digits"));
        digit5.setTextureRect(sf::IntRect(21*5, 0, 21, 32));

        sf::Sprite digit6(textures.getTexture("digits"));
        digit6.setTextureRect(sf::IntRect(21*6, 0, 21, 32));

        sf::Sprite digit7(textures.getTexture("digits"));
        digit7.setTextureRect(sf::IntRect(21*7, 0, 21, 32));

        sf::Sprite digit8(textures.getTexture("digits"));
        digit8.setTextureRect(sf::IntRect(21*8, 0, 21, 32));

        sf::Sprite digit9(textures.getTexture("digits"));
        digit9.setTextureRect(sf::IntRect(21*9, 0, 21, 32));

        sf::Sprite digitNeg(textures.getTexture("digits"));
        digitNeg.setTextureRect(sf::IntRect(21*10, 0, 21, 32));

        map<string, sf::Sprite> digitMap;
        digitMap["digit0"] = digit0;
        digitMap["digit1"] = digit1;
        digitMap["digit2"] = digit2;
        digitMap["digit3"] = digit3;
        digitMap["digit4"] = digit4;
        digitMap["digit5"] = digit5;
        digitMap["digit6"] = digit6;
        digitMap["digit7"] = digit7;
        digitMap["digit8"] = digit8;
        digitMap["digit9"] = digit9;
        digitMap["digitNeg"] = digitNeg;


        gameBoard.setBoard();

        while(Minesweeper.isOpen()) {
            auto start = chrono::high_resolution_clock::now();
            sf::Event event;
            //create graphics here

            while(Minesweeper.pollEvent(event)) {
                // look for events here
                if (event.type == sf::Event::Closed) {
                    Minesweeper.close();
                }
                if (event.type == sf::Event::MouseButtonPressed) {
                    auto mPosition = sf::Mouse::getPosition(Minesweeper);
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (playing == true) {
                            if (paused == false) {
                                gameBoard.updateBoard(mPosition, "L");
                                if (gameBoard.checkWin()) {
                                    playing = false;
                                    won = true;
                                } else if(gameBoard.checkLose()) {
                                    playing = false;
                                    lost = true;
                                    debugEnabled = true;
                                }
                            }
                        }

                        // to check the other buttons for left click
                        //reseting the board (smiley face)
                        if ((mPosition.x < winFace.getPosition().x + 64.0f && mPosition.y < winFace.getPosition().y + 64.0f) && (mPosition.x > winFace.getPosition().x && mPosition.y > winFace.getPosition().y)) {
                            gameBoard.resetBoard();
                            playing = true;
                            paused = false;
                            won = false;
                            lost = false;
                            debugEnabled = false;
                            timer = 0;
                        }
                        // pause button
                        if (((mPosition.x < pauseB.getPosition().x + 64.0f && mPosition.y < pauseB.getPosition().y + 64.0f) && (mPosition.x > pauseB.getPosition().x && mPosition.y > pauseB.getPosition().y)) && playing == true) {
                            if (paused) {
                                paused = false;
                            } else {
                                paused = true;
                            }
                        }
                        // leaderboard button
                        if (((mPosition.x < leaderboard.getPosition().x + 64.0f && mPosition.y < leaderboard.getPosition().y + 64.0f) && (mPosition.x > leaderboard.getPosition().x && mPosition.y > leaderboard.getPosition().y))) {
                            displayleaders = true;
                            if (playing == true) {
                                paused = true;
                            }
                        }

                    } else if (event.mouseButton.button == sf::Mouse::Right) {
                        if (playing == true && paused == false) {
                            gameBoard.updateBoard(mPosition, "R");
                        }
                    }
                }
            }
            currentMines = gameBoard.getMineCount();
            Minesweeper.clear(sf::Color::White);

            // draws the board with the current version
            vector<vector<tile>> boardVect = gameBoard.getBoardVector();
            for (int i = 0; i < boardVect.size(); i++) {
                for (int j = 0; j < boardVect[i].size(); j++) {
                    vector<sf::Sprite> tileSprites = boardVect[i][j].getTileSprites();
                    if (paused == false) {
                        if (boardVect[i][j].getType() == "mine" && boardVect[i][j].isflagged() == false && debugEnabled == false) {
                            sf::Sprite hidden(textures.getTexture("tile_hidden"));
                            hidden.setPosition(boardVect[i][j].getPosition()[0], boardVect[i][j].getPosition()[1]);
                            Minesweeper.draw(hidden);
                        } else {
                            for (int k = 0; k < tileSprites.size(); k++) {
                                sf::Sprite tileSprite = tileSprites[k];
                                vector<float> SpritePos = boardVect[i][j].getPosition();
                                tileSprite.setPosition(SpritePos[0], SpritePos[1]);
                                Minesweeper.draw(tileSprite);
                            }
                        }
                    } else {
                        sf::Sprite revealed(textures.getTexture("tile_revealed"));
                        revealed.setPosition(boardVect[i][j].getPosition()[0], boardVect[i][j].getPosition()[1]);
                        Minesweeper.draw(revealed);
                    }
                }
            }

            // draw the other buttons here

            if (playing == true) {
                Minesweeper.draw(happyFace);
            } else if (lost){
                Minesweeper.draw(loseFace);
            } else {
                Minesweeper.draw(winFace);
            }
            if (paused == false) {
                Minesweeper.draw(pauseB);
            } else {
                Minesweeper.draw(playB);
            }
            Minesweeper.draw(leaderboard);

            //draw everything here

            //mine counter drawing and positioning
            string MineCount1 = to_string(abs(currentMines)%10);
            string MineCount2 = to_string((abs(currentMines)/10)%10);
            string MineCount3 = to_string((abs(currentMines)/100)%10);
            digitMap["digit" + MineCount3].setPosition(33.0f, (32.0f*(rowCount+0.5f))+16.0f);
            Minesweeper.draw(digitMap["digit" + MineCount3]);
            digitMap["digit" + MineCount2].setPosition(54.0f, (32.0f*(rowCount+0.5f))+16.0f);
            Minesweeper.draw(digitMap["digit" + MineCount2]);
            digitMap["digit" + MineCount1].setPosition(75.0f, (32.0f*(rowCount+0.5f))+16.0f);
            Minesweeper.draw(digitMap["digit" + MineCount1]);
            if (currentMines < 0) {
                digitNeg.setPosition(12.0f, (32.0f*(rowCount+0.5f))+16.0f);
                Minesweeper.draw(digitNeg);
            }


            //drawing the timer
            int intTime = static_cast<int>(timer);
            mins = intTime/60;
            seconds = intTime%60;

            digitMap["digit" + to_string((seconds/10)%10)].setPosition((colCount*32.0f)-54.0f, (rowCount*32.0f)+16.0f);
            Minesweeper.draw(digitMap["digit" + to_string((seconds/10)%10)]);

            digitMap["digit" + to_string(seconds%10)].setPosition((colCount*32.0f)-33.0f, (rowCount*32.0f)+16.0f);
            Minesweeper.draw(digitMap["digit" + to_string(seconds%10)]);

            digitMap["digit" + to_string((mins/10)%10)].setPosition((colCount*32.0f)-97.0f, (rowCount*32.0f)+16.0f);
            Minesweeper.draw(digitMap["digit" + to_string((mins/10)%10)]);

            digitMap["digit" + to_string(mins%10)].setPosition((colCount*32.0f)-76.0f, (rowCount*32.0f)+16.0f);
            Minesweeper.draw(digitMap["digit" + to_string(mins%10)]);

            Minesweeper.display();

            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double> duration = end - start;
            if (paused == false && playing == true) {
                timer += duration.count();
            }


            if (displayleaders) {
                sf::RenderWindow leaderboardWind(sf::VideoMode((colCount*16), (rowCount*16) + 50), "Leaderboard", sf::Style::Close);
                int leadWidth = (colCount*16);
                int leadHeight = (rowCount*16) + 50;

                sf::Font welcomeFont;
                welcomeFont.loadFromFile("files/font.ttf");
                sf::Text titleText("LEADERBOARD", welcomeFont, 20);
                titleText.setStyle(sf::Text::Bold | sf::Text::Underlined);
                titleText.setFillColor(sf::Color::White);
                setText(titleText, leadWidth/2, (leadHeight/2)-120);



                if (won == true) {
                    scores.push_back(static_cast<int>(timer));
                    leaderMap[static_cast<int>(timer)] = username + "*";
                }
                sort(scores.begin(), scores.end());
                //reverse(scores.begin(), scores.end());
                auto it = leaderMap.begin();
                string leaderText = "";
                for (int i = 0; i < 5; i++) {
                    leaderText += ((to_string(i+1)) + ".\t");
                    if (scores[i]/60 < 10) {
                        leaderText += "0";
                    }
                    leaderText += (to_string(scores[i]/60)) + ":";
                    if (scores[i]%60 < 10) {
                        leaderText += "0";
                    }
                    leaderText += (to_string(scores[i]%60)) + "\t" + leaderMap[scores[i]] + "\n\n";
                }

                sf::Text leaderboardText(leaderText, welcomeFont, 18);
                leaderboardText.setStyle(sf::Text::Bold);
                leaderboardText.setFillColor(sf::Color::White);
                setText(leaderboardText, leadWidth/2, (leadHeight/2)+20);

                while(leaderboardWind.isOpen()) {
                    sf::Event event;
                    while(leaderboardWind.pollEvent(event)) {
                        if (event.type == sf::Event::Closed) {
                            leaderboardWind.close();
                            paused = false;
                            displayleaders = false;
                        }
                        // events here
                    }
                    leaderboardWind.clear(sf::Color::Blue);
                    // draw stuff
                    leaderboardWind.draw(leaderboardText);
                    leaderboardWind.draw(titleText);

                    leaderboardWind.display();
                }
            }


        }
    }
    return 0;
}