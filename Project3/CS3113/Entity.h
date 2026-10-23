#ifndef ENTITY_H
#define ENTITY_H

#include "cs3113.h"

enum Direction    { LEFT, UP, RIGHT, DOWN }; // For animation frame mapping

class Entity
{
private:
    Vector2 mPosition;
    Vector2 mVelocity;
    Vector2 mAcceleration;

    Vector2 mScale;

    Texture2D   mTexture;
    TextureType mTextureType;
    Vector2     mSpriteSheetDimensions;

    std::vector<int>                      mAnimationIndices;

    int   mCurrentFrameIndex = 0;

    float mAngle;

public:
    static constexpr int DEFAULT_SIZE = 250;

    Entity();
    Entity(Vector2 position, Vector2 scale, const char *textureFilepath);
    Entity(
        Vector2                               position,
        Vector2                               scale,
        const char                           *textureFilepath,
        TextureType                           textureType,
        Vector2                               spriteSheetDimensions,
        std::map<Direction, std::vector<int>> animationAtlas
    );
    ~Entity();

    void update(float deltaTime);
    void render();

    Vector2     getPosition()              const { return mPosition;              }
    Vector2     getVelocity()              const { return mVelocity;              }
    Vector2     getScale()                 const { return mScale;                 }

    void setPosition(Vector2 newPosition)
        { mPosition = newPosition;                 }
    void setAcceleration(Vector2 newAcceleration)
        { mAcceleration = newAcceleration;         }
    void setAnimationIndex(int newIndex)
        { mCurrentFrameIndex = newIndex;           }
};

#endif // ENTITY_H