#ifndef FANTOMA_H
#define FANTOMA_H

#include "entitate.h"
#include <string>

class Fantoma : public Entitate {
private:
    bool eCorporeala;

protected:
    void afisareImpl(std::ostream& os) const override;

public:
    Fantoma(const std::string& n, Pozitie p, int health = 40);

    void actioneaza() override;
    int calculeazaDamage() const override;
    Fantoma* clone() const override;

    void ataculFazic(Entitate& tinta);
    void schimbaStare() { eCorporeala = !eCorporeala; }
    bool esteCorporeala() const { return eCorporeala; }
};

#endif