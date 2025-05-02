#include "Animation.h"

Animation::Animation(sf::Texture& texture, int frameCount, float frameTime, sf::Vector2i frameSize, bool looping)
    : texture(texture), frameCount(frameCount), frameTime(frameTime), frameSize(frameSize), looping(looping)
{
    currentFrame = 0;
    elapsed = 0.f;
    sprite.setTexture(texture);
    sprite.setOrigin(frameSize.x / 2.f, frameSize.y / 18.f);
    updateFrame();
}

void Animation::update(float dt) {
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

void Animation::reset() {
    currentFrame = 0;
    elapsed = 0.f;
    updateFrame();
}

float Animation::getDuration() const {
    return frameCount * frameTime;
}

void Animation::setScale(float scaleX, float scaleY) {
    sprite.setScale(scaleX, scaleY);
}

void Animation::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

sf::Sprite& Animation::getSprite() {
    return sprite;
}

void Animation::updateFrame() {
    sprite.setTextureRect(sf::IntRect(
        frameSize.x * currentFrame, 0,
        frameSize.x, frameSize.y
    ));
}