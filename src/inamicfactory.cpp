#include "inamicfactory.h"
#include "inamic.h"
#include "fantoma.h"
#include "vrajitorinamic.h"
#include "schelet.h"

std::unique_ptr<Entitate> InamicFactory::creeaza(Tip tip, const std::string& nume,
                                                   Pozitie pos, int hp, int dmg) {
    switch (tip) {
    case Tip::Goblin:
        return std::make_unique<Inamic>(nume, pos, hp, dmg > 0 ? dmg : 15);
    case Tip::Orc:
        return std::make_unique<Inamic>(nume, pos, hp, dmg > 0 ? dmg : 30);
    case Tip::Fantoma:
        return std::make_unique<Fantoma>(nume, pos, hp);
    case Tip::Vrajitor:
        return std::make_unique<VrajitorInamic>(nume, pos, hp, 80, dmg > 0 ? dmg : 25);
    case Tip::Schelet:
        return std::make_unique<Schelet>(nume, pos, hp, dmg > 0 ? dmg : 12);
    case Tip::Boss:
        return std::make_unique<Inamic>(nume, pos, hp, dmg > 0 ? dmg : 60);
    default:
        return std::make_unique<Inamic>(nume, pos, hp, 10);
    }
}

std::string InamicFactory::numeImplicit(Tip tip) {
    switch (tip) {
    case Tip::Goblin:  return "Goblin";
    case Tip::Orc:     return "Orc";
    case Tip::Fantoma: return "Fantoma";
    case Tip::Vrajitor: return "Vrajitor";
    case Tip::Schelet: return "Schelet";
    case Tip::Boss:    return "Boss";
    default:           return "Inamic";
    }
}