#include "potiune.h"
#include <iostream>

Potiune::Potiune(const std::string& n, int v, int hp) : Obiect(n, v), puncteViata(hp) {}

void Potiune::foloseste() {
    std::cout << "Ai baut " << nume << " si ai recuperat " << puncteViata << " HP!\n";
}

void Potiune::afisare() const {
    Obiect::afisare();
    std::cout << " [Vindecare: " << puncteViata << " HP]\n";
}