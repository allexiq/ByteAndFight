//fisier ce le include pe toate celelalte

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <cmath>
#include <iostream>

enum class PlayerState { Idle, Run, Jump, Attack, Hurt, Dead };
enum class BossState { Idle, Walk, Attack, Hurt, Dead };
enum class Difficulty { Easy, Medium, Hard };
enum class ZombieState { Idle, Walk, Attack, Dead };

class Animation {
public:
    Animation(sf::Texture& texture, int frameCount, float frameTime, sf::Vector2i frameSize, bool looping = true)
        : texture(texture), frameCount(frameCount), frameTime(frameTime), frameSize(frameSize), looping(looping)
    {
        currentFrame = 0;
        elapsed = 0.f;
        sprite.setTexture(texture);
        sprite.setOrigin(frameSize.x / 2.f, frameSize.y / 18.f);
        updateFrame();
    }

    void update(float dt) {
        if (!looping && currentFrame == frameCount - 1) return;

        elapsed += dt;
        if (elapsed >= frameTime) {
            elapsed = 0.f;
            if (looping) {
                currentFrame = (currentFrame + 1) % frameCount;
            }
            else {
                if (currentFrame < frameCount - 1)
                    currentFrame++;
            }
            updateFrame();
        }
    }

    void reset() {
        currentFrame = 0;
        elapsed = 0.f;
        updateFrame();
    }

    float getDuration() const {
        return frameCount * frameTime;
    }

    void setScale(float scaleX, float scaleY) {
        sprite.setScale(scaleX, scaleY);
    }

    void setPosition(float x, float y) {
        sprite.setPosition(x, y);
    }

    sf::Sprite& getSprite() {
        return sprite;
    }

private:
    sf::Texture& texture;
    sf::Sprite sprite;
    int frameCount;
    int currentFrame;
    float frameTime;
    float elapsed;
    bool looping;
    sf::Vector2i frameSize;

    void updateFrame() {
        sprite.setTextureRect(sf::IntRect(
            frameSize.x * currentFrame, 0,
            frameSize.x, frameSize.y
        ));
    }
};

class Entity {
public:
    Entity(float scaleFactor)
        : health(100), alive(true), facingRight(true), velocityY(0), onGround(false), attacking(false),
        invulnerable(false), invulTimer(0.f)
    {
        scale = scaleFactor;
    }

    virtual void update(float groundY, float dt) = 0;
    virtual void drawTo(sf::RenderWindow& window) = 0;

    bool isAlive() const { return alive; }
    int getHealth() const { return health; }
    void takeDamage(int dmg) {
        if (alive && !invulnerable) {
            health -= dmg;
            invulnerable = true; // Entity devine invulnerabil cand ia damage
            invulTimer = 1.f;    // 1 sec invulnerabilitate
            if (health <= 0) {
                health = 0;
                alive = false;
                setDead();
            }
        }
    }

    void updateInvulnerability(float dt) {
        if (invulnerable) {
            invulTimer -= dt;
            if (invulTimer <= 0.f) {
                invulnerable = false; // Entity nu mai e invulnerabil
            }
        }
    }

protected:
    int health;
    bool alive;
    bool facingRight;
    float velocityY;
    bool onGround;
    bool attacking;
    bool invulnerable;
    float invulTimer; // Timer pt invulnerabilitate
    float scale;
    float attackTimer;

    float speed;
    virtual void setDead() = 0;
};

class Player : public Entity {
public:
    Player(std::map<PlayerState, Animation*>& animations)
        : Entity(5.f), animations(animations), state(PlayerState::Idle)
    {
        currentAnimation = animations[state];
        for (auto& pair : animations)
            pair.second->setScale(scale, scale);
    }

    void setPosition(float x, float y) {
        for (auto& pair : animations) {
            pair.second->setPosition(x, y);
        }
    }

    sf::Vector2f getPosition() const {
        return currentAnimation->getSprite().getPosition();
    }

