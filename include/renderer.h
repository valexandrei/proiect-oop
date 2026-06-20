#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>
#include "labirint.h"
#include "jucator.h"
#include "entitate.h"
#include "inamic.h"
#include "vrajitorinamic.h"
#include "fantoma.h"

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

struct FloatText {
    sf::Text text;
    sf::Vector2f pos;
    float life = 1.2f;
};

class Renderer {
public:
    Renderer(sf::RenderWindow& window, const sf::Font& font, const sf::Texture& tileTex);

    void drawTile(TileCoord tc, int sx, int sy);
    void drawUI(const Jucator& j, const std::string& radarInfo, int nivelCurent);
    void drawMessage(const std::string& msg);
    void drawBattleLog(const std::vector<std::string>& linii);
    void drawDoor(const Labirint& lab, int camX, int camY, int nivelCurent, float time);
    void drawEndScreen(bool won, const Jucator& j, int nivelCurent);
    void drawLabirint(const Labirint& lab, int camX, int camY);
    void drawEntitati(const std::vector<std::unique_ptr<Entitate>>& entitati, int camX, int camY);
    void drawJucator(const Pozitie& pos, int camX, int camY);
    void updateAndDrawFloats(std::vector<FloatText>& floats, float dt, int camX, int camY);
    void spawnFloatText(std::vector<FloatText>& floats, const std::string& str,
                        float tileCol, float tileRow, sf::Color color);

    static TileCoord getTileForEntitate(const Entitate& e);
    static sf::Color hpColor(float ratio);

private:
    sf::RenderWindow& window;
    const sf::Font& font;
    sf::Texture tileTex;
    sf::Sprite sprite;

    void drawBar(float x, float y, float w, float h, float ratio,
                 sf::Color fg, sf::Color bg, const std::string& label);

    static constexpr TileCoord TC_FLOOR  = {0, 0};
    static constexpr TileCoord TC_WALL   = {4, 3};
    static constexpr TileCoord TC_PLAYER = {1, 8};
    static constexpr TileCoord TC_WIZARD = {3, 9};
    static constexpr TileCoord TC_GHOST  = {1, 10};
    static constexpr TileCoord TC_GOBLIN = {0, 9};
    static constexpr TileCoord TC_DOOR   = {2, 2};
    static constexpr TileCoord TC_BOSS   = {0, 8};
};

#endif