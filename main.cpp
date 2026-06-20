#include <SFML/Graphics.hpp>
#include <cmath>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "jocdungeon.h"
#include "jucator.h"
#include "inamic.h"
#include "include/vrajitorinamic.h"
#include "fantoma.h"
#include "inventar.h"
#include "exceptii.h"
#include "pistoale.h"
#include "sabie.h"
#include "include/spear.h"
#include "potiune.h"
#include "battlelog.h"
#include "gamedata.h"
#include "radar.h"

static constexpr int TILE_SIZE   = 16;
static constexpr int SCALE       = 3;
static constexpr int TILE_SCALED = TILE_SIZE * SCALE;
static constexpr int SCREEN_W    = 1920;
static constexpr int SCREEN_H    = 1080;

struct TileCoord {
    int col, row;
    sf::IntRect toRect() const {
        return sf::IntRect(col * (TILE_SIZE + 1), row * (TILE_SIZE + 1), TILE_SIZE, TILE_SIZE);
    }
};
[[maybe_unused]] static constexpr TileCoord TC_FLOOR  = {0, 0};
[[maybe_unused]] static constexpr TileCoord TC_WALL   = {4, 3};
[[maybe_unused]] static constexpr TileCoord TC_PLAYER = {1, 8};
[[maybe_unused]] static constexpr TileCoord TC_WIZARD = {3, 9};
[[maybe_unused]] static constexpr TileCoord TC_GHOST  = {1, 10};
[[maybe_unused]] static constexpr TileCoord TC_ORC    = {2, 7};
[[maybe_unused]] static constexpr TileCoord TC_GOBLIN = {0, 9};
[[maybe_unused]] static constexpr TileCoord TC_DOOR   = {2, 2};
[[maybe_unused]] static constexpr TileCoord TC_BOSS   = {0, 8};

static void drawTile(sf::RenderWindow& window, sf::Sprite& sprite,
                     const sf::Texture& tex, TileCoord tc, int sx, int sy) {
    sprite.setTexture(tex);
    sprite.setTextureRect(tc.toRect());
    sprite.setScale(static_cast<float>(SCALE), static_cast<float>(SCALE));
    sprite.setPosition(static_cast<float>(sx), static_cast<float>(sy));
    window.draw(sprite);
}

static TileCoord getTileForEntitate(const Entitate& e) {
    if (dynamic_cast<const VrajitorInamic*>(&e)) return TC_WIZARD;
    if (dynamic_cast<const Fantoma*>(&e))        return TC_GHOST;
    if (dynamic_cast<const Inamic*>(&e)) {
        if (e.getNume().find("Orc") != std::string::npos ||
            e.getNume().find("BOSS") != std::string::npos ||
            e.getNume().find("Boss") != std::string::npos) return TC_BOSS;
        return TC_GOBLIN;
    }
    return TC_GOBLIN;
}

struct FloatText {
    sf::Text text;
    sf::Vector2f pos;
    float life = 1.2f;
};

static void spawnFloatText(std::vector<FloatText>& floats,
                           const sf::Font& font,
                           const std::string& str,
                           float tileCol, float tileRow,
                           sf::Color color) {
    FloatText ft;
    ft.text.setFont(font);
    ft.text.setCharacterSize(18);
    ft.text.setFillColor(color);
    ft.text.setOutlineColor(sf::Color::Black);
    ft.text.setOutlineThickness(2.f);
    ft.text.setString(str);
    ft.pos = {tileCol, tileRow};
    floats.push_back(std::move(ft));
}

static void updateAndDrawFloats(sf::RenderWindow& window,
                                std::vector<FloatText>& floats,
                                float dt, int camX, int camY) {
    for (auto& ft : floats) {
        ft.life -= dt;
        ft.pos.y -= dt * 1.5f;
        float alpha = std::max(0.f, ft.life / 1.2f);
        sf::Color c = ft.text.getFillColor();
        c.a = static_cast<sf::Uint8>(alpha * 255);
        ft.text.setFillColor(c);
        sf::Color oc = ft.text.getOutlineColor();
        oc.a = c.a;
        ft.text.setOutlineColor(oc);
        float sx = ft.pos.x * TILE_SCALED - static_cast<float>(camX);
        float sy = ft.pos.y * TILE_SCALED - static_cast<float>(camY) - 10.f;
        ft.text.setPosition(sx, sy);
        window.draw(ft.text);
    }
    floats.erase(std::remove_if(floats.begin(), floats.end(),
        [](const FloatText& ft){ return ft.life <= 0.f; }), floats.end());
}

