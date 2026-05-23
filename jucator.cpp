#include "jucator.h"
#include <iostream>

void swap(Jucator& a, Jucator& b) noexcept {
    using std::swap;
    swap(static_cast<Entitate&>(a), static_cast<Entitate&>(b));
    swap(a.attr, b.attr);
    swap(a.nivel, b.nivel);
    swap(a.xpCurent, b.xpCurent);
    swap(a.xpNecesar, b.xpNecesar);
}

Jucator::Jucator(const std::string& n, const Pozitie& p)
    : Entitate(n, p, 100, 15, 5), nivel(1), xpCurent(0), xpNecesar(100) {}

Jucator::Jucator(const Jucator& other)
    : Entitate(other), attr(other.attr), nivel(other.nivel),
      xpCurent(other.xpCurent), xpNecesar(other.xpNecesar) {}

Jucator& Jucator::operator=(const Jucator& other) {
    if (this != &other) {
        Entitate::operator=(other);
        attr = other.attr;
        nivel = other.nivel;
        xpCurent = other.xpCurent;
        xpNecesar = other.xpNecesar;
    }
    return *this;
}

Jucator::~Jucator() {}

Jucator* Jucator::clone() const {
    return new Jucator(*this);
}

void Jucator::actioneaza() {
    std::cout << "[Jucator] " << getNume()
              << " se pregateste de atac!\n";
}

int Jucator::calculeazaDamage() const {
    return getAtac() + attr.getStr() / 2;
}

void Jucator::crescInNivel() {
    nivel++;
    xpCurent -= xpNecesar;
    xpNecesar = static_cast<int>(xpNecesar * 1.2);
    setHPMax(getHPMax() + 20);
    setHP(getHPMax());
    std::cout << "[Level Up] " << getNume()
              << " a ajuns la nivelul " << nivel << "!\n";
}

void Jucator::adaugaXP(int xp) {
    xpCurent += xp;
    while (xpCurent >= xpNecesar) {
        crescInNivel();
    }
}

void Jucator::afisareImpl(std::ostream& os) const {
    os << static_cast<const Entitate&>(*this)
       << " | Nivel: " << nivel
       << " | " << attr;
}