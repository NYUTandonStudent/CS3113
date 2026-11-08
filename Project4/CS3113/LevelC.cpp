#include "LevelC.h"

LevelC::LevelC()
    : Scene{ { 0.0f, 0.0f }, "#000000" }
{}

LevelC::LevelC(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode }
{}

LevelC::~LevelC() { shutdown(); }

void LevelC::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    mGameState.collidingWithEnemy = false;

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

    // ENEMIES (faster than Level 2)

    mGameState.enemies[0] = new Entity(
        { mOrigin.x - 350.0f, mOrigin.y },
        { 50.0f, 50.0f },
        "assets/game/RedEnemy.png",
        ENEMY
    );

    mGameState.enemies[1] = new Entity(
        { mOrigin.x - 100.0f, mOrigin.y },
        { 50.0f, 50.0f },
        "assets/game/RedEnemy.png",
        ENEMY
    );

    mGameState.enemies[2] = new Entity(
        { mOrigin.x + 150.0f, mOrigin.y },
        { 50.0f, 50.0f },
        "assets/game/GreenEnemy.png",
        ENEMY
    );

    mGameState.enemies[3] = new Entity(
        { mOrigin.x + 350.0f, mOrigin.y },
        { 50.0f, 50.0f },
        "assets/game/GreenEnemy.png",
        ENEMY
    );

    mGameState.enemies[4] = new Entity(
        { mOrigin.x + 550.0f, mOrigin.y },
        { 50.0f, 50.0f },
        "assets/game/YellowEnemy.png",
        ENEMY
    );

    float topBoundary = mOrigin.y - 225.0f;
    float bottomBoundary = mOrigin.y + 150.0f;

    mGameState.enemies[0]->setAIType(VERTICAL);
    mGameState.enemies[0]->setSpeed(100);

    mGameState.enemies[1]->setAIType(VERTICAL);
    mGameState.enemies[1]->setSpeed(150);

    mGameState.enemies[2]->setAIType(FALLING);
    mGameState.enemies[2]->setSpeed(100);

    mGameState.enemies[3]->setAIType(FALLING);
    mGameState.enemies[3]->setSpeed(150);

    mGameState.enemies[4]->setAIType(FOLLOWING);
    mGameState.enemies[4]->setSpeed(75);
    mGameState.enemies[4]->setAIStartX(mGameState.enemies[4]->getPosition().x);

    for (int i = 0; i < 4; i++)
    {
            mGameState.enemies[i]->setAIVerticalBounds(topBoundary, bottomBoundary);
    }

    mGameState.enemyCount = 5;
}

void LevelC::update(float deltaTime)
{
    mGameState.xochitl->update(
        deltaTime,      // delta time / fixed timestep
        nullptr,        // player
        mGameState.map, // map
        nullptr,        // collidable entities
        0               // col. entity count
    );

    // Take one life away for each enemy collision.
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

    if (currentlyCollidingWithEnemy && !mGameState.collidingWithEnemy) (*mGameState.lives)--;
    mGameState.collidingWithEnemy = currentlyCollidingWithEnemy;

    // Lose when there are no lives left
    // or go to Level 4 when player hits right wall
    if (*mGameState.lives <= 0) mGameState.nextSceneID = LOSE;
    else if (mGameState.xochitl->isCollidingRight()) mGameState.nextSceneID = LEVEL_D;
}

void LevelC::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.map->render();
    mGameState.xochitl->render();

    for (int i = 0; i < mGameState.enemyCount; i++)
    {
            mGameState.enemies[i]->render();
    }
}

void LevelC::shutdown()
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