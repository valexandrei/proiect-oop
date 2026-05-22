#ifndef INVENTAR_H
#define INVENTAR_H

#include <vector>
#include <iostream>
#include "obiect.h"

class Inventar {
    std::vector<Obiect*> iteme;
    int capacitateMaxima;

public:
    explicit Inventar(int cap = 5);
    ~Inventar();

    void adaugaObiect(Obiect* obj);
    void afiseazaTot() const;
    void folosesteToate();

    Inventar(const Inventar&) = delete;
    Inventar& operator=(const Inventar&) = delete;

    friend std::ostream& operator<<(std::ostream& os, const Inventar& inv) {
        os << "Inventar (" << inv.iteme.size() << "/" << inv.capacitateMaxima << "):\n";
        for (const auto* obj : inv.iteme) {
            os << "  " << *obj << "\n";
        }
        return os;
    }
};

#endif