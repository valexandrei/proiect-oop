#ifndef SPEAR_H
#define SPEAR_H

#include "arma.h"

class Spear : public Arma {
public:
    explicit Spear(int v = 75, int d = 35);
    void foloseste() override;
    void afisare() const override;

    friend std::ostream& operator<<(std::ostream& os, const Spear& s) {
        os << static_cast<const Arma&>(s)
           << " [Tip: Polearm - Raza mare de actiune]";
        return os;
    }
};

#endif