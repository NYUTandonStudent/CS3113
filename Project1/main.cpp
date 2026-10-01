/**
 * Author: Kenneth Sun
 * Assignment: Project 1: Simple 2D Scene
 * Date due: 10/05/2026
 * I pledge that I have completed this assignment without
 * collaborating with anyone else, in conformance with the
 * NYU School of Engineering Policies and Procedures on
 * Academic Misconduct.
 **/

#include "CS3113/cs3113.h"
#include <math.h>

// Global Constants
constexpr int   SCREEN_WIDTH = 1600,
                SCREEN_HEIGHT = 900,
                FPS = 60,
                SIZE = 100;

constexpr float PACMAN_MAX_AMP = 40.0f,
                GHOST_MAX_AMP = 10.0f;

constexpr float PACMAN_ORBIT_RADIUS = 250.0f,
                BLINKY_ORBIT_RADIUS = 200.0f,
                INKY_ORBIT_RADIUS = 300.0f,
                PINKY_ORBIT_RADIUS = 275.0f,
                CLYDE_ORBIT_RADIUS = 225.0f;

constexpr float PACMAN_ORBIT_SPEED = 1.5f,
                BLINKY_ORBIT_SPEED = 1.0f,
                INKY_ORBIT_SPEED = 2.0f,
                PINKY_ORBIT_SPEED = 1.75f,
                CLYDE_ORBIT_SPEED = 1.25f;

constexpr float BLINKY_OFFSET = 0.0f,
                INKY_OFFSET = 1.0f,
                PINKY_OFFSET = 2.0f,
                CLYDE_OFFSET = 3.0f;

constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
constexpr Vector2 BASE_SIZE = { static_cast<float>(SIZE), static_cast<float>(SIZE) };

// Images of Pac-Man and his 4 enemy ghosts drawn by me
constexpr char PACMAN_FP[] = "assets/game/PacMan.png";
constexpr char BLINKY_FP[] = "assets/game/Blinky.png";
constexpr char INKY_FP[] = "assets/game/Inky.png";
constexpr char PINKY_FP[] = "assets/game/Pinky.png";
constexpr char CLYDE_FP[] = "assets/game/Clyde.png";

// Global Variables
AppStatus gAppStatus = RUNNING;

float gPacManAngle = 0.0f;
float gBlinkyAngle = 0.0f;
float gInkyAngle = 0.0f;
float gPinkyAngle = 0.0f;
float gClydeAngle = 0.0f;

float gPulseTime = 0.0f;

Vector2 gPacManPosition = ORIGIN;
Vector2 gBlinkyPosition = ORIGIN;
Vector2 gInkyPosition = ORIGIN;
Vector2 gPinkyPosition = ORIGIN;
Vector2 gClydePosition = ORIGIN;

Vector2 gPacManScale = BASE_SIZE,
        gGhostScale = BASE_SIZE;

float gPreviousTicks = 0.0f;

Texture2D gPacManTexture,
          gBlinkyTexture,
          gInkyTexture,
          gPinkyTexture,
          gClydeTexture;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

// Function Definitions
void initialise()
{
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Textures");

        gPacManTexture = LoadTexture(PACMAN_FP);
        gBlinkyTexture = LoadTexture(BLINKY_FP);
        gInkyTexture = LoadTexture(INKY_FP);
        gPinkyTexture = LoadTexture(PINKY_FP);
        gClydeTexture = LoadTexture(CLYDE_FP);

        SetTargetFPS(FPS);
}

