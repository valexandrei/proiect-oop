#ifndef JUCATOR_H
#define JUCATOR_H

#include "entitate.h"
#include <string>
#include <iostream>

class Atribute {
private:
    int str;
    int dex;
public:
    explicit Atribute(int s = 10, int d = 10) : str(s), dex(d) {}

    int getStr() const { return str; }
    int getDex() const { return dex; }

    friend void swap(Atribute& a, Atribute& b) noexcept {
        using std::swap;
        swap(a.str, b.str);
        swap(a.dex, b.dex);
    }

    friend std::ostream& operator<<(std::ostream& os, const Atribute& a) {
        os << "STR:" << a.str << " DEX:" << a.dex;
        return os;
    }
};

class Jucator : public Entitate {
private:
    Atribute attr;
    int nivel, xpCurent, xpNecesar;

protected:
    void afisareImpl(std::ostream& os) const override;

public:
    Jucator(const std::string& n, const Pozitie& p);
    Jucator(const Jucator& other);
    Jucator& operator=(const Jucator& other);
    ~Jucator() override;

    void actioneaza() override;
    [[nodiscard]] int calculeazaDamage() const override;
    [[nodiscard]] Jucator* clone() const override;

    void adaugaXP(int xp);
    void crescInNivel();
    int getNivel()    const { return nivel; }
    int getXP()       const { return xpCurent; }
    int getXPNecesar() const { return xpNecesar; }

    friend void swap(Jucator& a, Jucator& b) noexcept;

    friend std::ostream& operator<<(std::ostream& os, const Jucator& j) {
        j.afisareImpl(os);
        return os;
    }
};

#endif