static sf::Color hpColor(float ratio) {
    if (ratio > 0.6f) return sf::Color(40, 200, 40);
    if (ratio > 0.3f) return sf::Color(220, 200, 20);
    return sf::Color(220, 40, 40);
}

static void drawBar(sf::RenderWindow& window,
                    float x, float y, float w, float h,
                    float ratio, sf::Color fg, sf::Color bg,
                    const std::string& label, const sf::Font& font) {
    sf::RectangleShape bgShape({w, h});
    bgShape.setFillColor(bg);
    bgShape.setPosition(x, y);
    window.draw(bgShape);
    sf::RectangleShape fgShape({w * std::max(0.f, ratio), h});
    fgShape.setFillColor(fg);
    fgShape.setPosition(x, y);
    window.draw(fgShape);
    sf::RectangleShape border({w, h});
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color(200, 200, 200, 180));
    border.setOutlineThickness(1.5f);
    border.setPosition(x, y);
    window.draw(border);
    sf::Text txt;
    txt.setFont(font);
    txt.setCharacterSize(13);
    txt.setFillColor(sf::Color::White);
    txt.setOutlineColor(sf::Color::Black);
    txt.setOutlineThickness(1.f);
    txt.setString(label);
    txt.setPosition(x + 6.f, y + (h - 15.f) / 2.f);
    window.draw(txt);
}

static void drawUI(sf::RenderWindow& window, const sf::Font& font,
                   const Jucator& j, const std::string& radarInfo, int nivelCurent) {
    constexpr float x    = 14.f;
    constexpr float barW = 240.f;
    constexpr float barH = 22.f;
    float y = 14.f;
    float hpRatio = static_cast<float>(j.getHP()) / static_cast<float>(j.getHPMax());
    drawBar(window, x, y, barW, barH, hpRatio,
            hpColor(hpRatio), sf::Color(60, 0, 0),
            "HP: " + std::to_string(j.getHP()) + " / " + std::to_string(j.getHPMax()), font);
    y += barH + 8.f;
    float xpRatio = static_cast<float>(j.getXP()) / static_cast<float>(j.getXPNecesar());
    drawBar(window, x, y, barW, barH, xpRatio,
            sf::Color(30, 160, 220), sf::Color(0, 40, 70),
            "XP: " + std::to_string(j.getXP()) + " / " + std::to_string(j.getXPNecesar()), font);
    y += barH + 10.f;
    sf::Text nameText;
    nameText.setFont(font);
    nameText.setCharacterSize(15);
    nameText.setFillColor(sf::Color(255, 220, 100));
    nameText.setOutlineColor(sf::Color::Black);
    nameText.setOutlineThickness(1.5f);
    nameText.setString(j.getNume() + "  Niv." + std::to_string(j.getNivel()));
    nameText.setPosition(x, y);
    window.draw(nameText);
    y += 22.f;
    std::string nivelStr = (nivelCurent < 4)
        ? "Etaj " + std::to_string(nivelCurent) + " / 3  [D = usa spre urmatorul etaj]"
        : "ETAJ BOSS  -  Invinge bosul pentru a castiga!";
    sf::Text nivelText;
    nivelText.setFont(font);
    nivelText.setCharacterSize(13);
    nivelText.setFillColor(nivelCurent < 4 ? sf::Color(180, 180, 255) : sf::Color(255, 100, 100));
    nivelText.setOutlineColor(sf::Color::Black);
    nivelText.setOutlineThickness(1.f);
    nivelText.setString(nivelStr);
    nivelText.setPosition(x, y);
    window.draw(nivelText);
    if (!radarInfo.empty()) {
        y += 22.f;
        sf::Text radarText;
        radarText.setFont(font);
        radarText.setCharacterSize(12);
        radarText.setFillColor(sf::Color(150, 255, 150));
        radarText.setOutlineColor(sf::Color::Black);
        radarText.setOutlineThickness(1.f);
        radarText.setString(radarInfo);
        radarText.setPosition(x, y);
        window.draw(radarText);
    }
}

