#pragma once
#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <map>
#include "Entity.h"
#include "Animation.h"

enum class ZombieState { Idle, Walk, Attack, Dead };

class Zombie : public Entity {
public:
    Zombie(const std::map<ZombieState, Animation*>& anims);

    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;

    void update(float groundY, float dt) override;
    void update(float groundY, float dt, sf::Vector2f targetPos);

    void drawTo(sf::RenderWindow& window) override;

    bool isAttacking() const;

private:
    std::map<ZombieState, Animation*> animations;
    Animation* currentAnim;
    ZombieState currentState;
    float attackCooldown;
    float attackTimer;

    void move(float dx, float dy);
    void setDead() override;
};

#endif // ZOMBIE_H
