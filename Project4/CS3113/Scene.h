#ifndef SCENE_H
#define SCENE_H

#include "Entity.h"

struct GameState
{
    Entity *xochitl;
    Map    *map;

    SceneID nextSceneID;

    Entity *enemies[5];
    int enemyCount;
    int *lives = nullptr;
    bool collidingWithEnemy = false;
};

class Scene
{
protected:
    GameState   mGameState;
    Vector2     mOrigin;
    const char *mBGColourHexCode = "#000000";

public:
    Scene();
    Scene(Vector2 origin, const char *bgHexCode);
    virtual ~Scene() {}

    virtual void initialise() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void shutdown() = 0;

    GameState   getState()           const { return mGameState;        }

    void setLives(int *lives)
    {
            mGameState.lives = lives;
    }
};

#endif // SCENE_H