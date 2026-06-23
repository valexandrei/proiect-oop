#include "gamelogic.h"
#include "inamic.h"
#include "vrajitorinamic.h"
#include "fantoma.h"
#include "schelet.h"
#include "potiune.h"
#include "sabie.h"
#include "spear.h"
#include "pistoale.h"
#include "battlelog.h"
#include "gamedata.h"
#include "celula.h"
#include "exceptii.h"
#include "inamicfactory.h"
#include "stiva.h"
#include <iostream>
#include <cmath>
#include <sstream>

bool GameLogic::suntAdiacente(const Pozitie& a, const Pozitie& b) {
    int dr = std::abs(a.getX() - b.getX());
    int dc = std::abs(a.getY() - b.getY());
    return dr <= 1 && dc <= 1 && (dr + dc) > 0;
}

void GameLogic::initLogica(JocDungeon& joc, Inventar& rucsac) {
    joc.initSesiune();
    std::cout << GameData::getPovesteFundal() << "\n\n";
    std::cout << "Inamici in dungeon:\n";
    for (const auto& tip : GameData::getTipuriInamici())
        std::cout << "  - " << tip << "\n";
    std::cout << "\n";
    try {
        rucsac.adaugaObiect(std::make_unique<Potiune>("Potion of Healing", 30, 50));
        rucsac.adaugaObiect(std::make_unique<Sabie>(50, 30));
        rucsac.adaugaObiect(std::make_unique<Spear>(75, 35));
        auto pistolNou = std::make_unique<Pistoale>();
        pistolNou->reincarca();
        rucsac.adaugaObiect(std::move(pistolNou));
        rucsac.adaugaObiect(std::make_unique<Potiune>("Potion mica", 15, 30));
    } catch (const InventarException&) {}
    rucsac.afiseazaTot();
    rucsac.folosesteToate();
    BattleLog::getInstance().adaugaEveniment("Sesiune noua inceputa: " + joc.getNume());
    Celula tmp(true);
    tmp.seteazaPerete(false);
    tmp.toggle();
    BattleLog::getInstance().adaugaEveniment("Celula test: " + std::string(1, tmp.getSimbol()));
    std::cout << "Sesiuni create: " << JocDungeon::getSesiuniCreate() << "\n";
}

void GameLogic::populeazaNivel(JocDungeon& joc, int nivelCurent) {
    if (nivelCurent == 1) {
        joc.adaugaEntitate(InamicFactory::creeaza(InamicFactory::Tip::Goblin,
            "Goblin Infatometat", Pozitie(3, 3), 40, 15));
        joc.adaugaEntitate(InamicFactory::creeaza(InamicFactory::Tip::Orc,
            "Orc Distrugator", Pozitie(5, 7), 70, 30));
        joc.adaugaEntitate(InamicFactory::creeaza(InamicFactory::Tip::Fantoma,
            "Spectra Blestemata", Pozitie(14, 8), 35));
    } else if (nivelCurent == 2) {
        joc.adaugaEntitate(InamicFactory::creeaza(InamicFactory::Tip::Goblin,
            "Goblin Sangeros", Pozitie(4, 4), 55, 20));
        joc.adaugaEntitate(InamicFactory::creeaza(InamicFactory::Tip::Orc,
            "Orc Berserker", Pozitie(6, 10), 90, 35));
        joc.adaugaEntitate(InamicFactory::creeaza(InamicFactory::Tip::Vrajitor,
            "Vrajitor Umbra", Pozitie(10, 5), 45, 25));
        joc.adaugaEntitate(InamicFactory::creeaza(InamicFactory::Tip::Fantoma,
            "Fantoma Razbunarii", Pozitie(12, 12), 50));
        joc.adaugaEntitate(InamicFactory::creeaza(InamicFactory::Tip::Schelet,
            "Schelet Antic", Pozitie(8, 8), 30, 12));
    } else if (nivelCurent == 3) {
        joc.adaugaEntitate(InamicFactory::creeaza(InamicFactory::Tip::Goblin,
            "Goblin Elita", Pozitie(3, 5), 70, 25));
        joc.adaugaEntitate(InamicFactory::creeaza(InamicFactory::Tip::Orc,
            "Orc Campion", Pozitie(7, 9), 110, 40));
        joc.adaugaEntitate(InamicFactory::creeaza(InamicFactory::Tip::Vrajitor,
            "Arhivrajitor", Pozitie(9, 3), 65, 35));
        joc.adaugaEntitate(InamicFactory::creeaza(InamicFactory::Tip::Fantoma,
            "Spectra Antica", Pozitie(15, 15), 75));
        joc.adaugaEntitate(InamicFactory::creeaza(InamicFactory::Tip::Orc,
            "Orc Garda", Pozitie(12, 6), 80, 30));
    } else {
        joc.adaugaEntitate(InamicFactory::creeaza(InamicFactory::Tip::Boss,
            "BOSS - Lordul Intunericului", Pozitie(10, 15), 300, 60));
    }
}

