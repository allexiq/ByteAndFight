#pragma once
#ifndef BOSS_H
#define BOSS_H

#include "Entity.h"
#include "Animation.h"
#include <map>
enum class BossState { Idle, Walk, Attack, Hurt, Dead };
class Boss : public Entity {
public:
    Boss(std::map<BossState, Animation*>& animations);

    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    void setHealth(int hp);
    void setSpeed(float s);
    void update(float groundY, float dt, sf::Vector2f playerPos);
    void update(float groundY, float dt) override;
    bool isAttacking() const;
    sf::FloatRect getBounds() const;
    void hurt();
    void drawTo(sf::RenderWindow& window) override;
    //virtual void drawTo(sf::RenderWindow& window);
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

    void setState(BossState newState);
    void setDead() override;
};

#endif // BOSS_H