static void drawMessage(sf::RenderWindow& window, const sf::Font& font, const std::string& msg) {
    if (msg.empty()) return;
    sf::Text t;
    t.setFont(font);
    t.setCharacterSize(16);
    t.setFillColor(sf::Color(255, 255, 150));
    t.setOutlineColor(sf::Color::Black);
    t.setOutlineThickness(1.5f);
    t.setString(msg);
    t.setPosition(10.f, static_cast<float>(SCREEN_H) - 44.f);
    window.draw(t);
}

static void drawBattleLog(sf::RenderWindow& window, const sf::Font& font,
                          const std::vector<std::string>& linii) {
    constexpr int MAX_LINII = 6;
    constexpr float lineH   = 16.f;
    float startY = static_cast<float>(SCREEN_H) - 20.f - MAX_LINII * lineH;
    int start = static_cast<int>(linii.size()) - MAX_LINII;
    if (start < 0) start = 0;
    for (int i = start; i < static_cast<int>(linii.size()); ++i) {
        sf::Text t;
        t.setFont(font);
        t.setCharacterSize(12);
        t.setFillColor(sf::Color(200, 200, 200, 200));
        t.setOutlineColor(sf::Color::Black);
        t.setOutlineThickness(1.f);
        t.setString(linii[i]);
        t.setPosition(static_cast<float>(SCREEN_W) - 520.f,
                      startY + static_cast<float>(i - start) * lineH);
        window.draw(t);
    }
}

static void drawDoor(sf::RenderWindow& window, sf::Sprite& sprite,
                     const sf::Texture& tex, const Labirint& lab,
                     int camX, int camY, int nivelCurent, float time) {
    if (nivelCurent >= 4) return;
    Pozitie usa = lab.getPozitieUsa();
    int sx = usa.getY() * TILE_SCALED - camX;
    int sy = usa.getX() * TILE_SCALED - camY;
    if (sx < -TILE_SCALED || sx > SCREEN_W || sy < -TILE_SCALED || sy > SCREEN_H) return;
    sf::RectangleShape glow({static_cast<float>(TILE_SCALED + 8),
                             static_cast<float>(TILE_SCALED + 8)});
    float pulse = 0.5f + 0.5f * std::sin(time * 3.f);
    glow.setFillColor(sf::Color(100, 200, 255, static_cast<sf::Uint8>(60 + 60 * pulse)));
    glow.setPosition(static_cast<float>(sx - 4), static_cast<float>(sy - 4));
    window.draw(glow);
    drawTile(window, sprite, tex, TC_DOOR, sx, sy);
}

static bool suntAdiacente(const Pozitie& a, const Pozitie& b) {
    int dr = std::abs(a.getX() - b.getX());
    int dc = std::abs(a.getY() - b.getY());
    return dr <= 1 && dc <= 1 && (dr + dc) > 0;
}

