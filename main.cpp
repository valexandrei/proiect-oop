#include <iostream>
#include <string>
#include <memory>

#include "jucator.h"
#include "inamic.h"
#include "vrajitorinamic.h"
#include "jocdungeon.h"
#include "radar.h"
#include "gamedata.h"
#include "battlelog.h"
#include "exceptii.h"
#include "inventar.h"
#include "pistoale.h"
#include "sabie.h"
#include "spear.h"
#include "celula.h"
#include "potiune.h"

int main() {
    try {
        // --- Citire date ---
        std::string numeCitit;
        int dimL, dimC;
        if (!(std::cin >> numeCitit >> dimL >> dimC)) {
            numeCitit = "Aragorn";
            dimL = 5;
            dimC = 5;
        }

        std::cout << "\n" << GameData::getPovesteFundal() << "\n\n";

        // --- Jucator: constructor, copiere, operator= (copy-and-swap) ---
        Jucator erou(numeCitit, Pozitie(1, 1));
        Jucator copieErou = erou;
        Jucator p3("Test", Pozitie(0, 0));
        p3 = erou;

        erou.adaugaXP(150);
        std::cout << erou << "\n";
        std::cout << "Nivel: " << erou.getNivel()
                  << " | XP: " << erou.getXP()
                  << "/" << erou.getXPNecesar() << "\n\n";

        // --- Creare sesiune ---
        JocDungeon joc("Dungeon of Doom", dimL, dimC);
        joc.initSesiune();
        std::cout << "Sesiuni create (static): "
                  << JocDungeon::getSesiuniCreate() << "\n\n";

        // --- Adaugare entitati prin pointer de baza ---
        joc.adaugaEntitate(std::make_unique<Inamic>("Goblin", Pozitie(2, 2), 40, 20));
        joc.adaugaEntitate(std::make_unique<Inamic>("Orc", Pozitie(3, 1), 70, 35));
        joc.adaugaEntitate(std::make_unique<VrajitorInamic>("Lich", Pozitie(4, 4), 50));

        // --- Functii virtuale prin pointer de baza ---
        joc.ruleazaTurEntitati();
        joc.afiseazaStatisticiEntitati();

        // --- dynamic_cast cu sens ---
        joc.procesezaCombat(erou);
        joc.procesezaVrajitori(erou);

        std::cout << "\nHP erou dupa combat: " << erou.getHP() << "\n";

        // --- Labirint ---
        const Labirint& lab = joc.getLabirint();
        std::cout << "Labirint " << lab.getLinii() << "x" << lab.getColoane() << ":\n";
        std::vector<Inamic*> inamiciVechi;
        lab.afisareGrafica(erou.getPozitie(), inamiciVechi);

        // --- setPozitie ---
        erou.setPozitie(Pozitie(2, 2));

        // --- Radar ---
        Radar miniMap(3);
        std::cout << "\n" << miniMap << "\n";
        miniMap.afiseazaRadar(erou.getPozitie(), inamiciVechi);
        std::cout << miniMap.getDistantaPanaLaCelMaiApropiat(erou.getPozitie(), inamiciVechi) << "\n";

        try {
            Radar radarStricat(-1);
        } catch (const RadarDefectException& e) {
            std::cout << "[Exceptie Radar]: " << e.what() << "\n";
        }

        // --- Exceptie pozitie invalida ---
        try {
            if (!lab.estePozitieValida(99, 99)) {
                throw PozitieInvalidaException(99, 99);
            }
        } catch (const PozitieInvalidaException& e) {
            std::cout << "[Exceptie Pozitie]: " << e.what() << "\n";
        }

        // --- Inventar cu exceptie ---
        Inventar rucsac(2);
        try {
            Pistoale* pistol = new Pistoale();
            pistol->reincarca();
            rucsac.adaugaObiect(pistol);
            rucsac.adaugaObiect(new Sabie());
            rucsac.adaugaObiect(new Spear());
        } catch (const InventarException& e) {
            std::cout << "[Exceptie Inventar]: " << e.what() << "\n";
        }
        std::cout << "\n" << rucsac << "\n";
        rucsac.afiseazaTot();
        rucsac.folosesteToate();

        // --- BattleLog ---
        BattleLog::adaugaEveniment(
            BattleLog::genereazaDescriereLupta(erou.getNume(), "Goblin", 30));
        BattleLog::afiseazaLog();
        BattleLog::curataLog();

        // --- operator<< pentru toate clasele ---
        std::cout << "\n--- TEST operator<< ---\n";
        std::cout << joc << "\n";
        std::cout << copieErou << "\n";

        Potiune hp("Potion of Healing", 30, 50);
        Sabie sabie;
        Spear lance;
        Pistoale pistol2;
        std::cout << hp << "\n";
        std::cout << sabie << "\n";
        std::cout << lance << "\n";
        std::cout << pistol2 << "\n";

        // --- NVI afiseaza ---
        erou.afiseaza(std::cout);
        std::cout << "\n";

        // --- Inamic getXPReward ---
        Inamic inamicTest("Test", Pozitie(0, 0), 30);
        std::cout << "XP reward inamic: " << inamicTest.getXPReward() << "\n";

        // --- Celula ---
        Celula c(Pozitie(0, 0), '#');
        if (c.eWorldWall()) c.spargeZid();
        std::cout << c << "\n";

        // --- GameData ---
        std::cout << "\n" << GameData::getMesajLevelUp(3) << "\n";
        for (const auto& t : GameData::getTipuriInamici()) {
            std::cout << "  " << t << ": " << GameData::getDescriereInamic(t) << "\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Eroare neasteptata: " << e.what() << "\n";
        return 1;
    }

    return 0;
}