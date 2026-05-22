#include "labirint.h"
#include <random>
#include <algorithm>
#include <iostream>
#include <queue>

Labirint::Labirint(int linii, int coloane)
    : linii(linii), coloane(coloane),
      grid(linii, std::vector<Celula>(coloane, Celula(true))),
      pozitieUsa(-1, -1) {
    genereazaRandom();
}

void Labirint::genereazaRandom() {
    for (int r = 0; r < linii; ++r)
        for (int c = 0; c < coloane; ++c)
            grid[r][c] = Celula(true);
    carvePath(1, 1);
    grid[1][1] = Celula(false);
    calculeazaPozitieUsa();
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

void Labirint::calculeazaPozitieUsa() {
    // Cauta cea mai indepartata celula libera fata de (1,1) folosind BFS
    std::vector<std::vector<int>> dist(linii, std::vector<int>(coloane, -1));
    std::queue<Pozitie> q;
    q.push(Pozitie(1, 1));
    dist[1][1] = 0;

    const int dr[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};

    Pozitie celMaiDeparte(1, 1);
    int maxDist = 0;

    while (!q.empty()) {
        Pozitie cur = q.front(); q.pop();
        for (int d = 0; d < 4; ++d) {
            int nr = cur.getX() + dr[d];
            int nc = cur.getY() + dc[d];
            if (nr < 0 || nr >= linii || nc < 0 || nc >= coloane) continue;
            if (dist[nr][nc] != -1) continue;
            if (grid[nr][nc].estePerete()) continue;
            dist[nr][nc] = dist[cur.getX()][cur.getY()] + 1;
            if (dist[nr][nc] > maxDist) {
                maxDist = dist[nr][nc];
                celMaiDeparte = Pozitie(nr, nc);
            }
            q.push(Pozitie(nr, nc));
        }
    }
    pozitieUsa = celMaiDeparte;
}

Pozitie Labirint::urmatoarePozitie(const Pozitie& from, const Pozitie& to) const {
    if (from.getX() == to.getX() && from.getY() == to.getY())
        return from;

    std::vector<std::vector<bool>> vizitat(linii, std::vector<bool>(coloane, false));
    std::vector<std::vector<Pozitie>> parinte(linii, std::vector<Pozitie>(coloane, Pozitie(-1, -1)));

    std::queue<Pozitie> q;
    q.push(from);
    vizitat[from.getX()][from.getY()] = true;

    const int dr[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};

    bool gasit = false;
    while (!q.empty() && !gasit) {
        Pozitie cur = q.front(); q.pop();
        for (int d = 0; d < 4; ++d) {
            int nr = cur.getX() + dr[d];
            int nc = cur.getY() + dc[d];
            if (nr < 0 || nr >= linii || nc < 0 || nc >= coloane) continue;
            if (vizitat[nr][nc]) continue;
            if (grid[nr][nc].estePerete()) continue;
            vizitat[nr][nc] = true;
            parinte[nr][nc] = cur;
            if (nr == to.getX() && nc == to.getY()) { gasit = true; break; }
            q.push(Pozitie(nr, nc));
        }
    }

    if (!gasit) return from;

    Pozitie cur = to;
    while (!(parinte[cur.getX()][cur.getY()].getX() == from.getX() &&
             parinte[cur.getX()][cur.getY()].getY() == from.getY())) {
        cur = parinte[cur.getX()][cur.getY()];
        if (cur.getX() == -1) return from;
    }
    return cur;
}

void Labirint::afisareGrafica(const Pozitie& posJucator, const std::vector<Inamic*>& inamici) const {
    for (int r = 0; r < linii; ++r) {
        for (int c = 0; c < coloane; ++c) {
            if (posJucator.getX() == r && posJucator.getY() == c) { std::cout << '@'; continue; }
            bool eInamic = false;
            for (const auto* i : inamici) {
                if (i->getPozitie().getX() == r && i->getPozitie().getY() == c) { eInamic = true; break; }
            }
            if (eInamic) std::cout << 'E';
            else if (pozitieUsa.getX() == r && pozitieUsa.getY() == c) std::cout << 'D';
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