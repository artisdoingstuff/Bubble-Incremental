#pragma once

#include "../Misc/GIncludes.hpp"

struct Star {
    sf::VertexArray star;
};

inline void updateStar(Star& star, sf::Vector2f centre, float time, float currentScale) {
    size_t totalVertices = star.star.getVertexCount();
    const int numArms = 4;
    size_t pointsPerArm = totalVertices / numArms;

    for (int arm = 0; arm < numArms; ++arm) {
        float armOffset = (arm * 2.f * 3.14159f) / numArms;

        for (size_t i = 0; i < pointsPerArm; ++i) {
            size_t idx = arm * pointsPerArm + i;
            float ratio = i / static_cast<float>(pointsPerArm);

            float angle = ratio * 80.f - (time * 0.5f) + armOffset;

            float radius = ratio * 250.f * currentScale;

            float thickness = std::sin(time * 15.f + i) * 3.5f;

            float x = centre.x + (radius + thickness) * std::cos(angle);
            float y = centre.y + (radius + thickness) * std::sin(angle);

            star.star[idx].position = { x, y };
            star.star[idx].color = sf::Color(243, 238, 225, 120);
        }
    }
}