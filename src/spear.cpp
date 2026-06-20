#include "spear.h"
#include <iostream>

Spear::Spear(int v, int d) : Arma("Great Spear", v, d) {}

void Spear::foloseste() {
    std::cout << "Ai executat o lovitura de tip 'Thrust' cu " << nume 
              << "! Damage-ul provocat: " << damage << ".\n";
}

void Spear::afisare() const {
    Arma::afisare();
    std::cout << " [Tip: Polearm - Raza mare de actiune]\n";
}