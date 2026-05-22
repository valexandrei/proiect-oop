#ifndef ENTITATE_H
#define ENTITATE_H

#include <string>
#include <iostream>
#include "pozitie.h"

class Entitate {
protected:
    std::string nume;
    Pozitie pos;
    int hp;
    int hpMax;
    int atac;
    int aparare;

public:
    Entitate(std::string n, Pozitie p, int health, int atk, int def);
    virtual ~Entitate() = default;

    void afiseaza(std::ostream& os) const { afisareImpl(os); }

    virtual void actioneaza() = 0;
    virtual int calculeazaDamage() const = 0;
    virtual Entitate* clone() const = 0;

    Pozitie getPozitie() const { return pos; }
    void setPozitie(Pozitie p) { pos = p; }
    const std::string& getNume() const { return nume; }
    int getHP() const { return hp; }
    int getHPMax() const { return hpMax; }
    bool esteViu() const { return hp > 0; }
    void primesteDamage(int dmg);

    friend void swap(Entitate& a, Entitate& b) noexcept {
        using std::swap;
        swap(a.nume, b.nume);
        swap(a.pos, b.pos);
        swap(a.hp, b.hp);
        swap(a.hpMax, b.hpMax);
        swap(a.atac, b.atac);
        swap(a.aparare, b.aparare);
    }

    friend std::ostream& operator<<(std::ostream& os, const Entitate& e) {
        e.afisareImpl(os);
        return os;
    }

protected:
    virtual void afisareImpl(std::ostream& os) const;
};

#endif