void GameLogic::treceLaNivelUrmator(JocDungeon& joc, int& nivelCurent,
                                     std::vector<std::string>& logLinii,
                                     std::string& message, sf::Clock& msgClock) {
    nivelCurent++;
    joc.getLabirint().genereazaRandom();
    joc.getJucator().setPozitie(Pozitie(1, 1));
    const_cast<std::vector<std::unique_ptr<Entitate>>&>(joc.getEntitati()).clear();
    populeazaNivel(joc, nivelCurent);
    std::string msg = (nivelCurent < 4)
        ? "Ai ajuns la etajul " + std::to_string(nivelCurent) + "!"
        : "CAMERA BOSSULUI! Invinge Lordul Intunericului!";
    logLinii.push_back(msg);
    BattleLog::getInstance().adaugaEveniment(msg);
    message = msg;
    msgClock.restart();
}

void GameLogic::miseazaInamici(JocDungeon& joc) {
    const Labirint& lab = joc.getLabirint();
    Pozitie posJucator = joc.getJucator().getPozitie();
    for (const auto& e : joc.getEntitati()) {
        if (!e->esteViu()) continue;
        Pozitie urm = lab.urmatoarePozitie(e->getPozitie(), posJucator);
        if (urm.getX() == posJucator.getX() && urm.getY() == posJucator.getY())
            continue;
        bool ocupat = false;
        for (const auto& alt : joc.getEntitati()) {
            if (alt.get() == e.get() || !alt->esteViu()) continue;
            if (alt->getPozitie().getX() == urm.getX() &&
                alt->getPozitie().getY() == urm.getY()) {
                ocupat = true;
                break;
            }
        }
        if (!ocupat) e->setPozitie(urm);
    }
}

void GameLogic::ruleazaTesteCLI(JocDungeon& joc, Radar& radar) {
    joc.ruleazaTurEntitati();
    joc.afiseazaStatisticiEntitati(std::cout);

    for (const auto& e : joc.getEntitati()) {
        e->afiseaza(std::cout);
        std::cout << "\n";
    }

    for (const auto& e : joc.getEntitati()) {
        if (const Fantoma* f = dynamic_cast<const Fantoma*>(e.get())) {
            std::cout << f->getNume()
                      << (f->esteCorporeala() ? " [corporala]" : " [incorporala]")
                      << "\n";
        }
    }

    std::vector<Inamic*> inamiciRaw;
    for (const auto& e : joc.getEntitati()) {
        if (Inamic* in = dynamic_cast<Inamic*>(e.get()))
            inamiciRaw.push_back(in);
    }
    joc.getLabirint().afisareGrafica(joc.getJucator().getPozitie(), inamiciRaw);
    radar.afiseazaRadar(joc.getJucator().getPozitie(), inamiciRaw);

    BattleLog::getInstance().afiseazaLog();
    BattleLog::getInstance().curataLog();

    try {
        [[maybe_unused]] const auto& c = joc.getLabirint().getCelula(-1, -1);
    } catch (const PozitieInvalidaException& e) {
        std::cout << "[Exceptie prinsa]: " << e.what() << "\n";
    }

    Schelet schelet("Schelet Test", Pozitie(5, 5), 30, 12);
    schelet.actioneaza();
    schelet.primesteDamage(100);
    std::cout << "Reinviere reusita: " << (schelet.incearcaReinviere() ? "da" : "nu") << "\n";
    std::cout << schelet << "\n";

    const Stiva<Pozitie>& istoric = joc.getIstoricPozitii();
    std::cout << "Istoric pozitii jucator (marime " << istoric.marime() << "):\n";
    istoric.afiseaza(std::cout);
    if (!istoric.esteGoala())
        std::cout << "Ultima pozitie inregistrata: " << istoric.top() << "\n";

    Stiva<int> scoruri(10);
    scoruri.push(100);
    scoruri.push(250);
    scoruri.push(75);
    try {
        std::cout << "Scor maxim: " << maximDinStiva(scoruri) << "\n";
    } catch (const std::exception& e) {
        std::cout << "[Eroare]: " << e.what() << "\n";
    }

    auto inamicTest = InamicFactory::creeaza(InamicFactory::Tip::Goblin,
        "Goblin Test", Pozitie(1, 1), 30, 10);
    inamicTest->afiseaza(std::cout);
    std::cout << "Nume implicit: " << InamicFactory::numeImplicit(InamicFactory::Tip::Orc) << "\n";
}

