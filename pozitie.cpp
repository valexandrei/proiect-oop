#include "pozitie.h"

Pozitie::Pozitie(int _x, int _y) : x(_x), y(_y) {}

int Pozitie::getX() const { return x; }
int Pozitie::getY() const { return y; }

std::ostream& operator<<(std::ostream& os, const Pozitie& p) {
    os << "(" << p.x << "," << p.y << ")";
    return os;
}