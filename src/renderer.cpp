#include "renderer.h"
#include <cmath>
#include <algorithm>
#include <memory>

Renderer::Renderer(sf::RenderWindow& window, const sf::Font& font, const sf::Texture& tileTex)
    : window(window), font(font), tileTex(tileTex) {
    sprite.setTexture(this->tileTex);
}

void Renderer::drawTile(TileCoord tc, int sx, int sy) {
    sprite.setTextureRect(tc.toRect());
    sprite.setScale(static_cast<float>(SCALE), static_cast<float>(SCALE));
    sprite.setPosition(static_cast<float>(sx), static_cast<float>(sy));
    window.draw(sprite);
}

sf::Color Renderer::hpColor(float ratio) {
    if (ratio > 0.6f) return sf::Color(40, 200, 40);
    if (ratio > 0.3f) return sf::Color(220, 200, 20);
    return sf::Color(220, 40, 40);
}

void Renderer::drawBar(float x, float y, float w, float h, float ratio,
                       sf::Color fg, sf::Color bg, const std::string& label) {
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

void Renderer::drawUI(const Jucator& j, const std::string& radarInfo, int nivelCurent) {
    constexpr float x    = 14.f;
    constexpr float barW = 240.f;
    constexpr float barH = 22.f;
    float y = 14.f;
    float hpRatio = static_cast<float>(j.getHP()) / static_cast<float>(j.getHPMax());
    drawBar(x, y, barW, barH, hpRatio,
            hpColor(hpRatio), sf::Color(60, 0, 0),
            "HP: " + std::to_string(j.getHP()) + " / " + std::to_string(j.getHPMax()));
    y += barH + 8.f;
    float xpRatio = static_cast<float>(j.getXP()) / static_cast<float>(j.getXPNecesar());
    drawBar(x, y, barW, barH, xpRatio,
            sf::Color(30, 160, 220), sf::Color(0, 40, 70),
            "XP: " + std::to_string(j.getXP()) + " / " + std::to_string(j.getXPNecesar()));
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

void Renderer::drawMessage(const std::string& msg) {
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

void Renderer::drawBattleLog(const std::vector<std::string>& linii) {
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

void Renderer::drawDoor(const Labirint& lab, int camX, int camY, int nivelCurent, float time) {
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
    drawTile(TC_DOOR, sx, sy);
}

TileCoord Renderer::getTileForEntitate(const Entitate& e) {
    if (dynamic_cast<const VrajitorInamic*>(&e)) return {3, 9};
    if (dynamic_cast<const Fantoma*>(&e))        return {1, 10};
    if (dynamic_cast<const Inamic*>(&e)) {
        if (e.getNume().find("Orc") != std::string::npos ||
            e.getNume().find("BOSS") != std::string::npos ||
            e.getNume().find("Boss") != std::string::npos) return {0, 8};
        return {0, 9};
    }
    return {0, 9};
}

void Renderer::drawLabirint(const Labirint& lab, int camX, int camY) {
    for (int r = 0; r < lab.getLinii(); ++r) {
        for (int c = 0; c < lab.getColoane(); ++c) {
            int sx = c * TILE_SCALED - camX;
            int sy = r * TILE_SCALED - camY;
            if (sx < -TILE_SCALED || sx > SCREEN_W ||
                sy < -TILE_SCALED || sy > SCREEN_H) continue;
            drawTile(lab.getCelula(r, c).estePerete() ? TC_WALL : TC_FLOOR, sx, sy);
        }
    }
}

void Renderer::drawEntitati(const std::vector<std::unique_ptr<Entitate>>& entitati,
                             int camX, int camY) {
    for (const auto& e : entitati) {
        if (!e->esteViu()) continue;
        Pozitie ep = e->getPozitie();
        int sx = ep.getY() * TILE_SCALED - camX;
        int sy = ep.getX() * TILE_SCALED - camY;
        if (sx < -TILE_SCALED || sx > SCREEN_W ||
            sy < -TILE_SCALED || sy > SCREEN_H) continue;
        drawTile(getTileForEntitate(*e), sx, sy);
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
}

void Renderer::drawJucator(const Pozitie& pos, int camX, int camY) {
    drawTile(TC_PLAYER, pos.getY() * TILE_SCALED - camX, pos.getX() * TILE_SCALED - camY);
}

void Renderer::spawnFloatText(std::vector<FloatText>& floats, const std::string& str,
                               float tileCol, float tileRow, sf::Color color) {
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

void Renderer::updateAndDrawFloats(std::vector<FloatText>& floats, float dt, int camX, int camY) {
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

void Renderer::drawEndScreen(bool won, const Jucator& j, int nivelCurent) {
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