#include "jocdungeon.h"
#include <iostream>
#include <utility>

int JocDungeon::sesiuniCreate = 0;

JocDungeon::JocDungeon(std::string nume, int l, int c)
    : numeSesiune(std::move(nume)), labirint(l, c) {
    ++sesiuniCreate;
}

JocDungeon::JocDungeon(const JocDungeon& other)
    : numeSesiune(other.numeSesiune), labirint(other.labirint) {
    for (const auto& e : other.entitati) {
        entitati.push_back(std::unique_ptr<Entitate>(e->clone()));
    }
    ++sesiuniCreate;
}

void swap(JocDungeon& a, JocDungeon& b) noexcept {
    using std::swap;
    swap(a.numeSesiune, b.numeSesiune);
    swap(a.labirint, b.labirint);
    swap(a.entitati, b.entitati);
}

JocDungeon& JocDungeon::operator=(JocDungeon other) {
    swap(*this, other);
    return *this;
}

void JocDungeon::initSesiune() {
    std::cout << "Sesiunea '" << numeSesiune << "' initializata."
              << " (Total sesiuni: " << sesiuniCreate << ")\n";
}

void JocDungeon::adaugaEntitate(std::unique_ptr<Entitate> e) {
    entitati.push_back(std::move(e));
}

void JocDungeon::ruleazaTurEntitati() {
    std::cout << "\n--- TUR NOU in '" << numeSesiune << "' ---\n";
    for (const auto& e : entitati) {
        if (e->esteViu()) {
            e->actioneaza();
        }
    }
}

void JocDungeon::afiseazaStatisticiEntitati() const {
    std::cout << "\n--- STATISTICI '" << numeSesiune << "' ---\n";
    for (const auto& e : entitati) {
        std::cout << *e << " | Damage potential: "
                  << e->calculeazaDamage() << "\n";
    }
}

void JocDungeon::procesezaCombat(Jucator& jucator) {
    for (const auto& e : entitati) {
        if (!e->esteViu()) continue;
        if (Inamic* inamic = dynamic_cast<Inamic*>(e.get())) {
            inamic->ataca(jucator);
        }
    }
}

void JocDungeon::procesezaVrajitori(Jucator& jucator) {
    for (const auto& e : entitati) {
        if (!e->esteViu()) continue;
        if (VrajitorInamic* vraj = dynamic_cast<VrajitorInamic*>(e.get())) {
            vraj->aruncaVraja(jucator);
        }
    }
}

void JocDungeon::procesezaFantome(Jucator& jucator) {
    for (const auto& e : entitati) {
        if (!e->esteViu()) continue;
        if (Fantoma* fantoma = dynamic_cast<Fantoma*>(e.get())) {
            fantoma->ataculFazic(jucator);
        }
    }
}

std::ostream& operator<<(std::ostream& os, const JocDungeon& joc) {
    os << "JocDungeon: '" << joc.numeSesiune << "'"
       << " | Entitati: " << joc.entitati.size();
    return os;
}