#include "jucator.h"
#include <iostream>
#include <utility>

Jucator::Jucator(const std::string& n, const Pozitie& p)
    : Entitate(n, p, 100, 15, 5), attr(), nivel(1), xpCurent(0), xpNecesar(100) {}

Jucator::Jucator(const Jucator& other)
    : Entitate(other), attr(other.attr), nivel(other.nivel),
      xpCurent(other.xpCurent), xpNecesar(other.xpNecesar) {}

void swap(Jucator& a, Jucator& b) noexcept {
    using std::swap;
    swap(static_cast<Entitate&>(a), static_cast<Entitate&>(b));
    swap(a.attr.str, b.attr.str);
    swap(a.attr.dex, b.attr.dex);
    swap(a.nivel, b.nivel);
    swap(a.xpCurent, b.xpCurent);
    swap(a.xpNecesar, b.xpNecesar);
}

Jucator& Jucator::operator=(Jucator other) {
    swap(*this, other);
    return *this;
}

Jucator* Jucator::clone() const {
    return new Jucator(*this);
}

void Jucator::actioneaza() {
    std::cout << "[Jucator] " << nume << " se pregateste de atac!\n";
}

int Jucator::calculeazaDamage() const {
    return atac + attr.dex / 2;
}

void Jucator::adaugaXP(int xp) {
    xpCurent += xp;
    while (xpCurent >= xpNecesar) {
        crescInNivel();
    }
}

void Jucator::crescInNivel() {
    nivel++;
    xpCurent -= xpNecesar;
    xpNecesar = static_cast<int>(xpNecesar * 1.2);
    hpMax += 20;
    hp = hpMax;
    std::cout << "[Level Up] " << nume << " a ajuns la nivelul " << nivel << "!\n";
}

void Jucator::afisareImpl(std::ostream& os) const {
    os << "Jucator: " << nume
       << " | Nivel: " << nivel
       << " | XP: " << xpCurent << "/" << xpNecesar
       << " | HP: " << hp << "/" << hpMax
       << " | " << attr
       << " | Damage: " << calculeazaDamage();
}