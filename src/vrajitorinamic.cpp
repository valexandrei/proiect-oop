#include "vrajitorinamic.h"
#include <iostream>

VrajitorInamic::VrajitorInamic(const std::string& n, Pozitie p, int health,
                                int mana, int dmgMagie)
    : Entitate(n, p, health, 5, 1),
      manaMax(mana), manaCurenta(mana), damageMagie(dmgMagie) {}

void VrajitorInamic::actioneaza() {
    std::cout << "[Vrajitor] " << getNume()
              << " isi concentreaza energia magica la "
              << getPozitie() << ".\n";
}

int VrajitorInamic::calculeazaDamage() const {
    if (manaCurenta > manaMax / 2)
        return damageMagie;
    return damageMagie / 2;
}

void VrajitorInamic::aruncaVraja(Entitate& tinta) {
    if (!areMana()) {
        std::cout << "[Vrajitor] " << getNume()
                  << " nu are destula mana!\n";
        return;
    }
    int dmg = calculeazaDamage();
    manaCurenta -= 20;
    std::cout << "[Vrajitor] " << getNume()
              << " arunca o vraja asupra "
              << tinta.getNume() << " pentru "
              << dmg << " damage magic!\n";
    tinta.primesteDamage(dmg);
}