#ifndef LABIRINT_H
#define LABIRINT_H
#include "celula.h"
#include <vector>
#include <iostream>

class Labirint {
private:
    int linii, coloane;
    std::vector<std::vector<Celula>> grija;

public:
    explicit Labirint(int L = 5, int C = 5);
    int getLinii() const { return linii; }
    int getColoane() const { return coloane; }

    bool estePozitieValida(int x, int y) const;
    void afisareGrafica(const Pozitie& posJucator, const std::vector<class Inamic*>& inamici) const;

    friend std::ostream& operator<<(std::ostream& os, const Labirint& l) {
        for (const auto& rand : l.grija) {
            for (const auto& cel : rand) {
                os << cel << " ";
            }
            os << "\n";
        }
        return os;
    }
};
#endif