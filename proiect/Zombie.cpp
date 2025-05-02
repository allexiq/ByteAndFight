#include "Zombie.h"

Zombie::Zombie(const std::map<ZombieState, Animation*>& anims)
    : Entity(4.f), animations(anims), currentState(ZombieState::Idle), attackCooldown(2.f), attackTimer(0.f)
{
    currentAnim = animations[currentState];
    currentAnim->setScale(scale, scale);
    health = 1;      // Moare dintr-un hit
    speed = 250.f;   // Mai rapid decat boss-ul
}

void Zombie::setPosition(float x, float y) {
    for (auto& pair : animations) {
        pair.second->setPosition(x, y);
    }
}

sf::Vector2f Zombie::getPosition() const {
    return animations.at(currentState)->getSprite().getPosition();
}

sf::FloatRect Zombie::getBounds() const {
    return animations.at(currentState)->getSprite().getGlobalBounds();
}

void Zombie::update(float groundY, float dt) {
    update(groundY, dt, sf::Vector2f(0.f, 0.f));
}

void Zombie::update(float groundY, float dt, sf::Vector2f targetPos) {
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

void Zombie::drawTo(sf::RenderWindow& window) {
    window.draw(currentAnim->getSprite());
}

bool Zombie::isAttacking() const {
    return attacking;
}

void Zombie::move(float dx, float dy) {
    sf::Vector2f pos = getPosition();
    setPosition(pos.x + dx, pos.y + dy);
}

void Zombie::setDead() {
    currentState = ZombieState::Dead;
}