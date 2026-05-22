#ifndef INAMIC_H
#define INAMIC_H

#include "entitate.h"
#include <string>

class Inamic : public Entitate {
private:
    int xpReward;

protected:
    void afisareImpl(std::ostream& os) const override;

public:
    Inamic(const std::string& n, Pozitie p, int health, int xpRew = 30);

    void actioneaza() override;
    int calculeazaDamage() const override;
    Inamic* clone() const override;

    void ataca(Entitate& tinta) const;
    int getXPReward() const { return xpReward; }
};

#endif