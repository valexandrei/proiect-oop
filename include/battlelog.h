#ifndef BATTLELOG_H
#define BATTLELOG_H

#include <string>
#include <vector>

class BattleLog {
private:
    BattleLog() = default;
    ~BattleLog() = default;
    BattleLog(const BattleLog&) = delete;
    BattleLog& operator=(const BattleLog&) = delete;

    std::vector<std::string> istoricEvenimente;

public:
    static BattleLog& getInstance() {
        static BattleLog instance;
        return instance;
    }

    void adaugaEveniment(const std::string& eveniment);
    void afiseazaLog() const;
    void curataLog();
    std::string genereazaDescriereLupta(const std::string& numeJucator,
                                         const std::string& numeInamic,
                                         int damage) const;
};

#endif