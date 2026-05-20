#ifndef SABIE_H
#define SABIE_H

#include "arma.h"

class Sabie : public Arma {
public:
    explicit Sabie(int v = 50, int d = 30);
    void foloseste() override;
    void afisare() const override;

    friend std::ostream& operator<<(std::ostream& os, const Sabie& s) {
        os << static_cast<const Arma&>(s)
           << " [Tip: Arma de contact]";
        return os;
    }
};

#endif