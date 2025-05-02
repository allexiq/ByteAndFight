#pragma once
#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include <SFML/Graphics.hpp>
#include "Entity.h"

void drawHealthBar(sf::RenderWindow& window, Entity& entity, float x, float y, sf::Font& font);

#endif // HEALTHBAR_H
