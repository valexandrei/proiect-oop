#ifndef OBIECT_H
#define OBIECT_H

#include <string>
#include <iostream>

class Obiect {
protected:
    std::string nume;
    int valoare;

public:
    Obiect(std::string n, int v);
    virtual ~Obiect() = default;

    virtual void foloseste() = 0;
    virtual void afisare() const;

    const std::string& getNume() const { return nume; }

    friend std::ostream& operator<<(std::ostream& os, const Obiect& o) {
        os << "Obiect: " << o.nume << " (Valoare: " << o.valoare << ")";
        return os;
    }
};

#endif