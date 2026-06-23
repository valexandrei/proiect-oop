#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "jocdungeon.h"
#include "inventar.h"
#include "renderer.h"
#include "radar.h"

class GameLogic {
public:
    static void initLogica(JocDungeon& joc, Inventar& rucsac);
    static void populeazaNivel(JocDungeon& joc, int nivelCurent);
    static void treceLaNivelUrmator(JocDungeon& joc, int& nivelCurent,
                                     std::vector<std::string>& logLinii,
                                     std::string& message, sf::Clock& msgClock);
    static void procesezaCombat(JocDungeon& joc,
                                 std::vector<FloatText>& floats,
                                 Renderer& renderer,
                                 std::vector<std::string>& logLinii,
                                 int& nivelAnterior);
    static void miseazaInamici(JocDungeon& joc);
    static bool suntAdiacente(const Pozitie& a, const Pozitie& b);
    static void ruleazaTesteCLI(JocDungeon& joc, Radar& radar);
};

#endif