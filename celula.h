#ifndef CELULA_H
#define CELULA_H
#include "pozitie.h"
#include <iostream>

class Celula {
private:
    Pozitie p;
    char tip;
public:
    Celula(Pozitie _p = Pozitie(), char _tip = '.') : p(_p), tip(_tip) {}
    friend std::ostream& operator<<(std::ostream& os, const Celula& c) {
        os << "[Coord:" << c.p << " Tip:" << c.tip << "]";
        return os;
    }
};
#endif