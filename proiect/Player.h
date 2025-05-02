#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Animation.h"
#include <map>
enum class PlayerState { Idle, Run, Jump, Attack, Hurt, Dead };
class Player : public Entity {
public:
    Player(std::map<PlayerState, Animation*>& animations);

    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    void processEvents(sf::Keyboard::Key key, bool pressed);
    void update(float groundY, float dt) override;
    bool isAttacking() const;
    sf::FloatRect getBounds() const;
    void hurt();
    void drawTo(sf::RenderWindow& window) override;

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

    void setState(PlayerState newState);
    void setDead() override;
};

#endif // PLAYER_H
