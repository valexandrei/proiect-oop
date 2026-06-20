#include "battlelog.h"
#include <iostream>
#include <algorithm>
#include <string>

std::vector<std::string> BattleLog::istoricEvenimente;

void BattleLog::adaugaEveniment(const std::string& eveniment) {
    if (istoricEvenimente.size() > 50) {
        istoricEvenimente.erase(istoricEvenimente.begin());
    }
    istoricEvenimente.push_back(eveniment);
}

void BattleLog::curataLog() {
    istoricEvenimente.clear();
}

void BattleLog::afiseazaLog() {
    std::cout << "\n--- JURNAL DE LUPTA ---\n";
    for(const auto& ev : istoricEvenimente) {
        std::cout << "[INFO]: " << ev << "\n";
    }
}

std::string BattleLog::genereazaDescriereLupta(const std::string& numeJucator, const std::string& numeInamic, int damage) {
    double critic = (damage > 20) ? 1.5 : 1.0;
    int fmg = static_cast<int>(damage * critic);
    std::string s = "Lupta: " + numeJucator + " vs " + numeInamic + ". ";
    s += "Rezultat: " + std::to_string(fmg) + " daune provocate.";
    return s;
}