void processInput()
{
        if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update()
{
        // Delta Time
        float ticks = static_cast<float>(GetTime());
        float deltaTime = ticks - gPreviousTicks;
        gPreviousTicks = ticks;

        // Pulse Time
        gPulseTime += 1.0f * deltaTime;

        // Object Scales
        gPacManScale = {
                BASE_SIZE.x + PACMAN_MAX_AMP * cos(gPulseTime),
                BASE_SIZE.y + PACMAN_MAX_AMP * cos(gPulseTime)
        };

        gGhostScale = {
                BASE_SIZE.x + GHOST_MAX_AMP * cos(gPulseTime),
                BASE_SIZE.y + GHOST_MAX_AMP * cos(gPulseTime)
        };

        // Object Angles
        gPacManAngle += 200.0f * deltaTime;
        gBlinkyAngle = gPacManAngle * 0.5f;
        gInkyAngle += 175.0f * deltaTime;
        gPinkyAngle += 150.0f * deltaTime;
        gClydeAngle += 125.0f * deltaTime;

        // Object Orbits
        gPacManPosition = {
                ORIGIN.x + PACMAN_ORBIT_RADIUS * cos(gPulseTime * PACMAN_ORBIT_SPEED),
                ORIGIN.y + PACMAN_ORBIT_RADIUS * sin(gPulseTime * PACMAN_ORBIT_SPEED)
        };

        gBlinkyPosition = {
                gPacManPosition.x + BLINKY_ORBIT_RADIUS * cos(gPulseTime * BLINKY_ORBIT_SPEED + BLINKY_OFFSET),
                gPacManPosition.y + BLINKY_ORBIT_RADIUS * sin(gPulseTime * BLINKY_ORBIT_SPEED + BLINKY_OFFSET)
        };

        gInkyPosition = {
                ORIGIN.x + INKY_ORBIT_RADIUS * cos(gPulseTime * INKY_ORBIT_SPEED + INKY_OFFSET),
                ORIGIN.y + INKY_ORBIT_RADIUS * sin(gPulseTime * INKY_ORBIT_SPEED + INKY_OFFSET)
        };

        gPinkyPosition = {
                ORIGIN.x + PINKY_ORBIT_RADIUS * cos(gPulseTime * PINKY_ORBIT_SPEED + PINKY_OFFSET),
                ORIGIN.y + PINKY_ORBIT_RADIUS * sin(gPulseTime * PINKY_ORBIT_SPEED + PINKY_OFFSET)
        };

        gClydePosition = {
                ORIGIN.x + CLYDE_ORBIT_RADIUS * cos(gPulseTime * CLYDE_ORBIT_SPEED + CLYDE_OFFSET),
                ORIGIN.y + CLYDE_ORBIT_RADIUS * sin(gPulseTime * CLYDE_ORBIT_SPEED + CLYDE_OFFSET)
        };
}

void render()
{
        BeginDrawing();

        // Background Change Pattern
        if (gPacManScale.x >= BASE_SIZE.x + PACMAN_MAX_AMP / 2) ClearBackground(ColorFromHex("#0000FF"));
        else if (gPacManScale.x <= BASE_SIZE.x - PACMAN_MAX_AMP / 2) ClearBackground(ColorFromHex("#FF0000"));
        else ClearBackground(ColorFromHex("#00FF00"));

        // Object Textures
        Rectangle pacManTextureArea = {
                0.0f, 0.0f,
                static_cast<float>(gPacManTexture.width),
                static_cast<float>(gPacManTexture.height)
        };

        Rectangle blinkyTextureArea = {
                0.0f, 0.0f,
                static_cast<float>(gBlinkyTexture.width),
                static_cast<float>(gBlinkyTexture.height)
        };

        Rectangle inkyTextureArea = {
                0.0f, 0.0f,
                static_cast<float>(gInkyTexture.width),
                static_cast<float>(gInkyTexture.height)
        };

        Rectangle pinkyTextureArea = {
                0.0f, 0.0f,
                static_cast<float>(gPinkyTexture.width),
                static_cast<float>(gPinkyTexture.height)
        };

        Rectangle clydeTextureArea = {
                0.0f, 0.0f,
                static_cast<float>(gClydeTexture.width),
                static_cast<float>(gClydeTexture.height)
        };

        // Destination Rectangles
        Rectangle pacManDestinationArea = {
                gPacManPosition.x,
                gPacManPosition.y,
                gPacManScale.x,
                gPacManScale.y
        };

        Rectangle blinkyDestinationArea = {
                gBlinkyPosition.x,
                gBlinkyPosition.y,
                gGhostScale.x,
                gGhostScale.y
        };

        Rectangle inkyDestinationArea = {
                gInkyPosition.x,
                gInkyPosition.y,
                gGhostScale.x,
                gGhostScale.y
        };

        Rectangle pinkyDestinationArea = {
                gPinkyPosition.x,
                gPinkyPosition.y,
                gGhostScale.x,
                gGhostScale.y
        };

        Rectangle clydeDestinationArea = {
                gClydePosition.x,
                gClydePosition.y,
                gGhostScale.x,
                gGhostScale.y
        };

        // Object Origins
        Vector2 pacManOrigin = {
                gPacManScale.x / 2.0f,
                gPacManScale.y / 2.0f
        };

        Vector2 ghostOrigin = {
                gGhostScale.x / 2.0f,
                gGhostScale.y / 2.0f
        };

        // Draw Objects
        DrawTexturePro(
                gPacManTexture,
                pacManTextureArea,
                pacManDestinationArea,
                pacManOrigin,
                gPacManAngle,
                WHITE
        );

        DrawTexturePro(
                gBlinkyTexture,
                blinkyTextureArea,
                blinkyDestinationArea,
                ghostOrigin,
                gBlinkyAngle,
                WHITE
        );

        DrawTexturePro(
                gInkyTexture,
                inkyTextureArea,
                inkyDestinationArea,
                ghostOrigin,
                gInkyAngle,
                WHITE
        );

        DrawTexturePro(
                gPinkyTexture,
                pinkyTextureArea,
                pinkyDestinationArea,
                ghostOrigin,
                gPinkyAngle,
                WHITE
        );

        DrawTexturePro(
                gClydeTexture,
                clydeTextureArea,
                clydeDestinationArea,
                ghostOrigin,
                gClydeAngle,
                WHITE
        );

        EndDrawing();
}

void shutdown()
{
        UnloadTexture(gPacManTexture);
        UnloadTexture(gBlinkyTexture);
        UnloadTexture(gInkyTexture);
        UnloadTexture(gPinkyTexture);
        UnloadTexture(gClydeTexture);
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