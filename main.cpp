#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>
#include "jocdungeon.h"
#include "jucator.h"
#include "inamic.h"
#include "vrajitorinamic.h"
#include "fantoma.h"
#include "inventar.h"
#include "exceptii.h"
#include "pistoale.h"
#include "sabie.h"
#include "spear.h"
#include "potiune.h"

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
static constexpr TileCoord TC_FLOOR  = {0, 0};
static constexpr TileCoord TC_WALL   = {4, 3};
static constexpr TileCoord TC_PLAYER = {1, 8};
static constexpr TileCoord TC_WIZARD = {3, 9};
static constexpr TileCoord TC_GHOST  = {1, 10};
static constexpr TileCoord TC_ORC    = {2, 7};
static constexpr TileCoord TC_GOBLIN = {0, 9};

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
        if (e.getNume().find("Orc") != std::string::npos) return TC_ORC;
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

static void drawUI(sf::RenderWindow& window, const sf::Font& font, const Jucator& j) {
    constexpr float x    = 14.f;
    constexpr float barW = 240.f;
    constexpr float barH = 22.f;
    float y = 14.f;

    float hpRatio = static_cast<float>(j.getHP()) / static_cast<float>(j.getHPMax());
    drawBar(window, x, y, barW, barH, hpRatio,
            hpColor(hpRatio), sf::Color(60, 0, 0),
            "HP: " + std::to_string(j.getHP()) + " / " + std::to_string(j.getHPMax()),
            font);

    y += barH + 8.f;
    float xpRatio = static_cast<float>(j.getXP()) / static_cast<float>(j.getXPNecesar());
    drawBar(window, x, y, barW, barH, xpRatio,
            sf::Color(30, 160, 220), sf::Color(0, 40, 70),
            "XP: " + std::to_string(j.getXP()) + " / " + std::to_string(j.getXPNecesar()),
            font);

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


static bool suntAdiacente(const Pozitie& a, const Pozitie& b) {
    int dr = std::abs(a.getX() - b.getX());
    int dc = std::abs(a.getY() - b.getY());
    return dr <= 1 && dc <= 1 && (dr + dc) > 0;
}

static void procesezaCombatLocal(JocDungeon& joc,
                                  std::vector<FloatText>& floats,
                                  const sf::Font& font) {
    Jucator& jucator = joc.getJucator();
    Pozitie jPos = jucator.getPozitie();

    for (const auto& e : joc.getEntitati()) {
        if (!e->esteViu()) continue;
        if (!suntAdiacente(jPos, e->getPozitie())) continue;

        int dmgDat = jucator.calculeazaDamage();
        e->primesteDamage(dmgDat);
        spawnFloatText(floats, font,
                       "-" + std::to_string(dmgDat),
                       static_cast<float>(e->getPozitie().getY()),
                       static_cast<float>(e->getPozitie().getX()),
                       sf::Color(255, 80, 80));

        if (!e->esteViu()) {
            if (const Inamic* in = dynamic_cast<const Inamic*>(e.get())) {
                jucator.adaugaXP(in->getXPReward());
                spawnFloatText(floats, font,
                               "+" + std::to_string(in->getXPReward()) + " XP",
                               static_cast<float>(jPos.getY()),
                               static_cast<float>(jPos.getX()),
                               sf::Color(80, 220, 255));
            }
            continue;
        }

        int dmgPrimit = e->calculeazaDamage();
        jucator.primesteDamage(dmgPrimit);
        spawnFloatText(floats, font,
                       "-" + std::to_string(dmgPrimit),
                       static_cast<float>(jPos.getY()),
                       static_cast<float>(jPos.getX()),
                       sf::Color(255, 60, 60));
    }
}

int main() {
    JocDungeon joc("Dungeon of Doom", 20, 30);

    try {
        joc.initSesiune();
    } catch (const DungeonException&) {
        return 1;
    }

    Inventar rucsac;
    try {
        rucsac.adaugaObiect(std::make_unique<Potiune>("Potion of Healing", 30, 50));
        rucsac.adaugaObiect(std::make_unique<Sabie>(50, 30));
        rucsac.adaugaObiect(std::make_unique<Spear>(75, 35));
        auto pistolNou = std::make_unique<Pistoale>();
        pistolNou->reincarca();
        rucsac.adaugaObiect(std::move(pistolNou));
        rucsac.adaugaObiect(std::make_unique<Potiune>("Potion mica", 15, 30));
    } catch (const InventarException&) {}

    joc.adaugaEntitate(std::make_unique<Inamic>("Goblin Infatometat", Pozitie(3, 3), 40, 15));
    joc.adaugaEntitate(std::make_unique<Inamic>("Orc Distrugator",    Pozitie(5, 7), 70, 30));
    joc.adaugaEntitate(std::make_unique<Fantoma>("Spectra Blestemata", Pozitie(14, 8), 35));

    sf::RenderWindow window(sf::VideoMode(SCREEN_W, SCREEN_H), "Dungeon Crawler", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    sf::Texture tileTex;
    if (!tileTex.loadFromFile("assets/tilemap.png")) return 1;
    tileTex.setSmooth(false);

    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
        font.loadFromFile("assets/font.ttf");

    sf::Sprite sprite;
    sf::Clock moveClock;
    sf::Clock deltaClock;
    constexpr float moveDelay = 0.15f;
    std::string message;
    sf::Clock msgClock;
    constexpr float msgDuration = 2.f;

    std::vector<FloatText> floatTexts;

    const Labirint& lab = joc.getLabirint();
    int labLinii   = lab.getLinii();
    int labColoane = lab.getColoane();

    while (window.isOpen()) {
        float dt = deltaClock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape) window.close();

            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Space) {
                procesezaCombatLocal(joc, floatTexts, font);
                message = "Atac!";
                msgClock.restart();
            }
        }

        if (moveClock.getElapsedTime().asSeconds() >= moveDelay) {
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
                    }
                }
                moveClock.restart();
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
        drawUI(window, font, joc.getJucator());

        if (msgClock.getElapsedTime().asSeconds() < msgDuration)
            drawMessage(window, font, message);

        window.display();
    }

    return 0;
}