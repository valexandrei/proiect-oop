#ifndef CELULA_H
#define CELULA_H
#include "pozitie.h"
#include <iostream>

class Celula {
private:
    Pozitie p;
    char tip;

public:
    explicit Celula(Pozitie _p = Pozitie(), char _tip = '.');

    void spargeZid();
    bool eWorldWall() const;

    friend std::ostream& operator<<(std::ostream& os, const Celula& c);
};
#endif