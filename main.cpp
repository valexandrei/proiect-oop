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

        Jucator erou(numeCitit, Pozitie(1, 1));
        Jucator copieErou = erou;
        Jucator p3("Test", Pozitie(0,0));
        p3 = erou;

        erou.adaugaXP(150);

        JocDungeon joc("Dungeon of Doom", dimL, dimC);
        joc.initSesiune();
        const Labirint& lab = joc.getLabirint();

        std::cout << "--- STATUS INITIAL JUCATOR ---\n";
        std::cout << erou << "\n";
        std::cout << "Linii labirint: " << lab.getLinii() << " | Coloane: " << lab.getColoane() << "\n";

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

        joc.verificaInteractiune(erou, inamici);

        for (auto* i : inamici) delete i;
        inamici.clear();

    } catch (const std::exception& e) {
        std::cerr << "A aparut o eroare neasteptata: " << e.what() << "\n";
        return 1;
    }

    return 0;
}