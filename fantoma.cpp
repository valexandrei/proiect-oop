#include "fantoma.h"
#include <iostream>

Fantoma::Fantoma(const std::string& n, Pozitie p, int health)
    : Entitate(n, p, health, 8, 0), eCorporeala(false) {}

Fantoma* Fantoma::clone() const {
    return new Fantoma(*this);
}

void Fantoma::actioneaza() {
    std::cout << "[Fantoma] " << getNume()
              << " pluteste amenintator la " << getPozitie()
              << (eCorporeala ? " [Corporala]" : " [Eterica]") << ".\n";
}

int Fantoma::calculeazaDamage() const {
    return eCorporeala ? getAtac() : getAtac() * 2;
}

void Fantoma::ataculFazic(Entitate& tinta) {
    int dmg = calculeazaDamage();
    std::cout << "[Fantoma] " << getNume()
              << " traverseaza apararea lui " << tinta.getNume()
              << " pentru " << dmg << " damage fazic!\n";
    tinta.primesteDamage(dmg * 2);
}

void Fantoma::afisareImpl(std::ostream& os) const {
    Entitate::afisareImpl(os);
    os << " [Fantoma | " << (eCorporeala ? "Corporala" : "Eterica")
       << " | Damage: " << calculeazaDamage() << "]";
}