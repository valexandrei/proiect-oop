#include "inamic.h"
#include <iostream>

Inamic::Inamic(const std::string& n, Pozitie p, int health, int xp)
    : Entitate(n, p, health, 10, 2), xpReward(xp) {}

void Inamic::actioneaza() {
    std::cout << "Inamicul " << getNume()
              << " pandeste la ("
              << getPozitie().getX() << ","
              << getPozitie().getY() << ").\n";
}

void Inamic::ataca(Entitate& tinta) const {
    std::cout << getNume() << " ataca pe "
              << tinta.getNume() << "!\n";
    tinta.primesteDamage(calculeazaDamage());
}