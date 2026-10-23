/**
 * Author: Kenneth Sun
 * Assignment: Lunar Lander!
 * Date due: 10/26/2026
 * I pledge that I have completed this assignment without
 * collaborating with anyone else, in conformance with the
 * NYU School of Engineering Policies and Procedures on
 * Academic Misconduct.
 **/

#include "CS3113/Entity.h"

// Global Constants
constexpr int SCREEN_WIDTH  = 1000,
              SCREEN_HEIGHT = 600,
              FPS           = 120;

constexpr char    BG_COLOUR[] = "#FFFF00";
constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

constexpr float ACCELERATION_OF_GRAVITY = 17.62f,
                HORIZONTAL_ACCELERATION = 300.0f,
                HORIZONTAL_DECELERATION = 600.0f,
                PLATFORM_SPEED = 150.0f,
                FIXED_TIMESTEP          = 1.0f / 60.0f;

// Global Variables
AppStatus gAppStatus       = RUNNING;
float     gPreviousTicks   = 0.0f,
          gTimeAccumulator = 0.0f;

bool win = false;
bool lose = false;
bool fuelDecrease = false;
bool noFuel = false;

float gPlatformDirection = 1.0f;
float fuel = 20.0f;

float gAnimationTime = 0.0f;
int gAnimationIndex = 0;

Entity *gPlayer = nullptr;
Entity *gMovingPlatform = nullptr;
Entity *gForbiddenPlatform = nullptr;
Entity *gMissionAccomplished = nullptr;
Entity *gMissionFailed = nullptr;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lunar Lander!");

    /*
        ----------- PLAYER -----------
    */
    std::map<Direction, std::vector<int>> animationAtlas = {
        { RIGHT, { 0, 1, 2 } },
    };

    // Assets made by me
    gPlayer = new Entity(
        { ORIGIN.x, 100.0f }, // position
        { 100.0f, 150.0f }, // scale
        "assets/game/SpacecraftSpritesheet.png",          // texture file address
        ATLAS,                           // sprite sheet
        { 1, 3 },                        // atlas dimensions
        animationAtlas                   // animation frames
    );

    gPlayer->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });

    /*
        ----------- PLATFORMS -----------
    */
    gMovingPlatform = new Entity(
        { ORIGIN.x, ORIGIN.y + 100.0f },
        { 150.0f, 30.0f },
        "assets/game/MovingPlatform.png"          // texture file address
    );

    gForbiddenPlatform = new Entity(
        { ORIGIN.x, SCREEN_HEIGHT - 50.0f },
        { SCREEN_WIDTH, 100.0f },
        "assets/game/ForbiddenPlatform.png"          // texture file address
    );

    /*
        ----------- GAME END MESSAGES -----------
    */
    gMissionAccomplished = new Entity(
        { ORIGIN.x, ORIGIN.y },
        { 300.0f, 50.0f },
        "assets/game/MissionAccomplished.png"          // texture file address
    );

    gMissionFailed = new Entity(
        { ORIGIN.x, ORIGIN.y },
        { 300.0f, 50.0f },
        "assets/game/MissionFailed.png"          // texture file address
    );

    SetTargetFPS(FPS);
}

