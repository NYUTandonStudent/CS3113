/**
 * Author: Kenneth Sun
 * Assignment: Pong Variations
 * Date due: 10/14/2026
 * I pledge that I have completed this assignment without
 * collaborating with anyone else, in conformance with the
 * NYU School of Engineering Policies and Procedures on
 * Academic Misconduct.
 **/

#include "CS3113/cs3113.h"

// Global Constants
constexpr int SCREEN_WIDTH = 1600,
                SCREEN_HEIGHT = 900,
                FPS = 60;
//                SIZE = 200;

constexpr float PLAYER_SPEED = 200.0f,
                PURP_SQR_SPEED = 150.0f;

constexpr char BG_COLOUR[] = "#00FF00";
constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 },
//                BASE_SIZE = { static_cast<float>(SIZE), static_cast<float>(SIZE) },
                PLAYER_SIZE = { 50.0f, 250.0f },
                PURP_SQR_SIZE = { 50.0f, 50.0f },
                GAME_TITLE_SIZE = { 400.0f, 100.0f },
                END_MESSAGE_SIZE = { 400.0f, 100.0f },
                RED_INIT_POS = { 100.0f, ORIGIN.y },
                BLUE_INIT_POS = { SCREEN_WIDTH - 100.0f, ORIGIN.y },
                PURP_SQR_INIT_POS = { ORIGIN.x, ORIGIN.y },
                GAME_TITLE_POS = { ORIGIN.x, 100.0f },
                END_MESSAGE_POS = { ORIGIN.x, ORIGIN.y };

// Images Made by Me
constexpr char RED_FP[] = "assets/game/Red.png";
constexpr char BLUE_FP[] = "assets/game/Blue.png";
constexpr char PURP_SQR_FP[] = "assets/game/PurpleSquare.png";
constexpr char GAME_TITLE_FP[] = "assets/game/GameTitle.png";
constexpr char RED_WINS_FP[] = "assets/game/RedWinsMessage.png";
constexpr char BLUE_WINS_FP[] = "assets/game/BlueWinsMessage.png";
constexpr char TIE_FP[] = "assets/game/TieMessage.png";

// Global Variables
AppStatus gAppStatus = RUNNING;
float gAngle = 0.0f,
      gPreviousTicks = 0.0f,
      gBlueAutoYDirection = 1.0f;

Vector2 gRedPosition = RED_INIT_POS,
        gRedMovement = { 0.0f, 0.0f },
        gRedScale = PLAYER_SIZE,

        gBluePosition = BLUE_INIT_POS,
        gBlueMovement = { 0.0f, 0.0f },
        gBlueScale = PLAYER_SIZE,

        gPurpSqr1Position = PURP_SQR_INIT_POS,
        gPurpSqr1Movement = { 1.0f, 1.0f },

        gPurpSqr2Position = { ORIGIN.x, ORIGIN.y - 100.0f },
        gPurpSqr2Movement = { -1.0f, 1.0f },

        gPurpSqr3Position = { ORIGIN.x, ORIGIN.y + 100.0f },
        gPurpSqr3Movement = { -1.0f, -1.0f },

        gPurpSqrScale = PURP_SQR_SIZE,

        gGameTitlePosition = GAME_TITLE_POS,
        gGameTitleScale = GAME_TITLE_SIZE,

        gEndMessagePosition = END_MESSAGE_POS,
        gEndMessageScale = END_MESSAGE_SIZE;

Texture2D gRedTexture;
Texture2D gBlueTexture;
Texture2D gPurpSqrTexture;
Texture2D gGameTitleTexture;
Texture2D gRedWinsTexture;
Texture2D gBlueWinsTexture;
Texture2D gTieTexture;

bool gSinglePlayer = false;
bool endGame = false;
bool ball1Out = false;
bool ball2Out = false;
bool ball3Out = false;
bool redWins = false;
bool blueWins = false;
bool tie = false;
bool redHit = false;
bool blueHit = false;
int gNumBalls = 1;
int redPoints = 0;
int bluePoints = 0;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();
bool isColliding(const Vector2 *positionA, const Vector2 *scaleA, const Vector2 *positionB, const Vector2 *scaleB);

