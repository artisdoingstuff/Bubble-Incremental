#pragma once

#include "../Misc/GIncludes.hpp"

struct Star {
    sf::VertexArray star;
};

inline sf::Color lerpColor(sf::Color a, sf::Color b, float t) {
    if (t < 0) t = 0; if (t > 1) t = 1;
    return sf::Color(
        static_cast<std::uint8_t>(a.r + (b.r - a.r) * t),
        static_cast<std::uint8_t>(a.g + (b.g - a.g) * t),
        static_cast<std::uint8_t>(a.b + (b.b - a.b) * t)
    );
}

inline void updateStar(Star& star, sf::Vector2f centre, float time, float currentScale, long double totalBits) {
    const int numArms = 16;
    const size_t segmentsPerArm = 50;
    const size_t verticesPerArm = segmentsPerArm * 2;
    const size_t totalVertices = verticesPerArm * numArms;

    if (star.star.getVertexCount() != totalVertices) {
        star.star.resize(totalVertices);
        star.star.setPrimitiveType(sf::PrimitiveType::Lines);
    }

    float logVal = (totalBits > 1.0L) ? static_cast<float>(std::log10(totalBits)) : 0.f;
    sf::Color starColor;

    if (logVal <= 9.f) starColor = sf::Color(255, 0, 0);
    
    else if (logVal <= 18.f) starColor = lerpColor(sf::Color(255, 0, 0), sf::Color(255, 255, 0), (logVal - 9.f) / 9.f);
    
    else if (logVal <= 45.f) starColor = lerpColor(sf::Color(255, 255, 0), sf::Color(255, 255, 255), (logVal - 18.f) / 27.f);
    
    else starColor = lerpColor(sf::Color(255, 255, 255), sf::Color(0, 100, 255), (logVal - 45.f) / 54.f);

    for (int arm = 0; arm < numArms; ++arm) {
        float armOffset = (arm * 2.f * pi) / numArms;

        for (size_t s = 0; s < segmentsPerArm; ++s) {
            size_t idx = (arm * verticesPerArm) + (s * 2);

            for (int j = 0; j < 2; ++j) {
                float ratio = (s + j) / static_cast<float>(segmentsPerArm);

                float angle = (ratio * 5.f) - (time * 0.4f) + armOffset;
                float radius = ratio * 350.f * currentScale;

                float wave = std::sin(time * 6.f + ratio * 2.f);
                float thickness = wave * 6.f * currentScale;

                float x = centre.x + (radius * std::cos(angle)) + (thickness * std::sin(angle));
                float y = centre.y + (radius * std::sin(angle)) - (thickness * std::cos(angle));

                star.star[idx + j].position = { x, y };

                float alphaFade = std::pow(std::sin(ratio * pi), 1.2f);

                star.star[idx + j].color = starColor;
                star.star[idx + j].color.a = static_cast<std::uint8_t>(160 * alphaFade);
            }
        }
    }
}