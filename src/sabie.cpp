#include "sabie.h"
#include <iostream>

Sabie::Sabie(int v, int d) : Arma("Sabie de otel", v, d) {}

void Sabie::foloseste() {
    std::cout << "Tai cu sabia! Lovitura a produs " << damage << " damage.\n";
}

void Sabie::afisare() const {
    Arma::afisare();
    std::cout << " [Tip: Arma de contact]\n";
}