#ifndef INAMICFACTORY_H
#define INAMICFACTORY_H

#include <memory>
#include <string>
#include "entitate.h"
#include "pozitie.h"

class InamicFactory {
public:
    enum class Tip {
        Goblin,
        Orc,
        Fantoma,
        Vrajitor,
        Schelet,
        Boss
    };

    static std::unique_ptr<Entitate> creeaza(Tip tip, const std::string& nume,
                                              Pozitie pos, int hp, int dmg = 0);
    static std::string numeImplicit(Tip tip);
};

#endif