static void procesezaCombatLocal(JocDungeon& joc,
                                  std::vector<FloatText>& floats,
                                  const sf::Font& font,
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
        BattleLog::adaugaEveniment(descriere);
        logLinii.push_back(descriere);
        spawnFloatText(floats, font,
                       "-" + std::to_string(dmgDat),
                       static_cast<float>(e->getPozitie().getY()),
                       static_cast<float>(e->getPozitie().getX()),
                       sf::Color(255, 80, 80));
        if (!e->esteViu()) {
            std::string evMoarte = e->getNume() + " a fost eliminat!";
            BattleLog::adaugaEveniment(evMoarte);
            logLinii.push_back(evMoarte);
            if (const Inamic* in = dynamic_cast<const Inamic*>(e.get())) {
                jucator.adaugaXP(in->getXPReward());
                spawnFloatText(floats, font,
                               "+" + std::to_string(in->getXPReward()) + " XP",
                               static_cast<float>(jPos.getY()),
                               static_cast<float>(jPos.getX()),
                               sf::Color(80, 220, 255));
                int nivelNou = jucator.getNivel();
                if (nivelNou > nivelAnterior) {
                    std::string msgLvl = GameData::getMesajLevelUp(nivelNou);
                    BattleLog::adaugaEveniment(msgLvl);
                    logLinii.push_back(msgLvl);
                    nivelAnterior = nivelNou;
                }
            }
            continue;
        }
        int dmgPrimit = e->calculeazaDamage();
        jucator.primesteDamage(dmgPrimit);
        std::string evContra = e->getNume() + " contraataca: -" + std::to_string(dmgPrimit) + " HP";
        BattleLog::adaugaEveniment(evContra);
        logLinii.push_back(evContra);
        logLinii.push_back(GameData::getDescriereInamic(e->getNume()));
        spawnFloatText(floats, font,
                       "-" + std::to_string(dmgPrimit),
                       static_cast<float>(jPos.getY()),
                       static_cast<float>(jPos.getX()),
                       sf::Color(255, 60, 60));
        std::ostringstream oss;
        e->afiseaza(oss);
        if (const Fantoma* f = dynamic_cast<const Fantoma*>(e.get())) {
            BattleLog::adaugaEveniment(oss.str() +
                (f->esteCorporeala() ? " [corp]" : " [incorp]"));
        }
    }
    if (!atacatCeva) {
        BattleLog::adaugaEveniment(jucator.getNume() + " loveste in gol.");
        logLinii.push_back(jucator.getNume() + " loveste in gol.");
    }
}

static void populeazaNivel(JocDungeon& joc, int nivelCurent) {
    if (nivelCurent == 1) {
        joc.adaugaEntitate(std::make_unique<Inamic>("Goblin Infatometat", Pozitie(3, 3), 40, 15));
        joc.adaugaEntitate(std::make_unique<Inamic>("Orc Distrugator",    Pozitie(5, 7), 70, 30));
        joc.adaugaEntitate(std::make_unique<Fantoma>("Spectra Blestemata", Pozitie(14, 8), 35));
    } else if (nivelCurent == 2) {
        joc.adaugaEntitate(std::make_unique<Inamic>("Goblin Sangeros",    Pozitie(4, 4), 55, 20));
        joc.adaugaEntitate(std::make_unique<Inamic>("Orc Berserker",      Pozitie(6, 10), 90, 35));
        joc.adaugaEntitate(std::make_unique<VrajitorInamic>("Vrajitor Umbra", Pozitie(10, 5), 45, 25));
        joc.adaugaEntitate(std::make_unique<Fantoma>("Fantoma Razbunarii", Pozitie(12, 12), 50));
    } else if (nivelCurent == 3) {
        joc.adaugaEntitate(std::make_unique<Inamic>("Goblin Elita",       Pozitie(3, 5),  70, 25));
        joc.adaugaEntitate(std::make_unique<Inamic>("Orc Campion",        Pozitie(7, 9), 110, 40));
        joc.adaugaEntitate(std::make_unique<VrajitorInamic>("Arhivrajitor", Pozitie(9, 3), 65, 35));
        joc.adaugaEntitate(std::make_unique<Fantoma>("Spectra Antica",    Pozitie(15, 15), 75));
        joc.adaugaEntitate(std::make_unique<Inamic>("Orc Garda",         Pozitie(12, 6), 80, 30));
    } else {
        joc.adaugaEntitate(std::make_unique<Inamic>("BOSS - Lordul Intunericului", Pozitie(10, 15), 300, 60));
    }
}

static void treceLaNivelUrmator(JocDungeon& joc, int& nivelCurent,
                                 std::vector<std::string>& logLinii,
                                 std::string& message, sf::Clock& msgClock) {
    nivelCurent++;
    joc.getLabirint().genereazaRandom();
    joc.getJucator().setPozitie(Pozitie(1, 1));
    while (!joc.getEntitati().empty()) {
        const_cast<std::vector<std::unique_ptr<Entitate>>&>(joc.getEntitati()).clear();
        break;
    }
    populeazaNivel(joc, nivelCurent);
    std::string msg;
    if (nivelCurent < 4)
        msg = "Ai ajuns la etajul " + std::to_string(nivelCurent) + "!";
    else
        msg = "CAMERA BOSSULUI! Invinge Lordul Intunericului!";
    logLinii.push_back(msg);
    BattleLog::adaugaEveniment(msg);
    message = msg;
    msgClock.restart();
}

