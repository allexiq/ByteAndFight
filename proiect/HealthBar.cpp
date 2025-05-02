#include "HealthBar.h"

void drawHealthBar(sf::RenderWindow& window, Entity& entity, float x, float y, sf::Font& font) {
    const float width = 600.f;
    const float height = 40.f;
    float healthPercent = entity.getHealth() / 100.f;

    // Fundal semitransparent
    sf::RectangleShape background(sf::Vector2f(600, 40));
    background.setFillColor(sf::Color(0, 0, 0, 150));  // Negru semi-transparent
    background.setOutlineThickness(5);
    background.setOutlineColor(sf::Color(34, 25, 3));
    background.setPosition(x, y);
    window.draw(background);

    // Culoarea depinde de procent
    sf::Color barColor = sf::Color(86, 147, 81);
    if (healthPercent < 0.3f)
        barColor = sf::Color(93, 20, 10);
    else if (healthPercent < 0.6f)
        barColor = sf::Color(227, 166, 26);

    // Health bar
    sf::RectangleShape healthBar(sf::Vector2f(width * healthPercent, height));
    healthBar.setFillColor(barColor);
    healthBar.setPosition(x, y);
    window.draw(healthBar);
}