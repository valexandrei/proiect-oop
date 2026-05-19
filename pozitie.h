#ifndef POZITIE_H
#define POZITIE_H
#include <iostream>

class Pozitie {
private:
    int x, y;
public:
    Pozitie(int _x = 0, int _y = 0) : x(_x), y(_y) {}
    int getX() const { return x; }
    int getY() const { return y; }
    friend std::ostream& operator<<(std::ostream& os, const Pozitie& p) {
        os << "(" << p.x << "," << p.y << ")";
        return os;
    }
};
#endif