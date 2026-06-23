#include "inamicfactory.h"
#include "inamic.h"
#include "fantoma.h"
#include "vrajitorinamic.h"
#include "schelet.h"

std::unique_ptr<Entitate> GoblinCreator::creeaza(const std::string& nume,
                                                   Pozitie pos, int hp, int dmg) const {
    return std::make_unique<Inamic>(nume, pos, hp, dmg > 0 ? dmg : 15);
}

std::unique_ptr<Entitate> OrcCreator::creeaza(const std::string& nume,
                                               Pozitie pos, int hp, int dmg) const {
    return std::make_unique<Inamic>(nume, pos, hp, dmg > 0 ? dmg : 30);
}

std::unique_ptr<Entitate> FantomaCreator::creeaza(const std::string& nume,
                                                    Pozitie pos, int hp, int dmg) const {
    (void)dmg;
    return std::make_unique<Fantoma>(nume, pos, hp);
}

std::unique_ptr<Entitate> VrajitorCreator::creeaza(const std::string& nume,
                                                     Pozitie pos, int hp, int dmg) const {
    return std::make_unique<VrajitorInamic>(nume, pos, hp, 80, dmg > 0 ? dmg : 25);
}

std::unique_ptr<Entitate> ScheletCreator::creeaza(const std::string& nume,
                                                    Pozitie pos, int hp, int dmg) const {
    return std::make_unique<Schelet>(nume, pos, hp, dmg > 0 ? dmg : 12);
}

std::unique_ptr<Entitate> BossCreator::creeaza(const std::string& nume,
                                                Pozitie pos, int hp, int dmg) const {
    return std::make_unique<Inamic>(nume, pos, hp, dmg > 0 ? dmg : 60);
}

const InamicCreator& InamicFactory::getCreator(Tip tip) {
    static GoblinCreator goblin;
    static OrcCreator orc;
    static FantomaCreator fantoma;
    static VrajitorCreator vrajitor;
    static ScheletCreator schelet;
    static BossCreator boss;

    switch (tip) {
        case Tip::Goblin:   return goblin;
        case Tip::Orc:      return orc;
        case Tip::Fantoma:  return fantoma;
        case Tip::Vrajitor: return vrajitor;
        case Tip::Schelet:  return schelet;
        case Tip::Boss:     return boss;
        default:            return goblin;
    }
}

std::unique_ptr<Entitate> InamicFactory::creeaza(Tip tip, const std::string& nume,
                                                   Pozitie pos, int hp, int dmg) {
    return getCreator(tip).creeaza(nume, pos, hp, dmg);
}

std::string InamicFactory::numeImplicit(Tip tip) {
    return getCreator(tip).numeImplicit();
}