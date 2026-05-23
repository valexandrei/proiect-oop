#ifndef ENTITATE_H
#define ENTITATE_H

#include <string>
#include <iostream>
#include "pozitie.h"

class Entitate {
private:
    std::string nume;
    Pozitie pos;
    int hp;
    int hpMax;
    int atac;
    int aparare;

protected:
    int getAtac() const { return atac; }
    int getAparare() const { return aparare; }
    void setHP(int val) { hp = val; }
    void setHPMax(int val) { hpMax = val; }

    virtual void afisareImpl(std::ostream& os) const;

public:
    Entitate(std::string n, Pozitie p, int health, int atk, int def);
    virtual ~Entitate() = default;

    virtual void actioneaza() = 0;
    virtual int calculeazaDamage() const { return atac; }
    virtual bool esteViu() const { return hp > 0; }
    virtual Entitate* clone() const = 0;

    void afiseaza(std::ostream& os) const { afisareImpl(os); }

    Pozitie getPozitie() const { return pos; }
    void setPozitie(Pozitie p) { pos = p; }
    const std::string& getNume() const { return nume; }
    int getHP() const { return hp; }
    int getHPMax() const { return hpMax; }
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
};

#endif