// Function Definitions
bool isColliding(const Vector2 *positionA, const Vector2 *scaleA,
                const Vector2 *positionB, const Vector2 *scaleB)
{
        float xDistance = fabs(positionA->x - positionB->x) - ((scaleA->x + scaleB->x) / 2.0f);
        float yDistance = fabs(positionA->y - positionB->y) - ((scaleA->y + scaleB->y) / 2.0f);

        return xDistance < 0.0f && yDistance < 0.0f;
}

void renderObject(const Texture2D *texture, const Vector2 *position,
                const Vector2 *scale)
{
        // Whole Texture
        Rectangle textureArea = {
                // Top-Left Corner
                0.0f, 0.0f,

                // Dimensions
                static_cast<float>(texture->width),
                static_cast<float>(texture->height)
        };

        // Destination Rectangle
        Rectangle destinationArea = {
                position->x,
                position->y,
                static_cast<float>(scale->x),
                static_cast<float>(scale->y)
        };

        // Origin Inside Source Texture
        Vector2 originOffset = {
                static_cast<float>(scale->x) / 2.0f,
                static_cast<float>(scale->y) / 2.0f
        };

        // Render Texture on Screen
        DrawTexturePro(
                *texture,
                textureArea, destinationArea, originOffset,
                gAngle, WHITE
        );
}

void initialise()
{
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Red vs Blue");

        gRedTexture = LoadTexture(RED_FP);
        gBlueTexture = LoadTexture(BLUE_FP);
        gPurpSqrTexture = LoadTexture(PURP_SQR_FP);
        gGameTitleTexture = LoadTexture(GAME_TITLE_FP);
        gRedWinsTexture = LoadTexture(RED_WINS_FP);
        gBlueWinsTexture = LoadTexture(BLUE_WINS_FP);
        gTieTexture = LoadTexture(TIE_FP);

        SetTargetFPS(FPS);
}

