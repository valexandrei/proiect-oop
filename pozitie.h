#ifndef POZITIE_H
#define POZITIE_H
#include <iostream>

class Pozitie {
private:
    int x, y;

public:
    Pozitie(int _x = 0, int _y = 0);
    int getX() const;
    int getY() const;

    friend std::ostream& operator<<(std::ostream& os, const Pozitie& p);
};
#endif