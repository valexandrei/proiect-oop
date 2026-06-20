#ifndef BATTLELOG_H
#define BATTLELOG_H

#include <string>
#include <vector>

class BattleLog {
private:
    static std::vector<std::string> istoricEvenimente;
public:
    static void adaugaEveniment(const std::string& eveniment);
    static void afiseazaLog();
    static void curataLog();
    static std::string genereazaDescriereLupta(const std::string& numeJucator, const std::string& numeInamic, int damage);
};

#endif