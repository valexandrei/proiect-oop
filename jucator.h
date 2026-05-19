#ifndef JUCATOR_H
#define JUCATOR_H
#include "entitate.h"
#include <string>
#include <iostream>

class Atribute {
public:
    int str = 10, dex = 10;
    friend std::ostream& operator<<(std::ostream& os, const Atribute& a) {
        os << "STR:" << a.str << " DEX:" << a.dex;
        return os;
    }
};

class Jucator : public Entitate {
private:
    Atribute attr;
    int nivel, xpCurent, xpNecesar;

public:
    Jucator(const std::string& n, const Pozitie& p);
    Jucator(const Jucator& other);
    Jucator& operator=(const Jucator& other);
    ~Jucator() override;

    void actioneaza() override;
    void afisare() const override;

    void adaugaXP(int xp);
    void crescInNivel();
    int getNivel() const { return nivel; }
    int getXP() const { return xpCurent; }
    int getXPNecesar() const { return xpNecesar; }

    friend std::ostream& operator<<(std::ostream& os, const Jucator& j);
};
#endif