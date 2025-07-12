//
// Created by Gage Carlson on 7/26/24.
//
#include <map>
using namespace std;
class Textures {
    map<string, sf::Texture> textureMap;
public:
    Textures() {
        sf::Texture debugTexture;
        debugTexture.loadFromFile("files/images/debug.png");
        textureMap["debug"] = debugTexture;

        sf::Texture digitsTexture;
        digitsTexture.loadFromFile("files/images/digits.png");
        textureMap["digits"] = digitsTexture;

        sf::Texture face_happyTexture;
        face_happyTexture.loadFromFile("files/images/face_happy.png");
        textureMap["face_happy"] = face_happyTexture;

        sf::Texture face_loseTexture;
        face_loseTexture.loadFromFile("files/images/face_lose.png");
        textureMap["face_lose"] = face_loseTexture;

        sf::Texture face_winTexture;
        face_winTexture.loadFromFile("files/images/face_win.png");
        textureMap["face_win"] = face_winTexture;

        sf::Texture flagTexture;
        flagTexture.loadFromFile("files/images/flag.png");
        textureMap["flag"] = flagTexture;

        sf::Texture leaderboardTexture;
        leaderboardTexture.loadFromFile("files/images/leaderboard.png");
        textureMap["leaderboard"] = leaderboardTexture;

        sf::Texture mineTexture;
        mineTexture.loadFromFile("files/images/mine.png");
        textureMap["mine"] = mineTexture;

        sf::Texture number_1Texture;
        number_1Texture.loadFromFile("files/images/number_1.png");
        textureMap["number_1"] = number_1Texture;

        sf::Texture number_2Texture;
        number_2Texture.loadFromFile("files/images/number_2.png");
        textureMap["number_2"] = number_2Texture;

        sf::Texture number_3Texture;
        number_3Texture.loadFromFile("files/images/number_3.png");
        textureMap["number_3"] = number_3Texture;

        sf::Texture number_4Texture;
        number_4Texture.loadFromFile("files/images/number_4.png");
        textureMap["number_4"] = number_4Texture;

        sf::Texture number_5Texture;
        number_5Texture.loadFromFile("files/images/number_5.png");
        textureMap["number_5"] = number_5Texture;

        sf::Texture number_6Texture;
        number_6Texture.loadFromFile("files/images/number_6.png");
        textureMap["number_6"] = number_6Texture;

        sf::Texture number_7Texture;
        number_7Texture.loadFromFile("files/images/number_7.png");
        textureMap["number_7"] = number_7Texture;

        sf::Texture number_8Texture;
        number_8Texture.loadFromFile("files/images/number_8.png");
        textureMap["number_8"] = number_8Texture;

        sf::Texture pauseTexture;
        pauseTexture.loadFromFile("files/images/pause.png");
        textureMap["pause"] = pauseTexture;

        sf::Texture playTexture;
        playTexture.loadFromFile("files/images/play.png");
        textureMap["play"] = playTexture;

        sf::Texture tile_hiddenTexture;
        tile_hiddenTexture.loadFromFile("files/images/tile_hidden.png");
        textureMap["tile_hidden"] = tile_hiddenTexture;

        sf::Texture tile_revealedTexture;
        tile_revealedTexture.loadFromFile("files/images/tile_revealed.png");
        textureMap["tile_revealed"] = tile_revealedTexture;
    }
    sf::Texture& getTexture(string name) {
        return textureMap[name];
    };
};

#ifndef PROJECT3FILE_WELCOMESCREEN_H
#define PROJECT3FILE_WELCOMESCREEN_H

#endif //PROJECT3FILE_WELCOMESCREEN_H
