#ifndef PISTOALE_H
#define PISTOALE_H

#include "arma.h"

class Pistoale : public Arma {
private:
    int munitie;
public:
    explicit Pistoale(int v = 120, int d = 45, int m = 6);
    void foloseste() override;
    void afisare() const override;
    void reincarca();

    friend std::ostream& operator<<(std::ostream& os, const Pistoale& p) {
        os << static_cast<const Arma&>(p)
           << " [Tip: Arma de foc | Munitie: " << p.munitie << "]";
        return os;
    }
};

#endif