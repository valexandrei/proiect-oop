#ifndef EXCEPTII_H
#define EXCEPTII_H

#include <stdexcept>
#include <string>

class DungeonException : public std::runtime_error {
public:
    explicit DungeonException(const std::string& mesaj)
        : std::runtime_error(mesaj) {}
};

class PozitieInvalidaException : public DungeonException {
public:
    explicit PozitieInvalidaException(int x, int y)
        : DungeonException("Pozitie invalida: (" + std::to_string(x)
                           + ", " + std::to_string(y) + ") este in afara hartii!") {}
};

class RadarDefectException : public DungeonException {
public:
    explicit RadarDefectException(int raza)
        : DungeonException("Raza radar invalida: " + std::to_string(raza)
                           + ". Raza trebuie sa fie >= 0!") {}
};

class InventarException : public DungeonException {
public:
    explicit InventarException(const std::string& numeObiect)
        : DungeonException("Eroare inventar: nu s-a putut adauga '"
                           + numeObiect + "'. Inventar plin sau obiect invalid!") {}
};

#endif