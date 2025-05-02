#include "GameFunctions.h"
#include "Difficulty.h"
void initializeTextures(sf::Texture& menuBgTex, sf::Texture& bgEasy, sf::Texture& bgMedium, sf::Texture& bgHard,
    sf::Texture& easyTex, sf::Texture& mediumTex, sf::Texture& hardTex) {
    menuBgTex.loadFromFile("postapocalypse3.png");
    bgEasy.loadFromFile("postapocalypse2.png");
    bgMedium.loadFromFile("postapocalypse4.png");
    bgHard.loadFromFile("postapocalypse1.png");

    easyTex.loadFromFile("Easy.png");
    mediumTex.loadFromFile("Medium.png");
    hardTex.loadFromFile("Hard.png");
}

void initializeAnimations(std::map<PlayerState, Animation*>& playerAnims, std::map<BossState, Animation*>& bossAnims,
    std::map<ZombieState, Animation*>& zombieAnims) {
    
    sf::Texture idleTex, runTex, jumpTex, attackTex, hurtTex, deadTex;
    idleTex.loadFromFile("Idle.png");
    runTex.loadFromFile("Run.png");
    jumpTex.loadFromFile("Jump.png");
    attackTex.loadFromFile("Shot.png");
    hurtTex.loadFromFile("Hurt.png");
    deadTex.loadFromFile("Dead.png");

    Animation idleAnim(idleTex, 6, 0.15f, { 128, 128 });
    Animation runAnim(runTex, 8, 0.1f, { 128, 128 });
    Animation jumpAnim(jumpTex, 11, 0.08f, { 128, 128 });
    Animation attackAnim(attackTex, 12, 0.05f, { 128, 128 });
    Animation hurtAnim(hurtTex, 2, 0.1f, { 128, 128 });
    Animation deadAnim(deadTex, 4, 0.1f, { 128, 128 }, false);

    playerAnims[PlayerState::Idle] = &idleAnim;
    playerAnims[PlayerState::Run] = &runAnim;
    playerAnims[PlayerState::Jump] = &jumpAnim;
    playerAnims[PlayerState::Attack] = &attackAnim;
    playerAnims[PlayerState::Hurt] = &hurtAnim;
    playerAnims[PlayerState::Dead] = &deadAnim;

  
    sf::Texture bossIdleTex, bossWalkTex, bossAttackTex, bossHurtTex, bossDeadTex;
    bossIdleTex.loadFromFile("Idle1.png");
    bossWalkTex.loadFromFile("Walk1.png");
    bossAttackTex.loadFromFile("Attack1.png");
    bossHurtTex.loadFromFile("Hurt1.png");
    bossDeadTex.loadFromFile("Dead1.png");

    Animation bossIdle(bossIdleTex, 10, 0.15f, { 128, 128 });
    Animation bossWalk(bossWalkTex, 12, 0.12f, { 128, 128 });
    Animation bossAttack(bossAttackTex, 5, 0.1f, { 128, 128 });
    Animation bossHurt(bossHurtTex, 3, 0.1f, { 128, 128 });
    Animation bossDead(bossDeadTex, 5, 0.1f, { 128, 128 }, false);

    bossAnims[BossState::Idle] = &bossIdle;
    bossAnims[BossState::Walk] = &bossWalk;
    bossAnims[BossState::Attack] = &bossAttack;
    bossAnims[BossState::Hurt] = &bossHurt;
    bossAnims[BossState::Dead] = &bossDead;


    sf::Texture idleZTex, walkZTex, attackZTex, deadZTex;
    idleZTex.loadFromFile("Idlez.png");
    walkZTex.loadFromFile("Walkz.png");
    attackZTex.loadFromFile("Attackz.png");
    deadZTex.loadFromFile("Deadz.png");

    Animation idleZ(idleZTex, 7, 0.2f, { 128, 128 });
    Animation walkZ(walkZTex, 12, 0.15f, { 128, 128 });
    Animation attackZ(attackZTex, 10, 0.1f, { 128, 128 });
    Animation deadZ(deadZTex, 5, 0.15f, { 128, 128 }, false);

    zombieAnims[ZombieState::Idle] = &idleZ;
    zombieAnims[ZombieState::Walk] = &walkZ;
    zombieAnims[ZombieState::Attack] = &attackZ;
    zombieAnims[ZombieState::Dead] = &deadZ;
}

