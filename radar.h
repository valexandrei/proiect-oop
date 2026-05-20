#ifndef RADAR_H
#define RADAR_H

#include <vector>
#include <string>
#include <iostream>
#include "pozitie.h"
#include "inamic.h"

class Radar {
private:
    int razaScanare;
    char simbolJucator;
    char simbolInamic;
    char simbolGol;

public:
    explicit Radar(int raza = 5);
    void afiseazaRadar(const Pozitie& posJucator, const std::vector<Inamic*>& inamici) const;
    std::string getDistantaPanaLaCelMaiApropiat(const Pozitie& posJucator, const std::vector<Inamic*>& inamici) const;

    friend std::ostream& operator<<(std::ostream& os, const Radar& r) {
        os << "Radar [Raza: " << r.razaScanare
           << " | Simboluri: jucator='" << r.simbolJucator
           << "' inamic='" << r.simbolInamic
           << "' gol='" << r.simbolGol << "']";
        return os;
    }
};

#endif