#include "vrajitorinamic.h"
#include <iostream>

VrajitorInamic::VrajitorInamic(const std::string& n, Pozitie p, int health,
                                int mana, int dmgMagie)
    : Entitate(n, p, health, 5, 1),
      manaMax(mana), manaCurenta(mana), damageMagie(dmgMagie) {}

VrajitorInamic* VrajitorInamic::clone() const {
    return new VrajitorInamic(*this);
}

void VrajitorInamic::actioneaza() {
    std::cout << "[Vrajitor] " << nume
              << " isi concentreaza energia magica la " << pos << ".\n";
}

int VrajitorInamic::calculeazaDamage() const {
    if (manaCurenta > manaMax / 2)
        return damageMagie;
    return damageMagie / 2;
}

void VrajitorInamic::aruncaVraja(Entitate& tinta) {
    if (!areMana()) {
        std::cout << "[Vrajitor] " << nume << " nu are destula mana!\n";
        return;
    }
    int dmg = calculeazaDamage();
    manaCurenta -= 20;
    std::cout << "[Vrajitor] " << nume << " arunca o vraja asupra "
              << tinta.getNume() << " pentru " << dmg << " damage magic!\n";
    tinta.primesteDamage(dmg);
}

void VrajitorInamic::afisareImpl(std::ostream& os) const {
    os << "[Vrajitor Inamic] " << nume
       << " | HP: " << hp << "/" << hpMax
       << " | Mana: " << manaCurenta << "/" << manaMax
       << " | Damage magie: " << calculeazaDamage();
}