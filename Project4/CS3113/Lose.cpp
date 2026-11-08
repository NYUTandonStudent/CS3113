#include "Lose.h"

Lose::Lose()
    : Scene{ { 0.0f, 0.0f }, "#000000" }, mLoseTexture{}
{}

Lose::Lose(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode }, mLoseTexture{}
{}

Lose::~Lose() { shutdown(); }

void Lose::initialise()
{
    mGameState.nextSceneID = NO_SCENE;

    mGameState.xochitl = nullptr;

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

    // Load defeat message after losing game
    mLoseTexture = LoadTexture("assets/game/LoseMessage.png");
}

void Lose::update(float deltaTime)
{
}

// Render defeat message
void Lose::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.map->render();

    Rectangle source = {
            0.0f, 0.0f,
            static_cast<float>(mLoseTexture.width),
            static_cast<float>(mLoseTexture.height)
    };

    Rectangle destination = {
            mOrigin.x, mOrigin.y,
            600.0f, 150.0f
    };

    Vector2 originOffset = { 300.0f, 75.0f };

    DrawTexturePro(
        mLoseTexture,
        source,
        destination,
        originOffset,
        0.0f,
        WHITE
    );
}

void Lose::shutdown()
{
    delete mGameState.map;
    mGameState.map = nullptr;

    if (mLoseTexture.id != 0)
    {
            UnloadTexture(mLoseTexture);
            mLoseTexture = {};
    }
}