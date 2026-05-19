#include "jucator.h"
#include <iostream>

Jucator::Jucator(const std::string& n, const Pozitie& p)
    : Entitate(n, p, 100, 15, 5), nivel(1), xpCurent(0), xpNecesar(100) {}

Jucator::Jucator(const Jucator& other)
    : Entitate(other), attr(other.attr), nivel(other.nivel),
      xpCurent(other.xpCurent), xpNecesar(other.xpNecesar) {}

Jucator& Jucator::operator=(const Jucator& other) {
    if(this != &other) {
        Entitate::operator=(other);
        attr = other.attr;
        nivel = other.nivel;
        xpCurent = other.xpCurent;
        xpNecesar = other.xpNecesar;
    }
    return *this;
}

Jucator::~Jucator() {}

void Jucator::actioneaza() {}

void Jucator::afisare() const {
    std::cout << *this << "\n";
}

void Jucator::adaugaXP(int xp) {
    xpCurent += xp;
    while(xpCurent >= xpNecesar) {
        crescInNivel();
    }
}

void Jucator::crescInNivel() {
    nivel++;
    xpCurent -= xpNecesar;
    xpNecesar = static_cast<int>(xpNecesar * 1.2);
    hpMax += 20;
    hp = hpMax;
}

std::ostream& operator<<(std::ostream& os, const Jucator& j) {
    os << static_cast<const Entitate&>(j) << " | Nivel:" << j.nivel << " | " << j.attr;
    return os;
}