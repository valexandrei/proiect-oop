#include "celula.h"
#include <iostream>

Celula::Celula(Pozitie _p, char _tip) : p(_p), tip(_tip) {}

void Celula::spargeZid() {
    tip = '.';
}

bool Celula::eWorldWall() const {
    return tip == '#';
}

std::ostream& operator<<(std::ostream& os, const Celula& c) {
    os << c.tip;
    return os;
}