static void initLogica(JocDungeon& joc, Inventar& rucsac) {
    joc.initSesiune();
    std::cout << GameData::getPovesteFundal() << "\n\n";
    std::cout << "Inamici in dungeon:\n";
    for (const auto& tip : GameData::getTipuriInamici())
        std::cout << "  - " << tip << "\n";
    std::cout << "\n";
    std::cout << "Aparare jucator: " << joc.getJucator().getAparare() << "\n";
    std::cout << "DEX jucator: " << joc.getJucator().getDex() << "\n";
    joc.getJucator().setStr(10);
    joc.getJucator().setDex(10);
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
    BattleLog::adaugaEveniment("Sesiune noua inceputa: " + joc.getNume());
    Celula tmp(true);
    tmp.seteazaPerete(false);
    tmp.toggle();
    BattleLog::adaugaEveniment("Celula test: " + std::string(1, tmp.getSimbol()));
    std::cout << "Sesiuni create: " << JocDungeon::getSesiuniCreate() << "\n";
}

static void drawEndScreen(sf::RenderWindow& window, const sf::Font& font,
                          bool won, const Jucator& j, int nivelCurent) {
    sf::RectangleShape overlay({static_cast<float>(SCREEN_W), static_cast<float>(SCREEN_H)});
    overlay.setFillColor(sf::Color(0, 0, 0, won ? 160 : 200));
    window.draw(overlay);
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setCharacterSize(56);
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setOutlineThickness(4.f);
    sf::Text subText;
    subText.setFont(font);
    subText.setCharacterSize(22);
    subText.setFillColor(sf::Color(200, 200, 200));
    subText.setOutlineColor(sf::Color::Black);
    subText.setOutlineThickness(2.f);
    sf::Text statsText;
    statsText.setFont(font);
    statsText.setCharacterSize(16);
    statsText.setFillColor(sf::Color(180, 180, 180));
    statsText.setOutlineColor(sf::Color::Black);
    statsText.setOutlineThickness(1.5f);
    if (won) {
        titleText.setFillColor(sf::Color(255, 215, 0));
        titleText.setString("VICTORIE!");
        subText.setString("Lordul Intunericului a cazut. Dungeon-ul e liber!\nApasa ESC pentru a iesi.");
    } else {
        titleText.setFillColor(sf::Color(200, 40, 40));
        titleText.setString("AI MURIT.");
        subText.setString("Dungeon-ul te-a invins.\nApasa ESC pentru a iesi.");
    }
    statsText.setString(
        "Nivel atins: " + std::to_string(nivelCurent) + " / 4" +
        "     Nivel jucator: " + std::to_string(j.getNivel()) +
        "     HP ramas: " + std::to_string(std::max(0, j.getHP())) + " / " + std::to_string(j.getHPMax())
    );
    sf::FloatRect tb = titleText.getLocalBounds();
    titleText.setPosition((SCREEN_W - tb.width) / 2.f, SCREEN_H / 2.f - 90.f);
    window.draw(titleText);
    sf::FloatRect sb = subText.getLocalBounds();
    subText.setPosition((SCREEN_W - sb.width) / 2.f, SCREEN_H / 2.f + 10.f);
    window.draw(subText);
    sf::FloatRect stb = statsText.getLocalBounds();
    statsText.setPosition((SCREEN_W - stb.width) / 2.f, SCREEN_H / 2.f + 80.f);
    window.draw(statsText);
}

