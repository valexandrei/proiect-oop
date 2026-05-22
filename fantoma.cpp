#include "fantoma.h"
#include <iostream>

Fantoma::Fantoma(const std::string& n, Pozitie p, int health)
    : Entitate(n, p, health, 8, 0), eCorporeala(false) {}

Fantoma* Fantoma::clone() const {
    return new Fantoma(*this);
}

void Fantoma::actioneaza() {
    std::cout << "[Fantoma] " << nume
              << " pluteste amenintator la " << pos
              << (eCorporeala ? " [Corporala]" : " [Eterica]") << ".\n";
}

int Fantoma::calculeazaDamage() const {
    return eCorporeala ? atac : atac * 2;
}

void Fantoma::ataculFazic(Entitate& tinta) {
    int dmg = calculeazaDamage();
    std::cout << "[Fantoma] " << nume
              << " traverseaza apararea lui " << tinta.getNume()
              << " pentru " << dmg << " damage fazic!\n";
    tinta.primesteDamage(dmg * 2);
}

void Fantoma::afisareImpl(std::ostream& os) const {
    os << "[Fantoma] " << nume
       << " | HP: " << hp << "/" << hpMax
       << " | Stare: " << (eCorporeala ? "Corporala" : "Eterica")
       << " | Damage: " << calculeazaDamage();
}