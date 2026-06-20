#ifndef SCHELET_H
#define SCHELET_H

#include "entitate.h"
#include <string>

class Schelet : public Entitate {
private:
    bool aReinviat;
    int dmgOase;

protected:
    void afisareImpl(std::ostream& os) const override {
        Entitate::afisareImpl(os);
        os << " [Schelet | Reinviere: " << (aReinviat ? "folosita" : "disponibila")
           << " | Damage: " << dmgOase << "]";
    }

public:
    Schelet(const std::string& n, Pozitie p, int health = 30, int dmg = 12);

    void actioneaza() override;
    int calculeazaDamage() const override { return dmgOase; }
    Schelet* clone() const override { return new Schelet(*this); }

    void primesteDamageSchelet(int dmg);
    bool poateReinvia() const { return !aReinviat; }

    friend std::ostream& operator<<(std::ostream& os, const Schelet& s) {
        s.afisareImpl(os);
        return os;
    }
};

#endif