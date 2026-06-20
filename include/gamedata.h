#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <string>
#include <vector>
#include <map>

class GameData {
public:
    static std::string getDescriereInamic(const std::string& nume);
    static std::string getMesajLevelUp(int nivel);
    static std::vector<std::string> getTipuriInamici();
    static std::string getPovesteFundal();
};

#endif