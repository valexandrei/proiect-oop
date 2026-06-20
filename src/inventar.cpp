#include "inventar.h"
#include "exceptii.h"
#include <iostream>

Inventar::Inventar(int cap) : capacitateMaxima(cap) {
    iteme.reserve(cap);
}

void Inventar::adaugaObiect(std::unique_ptr<Obiect> obj) {
    if (!obj) {
        throw InventarException("null");
    }
    if (static_cast<int>(iteme.size()) >= capacitateMaxima) {
        throw InventarException(obj->getNume());
    }
    std::cout << "Adaugat in inventar: " << obj->getNume() << "\n";
    iteme.push_back(std::move(obj));
}

void Inventar::afiseazaTot() const {
    std::cout << "--- CONTINUT INVENTAR ("
              << iteme.size() << "/" << capacitateMaxima << ") ---\n";
    if (iteme.empty()) {
        std::cout << "Inventarul este gol.\n";
        return;
    }
    for (const auto& obj : iteme) {
        obj->afisare();
    }
    std::cout << "-------------------------------\n";
}

void Inventar::folosesteToate() {
    std::cout << "Se folosesc toate obiectele...\n";
    for (const auto& obj : iteme) {
        obj->foloseste();
    }
}