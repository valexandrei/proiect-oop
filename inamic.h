#ifndef INAMIC_H
#define INAMIC_H

#include "entitate.h"
#include <string>

class Inamic : public Entitate {
private:
    int xpReward;

protected:
    void afisareImpl(std::ostream& os) const override {
        Entitate::afisareImpl(os);
        os << " [Inamic | XP: " << xpReward << "]";
    }

public:
    Inamic(const std::string& n, Pozitie p, int health, int xp = 20);

    void actioneaza() override;
    [[nodiscard]] int calculeazaDamage() const override { return getAtac(); }
    [[nodiscard]] Inamic* clone() const override { return new Inamic(*this); }

    void ataca(Entitate& tinta) const;
    [[nodiscard]] int getXPReward() const { return xpReward; }

    friend std::ostream& operator<<(std::ostream& os, const Inamic& i) {
        i.afisareImpl(os);
        return os;
    }
};

#endif