#include "LevelD.h"

LevelD::LevelD()
    : Scene{ { 0.0f, 0.0f }, "#000000" }
{}

LevelD::LevelD(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode }
{}

LevelD::~LevelD() { shutdown(); }

void LevelD::initialise()
{
    mGameState.nextSceneID = NO_SCENE;

    mGameState.enemyCount = 0;

    for (int row = 0; row < LEVEL_HEIGHT; row++)
    {
            for (int col = 0; col < LEVEL_WIDTH; col++)
            {
                    bool isPlatform = (row == LEVEL_HEIGHT - 1);
                    bool isWall = (col == 0 || col == LEVEL_WIDTH - 1);

                    if (isPlatform || isWall) mLevelData[row * LEVEL_WIDTH + col] = 1;
                    else mLevelData[row * LEVEL_WIDTH + col] = 0;
            }
    }

    /*
        ----------- MAP -----------
    */
    mGameState.map = new Map(
        LEVEL_WIDTH, LEVEL_HEIGHT, // map grid cols & rows
        mLevelData,                // grid data
        "assets/game/PlatformBlock.png", // texture filepath
        TILE_DIMENSION,            // tile size
        1, 1,                      // texture cols & rows
        mOrigin                    // in-game origin
    );

    /*
        ----------- PROTAGONIST -----------
    */

    // Assets made by me
    mGameState.xochitl = new Entity(
        { mOrigin.x - 600.0f, mOrigin.y + 120.0f }, // position
        { 100.0f, 60.0f },             // scale
        "assets/game/CarWithoutFlame.png",                     // texture file address
        PLAYER                                      // entity type
    );

    mGameState.xochitl->setFlameTexture("assets/game/CarWithFlame.png");

    // BOSSES (bigger than enemies in previous levels)

    mGameState.enemies[0] = new Entity(
        { mOrigin.x - 300.0f, mOrigin.y },
        { 200.0f, 200.0f },
        "assets/game/BossEnemy.png",
        ENEMY
    );

    mGameState.enemies[1] = new Entity(
        { mOrigin.x + 100.0f, mOrigin.y },
        { 200.0f, 200.0f },
        "assets/game/BossEnemy.png",
        ENEMY
    );

    mGameState.enemies[2] = new Entity(
        { mOrigin.x + 500.0f, mOrigin.y },
        { 200.0f, 200.0f },
        "assets/game/BossEnemy.png",
        ENEMY
    );

    float topBoundary = mOrigin.y - 225.0f;
    float bottomBoundary = mOrigin.y + 150.0f;

    mGameState.enemies[0]->setAIType(VERTICAL);
    mGameState.enemies[0]->setSpeed(75);

    mGameState.enemies[1]->setAIType(VERTICAL);
    mGameState.enemies[1]->setSpeed(100);

    mGameState.enemies[2]->setAIType(VERTICAL);
    mGameState.enemies[2]->setSpeed(125);

    for (int i = 0; i < 3; i++)
    {
            mGameState.enemies[i]->setAIVerticalBounds(topBoundary, bottomBoundary);
    }

    mGameState.enemyCount = 3;
}

void LevelD::update(float deltaTime)
{
    mGameState.xochitl->update(
        deltaTime,      // delta time / fixed timestep
        nullptr,        // player
        mGameState.map, // map
        nullptr,        // collidable entities
        0               // col. entity count
    );

    // Take all remaining lives away for any collision with any boss
    bool currentlyCollidingWithEnemy = false;

    for (int i = 0; i < mGameState.enemyCount; i++)
    {
            mGameState.enemies[i]->update(
                deltaTime,
                mGameState.xochitl,
                nullptr,
                nullptr,
                0
            );

            if (mGameState.xochitl->isCollidingEnemy(mGameState.enemies[i])) currentlyCollidingWithEnemy = true;
    }

    if (currentlyCollidingWithEnemy) (*mGameState.lives) = 0;

    // Lose when player hits any boss
    // or win when player hits right wall
    if (*mGameState.lives <= 0) mGameState.nextSceneID = LOSE;
    else if (mGameState.xochitl->isCollidingRight()) mGameState.nextSceneID = WIN;
}

void LevelD::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.map->render();
    mGameState.xochitl->render();

    for (int i = 0; i < mGameState.enemyCount; i++)
    {
            mGameState.enemies[i]->render();
    }
}

void LevelD::shutdown()
{
    for (int i = 0; i < mGameState.enemyCount; i++)
    {
            delete mGameState.enemies[i];
            mGameState.enemies[i] = nullptr;
    }

    mGameState.enemyCount = 0;

    delete mGameState.xochitl;
    mGameState.xochitl = nullptr;

    delete mGameState.map;
    mGameState.map = nullptr;
}