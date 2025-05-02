#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>

class Entity {
public:
    Entity(float scaleFactor);

    virtual void update(float groundY, float dt) = 0;
    virtual void drawTo(sf::RenderWindow& window) = 0;

    bool isAlive() const;
    int getHealth() const;
    void takeDamage(int dmg);
    void updateInvulnerability(float dt);

protected:
    int health;
    bool alive;
    bool facingRight;
    float velocityY;
    bool onGround;
    bool attacking;
    bool invulnerable;
    float invulTimer;  // Timer pentru invulnerabilitate
    float scale;
    float attackTimer;
    float speed;

    virtual void setDead() = 0;
};

#endif // ENTITY_H