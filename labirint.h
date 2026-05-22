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

    // Returneaza urmatorul pas din pozitia 'from' spre 'to' evitand peretii.
    // Returneaza 'from' daca nu exista drum.
    [[nodiscard]] Pozitie urmatoarePozitie(const Pozitie& from, const Pozitie& to) const;

private:
    int linii;
    int coloane;
    std::vector<std::vector<Celula>> grid;

    void carvePath(int r, int c);
};