void processInput()
{
    if (!win && !lose)
    {
        fuelDecrease = false;
        // Apply horizontal acceleration while fuel has not run out
        if      (IsKeyDown(KEY_A) && !noFuel)
        {
                if (gPlayer->getVelocity().x > 0.0f) gPlayer->setAcceleration({ -HORIZONTAL_DECELERATION, ACCELERATION_OF_GRAVITY });
                else gPlayer->setAcceleration({ -HORIZONTAL_ACCELERATION, ACCELERATION_OF_GRAVITY });
                fuelDecrease = true;
        }
        else if (IsKeyDown(KEY_D) && !noFuel)
        {
                if (gPlayer->getVelocity().x < 0.0f) gPlayer->setAcceleration({ HORIZONTAL_DECELERATION, ACCELERATION_OF_GRAVITY });
                else gPlayer->setAcceleration({ HORIZONTAL_ACCELERATION, ACCELERATION_OF_GRAVITY });
                fuelDecrease = true;
        }
        // Decelerate horizontally when neither key is down or fuel runs out
        else if (gPlayer->getVelocity().x < 0.0f) gPlayer->setAcceleration({ HORIZONTAL_DECELERATION, ACCELERATION_OF_GRAVITY });
        else if (gPlayer->getVelocity().x > 0.0f) gPlayer->setAcceleration({ -HORIZONTAL_DECELERATION, ACCELERATION_OF_GRAVITY });
        else gPlayer->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });
    }

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update()
{
    float ticks     = static_cast<float>(GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    deltaTime += gTimeAccumulator; // Fold in any leftover from last frame

    if (deltaTime < FIXED_TIMESTEP) // Not enough time for a full step yet
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) // Consume as many full steps as possible
    {
        if (!win && !lose)
        {
                // Make platform move horizontally and reverse direction at screen edges
                Vector2 platformPosition = gMovingPlatform->getPosition();
                platformPosition.x += gPlatformDirection * PLATFORM_SPEED * FIXED_TIMESTEP;

                float platformMiddle = gMovingPlatform->getScale().x / 2.0f;

                if (platformPosition.x + platformMiddle >= SCREEN_WIDTH)
                {
                        platformPosition.x = SCREEN_WIDTH - platformMiddle;
                        gPlatformDirection = -1.0f;
                }
                else if (platformPosition.x - platformMiddle <= 0.0f)
                {
                        platformPosition.x = platformMiddle;
                        gPlatformDirection = 1.0f;
                }

                gMovingPlatform->setPosition(platformPosition);

                // Determine whether spacecraft successfully lands on moving platform
                Vector2 prevPos = gPlayer->getPosition();
                gPlayer->update(FIXED_TIMESTEP);
                Vector2 curPos = gPlayer->getPosition();

                float playerHalfWidth = gPlayer->getScale().x / 2.0f;
                float playerHalfHeight = gPlayer->getScale().y / 2.0f;
                float platformHalfWidth = gMovingPlatform->getScale().x / 2.0f;
                float platformTop = gMovingPlatform->getPosition().y - gMovingPlatform->getScale().y / 2.0f;
                float platformX = gMovingPlatform->getPosition().x;

                bool crossedTop = (prevPos.y + playerHalfHeight <= platformTop) && (curPos.y + playerHalfHeight >= platformTop);
                bool overPlatform = fabs(curPos.x - platformX) < playerHalfWidth * 0.5f + platformHalfWidth;

                if (crossedTop && overPlatform)
                {
                        curPos.y = platformTop - playerHalfHeight;
                        gPlayer->setPosition(curPos);

                        float playerLeft = curPos.x - playerHalfWidth * 0.5f;
                        float playerRight = curPos.x + playerHalfWidth * 0.5f;
                        float platformLeft = platformX - platformHalfWidth;
                        float platformRight = platformX + platformHalfWidth;

                        if (playerLeft >= platformLeft && playerRight <= platformRight) win = true;
                        else lose = true;
                }
                else
                {
                        float platformBottom = gMovingPlatform->getPosition().y + gMovingPlatform->getScale().y / 2.0f;
                        float playerTop = curPos.y - playerHalfHeight;
                        float playerBottom = curPos.y + playerHalfHeight;

                        bool touchingPlatformSide = fabs(curPos.x - platformX) < playerHalfWidth * 0.5f + platformHalfWidth;

                        if (touchingPlatformSide && playerBottom > platformTop && playerTop < platformBottom) lose = true;
                }

                // Consume fuel while player horizontally accelerates
                if (fuelDecrease && fuel > 0.0f && !noFuel) fuel -= 5.0f * FIXED_TIMESTEP;
                if (fuel <= 0.0f)
                {
                        fuelDecrease = false;
                        fuel = 0.0f;
                        noFuel = true;
                        gPlayer->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });
                }

                // Switch spacecraft spritesheet frames while player horizontally accelerates
                if (fuelDecrease && !noFuel)
                {
                        gAnimationTime += FIXED_TIMESTEP;

                        if (gAnimationTime >= 0.1f)
                        {
                                gAnimationTime = 0.0f;
                                gAnimationIndex = (gAnimationIndex + 1) % 2;
                        }
                }
                else
                {
                        gAnimationTime = 0.0f;
                        gAnimationIndex = 0;
                }

                // Keep spacecraft inside screen
                Vector2 playerPosition = gPlayer->getPosition();
                float playerMiddle = gPlayer->getScale().x / 2.0f;

                if (playerPosition.x - playerMiddle < 0.0f) playerPosition.x = playerMiddle;
                else if (playerPosition.x + playerMiddle > SCREEN_WIDTH) playerPosition.x = SCREEN_WIDTH - playerMiddle;
                gPlayer->setPosition(playerPosition);

                // End game if spacecraft lands on forbidden platform
                if (!win && !lose)
                {
                        float playerBottom = gPlayer->getPosition().y + gPlayer->getScale().y / 2.0f;
                        float forbiddenPlatformTop = gForbiddenPlatform->getPosition().y + 20.0f;
                        float playerLeft = gPlayer->getPosition().x - gPlayer->getScale().x / 2.0f;
                        float playerRight = gPlayer->getPosition().x + gPlayer->getScale().x / 2.0f;
                        float forbiddenPlatformLeft = gForbiddenPlatform->getPosition().x - gForbiddenPlatform->getScale().x / 2.0f;
                        float forbiddenPlatformRight = gForbiddenPlatform->getPosition().x + gForbiddenPlatform->getScale().x / 2.0f;

                        if (playerBottom >= forbiddenPlatformTop && playerRight >= forbiddenPlatformLeft && playerLeft <= forbiddenPlatformRight)
                        {
                                Vector2 playerPosition = gPlayer->getPosition();

                                playerPosition.y = forbiddenPlatformTop - gPlayer->getScale().y / 2.0f;

                                gPlayer->setPosition(playerPosition);

                                lose = true;
                        }
                }
        }

        deltaTime -= FIXED_TIMESTEP;
    }

    gTimeAccumulator = deltaTime; // Save the remainder for next frame
}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    // Select spacecraft spritesheet frame
    if (lose) gPlayer->setAnimationIndex(2);
    else if (win || noFuel) gPlayer->setAnimationIndex(0);
    else gPlayer->setAnimationIndex(gAnimationIndex);

    gPlayer->render();
    gMovingPlatform->render();
    gForbiddenPlatform->render();

    // Display game end message when game ends
    if (win) gMissionAccomplished->render();
    if (lose) gMissionFailed->render();

    // Display remaining fuel on screen
    DrawText(TextFormat("Fuel: %.0f", fuel), 10, 10, 50, BLUE);

    EndDrawing();
}

void shutdown()
{
    // Unload the textures while the window (GL context) still exists
    delete gPlayer;
    delete gMovingPlatform;
    delete gForbiddenPlatform;
    delete gMissionAccomplished;
    delete gMissionFailed;
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}