#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>

class Animation {
public:
    Animation(sf::Texture& texture, int frameCount, float frameTime, sf::Vector2i frameSize, bool looping = true);

    void update(float dt);
    void reset();
    float getDuration() const;
    void setScale(float scaleX, float scaleY);
    void setPosition(float x, float y);
    sf::Sprite& getSprite();

private:
    sf::Texture& texture;
    sf::Sprite sprite;
    int frameCount;
    int currentFrame;
    float frameTime;
    float elapsed;
    bool looping;
    sf::Vector2i frameSize;

    void updateFrame();
};

#endif // ANIMATION_H#pragma once