int main() {
    JocDungeon joc("Dungeon of Doom", 20, 30);
    Inventar rucsac;
    Radar radar(8);
    int nivelCurent = 1;

    try {
        initLogica(joc, rucsac);
    } catch (const DungeonException&) {
        return 1;
    }

    populeazaNivel(joc, nivelCurent);

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
        BattleLog::afiseazaLog();
        BattleLog::curataLog();
        return 0;
    }

    {
        Pozitie startPos = joc.getJucator().getPozitie();
        const Celula& startCelula = joc.getLabirint().getCelula(
            startPos.getX(), startPos.getY());
        BattleLog::adaugaEveniment("Start pe: " +
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

    sf::Sprite sprite;
    sf::Clock moveClock;
    sf::Clock enemyMoveClock;
    sf::Clock deltaClock;
    sf::Clock gameClock;
    constexpr float moveDelay      = 0.15f;
    constexpr float enemyMoveDelay = 0.5f;
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
                BattleLog::afiseazaLog();
                window.close();
            }
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Space &&
                !gameWon && !gameDead) {
                procesezaCombatLocal(joc, floatTexts, font, logLinii, nivelAnterior);
                joc.procesezaCombat(joc.getJucator());
                joc.procesezaVrajitori(joc.getJucator());
                joc.procesezaFantome(joc.getJucator());
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
                BattleLog::afiseazaLog();
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
                                treceLaNivelUrmator(joc, nivelCurent, logLinii, message, msgClock);
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

        Pozitie posJ = joc.getJucator().getPozitie();
        int camX = posJ.getY() * TILE_SCALED - SCREEN_W / 2;
        int camY = posJ.getX() * TILE_SCALED - SCREEN_H / 2;

        window.clear(sf::Color(15, 10, 20));

        for (int r = 0; r < labLinii; ++r) {
            for (int c = 0; c < labColoane; ++c) {
                int sx = c * TILE_SCALED - camX;
                int sy = r * TILE_SCALED - camY;
                if (sx < -TILE_SCALED || sx > SCREEN_W ||
                    sy < -TILE_SCALED || sy > SCREEN_H) continue;
                TileCoord tc = lab.getCelula(r, c).estePerete() ? TC_WALL : TC_FLOOR;
                drawTile(window, sprite, tileTex, tc, sx, sy);
            }
        }

        drawDoor(window, sprite, tileTex, lab, camX, camY, nivelCurent, totalTime);

        for (const auto& e : joc.getEntitati()) {
            if (!e->esteViu()) continue;
            Pozitie ep = e->getPozitie();
            int sx = ep.getY() * TILE_SCALED - camX;
            int sy = ep.getX() * TILE_SCALED - camY;
            if (sx < -TILE_SCALED || sx > SCREEN_W ||
                sy < -TILE_SCALED || sy > SCREEN_H) continue;
            drawTile(window, sprite, tileTex, getTileForEntitate(*e), sx, sy);
            constexpr float miniW = static_cast<float>(TILE_SCALED);
            constexpr float miniH = 5.f;
            float ratio = static_cast<float>(e->getHP()) / 100.f;
            ratio = std::min(1.f, std::max(0.f, ratio));
            sf::RectangleShape miniBg({miniW, miniH});
            miniBg.setFillColor(sf::Color(80, 0, 0));
            miniBg.setPosition(static_cast<float>(sx), static_cast<float>(sy) - miniH - 2.f);
            window.draw(miniBg);
            sf::RectangleShape miniFg({miniW * ratio, miniH});
            miniFg.setFillColor(hpColor(ratio));
            miniFg.setPosition(static_cast<float>(sx), static_cast<float>(sy) - miniH - 2.f);
            window.draw(miniFg);
        }

        drawTile(window, sprite, tileTex, TC_PLAYER,
                 posJ.getY() * TILE_SCALED - camX,
                 posJ.getX() * TILE_SCALED - camY);

        updateAndDrawFloats(window, floatTexts, dt, camX, camY);
        drawUI(window, font, joc.getJucator(), radarInfo, nivelCurent);
        drawBattleLog(window, font, logLinii);

        if (msgClock.getElapsedTime().asSeconds() < msgDuration)
            drawMessage(window, font, message);

        if (gameWon || gameDead)
            drawEndScreen(window, font, gameWon, joc.getJucator(), nivelCurent);

        window.display();
    }

    return 0;
}