    void processEvents(sf::Keyboard::Key key, bool pressed) {
        if (attacking || !alive) return;

        if (pressed) {
            if (key == sf::Keyboard::A) left = true;
            if (key == sf::Keyboard::D) right = true;
            if (key == sf::Keyboard::Space && onGround) {
                velocityY = -jumpStrength;
                onGround = false;
            }
            if (key == sf::Keyboard::J) {
                attacking = true;
                attackTimer = 0;
                hasHitDuringAttack = false;
                setState(PlayerState::Attack);
            }
        }
        else {
            if (key == sf::Keyboard::A) left = false;
            if (key == sf::Keyboard::D) right = false;
        }
    }

    void update(float groundY, float dt) override {

        if (!alive) {
            setState(PlayerState::Dead);
            currentAnimation->update(dt);
            return;
        }

        sf::Vector2f movement;

        if (!attacking) {
            if (left) { movement.x -= moveSpeed; facingRight = false; }
            if (right) { movement.x += moveSpeed; facingRight = true; }
        }
        
        currentAnimation->setScale(facingRight ? scale : -scale, scale);

        velocityY += gravity;
        movement.y += velocityY;
        currentAnimation->getSprite().move(movement);

        if (currentAnimation->getSprite().getPosition().y + frameHeight >= groundY) {
            currentAnimation->getSprite().setPosition(currentAnimation->getSprite().getPosition().x, groundY - frameHeight);
            velocityY = 0;
            onGround = true;
        }
        else {
            onGround = false;
        }

        if (attacking) {
            attackTimer += dt;
            if (attackTimer >= animations[PlayerState::Attack]->getDuration()) {
                attacking = false;
            }
        }

        if (!attacking) {
            if (!onGround) setState(PlayerState::Jump);
            else if (left || right) setState(PlayerState::Run);
            else setState(PlayerState::Idle);
        }

        currentAnimation->update(dt);

        updateInvulnerability(dt); // Update status invulnerabilitate
    }
    bool isAttacking() const { return attacking; }

    sf::FloatRect getBounds() const {
        return currentAnimation->getSprite().getGlobalBounds();
    }

    void hurt() {
        setState(PlayerState::Hurt);
    }
    void drawTo(sf::RenderWindow& window) override {
        window.draw(currentAnimation->getSprite());
    }

private:
    std::map<PlayerState, Animation*>& animations;
    Animation* currentAnimation;
    PlayerState state;

    bool left = false, right = false;
    bool hasHitDuringAttack = false;
    const float gravity = 0.5f;
    const float jumpStrength = 12.0f;
    const float moveSpeed = 5.0f;
    const float frameHeight = 128.f * 5.f;


    void setState(PlayerState newState) {
        if (state != newState) {
            sf::Vector2f pos = currentAnimation->getSprite().getPosition();
            state = newState;
            currentAnimation = animations[state];
            currentAnimation->setPosition(pos.x, pos.y);
            currentAnimation->reset();
        }
    }

    void setDead() override {
        setState(PlayerState::Dead);
    }
};

class Boss : public Entity {
public:
    Boss(std::map<BossState, Animation*>& animations)
        : Entity(6.f), animations(animations), state(BossState::Idle)
    {
        currentAnimation = animations[state];
        for (auto& pair : animations)
            pair.second->setScale(scale, scale);
    }

    void setPosition(float x, float y) {
        for (auto& pair : animations) {
            pair.second->setPosition(x, y);
        }
    }

    sf::Vector2f getPosition() const {
        return currentAnimation->getSprite().getPosition();
    }
    void setHealth(int hp) { this->health = hp; }
    void setSpeed(float s) { this->speed = s; }

    void update(float groundY, float dt, sf::Vector2f playerPos) {
        if (!alive){
            setState(BossState::Dead);
            currentAnimation->update(dt);
            return;
        }
        if (playerPos == sf::Vector2f(0.f, 0.f)) {
            setState(BossState::Idle);
            currentAnimation->update(dt);
            updateInvulnerability(dt);
            return;
        }
        if (isHurt) {
            hurtTimer -= dt;
            if (hurtTimer <= 0.f) {
                isHurt = false;
                setState(BossState::Idle); // revine în Idle dupa Hurt
            }
            currentAnimation->update(dt);
            return; // sta pe loc cat timp e in hurt
        }
        sf::Vector2f bossPos = currentAnimation->getSprite().getPosition();
        float distance = std::abs(playerPos.x - bossPos.x);

        if (distance > attackRange) {
            attacking = false;  // nu ataca cand merge
            if (playerPos.x < bossPos.x) { // player stg
                currentAnimation->getSprite().move(-moveSpeed, 0);
                facingRight = true;
            }
            else { // player dr
                currentAnimation->getSprite().move(moveSpeed, 0);
                facingRight = false;
            }
            setState(BossState::Walk);
        }
        else {
            attacking = true;
            attackTimer += dt;
            setState(BossState::Attack);
            if (attackTimer >= attackCooldown) {
                attackTimer = 0;
                // Boss ataca Player aici
                
            }
        }

        currentAnimation->setScale(facingRight ? -scale : scale, scale);
        currentAnimation->update(dt);

        updateInvulnerability(dt);
    }

