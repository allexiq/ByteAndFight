#pragma once
#ifndef GAME_FUNCTIONS_H
#define GAME_FUNCTIONS_H

#include <SFML/Graphics.hpp>
#include <map>
#include "Player.h"
#include "Boss.h"
#include "Zombie.h"
#include "Animation.h"
#include "Difficulty.h"
#include "HealthBar.h"


void initializeTextures(sf::Texture& menuBgTex, sf::Texture& bgEasy, sf::Texture& bgMedium, sf::Texture& bgHard,
    sf::Texture& easyTex, sf::Texture& mediumTex, sf::Texture& hardTex);


void initializeAnimations(std::map<PlayerState, Animation*>& playerAnims, std::map<BossState, Animation*>& bossAnims,
    std::map<ZombieState, Animation*>& zombieAnims);
    

void handleDifficultySelection(sf::RenderWindow& window, bool& difficultyChosen, Difficulty& selectedDifficulty,
    sf::Sprite& easySprite, sf::Sprite& mediumSprite, sf::Sprite& hardSprite,
    sf::Sprite& background, sf::Texture& bgEasy, sf::Texture& bgMedium, sf::Texture& bgHard);


void gameLoop(sf::RenderWindow& window, Player& player, Boss& boss, Zombie* zombie, sf::Sprite& background,
    float GROUND_Y, sf::Font& font, sf::Texture& bgEasy, sf::Texture& bgMedium, sf::Texture& bgHard);

#endif // GAME_FUNCTIONS_H
