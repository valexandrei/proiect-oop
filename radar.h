#ifndef RADAR_H
#define RADAR_H
#include <vector>
#include <string>
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
};
#endif