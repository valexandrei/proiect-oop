#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "jocdungeon.h"
#include "inventar.h"
#include "radar.h"
#include "exceptii.h"
#include "battlelog.h"
#include "fantoma.h"
#include "inamic.h"
#include "schelet.h"
#include "renderer.h"
#include "gamelogic.h"
#include "stiva.h"
#include "inamicfactory.h"

int main() {
    JocDungeon joc("Dungeon of Doom", 20, 30);
    Inventar rucsac;
    Radar radar(8);
    int nivelCurent = 1;

    try {
        GameLogic::initLogica(joc, rucsac);
    } catch (const DungeonException&) {
        return 1;
    }

    GameLogic::populeazaNivel(joc, nivelCurent);

    for (const auto& e : joc.getEntitati()) {
        if (Fantoma* f = dynamic_cast<Fantoma*>(e.get()))
            f->schimbaStare();
    }

    if (std::getenv("GITHUB_ACTIONS") != nullptr) {
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
        schelet.primesteDamageSchelet(25);
        schelet.primesteDamageSchelet(20);
        std::cout << "Poate reinvia: " << (schelet.poateReinvia() ? "da" : "nu") << "\n";
        std::cout << schelet << "\n";

        Stiva<int> stivaScoruri(10);
        stivaScoruri.push(100);
        stivaScoruri.push(250);
        stivaScoruri.push(75);
        stivaScoruri.afiseaza(std::cout);
        std::cout << "Marime stiva: " << stivaScoruri.marime() << "\n";
        try {
            std::cout << "Max scor: " << maximDinStiva(stivaScoruri) << "\n";
        } catch (const std::exception& e) {
            std::cout << "[Exceptie]: " << e.what() << "\n";
        }

        Stiva<std::string> stivaEvenimente(5);
        stivaEvenimente.push("Goblin eliminat");
        stivaEvenimente.push("Level up");
        stivaEvenimente.afiseaza(std::cout);
        std::cout << "Ultimul eveniment: " << stivaEvenimente.top() << "\n";

        auto inamicTest = InamicFactory::creeaza(InamicFactory::Tip::Goblin,
            "Goblin Test", Pozitie(1, 1), 30, 10);
        inamicTest->afiseaza(std::cout);
        std::cout << "Nume implicit: " << InamicFactory::numeImplicit(InamicFactory::Tip::Orc) << "\n";

        return 0;
    }

    {
        Pozitie startPos = joc.getJucator().getPozitie();
        const Celula& startCelula = joc.getLabirint().getCelula(
            startPos.getX(), startPos.getY());
        BattleLog::getInstance().adaugaEveniment("Start pe: " +
            std::string(1, startCelula.getSimbol()));
    }

    sf::RenderWindow window(sf::VideoMode(SCREEN_W, SCREEN_H), "Dungeon Crawler", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    sf::Texture tileTex;
    if (!tileTex.loadFromFile("assets/tilemap.png")) return 1;
    tileTex.setSmooth(false);

    sf::Font font;
    if (!font.loadFromFile("assets/font.ttf") &&
        !font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf") &&
        !font.loadFromFile("/System/Library/Fonts/Helvetica.ttc") &&
        !font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
        return 1;

    Renderer renderer(window, font, tileTex);

    sf::Clock moveClock;
    sf::Clock enemyMoveClock;
    sf::Clock deltaClock;
    sf::Clock gameClock;
    constexpr float moveDelay      = 0.15f;
    constexpr float enemyMoveDelay = 0.6f;
    std::string message;
    sf::Clock msgClock;
    constexpr float msgDuration = 3.f;

    std::vector<FloatText> floatTexts;
    std::vector<std::string> logLinii;
    std::string radarInfo;
    int nivelAnterior = joc.getJucator().getNivel();
    bool gameWon  = false;
    bool gameDead = false;

    while (window.isOpen()) {
        float dt = deltaClock.restart().asSeconds();
        float totalTime = gameClock.getElapsedTime().asSeconds();
        const Labirint& lab = joc.getLabirint();
        int labLinii   = lab.getLinii();
        int labColoane = lab.getColoane();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape) {
                BattleLog::getInstance().afiseazaLog();
                window.close();
            }
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Space &&
                !gameWon && !gameDead) {
                GameLogic::procesezaCombat(joc, floatTexts, renderer, logLinii, nivelAnterior);
                message = "Atac!";
                msgClock.restart();
                if (!joc.getJucator().esteViu()) {
                    gameDead = true;
                } else if (nivelCurent == 4) {
                    bool bossMort = true;
                    for (const auto& e : joc.getEntitati()) {
                        if (e->esteViu()) { bossMort = false; break; }
                    }
                    if (bossMort) {
                        gameWon = true;
                        logLinii.push_back("AI CASTIGAT! Lordul Intunericului a fost invins!");
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::L) {
                BattleLog::getInstance().afiseazaLog();
                joc.afiseazaStatisticiEntitati(std::cout);
            }
        }

        if (!gameWon && !gameDead && moveClock.getElapsedTime().asSeconds() >= moveDelay) {
            Jucator& jucator = joc.getJucator();
            Pozitie pos    = jucator.getPozitie();
            Pozitie newPos = pos;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                newPos.setX(newPos.getX() - 1);
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
                     sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                newPos.setX(newPos.getX() + 1);
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
                     sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                newPos.setY(newPos.getY() - 1);
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
                     sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                newPos.setY(newPos.getY() + 1);
            if (newPos.getX() != pos.getX() || newPos.getY() != pos.getY()) {
                if (newPos.getX() >= 0 && newPos.getX() < labLinii &&
                    newPos.getY() >= 0 && newPos.getY() < labColoane) {
                    if (!lab.getCelula(newPos.getX(), newPos.getY()).estePerete()) {
                        jucator.setPozitie(newPos);
                        joc.verificaInteractiune();
                        if (nivelCurent < 4) {
                            Pozitie usa = lab.getPozitieUsa();
                            if (newPos.getX() == usa.getX() && newPos.getY() == usa.getY()) {
                                GameLogic::treceLaNivelUrmator(joc, nivelCurent, logLinii, message, msgClock);
                            }
                        }
                    }
                }
                moveClock.restart();
                std::vector<Inamic*> inamiciRaw;
                for (const auto& e : joc.getEntitati()) {
                    if (!e->esteViu()) continue;
                    if (Inamic* in = dynamic_cast<Inamic*>(e.get()))
                        inamiciRaw.push_back(in);
                }
                radarInfo = radar.getDistantaPanaLaCelMaiApropiat(
                    jucator.getPozitie(), inamiciRaw);
            }
        }

        if (!gameWon && !gameDead && enemyMoveClock.getElapsedTime().asSeconds() >= enemyMoveDelay) {
            enemyMoveClock.restart();
            GameLogic::miseazaInamici(joc);
        }

        Pozitie posJ = joc.getJucator().getPozitie();
        int camX = posJ.getY() * TILE_SCALED - SCREEN_W / 2;
        int camY = posJ.getX() * TILE_SCALED - SCREEN_H / 2;

        window.clear(sf::Color(15, 10, 20));
        renderer.drawLabirint(lab, camX, camY);
        renderer.drawDoor(lab, camX, camY, nivelCurent, totalTime);
        renderer.drawEntitati(joc.getEntitati(), camX, camY);
        renderer.drawJucator(posJ, camX, camY);
        renderer.updateAndDrawFloats(floatTexts, dt, camX, camY);
        renderer.drawUI(joc.getJucator(), radarInfo, nivelCurent);
        renderer.drawBattleLog(logLinii);

        if (msgClock.getElapsedTime().asSeconds() < msgDuration)
            renderer.drawMessage(message);

        if (gameWon || gameDead)
            renderer.drawEndScreen(gameWon, joc.getJucator(), nivelCurent);

        window.display();
    }

    return 0;
}