#ifndef JUCATOR_H
#define JUCATOR_H

#include "entitate.h"
#include <string>
#include <iostream>

class Atribute {
public:
    int str;
    int dex;

    explicit Atribute(int s = 10, int d = 10) : str(s), dex(d) {}

    friend std::ostream& operator<<(std::ostream& os, const Atribute& a) {
        os << "STR:" << a.str << " DEX:" << a.dex;
        return os;
    }
};

class Jucator : public Entitate {
private:
    Atribute attr;
    int nivel;
    int xpCurent;
    int xpNecesar;

    void crescInNivel();

protected:
    void afisareImpl(std::ostream& os) const override;

public:
    Jucator(const std::string& n, const Pozitie& p);
    Jucator(const Jucator& other);
    Jucator& operator=(Jucator other);
    ~Jucator() override = default;

    void actioneaza() override;
    int calculeazaDamage() const override;
    Jucator* clone() const override;

    void adaugaXP(int xp);
    int getNivel() const { return nivel; }
    int getXP() const { return xpCurent; }
    int getXPNecesar() const { return xpNecesar; }

    friend void swap(Jucator& a, Jucator& b) noexcept;
};

#endif