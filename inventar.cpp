#include "inventar.h"
#include "exceptii.h"
#include <iostream>

Inventar::Inventar(int cap) : capacitateMaxima(cap) {
    iteme.reserve(cap);
}

Inventar::~Inventar() {
    for (Obiect* obj : iteme) {
        delete obj;
    }
    iteme.clear();
}

void Inventar::adaugaObiect(Obiect* obj) {
    if (obj == nullptr) {
        throw InventarException("null");
    }
    if (static_cast<int>(iteme.size()) >= capacitateMaxima) {
        throw InventarException(obj->getNume());
    }
    iteme.push_back(obj);
    std::cout << "Adaugat in inventar: " << obj->getNume() << "\n";
}

void Inventar::afiseazaTot() const {
    std::cout << "--- CONTINUT INVENTAR ("
              << iteme.size() << "/" << capacitateMaxima << ") ---\n";
    if (iteme.empty()) {
        std::cout << "Inventarul este gol.\n";
        return;
    }
    for (const auto* obj : iteme) {
        obj->afisare();
    }
    std::cout << "-------------------------------\n";
}

void Inventar::folosesteToate() {
    std::cout << "Se folosesc toate obiectele...\n";
    for (auto* obj : iteme) {
        obj->foloseste();
    }
}