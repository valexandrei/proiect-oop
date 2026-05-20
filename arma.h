#ifndef ARMA_H
#define ARMA_H

#include "obiect.h"

class Arma : public Obiect {
protected:
    int damage;
public:
    Arma(const std::string& n, int v, int d);
    void afisare() const override;

    friend std::ostream& operator<<(std::ostream& os, const Arma& a) {
        os << static_cast<const Obiect&>(a)
           << " [Damage: " << a.damage << "]";
        return os;
    }
};

#endif