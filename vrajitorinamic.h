#ifndef VRAJITORINAMIC_H
#define VRAJITORINAMIC_H

#include "entitate.h"
#include <string>

class VrajitorInamic : public Entitate {
private:
    int manaMax;
    int manaCurenta;
    int damageMagie;

protected:
    void afisareImpl(std::ostream& os) const override;

public:
    VrajitorInamic(const std::string& n, Pozitie p, int health,
                   int mana = 80, int dmgMagie = 25);

    void actioneaza() override;
    [[nodiscard]] int calculeazaDamage() const override;
    [[nodiscard]] VrajitorInamic* clone() const override;

    void aruncaVraja(Entitate& tinta);
    [[nodiscard]] bool areMana() const { return manaCurenta >= 20; }
};

#endif