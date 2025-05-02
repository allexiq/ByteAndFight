#include "Player.h"

Player::Player(std::map<PlayerState, Animation*>& animations)
    : Entity(5.f), animations(animations), state(PlayerState::Idle)
{
    currentAnimation = animations[state];
    for (auto& pair : animations)
        pair.second->setScale(scale, scale);
}

void Player::setPosition(float x, float y) {
    for (auto& pair : animations) {
        pair.second->setPosition(x, y);
    }
}

sf::Vector2f Player::getPosition() const {
    return currentAnimation->getSprite().getPosition();
}

void Player::processEvents(sf::Keyboard::Key key, bool pressed) {
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

void Player::update(float groundY, float dt) {
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

bool Player::isAttacking() const {
    return attacking;
}

sf::FloatRect Player::getBounds() const {
    return currentAnimation->getSprite().getGlobalBounds();
}

void Player::hurt() {
    setState(PlayerState::Hurt);
}

void Player::drawTo(sf::RenderWindow& window) {
    window.draw(currentAnimation->getSprite());
}

void Player::setState(PlayerState newState) {
    if (state != newState) {
        sf::Vector2f pos = currentAnimation->getSprite().getPosition();
        state = newState;
        currentAnimation = animations[state];
        currentAnimation->setPosition(pos.x, pos.y);
        currentAnimation->reset();
    }
}

void Player::setDead() {
    setState(PlayerState::Dead);
}