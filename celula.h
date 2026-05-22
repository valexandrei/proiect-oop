#pragma once
#include <ostream>

class Celula {
public:
    explicit Celula(bool ePerete = false) : perete(ePerete) {}

    [[nodiscard]] bool estePerete() const { return perete; }
    void seteazaPerete(bool val) { perete = val; }

    // Returneaza '#' sau '.' pentru afisare
    [[nodiscard]] char getSimbol() const;

    // Inverseaza tipul celulei (perete <-> coridor)
    void toggle();

    bool operator==(const Celula& other) const;
    bool operator!=(const Celula& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Celula& c) {
        os << (c.perete ? '#' : '.');
        return os;
    }

private:
    bool perete;
};