#include "jucator.h"
#include <iostream>

Jucator::Jucator(const std::string& n, const Pozitie& p)
    : Entitate(n, p, 100, 15, 5), nivel(1), xpCurent(0), xpNecesar(100) {
    logBuffer = new int[5]{0,0,0,0,0};
}

Jucator::Jucator(const Jucator& other)
    : Entitate(other), attr(other.attr), nivel(other.nivel),
      xpCurent(other.xpCurent), xpNecesar(other.xpNecesar) {
    logBuffer = new int[5];
    for(int i = 0; i < 5; ++i) logBuffer[i] = other.logBuffer[i];
}

Jucator& Jucator::operator=(const Jucator& other) {
    if(this != &other) {
        Entitate::operator=(other);
        attr = other.attr;
        nivel = other.nivel;
        xpCurent = other.xpCurent;
        xpNecesar = other.xpNecesar;
        delete[] logBuffer;
        logBuffer = new int[5];
        for(int i = 0; i < 5; ++i) logBuffer[i] = other.logBuffer[i];
    }
    return *this;
}

Jucator::~Jucator() {
    delete[] logBuffer;
}

void Jucator::actioneaza() {}

void Jucator::afisare() const {
    std::cout << *this << "\n";
}

void Jucator::adaugaXP(int xp) {
    xpCurent += xp;
    while(xpCurent >= xpNecesar) crescInNivel();
}

void Jucator::crescInNivel() {
    nivel++;
    xpCurent -= xpNecesar;
    xpNecesar *= 1.2;
    hpMax += 20;
    hp = hpMax;
}