void GameLogic::procesezaCombat(JocDungeon& joc,
                                 std::vector<FloatText>& floats,
                                 Renderer& renderer,
                                 std::vector<std::string>& logLinii,
                                 int& nivelAnterior) {
    Jucator& jucator = joc.getJucator();
    Pozitie jPos = jucator.getPozitie();
    bool atacatCeva = false;
    for (const auto& e : joc.getEntitati()) {
        if (!e->esteViu()) continue;
        if (!suntAdiacente(jPos, e->getPozitie())) continue;
        atacatCeva = true;
        int dmgDat = jucator.calculeazaDamage();
        e->primesteDamage(dmgDat);
        std::string descriere = BattleLog::genereazaDescriereLupta(
            jucator.getNume(), e->getNume(), dmgDat);
        BattleLog::getInstance().adaugaEveniment(descriere);
        logLinii.push_back(descriere);
        renderer.spawnFloatText(floats, "-" + std::to_string(dmgDat),
                                static_cast<float>(e->getPozitie().getY()),
                                static_cast<float>(e->getPozitie().getX()),
                                sf::Color(255, 80, 80));
        if (!e->esteViu()) {
            if (e->incearcaReinviere()) {
                std::string evReinviere = e->getNume() + " a reInviat!";
                BattleLog::getInstance().adaugaEveniment(evReinviere);
                logLinii.push_back(evReinviere);
                renderer.spawnFloatText(floats, "REINVIERE!",
                                        static_cast<float>(e->getPozitie().getY()),
                                        static_cast<float>(e->getPozitie().getX()),
                                        sf::Color(180, 80, 255));
                continue;
            }
            std::string evMoarte = e->getNume() + " a fost eliminat!";
            BattleLog::getInstance().adaugaEveniment(evMoarte);
            logLinii.push_back(evMoarte);
            if (const Inamic* in = dynamic_cast<const Inamic*>(e.get())) {
                jucator.adaugaXP(in->getXPReward());
                renderer.spawnFloatText(floats,
                                        "+" + std::to_string(in->getXPReward()) + " XP",
                                        static_cast<float>(jPos.getY()),
                                        static_cast<float>(jPos.getX()),
                                        sf::Color(80, 220, 255));
                int nivelNou = jucator.getNivel();
                if (nivelNou > nivelAnterior) {
                    std::string msgLvl = GameData::getMesajLevelUp(nivelNou);
                    BattleLog::getInstance().adaugaEveniment(msgLvl);
                    logLinii.push_back(msgLvl);
                    nivelAnterior = nivelNou;
                }
            }
            continue;
        }
        if (VrajitorInamic* vraj = dynamic_cast<VrajitorInamic*>(e.get())) {
            vraj->aruncaVraja(jucator);
        } else if (Fantoma* f = dynamic_cast<Fantoma*>(e.get())) {
            f->ataculFazic(jucator);
        } else {
            int dmgPrimit = e->calculeazaDamage();
            jucator.primesteDamage(dmgPrimit);
            std::string evContra = e->getNume() + " contraataca: -" + std::to_string(dmgPrimit) + " HP";
            BattleLog::getInstance().adaugaEveniment(evContra);
            logLinii.push_back(evContra);
            logLinii.push_back(GameData::getDescriereInamic(e->getNume()));
            renderer.spawnFloatText(floats, "-" + std::to_string(dmgPrimit),
                                    static_cast<float>(jPos.getY()),
                                    static_cast<float>(jPos.getX()),
                                    sf::Color(255, 60, 60));
        }
        std::ostringstream oss;
        e->afiseaza(oss);
        if (const Fantoma* f = dynamic_cast<const Fantoma*>(e.get())) {
            BattleLog::getInstance().adaugaEveniment(oss.str() +
                (f->esteCorporeala() ? " [corp]" : " [incorp]"));
        }
    }
    if (!atacatCeva) {
        BattleLog::getInstance().adaugaEveniment(jucator.getNume() + " loveste in gol.");
        logLinii.push_back(jucator.getNume() + " loveste in gol.");
    }
}