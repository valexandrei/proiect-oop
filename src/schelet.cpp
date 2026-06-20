#include "schelet.h"
#include <iostream>

Schelet::Schelet(const std::string& n, Pozitie p, int health, int dmg)
    : Entitate(n, p, health, dmg, 0), aReinviat(false), dmgOase(dmg) {}

void Schelet::actioneaza() {
    std::cout << "[Schelet] " << getNume()
              << " zornaire oasele la " << getPozitie() << ".\n";
}

void Schelet::primesteDamageSchelet(int dmg) {
    primesteDamage(dmg);
    if (!esteViu() && !aReinviat) {
        aReinviat = true;
        setHP(getHPMax() / 2);
        std::cout << "[Schelet] " << getNume()
                  << " reInvie cu " << getHP() << " HP!\n";
    }
}