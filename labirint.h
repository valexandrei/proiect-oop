#pragma once
#include <vector>
#include <ostream>
#include "celula.h"
#include "pozitie.h"
#include "inamic.h"

class Labirint {
public:
    Labirint(int linii, int coloane);
    Labirint(const Labirint&) = default;
    Labirint& operator=(const Labirint&) = default;
    ~Labirint() = default;

    friend std::ostream& operator<<(std::ostream& os, const Labirint& l);

    void genereazaRandom();
    void afisareGrafica(const Pozitie& posJucator, const std::vector<Inamic*>& inamici) const;

    [[nodiscard]] int getLinii() const { return linii; }
    [[nodiscard]] int getColoane() const { return coloane; }
    [[nodiscard]] const Celula& getCelula(int r, int c) const { return grid[r][c]; }
    [[nodiscard]] Celula& getCelula(int r, int c) { return grid[r][c]; }

    [[nodiscard]] Pozitie urmatoarePozitie(const Pozitie& from, const Pozitie& to) const;

    [[nodiscard]] Pozitie getPozitieUsa() const { return pozitieUsa; }

private:
    int linii;
    int coloane;
    std::vector<std::vector<Celula>> grid;
    Pozitie pozitieUsa;

    void carvePath(int r, int c);
    void calculeazaPozitieUsa();
};