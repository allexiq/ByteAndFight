#include "Boss.h"

Boss::Boss(std::map<BossState, Animation*>& animations)
    : Entity(6.f), animations(animations), state(BossState::Idle)
{
    currentAnimation = animations[state];
    for (auto& pair : animations)
        pair.second->setScale(scale, scale);
}

void Boss::setPosition(float x, float y) {
    for (auto& pair : animations) {
        pair.second->setPosition(x, y);
    }
}

sf::Vector2f Boss::getPosition() const {
    return currentAnimation->getSprite().getPosition();
}

void Boss::setHealth(int hp) {
    this->health = hp;
}

void Boss::setSpeed(float s) {
    this->speed = s;
}

void Boss::update(float groundY, float dt, sf::Vector2f playerPos) {
    if (!alive) {
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
            setState(BossState::Idle); // Revine în Idle dupa Hurt
        }
        currentAnimation->update(dt);
        return; // Sta pe loc cat timp este in hurt
    }

    sf::Vector2f bossPos = currentAnimation->getSprite().getPosition();
    float distance = std::abs(playerPos.x - bossPos.x);

    if (distance > attackRange) {
        attacking = false;  // Nu ataca cand se misca
        if (playerPos.x < bossPos.x) { // Player stg
            currentAnimation->getSprite().move(-moveSpeed, 0);
            facingRight = true;
        }
        else { // Player dr
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

void Boss::update(float groundY, float dt) {}

bool Boss::isAttacking() const {
    return attacking;
}

sf::FloatRect Boss::getBounds() const {
    return currentAnimation->getSprite().getGlobalBounds();
}

void Boss::hurt() {
    setState(BossState::Hurt);
    currentAnimation->setScale(facingRight ? -scale : scale, scale);
    isHurt = true;
    hurtTimer = hurtDuration;
}

void Boss::drawTo(sf::RenderWindow& window) {
    window.draw(currentAnimation->getSprite());
}

void Boss::setState(BossState newState) {
    if (state != newState) {
        sf::Vector2f pos = currentAnimation->getSprite().getPosition();
        state = newState;
        currentAnimation = animations[state];
        currentAnimation->setPosition(pos.x, pos.y);
        currentAnimation->reset();
    }
}

void Boss::setDead() {
    setState(BossState::Dead);
}