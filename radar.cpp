#include "radar.h"
#include "exceptii.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <sstream>

Radar::Radar(int raza)
    : razaScanare(raza), simbolJucator('@'), simbolInamic('!'), simbolGol('.') {
    if (raza < 0) {
        throw RadarDefectException();
    }
}

void Radar::afiseazaRadar(const Pozitie& posJucator, const std::vector<Inamic*>& inamici) const {
    std::cout << "\n--- RADAR MINIMAP (Raza: " << razaScanare << ") ---\n";
    for (int i = -razaScanare; i <= razaScanare; ++i) {
        std::cout << "  ";
        for (int j = -razaScanare; j <= razaScanare; ++j) {
            if (i == 0 && j == 0) {
                std::cout << simbolJucator << " ";
                continue;
            }
            bool inamicGasit = false;
            for (const auto* inamic : inamici) {
                if (inamic->getPozitie().getX() == posJucator.getX() + i &&
                    inamic->getPozitie().getY() == posJucator.getY() + j) {
                    inamicGasit = true;
                    break;
                    }
            }
            if (inamicGasit) std::cout << simbolInamic << " ";
            else std::cout << simbolGol << " ";
        }
        std::cout << "\n";
    }
    std::cout << "---------------------\n";
}

std::string Radar::getDistantaPanaLaCelMaiApropiat(const Pozitie& posJucator, const std::vector<Inamic*>& inamici) const {
    if (inamici.empty()) return "Radarul (raza " + std::to_string(razaScanare) + "): Nicio tinta.";
    double distMin = 999.0;
    for (const auto* inamic : inamici) {
        double d = std::sqrt(std::pow(inamic->getPozitie().getX() - posJucator.getX(), 2) +
                             std::pow(inamic->getPozitie().getY() - posJucator.getY(), 2));
        if (d < distMin) distMin = d;
    }
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << distMin;
    return "Inamic la distanta: " + ss.str() + " m (Scanare activa)";
}