    void update(float groundY, float dt) override {}
    bool isAttacking() const { return attacking; }

    sf::FloatRect getBounds() const {
        return currentAnimation->getSprite().getGlobalBounds();
    }

    void hurt() {
        setState(BossState::Hurt);
        currentAnimation->setScale(facingRight ? -scale : scale, scale);
        isHurt = true;
        hurtTimer = hurtDuration;
    }
    void drawTo(sf::RenderWindow& window) override {
        window.draw(currentAnimation->getSprite());
    }

private:
    std::map<BossState, Animation*>& animations;
    Animation* currentAnimation;
    BossState state;

    const float moveSpeed = 1.5f;
    const float attackRange = 100.f;
    const float attackCooldown = 1.f;
    bool isHurt = false;
    float hurtTimer = 0.f;
    float hurtDuration = 0.3f;

    void setState(BossState newState) {
        if (state != newState) {
            sf::Vector2f pos = currentAnimation->getSprite().getPosition();
            state = newState;
            currentAnimation = animations[state];
            currentAnimation->setPosition(pos.x, pos.y);
            currentAnimation->reset();
        }
    }

    void setDead() override {
        setState(BossState::Dead);
    }
};
class Zombie : public Entity {
public:
    Zombie(const std::map<ZombieState, Animation*> anims)
        : Entity(4.f), animations(anims), currentState(ZombieState::Idle)
    {
        currentAnim = animations[currentState];
        currentAnim->setScale(scale, scale);
        health = 1;      // Moare dintr-un hit
        speed = 250.f;   // Mai rapid decât boss-ul
        attackCooldown = 2.f;
        attackTimer = 0.f;
    }

    void setPosition(float x, float y) {
        for (auto& pair : animations)
            pair.second->setPosition(x, y);
    }

    sf::Vector2f getPosition() const {
        return animations.at(currentState)->getSprite().getPosition();
    }

    sf::FloatRect getBounds() const {
        return animations.at(currentState)->getSprite().getGlobalBounds();
    }
    void update(float groundY, float dt) override {
        update(groundY, dt, sf::Vector2f(0.f, 0.f));  
    }

    void update(float groundY, float dt, sf::Vector2f targetPos) {
        updateInvulnerability(dt);
        attackTimer += dt;

        if (!alive) {
            currentState = ZombieState::Dead;        
        }
        else {
            float dx = targetPos.x - getPosition().x;
            facingRight = dx >= 0;

            float distance = std::abs(dx);
            if (distance > 100.f) {
                currentState = ZombieState::Walk;
                float direction = facingRight ? 1.f : -1.f;
                move(direction * speed * dt, 0.f);
            }
            else if (attackTimer >= attackCooldown) {
                currentState = ZombieState::Attack;
                attacking = true;
                attackTimer = 0.f;
            }
            else {
                currentState = ZombieState::Idle;
            }
        }

        currentAnim = animations[currentState];

        if (currentState != ZombieState::Dead)
            currentAnim->update(dt);
        currentAnim->update(dt);
        currentAnim->setScale(facingRight ? scale : -scale, scale);

        sf::Vector2f pos = getPosition();
        if (pos.y + getBounds().height < groundY) {
            velocityY += 981.f * dt;
            move(0.f, velocityY * dt);
            onGround = false;
        }
        else {
            setPosition(pos.x, groundY - getBounds().height);
            velocityY = 0.f;
            onGround = true;
        }

    }
    

    void drawTo(sf::RenderWindow& window) override {
        window.draw(currentAnim->getSprite());
    }

