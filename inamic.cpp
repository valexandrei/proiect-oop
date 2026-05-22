#include "inamic.h"
#include <iostream>

Inamic::Inamic(const std::string& n, Pozitie p, int health, int xpRew)
    : Entitate(n, p, health, 10, 2), xpReward(xpRew) {}

Inamic* Inamic::clone() const {
    return new Inamic(*this);
}

void Inamic::actioneaza() {
    std::cout << "[Inamic] " << nume
              << " pandeste la " << pos << ".\n";
}

int Inamic::calculeazaDamage() const {
    return atac;
}

void Inamic::ataca(Entitate& tinta) const {
    int dmg = calculeazaDamage();
    std::cout << nume << " ataca pe " << tinta.getNume()
              << " pentru " << dmg << " damage!\n";
    tinta.primesteDamage(dmg);
}

void Inamic::afisareImpl(std::ostream& os) const {
    os << "[Inamic] " << nume
       << " | HP: " << hp << "/" << hpMax
       << " | Damage: " << calculeazaDamage()
       << " | XP Reward: " << xpReward;
}