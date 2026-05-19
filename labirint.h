#ifndef LABIRINT_H
#define LABIRINT_H
#include <vector>
#include <iostream>
#include "celula.h"

class Labirint {
private:
    int linii, coloane;
    std::vector<std::vector<Celula>> grija;
public:
    Labirint(int L = 5, int C = 5);
    bool estePozitieValida(int x, int y) const;
    void afisareGrafica(const Pozitie& posJucator, const std::vector<class Inamic*>& inamici) const;
    int getLinii() const { return linii; }
    int getColoane() const { return coloane; }
    friend std::ostream& operator<<(std::ostream& os, const Labirint& l) {
        os << "Dungeon " << l.linii << "x" << l.coloane << "\n";
        for(const auto& rand : l.grija) {
            for(const auto& cel : rand) os << cel << " ";
            os << "\n";
        }
        return os;
    }
};
#endif