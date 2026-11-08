#ifndef ENTITY_H
#define ENTITY_H

#include "Map.h"

enum EntityStatus { ACTIVE, INACTIVE };
enum EntityType   { PLAYER, NONE, ENEMY };
enum AIType       { VERTICAL, FALLING, FOLLOWING };

class Entity
{
private:
    Vector2 mPosition;
    Vector2 mMovement;
    Vector2 mVelocity;

    Vector2 mScale;
    Vector2 mColliderDimensions;

    Texture2D   mTexture;
    Texture2D   mFlameTexture;

    bool mHasFlameTexture = false;

    int   mSpeed;
    float mAngle;

    bool mIsCollidingTop    = false;
    bool mIsCollidingBottom = false;
    bool mIsCollidingRight  = false;
    bool mIsCollidingLeft   = false;

    EntityStatus mEntityStatus = ACTIVE;
    EntityType   mEntityType;

    AIType  mAIType;

    float mAITopBoundary = 0.0f;
    float mAIBottomBoundary = 0.0f;
    float mAIStartX = 0.0f;

    bool mAIMovingDown = true;
    bool mAIFollowing = false;

    bool isColliding(Entity *other) const;

    void checkCollisionY(Entity *collidableEntities, int collisionCheckCount);
    void checkCollisionY(Map *map);

    void checkCollisionX(Entity *collidableEntities, int collisionCheckCount);
    void checkCollisionX(Map *map);

    void resetColliderFlags()
    {
        mIsCollidingTop    = false;
        mIsCollidingBottom = false;
        mIsCollidingRight  = false;
        mIsCollidingLeft   = false;
    }

    void AIActivate(Entity *target);
    void AIVertical();
    void AIFalling();
    void AIFollowing(Entity *target);

public:
    static constexpr int   DEFAULT_SIZE          = 250;
    static constexpr int   DEFAULT_SPEED         = 200;
    static constexpr float Y_COLLISION_THRESHOLD = 0.5f;

    Entity();
    Entity(
        Vector2     position,
        Vector2     scale,
        const char *textureFilepath,
        EntityType  entityType
    );
    ~Entity();

    void update(
        float   deltaTime,
        Entity *player,
        Map    *map,
        Entity *collidableEntities,
        int     collisionCheckCount
    );
    void render();

    void activate()   { mEntityStatus = ACTIVE;    }
    void deactivate() { mEntityStatus = INACTIVE;  }

    bool isActive() { return mEntityStatus == ACTIVE; }

    void moveUp()    { mMovement.y = -1; }
    void moveDown()  { mMovement.y =  1; }
    void moveLeft()  { mMovement.x = -1; }
    void moveRight() { mMovement.x =  1; }

    void resetMovement() { mMovement = { 0.0f, 0.0f }; }

    Vector2     getPosition()              const { return mPosition;              }
    Vector2     getColliderDimensions()    const { return mColliderDimensions;    }
    int         getSpeed()                 const { return mSpeed;                 }

    bool isCollidingTop()    const { return mIsCollidingTop;    }
    bool isCollidingBottom() const { return mIsCollidingBottom; }
    bool isCollidingRight()  const { return mIsCollidingRight;  }
    bool isCollidingEnemy(Entity* enemy) const { return isColliding(enemy); }

    void setPosition(Vector2 newPosition)
        { mPosition = newPosition;                 }
    void setFlameTexture(const char *textureFilepath)
        {
                mFlameTexture = LoadTexture(textureFilepath);
                mHasFlameTexture = true;
        }
    void setColliderDimensions(Vector2 newDimensions)
        { mColliderDimensions = newDimensions;     }
    void setSpeed(int newSpeed)
        { mSpeed = newSpeed;                       }
    void setAIType(AIType newType)
        { mAIType = newType;                       }
    void setAIVerticalBounds(float top, float bottom)
    {
            mAITopBoundary = top;
            mAIBottomBoundary = bottom;
    }
    void setAIStartX(float startX)
    {
            mAIStartX = startX;
    }
};

#endif // ENTITY_H