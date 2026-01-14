#pragma once

#include "../Misc/GIncludes.hpp"

struct DataParticle {
    sf::Text bit;
    sf::Vector2f pos;
    sf::Vector2f vel;
    float speed;
    float spiral;

    DataParticle(sf::Font& font, sf::Vector2f startPos, sf::Vector2f initialVel = { 0,0 }) : bit(font), pos(startPos), vel(initialVel) {
        bit.setString(rand() % 2 == 0 ? "0" : "1");
        bit.setCharacterSize(16);
        bit.setFillColor(sf::Color(243, 238, 225, 150));
        speed = 100.f + (rand() % 150);
        spiral = 0.5f + (static_cast<float>(rand() % 100) / 100.f);
    }
};

inline std::vector<DataParticle> dataStream;
inline sf::Clock spawnClock;

inline void updateStream(sf::RenderWindow& window, sf::Vector2f centre, float dt) {
    if (spawnClock.getElapsedTime().asSeconds() > 0.10f) {
        sf::Vector2f spawnPos;
        int edge = rand() % 4;
        if (edge == 0) spawnPos = { (float)(rand() % window.getSize().x), -50.f };
        else if (edge == 1) spawnPos = { (float)(rand() % window.getSize().x), (float)window.getSize().y + 50.f };
        else if (edge == 2) spawnPos = { -50.f, (float)(rand() % window.getSize().y) };
        else spawnPos = { (float)window.getSize().x + 50.f, (float)(rand() % window.getSize().y) };

        dataStream.emplace_back(jetBrainsMono, spawnPos);
        spawnClock.restart();
    }

    for (auto it = dataStream.begin(); it != dataStream.end();) {
        sf::Vector2f dir = centre - it->pos;
        float distance = std::sqrt(dir.x * dir.x + dir.y * dir.y);

        if (distance < 100.f || distance > 2500.f) {
            it = dataStream.erase(it);
            continue;
        }

        dir /= distance;
        sf::Vector2f spiralPos(-dir.y, dir.x);

        float pullStrength = it->speed * (1.f + (600.f / (distance + 1.f)));

        it->pos += dir * pullStrength * dt;
        it->pos += spiralPos * (pullStrength * it->spiral) * dt;

        it->bit.setPosition(it->pos);

        float alpha = std::min(255.f, distance / 1.5f);
        it->bit.setFillColor(sf::Color(243, 238, 225, (std::uint8_t)alpha));

        ++it;
    }
}