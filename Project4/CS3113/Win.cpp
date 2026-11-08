#include "Win.h"

Win::Win()
    : Scene{ { 0.0f, 0.0f }, "#000000" }, mWinTexture{}
{}

Win::Win(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode }, mWinTexture{}
{}

Win::~Win() { shutdown(); }

void Win::initialise()
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

    // Load victory message after winning game
    mWinTexture = LoadTexture("assets/game/WinMessage.png");
}

void Win::update(float deltaTime)
{
}

// Render victory message
void Win::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.map->render();

    Rectangle source = {
            0.0f, 0.0f,
            static_cast<float>(mWinTexture.width),
            static_cast<float>(mWinTexture.height)
    };

    Rectangle destination = {
            mOrigin.x, mOrigin.y,
            600.0f, 150.0f
    };

    Vector2 originOffset = { 300.0f, 75.0f };

    DrawTexturePro(
        mWinTexture,
        source,
        destination,
        originOffset,
        0.0f,
        WHITE
    );
}

void Win::shutdown()
{
    delete mGameState.map;
    mGameState.map = nullptr;

    if (mWinTexture.id != 0)
    {
            UnloadTexture(mWinTexture);
            mWinTexture = {};
    }
}