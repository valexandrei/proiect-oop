#include "labirint.h"
#include <random>
#include <algorithm>
#include <iostream>

Labirint::Labirint(int linii, int coloane)
    : linii(linii), coloane(coloane), grid(linii, std::vector<Celula>(coloane, Celula(true))) {
    genereazaRandom();
}

void Labirint::genereazaRandom() {
    for (int r = 0; r < linii; ++r)
        for (int c = 0; c < coloane; ++c)
            grid[r][c] = Celula(true);
    carvePath(1, 1);
    grid[1][1] = Celula(false);
}

void Labirint::carvePath(int r, int c) {
    static std::mt19937 rng(std::random_device{}());
    grid[r][c] = Celula(false);
    std::vector<std::pair<int,int>> dirs = {{0,2},{0,-2},{2,0},{-2,0}};
    std::shuffle(dirs.begin(), dirs.end(), rng);
    for (auto [dr, dc] : dirs) {
        int nr = r + dr;
        int nc = c + dc;
        if (nr > 0 && nr < linii - 1 && nc > 0 && nc < coloane - 1 && grid[nr][nc].estePerete()) {
            grid[r + dr/2][c + dc/2] = Celula(false);
            carvePath(nr, nc);
        }
    }
}

void Labirint::afisareGrafica(const Pozitie& posJucator, const std::vector<Inamic*>& inamici) const {
    for (int r = 0; r < linii; ++r) {
        for (int c = 0; c < coloane; ++c) {
            if (posJucator.getX() == r && posJucator.getY() == c) {
                std::cout << '@';
                continue;
            }
            bool eInamic = false;
            for (const auto* i : inamici) {
                if (i->getPozitie().getX() == r && i->getPozitie().getY() == c) {
                    eInamic = true;
                    break;
                }
            }
            if (eInamic) std::cout << 'E';
            else std::cout << (grid[r][c].estePerete() ? '#' : '.');
        }
        std::cout << '\n';
    }
}

std::ostream& operator<<(std::ostream& os, const Labirint& l) {
    for (int r = 0; r < l.linii; ++r) {
        for (int c = 0; c < l.coloane; ++c)
            os << (l.grid[r][c].estePerete() ? '#' : '.');
        os << '\n';
    }
    return os;
}