    bool isAttacking() const { return attacking; }

private:
    std::map<ZombieState, Animation*> animations;
    Animation* currentAnim;
    ZombieState currentState;
    float attackCooldown;

    void move(float dx, float dy) {
        sf::Vector2f pos = getPosition();
        setPosition(pos.x + dx, pos.y + dy);
    }

    void setDead() override {
        currentState = ZombieState::Dead;
    }
};
// fct de desenat health bar-ul pt Boss si Player
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

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Byte&Fight");
    sf::RectangleShape overlay(sf::Vector2f(window.getSize().x, window.getSize().y));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
   // sf::Event event;
    sf::Clock clock;

    Difficulty selectedDifficulty;
    bool difficultyChosen = false;

    const float GROUND_Y = 920.f;

    sf::Texture idleTex, runTex, jumpTex, attackTex, hurtTex, deadTex;
    sf::Texture bossIdleTex, bossWalkTex, bossAttackTex, bossHurtTex, bossDeadTex;
    sf::Texture bgTex, menuTex, easyTex, mediumTex, hardTex;
    sf::Font font;
   
    sf::Texture menuBgTex, gameBgTex;
    menuBgTex.loadFromFile("postapocalypse3.png");
    //gameBgTex.loadFromFile("postapocalypse4.png");
    sf::Sprite menuBackground(menuBgTex);
    //sf::Sprite gameBackground(gameBgTex);
    

    sf::Texture bgEasy, bgMedium, bgHard;
    bgEasy.loadFromFile("postapocalypse2.png");
    bgMedium.loadFromFile("postapocalypse4.png");
    bgHard.loadFromFile("postapocalypse1.png");

    easyTex.loadFromFile("Easy.png");
    mediumTex.loadFromFile("Medium.png");
    hardTex.loadFromFile("Hard.png");

    sf::Sprite easySprite(easyTex), mediumSprite(mediumTex), hardSprite(hardTex);
    

    easySprite.setPosition(600.f, 200.f);
    mediumSprite.setPosition(600.f, 400.f);
    hardSprite.setPosition(600.f, 600.f);
    sf::Texture idleZTex, walkZTex, attackZTex, deadZTex;
    idleZTex.loadFromFile("Idlez.png");
    walkZTex.loadFromFile("Walkz.png");
    attackZTex.loadFromFile("Attackz.png");
    deadZTex.loadFromFile("Deadz.png");

    Animation idleZ(idleZTex, 7, 0.2f, { 128, 128 });
    Animation walkZ(walkZTex, 12, 0.15f, { 128, 128 });
    Animation attackZ(attackZTex, 10, 0.1f, { 128, 128 });
    Animation deadZ(deadZTex, 5, 0.15f, { 128, 128 }, false);

    std::map<ZombieState, Animation*> zombieAnims = {
        { ZombieState::Idle, &idleZ },
        { ZombieState::Walk, &walkZ },
        { ZombieState::Attack, &attackZ },
        { ZombieState::Dead, &deadZ }
    };

    sf::Sprite background;

    hurtTex.loadFromFile("Hurt.png");
    idleTex.loadFromFile("Idle.png");
    runTex.loadFromFile("Run.png");
    jumpTex.loadFromFile("Jump.png");
    attackTex.loadFromFile("Shot.png");
    deadTex.loadFromFile("Dead.png");
    bossIdleTex.loadFromFile("Idle1.png");
    bossWalkTex.loadFromFile("Walk1.png");
    bossAttackTex.loadFromFile("Attack1.png");
    bossHurtTex.loadFromFile("Hurt1.png");
    bossDeadTex.loadFromFile("Dead1.png");
    //bgTex.loadFromFile("postapocalypse4.png");
    menuTex.loadFromFile("postapocalypse3.png");
    


    // Animatii
    Animation hurtAnim(hurtTex, 2, 0.1f, { 128, 128 });
    Animation idleAnim(idleTex, 6, 0.15f, { 128, 128 });
    Animation runAnim(runTex, 8, 0.1f, { 128, 128 });
    Animation jumpAnim(jumpTex, 11, 0.08f, { 128, 128 });
    Animation attackAnim(attackTex, 12, 0.05f, { 128, 128 });
    Animation deadAnim(deadTex, 4, 0.1f, { 128, 128 }, false);

    Animation bossIdle(bossIdleTex, 10, 0.15f, { 128, 128 });
    Animation bossWalk(bossWalkTex, 12, 0.12f, { 128, 128 });
    Animation bossAttack(bossAttackTex, 5, 0.1f, { 128, 128 });
    Animation bossHurt(bossHurtTex, 3, 0.1f, { 128, 128 });
    Animation bossDead(bossDeadTex, 5, 0.1f, { 128, 128 }, false);

    std::map<PlayerState, Animation*> playerAnims = {
        { PlayerState::Idle, &idleAnim },
        { PlayerState::Run, &runAnim },
        { PlayerState::Jump, &jumpAnim },
        { PlayerState::Attack, &attackAnim },
        { PlayerState::Hurt, &hurtAnim },
        { PlayerState::Dead, &deadAnim}
    };

    std::map<BossState, Animation*> bossAnims = {
        { BossState::Idle, &bossIdle },
        { BossState::Walk, &bossWalk },
        { BossState::Attack, &bossAttack },
        { BossState::Hurt, &bossHurt },
        { BossState::Dead, &bossDead }
    };

    int bossHealth = 50; // default pt easy
    float enemySpeed = 1.f;
    //Zombie* zombie = nullptr;

    while (window.isOpen() && !difficultyChosen) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                if (easySprite.getGlobalBounds().contains(mousePos)) {
                    selectedDifficulty = Difficulty::Easy;
                    background.setTexture(bgEasy);
                    bossHealth = 50;
                    enemySpeed = 1.f;
                    difficultyChosen = true;
                }
                else if (mediumSprite.getGlobalBounds().contains(mousePos)) {
                    selectedDifficulty = Difficulty::Medium;
                    background.setTexture(bgMedium);
                    bossHealth = 70;
                    enemySpeed = 2.f;
                    difficultyChosen = true;
                }
                else if (hardSprite.getGlobalBounds().contains(mousePos)) {
                    selectedDifficulty = Difficulty::Hard;
                    background.setTexture(bgHard);
                    bossHealth = 100;
                    enemySpeed = 3.f;
                    //zombie = new Zombie(zombieAnims);
                    
                    difficultyChosen = true;
                }
            }
                    
        }

        window.clear();
        window.draw(menuBackground);
        window.draw(easySprite);
        window.draw(mediumSprite);
        window.draw(hardSprite);

        window.display();
    }
    
    Player player(playerAnims);
    player.setPosition(300.f, GROUND_Y - 128.f * 5.f);

    Boss boss(bossAnims);
    boss.setHealth(bossHealth);
    boss.setPosition(1500.f, GROUND_Y - 128.f * 6.f); 
    
    boss.setSpeed(enemySpeed);
    Zombie* zombie = new Zombie(zombieAnims);
    //zombie.setPosition(1500.f, GROUND_Y - 128.f * 6.f);

    // loop ul jocului
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
        if (zombie && zombie->isAlive())
            zombie->drawTo(window);
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
            if (zombie->isAlive() && player.isAttacking() && zombie->getBounds().intersects(zombie->getBounds())) {
                zombie->takeDamage(1);   // Boss pierde 10 HP
                
            }
        }
        sf::Texture endTexture;
        sf::Sprite endSprite;

        window.clear();
        window.draw(background);

        player.drawTo(window);
        boss.drawTo(window);
        zombie->drawTo(window);

        drawHealthBar(window, player, 10.f, 20.f, font); // viata player ului
        drawHealthBar(window, boss, 1310.f, 20.f, font);  // viata boss ului
        if (!player.isAlive() || !boss.isAlive()) {
            window.draw(overlay);
        }
        if (!boss.isAlive()) {
            endTexture.loadFromFile("Victory.png");
        }
        else if (!player.isAlive()) {
            endTexture.loadFromFile("GameOver.png");
        }

        endSprite.setTexture(endTexture);

        // centrare in fereastra
        sf::FloatRect spriteBounds = endSprite.getLocalBounds();
        endSprite.setOrigin(spriteBounds.width / 2.f, spriteBounds.height / 2.f);
        endSprite.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
        window.draw(endSprite);
        window.display();
    }
    
    return 0;
}
