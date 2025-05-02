#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Boss.h"
#include "Zombie.h"
#include "Animation.h"
#include "GameFunctions.h" 

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Byte&Fight");
    sf::Clock clock;
    sf::Font font;
    font.loadFromFile("arial.ttf");

    Difficulty selectedDifficulty;
    bool difficultyChosen = false;

    const float GROUND_Y = 920.f;

    // Texturi
    sf::Texture menuBgTex, bgEasy, bgMedium, bgHard, easyTex, mediumTex, hardTex;
    initializeTextures(menuBgTex, bgEasy, bgMedium, bgHard, easyTex, mediumTex, hardTex);

    // Animatii
    std::map<PlayerState, Animation*> playerAnims;
    std::map<BossState, Animation*> bossAnims;
    std::map<ZombieState, Animation*> zombieAnims;
    initializeAnimations(playerAnims, bossAnims, zombieAnims);

    // Sprites pentru dificultate
    sf::Sprite easySprite(easyTex), mediumSprite(mediumTex), hardSprite(hardTex);
    sf::Sprite background(menuBgTex);

    Player player(playerAnims);
    Boss boss(bossAnims);
    Zombie* zombie = new Zombie(zombieAnims);

    // Selectarea dif
    while (!difficultyChosen) {
        handleDifficultySelection(window, difficultyChosen, selectedDifficulty, easySprite, mediumSprite, hardSprite, background, bgEasy, bgMedium, bgHard);
        window.clear();
        window.draw(background);
        window.draw(easySprite);
        window.draw(mediumSprite);
        window.draw(hardSprite);
        window.display();
    }

    // Loopul principal al jocului
    gameLoop(window, player, boss, zombie, background, GROUND_Y, font, bgEasy, bgMedium, bgHard);

    delete zombie;
    return 0;
}