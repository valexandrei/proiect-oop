#ifndef EXCEPTII_H
#define EXCEPTII_H
#include <stdexcept>
#include <string>

class DungeonException : public std::runtime_error {
public:
    explicit DungeonException(const std::string& mesaj) : std::runtime_error(mesaj) {}
};

class PozitieInvalidaException : public DungeonException {
public:
    PozitieInvalidaException() : DungeonException("Eroare: Incercare de miscare in afara harti!") {}
};

class RadarDefectException : public DungeonException {
public:
    RadarDefectException() : DungeonException("Eroare: Radarul nu poate scana o raza negativa!") {}
};
#endif