void handleDifficultySelection(sf::RenderWindow& window, bool& difficultyChosen, Difficulty& selectedDifficulty,
    sf::Sprite& easySprite, sf::Sprite& mediumSprite, sf::Sprite& hardSprite,
    sf::Sprite& background, sf::Texture& bgEasy, sf::Texture& bgMedium, sf::Texture& bgHard) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
            if (easySprite.getGlobalBounds().contains(mousePos)) {
                selectedDifficulty = Difficulty::Easy;
                background.setTexture(bgEasy);
                difficultyChosen = true;
            }
            else if (mediumSprite.getGlobalBounds().contains(mousePos)) {
                selectedDifficulty = Difficulty::Medium;
                background.setTexture(bgMedium);
                difficultyChosen = true;
            }
            else if (hardSprite.getGlobalBounds().contains(mousePos)) {
                selectedDifficulty = Difficulty::Hard;
                background.setTexture(bgHard);
                difficultyChosen = true;
            }
        }
    }
}

void gameLoop(sf::RenderWindow& window, Player& player, Boss& boss, Zombie* zombie, sf::Sprite& background,
    float GROUND_Y, sf::Font& font, sf::Texture& bgEasy, sf::Texture& bgMedium, sf::Texture& bgHard) {
    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
                player.processEvents(event.key.code, true);
            if (event.type == sf::Event::KeyReleased)
                player.processEvents(event.key.code, false);
        }

        player.update(GROUND_Y, dt);
        sf::Vector2f pos = player.getPosition();
        sf::FloatRect bounds = player.getBounds();
        if (pos.x < 100.f)
            player.setPosition(100.f, pos.y);
        if (pos.x + bounds.width > 2500)
            player.setPosition(2500 - bounds.width, pos.y);
        if (pos.y + bounds.height > GROUND_Y)
            player.setPosition(pos.x, GROUND_Y - bounds.height);

        boss.update(GROUND_Y, dt, player.isAlive() ? player.getPosition() : sf::Vector2f(0.f, 0.f));

        if (zombie && zombie->isAlive()) {
            zombie->update(GROUND_Y, dt, player.getPosition());
            if (zombie->isAttacking() && zombie->getBounds().intersects(player.getBounds()))
                player.takeDamage(30);
            player.hurt();
        }

        // Detect Boss attack hitting Player
        if (player.isAlive() && boss.isAlive()) {
            if (boss.isAttacking() && boss.getBounds().intersects(player.getBounds())) {
                player.takeDamage(10); // Player pierde 10 HP
                player.hurt();
            }
        }

        // Detect Player attack hitting Boss
        if (player.isAlive() && boss.isAlive()) {
            if (player.isAttacking() && player.getBounds().intersects(boss.getBounds())) {
                boss.takeDamage(10);   // Boss pierde 10 HP
                boss.hurt();
            }
        }

        window.clear();
        window.draw(background);
        player.drawTo(window);
        boss.drawTo(window);
        zombie->drawTo(window);
        player.setPosition(300.f, GROUND_Y - 128.f * 5.f);
        boss.setPosition(1500.f, GROUND_Y - 128.f * 6.f);
        drawHealthBar(window, player, 10.f, 20.f, font); // viata player ului
        drawHealthBar(window, boss, 1310.f, 20.f, font);  // viata boss ului

        if (!player.isAlive() || !boss.isAlive()) {
            sf::Texture endTexture;
            sf::Sprite endSprite;
            if (!boss.isAlive()) {
                endTexture.loadFromFile("Victory.png");
            }
            else if (!player.isAlive()) {
                endTexture.loadFromFile("GameOver.png");
            }

            endSprite.setTexture(endTexture);
            sf::FloatRect spriteBounds = endSprite.getLocalBounds();
            endSprite.setOrigin(spriteBounds.width / 2.f, spriteBounds.height / 2.f);
            endSprite.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
            window.draw(endSprite);
        }

        window.display();
    }
}