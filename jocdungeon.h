#ifndef JOCDUNGEON_H
#define JOCDUNGEON_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "labirint.h"
#include "jucator.h"
#include "inamic.h"
#include "vrajitor_inamic.h"
#include "fantoma.h"

class JocDungeon {
private:
    std::string numeSesiune;
    Labirint labirint;
    std::vector<std::unique_ptr<Entitate>> entitati;

    static int sesiuniCreate;

public:
    JocDungeon(std::string nume, int l, int c);
    JocDungeon(const JocDungeon& other);
    JocDungeon& operator=(JocDungeon other);

    friend void swap(JocDungeon& a, JocDungeon& b) noexcept;

    void initSesiune();
    void adaugaEntitate(std::unique_ptr<Entitate> e);

    void ruleazaTurEntitati();
    void afiseazaStatisticiEntitati() const;
    void procesezaCombat(Jucator& jucator);
    void procesezaVrajitori(Jucator& jucator);
    void procesezaFantome(Jucator& jucator);

    Labirint& getLabirint() { return labirint; }
    const Labirint& getLabirint() const { return labirint; }
    static int getSesiuniCreate() { return sesiuniCreate; }

    friend std::ostream& operator<<(std::ostream& os, const JocDungeon& joc);
};

#endif