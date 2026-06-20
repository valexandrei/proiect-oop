#pragma once
#include <vector>
#include <memory>
#include <string>
#include "entitate.h"
#include "jucator.h"
#include "inamic.h"
#include "vrajitorinamic.h"
#include "fantoma.h"
#include "labirint.h"
#include "inventar.h"
#include "battlelog.h"

class JocDungeon {
public:
    JocDungeon(std::string nume, int linii, int coloane);
    JocDungeon(const JocDungeon& other);
    JocDungeon& operator=(JocDungeon other);
    ~JocDungeon() = default;

    friend void swap(JocDungeon& a, JocDungeon& b) noexcept;
    friend std::ostream& operator<<(std::ostream& os, const JocDungeon& joc);

    void initSesiune();
    void verificaInteractiune();
    void ruleazaTurEntitati();
    void afiseazaStatisticiEntitati(std::ostream& os) const;
    void procesezaCombat(Jucator& jucator);
    void adaugaEntitate(std::unique_ptr<Entitate> e);

    Jucator& getJucator() { return jucator; }
    const Jucator& getJucator() const { return jucator; }
    Labirint& getLabirint() { return labirint; }
    const Labirint& getLabirint() const { return labirint; }
    const std::vector<std::unique_ptr<Entitate>>& getEntitati() const { return entitati; }
    const std::string& getNume() const { return numeSesiune; }

    static int getSesiuniCreate() { return sesiuniCreate; }

private:
    std::string numeSesiune;
    Jucator jucator;
    Labirint labirint;
    Inventar inventar;
    std::vector<std::unique_ptr<Entitate>> entitati;
    static int sesiuniCreate;
};