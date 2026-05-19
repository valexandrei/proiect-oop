#include "labirint.h"
#include <iostream>

Labirint::Labirint(int L, int C) : linii(L), coloane(C) {
    for (int i = 0; i < linii; ++i) {
        std::vector<Celula> rand;
        for (int j = 0; j < coloane; ++j) {
            rand.push_back(Celula(Pozitie(i, j), '.'));
        }
        grija.push_back(rand);
    }
}

bool Labirint::estePozitieValida(int x, int y) const {
    return (x >= 0 && x < linii && y >= 0 && y < coloane);
}

void Labirint::afisareGrafica(const Pozitie& posJucator, const std::vector<class Inamic*>& inamici) const {
    (void)posJucator;
    (void)inamici;
    std::cout << *this;
}