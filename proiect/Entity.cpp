#include "Entity.h"

Entity::Entity(float scaleFactor)
    : health(100), alive(true), facingRight(true), velocityY(0), onGround(false), attacking(false),
    invulnerable(false), invulTimer(0.f), scale(scaleFactor), attackTimer(0.f), speed(200.f) {
}

bool Entity::isAlive() const {
    return alive;
}

int Entity::getHealth() const {
    return health;
}

void Entity::takeDamage(int dmg) {
    if (alive && !invulnerable) {
        health -= dmg;
        invulnerable = true; // Entitatea devine invulnerabilă când primește damage
        invulTimer = 1.f;    // Invulnerabilitate de 1 secundă
        if (health <= 0) {
            health = 0;
            alive = false;
            setDead();
        }
    }
}

void Entity::updateInvulnerability(float dt) {
    if (invulnerable) {
        invulTimer -= dt;
        if (invulTimer <= 0.f) {
            invulnerable = false; // Entitatea nu mai este invulnerabilă
        }
    }
}