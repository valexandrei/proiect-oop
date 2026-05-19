#include "jocdungeon.h"
#include <iostream>

JocDungeon::JocDungeon(std::string nume, int l, int c) : numeSesiune(nume), labirint(l, c) {}

void JocDungeon::initSesiune() {
    //labirint.genereazaLabirint();
    std::cout << "Sesiunea " << numeSesiune << " a fost initializata.\n";
}

void JocDungeon::verificaInteractiune(Jucator& p, std::vector<Inamic*>& inamici) {
    if(numeSesiune.empty()) return;

    for (auto it = inamici.begin(); it != inamici.end(); ) {
        if ((*it)->getPozitie().getX() == p.getPozitie().getX() &&
            (*it)->getPozitie().getY() == p.getPozitie().getY()) {
            std::cout << "[" << numeSesiune << "] Lupta cu " << (*it)->getNume() << "!\n";
            p.actioneaza();
            delete *it;
            it = inamici.erase(it);
            } else {
                ++it;
            }
    }
}

std::ostream& operator<<(std::ostream& os, const JocDungeon& joc) {
    os << "Joc: " << joc.numeSesiune;
    return os;
}