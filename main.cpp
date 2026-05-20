#include <iostream>
#include <string>
#include <vector>
#include "jucator.h"
#include "inamic.h"
#include "jocdungeon.h"
#include "radar.h"
#include "gamedata.h"
#include "battlelog.h"
#include "exceptii.h"
#include "inventar.h"
#include "pistoale.h"
#include "celula.h"
#include "potiune.h"
#include "sabie.h"
#include "spear.h"

int main() {
    try {
        std::string numeCitit;
        int dimL, dimC;

        if (!(std::cin >> numeCitit >> dimL >> dimC)) {
            numeCitit = "Aragorn";
            dimL = 5;
            dimC = 5;
        }

        std::cout << GameData::getPovesteFundal() << "\n";
        std::cout << GameData::getMesajLevelUp(2) << "\n";

        Jucator erou(numeCitit, Pozitie(1, 1));
        Jucator copieErou = erou;
        Jucator p3("Test", Pozitie(0,0));
        p3 = erou;

        erou.setPozitie(Pozitie(1, 2));
        erou.adaugaXP(150);

        std::cout << "Nivel: " << erou.getNivel() << " | XP: " << erou.getXP()
                  << " / " << erou.getXPNecesar() << " | HP: " << erou.getHP() << "\n";

        JocDungeon joc("Dungeon of Doom", dimL, dimC);
        joc.initSesiune();
        const Labirint& lab = joc.getLabirint();

        std::cout << "--- STATUS INITIAL JUCATOR ---\n";
        std::cout << erou << "\n";
        std::cout << "Linii labirint: " << lab.getLinii() << " | Coloane: " << lab.getColoane() << "\n";
        lab.afisareGrafica(erou.getPozitie(), std::vector<Inamic*>());

        std::vector<Inamic*> inamici;
        Inamic* boss = new Inamic("Seful Goblins", Pozitie(2, 2), 50);
        inamici.push_back(boss);

        Radar miniMap(3);
        miniMap.afiseazaRadar(erou.getPozitie(), inamici);
        std::cout << miniMap.getDistantaPanaLaCelMaiApropiat(erou.getPozitie(), inamici) << "\n";

        try {
            Radar radarStricat(-1);
        } catch (const DungeonException& e) {
            std::cout << "[Sistemul de securitate]: " << e.what() << "\n";
        }

        if (lab.estePozitieValida(2, 2)) {
            boss->ataca(erou);
        }

        std::string jurnal = BattleLog::genereazaDescriereLupta(erou.getNume(), boss->getNume(), 30);
        BattleLog::adaugaEveniment(jurnal);
        BattleLog::afiseazaLog();
        BattleLog::curataLog();

        std::vector<std::string> tipuri = GameData::getTipuriInamici();
        for(const auto& t : tipuri) {
            std::cout << "Inamic potential: " << GameData::getDescriereInamic(t) << "\n";
        }

        Inventar rucsac;
        Pistoale* armaGasita = new Pistoale();
        armaGasita->reincarca();
        if(rucsac.adaugaObiect(armaGasita)) {
            rucsac.afiseazaTot();
            rucsac.folosesteToate();
        }

        Celula c(Pozitie(0,0), '#');
        if (c.eWorldWall()) {
            c.spargeZid();
        }

        joc.verificaInteractiune(erou, inamici);
        std::cout << "\n--- TEST operator<< ---\n";
        std::cout << erou << "\n";
        std::cout << *boss << "\n";
        std::cout << joc << "\n";
        std::cout << miniMap << "\n";
        std::cout << rucsac << "\n";

        Sabie sabie;
        Spear lance;
        Potiune potion("Potion of Healing", 30, 50);
        std::cout << sabie << "\n";
        std::cout << lance << "\n";
        std::cout << potion << "\n";

        Pistoale pistol;
        std::cout << pistol << "\n";
        for (auto* i : inamici) delete i;
        inamici.clear();
    } catch (const std::exception& e) {
        std::cerr << "A aparut o eroare neasteptata: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