void processInput()
{
        gRedMovement = { 0.0f, 0.0f };
        gBlueMovement = { 0.0f, 0.0f };

        if (!endGame)
        {
                if (IsKeyPressed(KEY_T)) gSinglePlayer = !gSinglePlayer;

                if (IsKeyPressed(KEY_ONE)) gNumBalls = 1;
                else if (IsKeyPressed(KEY_TWO)) gNumBalls = 2;
                else if (IsKeyPressed(KEY_THREE)) gNumBalls = 3;

                if (IsKeyDown(KEY_W)) gRedMovement.y = -1;
                else if (IsKeyDown(KEY_S)) gRedMovement.y = 1;

                if (gSinglePlayer)
                {
                        gBlueMovement.y = gBlueAutoYDirection;
                }
                else
                {
                        if (IsKeyDown(KEY_UP)) gBlueMovement.y = -1;
                        else if (IsKeyDown(KEY_DOWN)) gBlueMovement.y = 1;
                }
        }

        if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update()
{
        // Delta Time
        float ticks = static_cast<float>(GetTime());
        float deltaTime = ticks - gPreviousTicks;
        gPreviousTicks = ticks;

        if (endGame) return;

        gRedPosition = {
                gRedPosition.x + PLAYER_SPEED * gRedMovement.x * deltaTime,
                gRedPosition.y + PLAYER_SPEED * gRedMovement.y * deltaTime
        };

        gBluePosition = {
                gBluePosition.x + PLAYER_SPEED * gBlueMovement.x * deltaTime,
                gBluePosition.y + PLAYER_SPEED * gBlueMovement.y * deltaTime
        };

        gPurpSqr1Position = {
                gPurpSqr1Position.x + PURP_SQR_SPEED * gPurpSqr1Movement.x * deltaTime,
                gPurpSqr1Position.y + PURP_SQR_SPEED * gPurpSqr1Movement.y * deltaTime
        };

        if (gNumBalls >= 2)
        {
                gPurpSqr2Position = {
                        gPurpSqr2Position.x + PURP_SQR_SPEED * gPurpSqr2Movement.x * deltaTime,
                        gPurpSqr2Position.y + PURP_SQR_SPEED * gPurpSqr2Movement.y * deltaTime
                };
        }

        if (gNumBalls >= 3)
        {
                gPurpSqr3Position = {
                        gPurpSqr3Position.x + PURP_SQR_SPEED * gPurpSqr3Movement.x * deltaTime,
                        gPurpSqr3Position.y + PURP_SQR_SPEED * gPurpSqr3Movement.y * deltaTime
                };
        }

        if (isColliding(
                &gPurpSqr1Position, &gPurpSqrScale,
                &gRedPosition, &gRedScale
        ))
        {
                gPurpSqr1Position.x = gRedPosition.x + gRedScale.x / 2.0f + gPurpSqrScale.x / 2.0f;
                gPurpSqr1Movement.x *= -1.0f;
                redPoints++;
                redHit = true;
                blueHit = false;
        }

        if (isColliding(
                &gPurpSqr1Position, &gPurpSqrScale,
                &gBluePosition, &gBlueScale
        ))
        {
                gPurpSqr1Position.x = gBluePosition.x - gBlueScale.x / 2.0f - gPurpSqrScale.x / 2.0f;
                gPurpSqr1Movement.x *= -1.0f;
                bluePoints++;
                redHit = false;
                blueHit = true;
        }

        if (gNumBalls >= 2)
        {
                if (isColliding(
                        &gPurpSqr2Position, &gPurpSqrScale,
                        &gRedPosition, &gRedScale
                ))
                {
                        gPurpSqr2Position.x = gRedPosition.x + gRedScale.x / 2.0f + gPurpSqrScale.x / 2.0f;
                        gPurpSqr2Movement.x *= -1.0f;
                        redPoints++;
                        redHit = true;
                        blueHit = false;
                }

                if (isColliding(
                        &gPurpSqr2Position, &gPurpSqrScale,
                        &gBluePosition, &gBlueScale
                ))
                {
                        gPurpSqr2Position.x = gBluePosition.x - gBlueScale.x / 2.0f - gPurpSqrScale.x / 2.0f;
                        gPurpSqr2Movement.x *= -1.0f;
                        bluePoints++;
                        redHit = false;
                        blueHit = true;
                }
        }

        if (gNumBalls >= 3)
        {
                if (isColliding(
                        &gPurpSqr3Position, &gPurpSqrScale,
                        &gRedPosition, &gRedScale
                ))
                {
                        gPurpSqr3Position.x = gRedPosition.x + gRedScale.x / 2.0f + gPurpSqrScale.x / 2.0f;
                        gPurpSqr3Movement.x *= -1.0f;
                        redPoints++;
                        redHit = true;
                        blueHit = false;
                }

                if (isColliding(
                        &gPurpSqr3Position, &gPurpSqrScale,
                        &gBluePosition, &gBlueScale
                ))
                {
                        gPurpSqr3Position.x = gBluePosition.x - gBlueScale.x / 2.0f - gPurpSqrScale.x / 2.0f;
                        gPurpSqr3Movement.x *= -1.0f;
                        bluePoints++;
                        redHit = false;
                        blueHit = true;
                }
        }

        if (gPurpSqr1Position.y - gPurpSqrScale.y / 2.0f < 0 || gPurpSqr1Position.y + gPurpSqrScale.y / 2.0f > SCREEN_HEIGHT)
        {
                gPurpSqr1Movement.y *= -1.0f;
        }

        if (gPurpSqr1Position.x - gPurpSqrScale.x / 2.0f < 0 || gPurpSqr1Position.x + gPurpSqrScale.x / 2.0f > SCREEN_WIDTH)
        {
                ball1Out = true;
        }

        if (gNumBalls >= 2)
        {
                if (gPurpSqr2Position.y - gPurpSqrScale.y / 2.0f < 0 || gPurpSqr2Position.y + gPurpSqrScale.y / 2.0f > SCREEN_HEIGHT)
                {
                        gPurpSqr2Movement.y *= -1.0f;
                }

                if (gPurpSqr2Position.x - gPurpSqrScale.x / 2.0f < 0 || gPurpSqr2Position.x + gPurpSqrScale.x / 2.0f > SCREEN_WIDTH)
                {
                        ball2Out = true;
                }
        }

        if (gNumBalls >= 3)
        {
                if (gPurpSqr3Position.y - gPurpSqrScale.y / 2.0f < 0 || gPurpSqr3Position.y + gPurpSqrScale.y / 2.0f > SCREEN_HEIGHT)
                {
                        gPurpSqr3Movement.y *= -1.0f;
                }

                if (gPurpSqr3Position.x - gPurpSqrScale.x / 2.0f < 0 || gPurpSqr3Position.x + gPurpSqrScale.x / 2.0f > SCREEN_WIDTH)
                {
                        ball3Out = true;
                }
        }

        if (!endGame)
        {
                if (gNumBalls == 1) endGame = ball1Out;
                else if (gNumBalls == 2) endGame = ball1Out && ball2Out;
                else endGame = ball1Out && ball2Out && ball3Out;

                if (endGame)
                {
                        if (redPoints > bluePoints) redWins = true;
                        else if (redPoints < bluePoints) blueWins = true;
                        else tie = true;
                }
        }

        if (gRedPosition.y - gRedScale.y / 2.0f < 0)
        {
                gRedPosition.y = gRedScale.y / 2.0f;
        }

        if (gRedPosition.y + gRedScale.y / 2.0f > SCREEN_HEIGHT)
        {
                gRedPosition.y = SCREEN_HEIGHT - gRedScale.y / 2.0f;
        }

        if (gBluePosition.y - gBlueScale.y / 2.0f < 0)
        {
                gBluePosition.y = gBlueScale.y / 2.0f;
                if (gSinglePlayer) gBlueAutoYDirection = 1.0f;
        }
        else if (gBluePosition.y + gBlueScale.y / 2.0f > SCREEN_HEIGHT)
        {
                gBluePosition.y = SCREEN_HEIGHT - gBlueScale.y / 2.0f;
                if (gSinglePlayer) gBlueAutoYDirection = -1.0f;
        }
}

void render()
{
        BeginDrawing();
        ClearBackground(ColorFromHex(BG_COLOUR));

        if (!endGame)
        {
                if (redHit) ClearBackground(ColorFromHex("#FFFF00"));
                else if (blueHit) ClearBackground(ColorFromHex("#00FFFF"));

                // Render Red
                renderObject(&gRedTexture, &gRedPosition, &gRedScale);

                // Render Blue
                renderObject(&gBlueTexture, &gBluePosition, &gBlueScale);

                // Render Purple Square
                renderObject(&gPurpSqrTexture, &gPurpSqr1Position, &gPurpSqrScale);

                if (gNumBalls >= 2) renderObject(&gPurpSqrTexture, &gPurpSqr2Position, &gPurpSqrScale);
                if (gNumBalls >= 3) renderObject(&gPurpSqrTexture, &gPurpSqr3Position, &gPurpSqrScale);
        }
        else
        {
                if (redWins) renderObject(&gRedWinsTexture, &gEndMessagePosition, &gEndMessageScale);
                else if (blueWins) renderObject(&gBlueWinsTexture, &gEndMessagePosition, &gEndMessageScale);
                else if (tie) renderObject(&gTieTexture, &gEndMessagePosition, &gEndMessageScale);
        }

        renderObject(&gGameTitleTexture, &gGameTitlePosition, &gGameTitleScale);

        EndDrawing();
}

void shutdown()
{
        UnloadTexture(gRedTexture);
        UnloadTexture(gBlueTexture);
        UnloadTexture(gPurpSqrTexture);
        UnloadTexture(gGameTitleTexture);
        UnloadTexture(gRedWinsTexture);
        UnloadTexture(gBlueWinsTexture);
        UnloadTexture(gTieTexture);
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