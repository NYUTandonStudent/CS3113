/**
 * Author: Kenneth Sun
 * Assignment: Rise of the AI
 * Date due: 11/16/2026
 * I pledge that I have completed this assignment without
 * collaborating with anyone else, in conformance with the
 * NYU School of Engineering Policies and Procedures on
 * Academic Misconduct.
 **/

#include "CS3113/LevelB.h"
#include "CS3113/LevelC.h"
#include "CS3113/LevelD.h"
#include "CS3113/Menu.h"
#include "CS3113/Win.h"
#include "CS3113/Lose.h"

// Global Constants
constexpr int SCREEN_WIDTH  = 1800,
              SCREEN_HEIGHT = 600,
              FPS           = 120;

constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

// Global Variables
AppStatus gAppStatus       = RUNNING;
float     gPreviousTicks   = 0.0f,
          gTimeAccumulator = 0.0f;

Camera2D gCamera = {};

Scene                     *gCurrentScene = nullptr;
std::map<SceneID, Scene *> gLevels       = {};

bool keyNotPressedAfterLevelSwitch = true;

int lives = 3;

Music backgroundMusic;
Sound engineSound;
Sound crashSound;
Sound cheeringSound;
Sound failSound;

bool wasCarMoving = false;

// Function Declarations
void switchToScene(Scene *scene);
void initialise();
void processInput();
void update();
void render();
void shutdown();

// Shut down previous scene, initialize new scene, and reset camera and movement state
void switchToScene(Scene *scene)
{
    if (gCurrentScene) gCurrentScene->shutdown();

    gCurrentScene = scene;

    if (gCurrentScene == gLevels[LEVEL_A] || gCurrentScene == gLevels[LEVEL_B] || gCurrentScene == gLevels[LEVEL_C] || gCurrentScene == gLevels[LEVEL_D])
    {
            gCurrentScene->setLives(&lives);
    }

    gCurrentScene->initialise();
    if (gCurrentScene->getState().xochitl == nullptr) gCamera.target = ORIGIN;
    else gCamera.target = gCurrentScene->getState().xochitl->getPosition();
    keyNotPressedAfterLevelSwitch = false;
    wasCarMoving = false;
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Scenes");
    InitAudioDevice();

    backgroundMusic = LoadMusicStream("assets/game/SilentPartnerSugarZone.mp3");
    backgroundMusic.looping = true;

    engineSound = LoadSound("assets/game/CarEngineRoar.mp3");
    crashSound = LoadSound("assets/game/CarCrash.mp3");
    cheeringSound = LoadSound("assets/game/CrowdCheer.mp3");
    failSound = LoadSound("assets/game/Fail.mp3");

    PlayMusicStream(backgroundMusic);

    SetMusicVolume(backgroundMusic, 0.5f);

    gLevels[MENU] = new Menu(ORIGIN, "#FFA500");
    gLevels[LEVEL_A] = new LevelA(ORIGIN, "#87CEEB");
    gLevels[LEVEL_B] = new LevelB(ORIGIN, "#87CEEB");
    gLevels[LEVEL_C] = new LevelC(ORIGIN, "#87CEEB");
    gLevels[LEVEL_D] = new LevelD(ORIGIN, "#87CEEB");
    gLevels[WIN] = new Win(ORIGIN, "#00FF00");
    gLevels[LOSE] = new Lose(ORIGIN, "#BB0000");

    switchToScene(gLevels[MENU]);

    gCamera.offset   = ORIGIN;
    gCamera.rotation = 0.0f;
    gCamera.zoom     = 1.0f;

    SetTargetFPS(FPS);
}

void processInput()
{
    // Start new game with 3 lives
    if (gCurrentScene == gLevels[MENU] && IsKeyPressed(KEY_ENTER))
    {
            lives = 3;
            switchToScene(gLevels[LEVEL_A]);
    }

    // Return to menu and reset lives and background music
    if ((gCurrentScene == gLevels[WIN] || gCurrentScene == gLevels[LOSE]) && IsKeyPressed(KEY_ENTER))
    {
            lives = 3;
            switchToScene(gLevels[MENU]);
            StopSound(cheeringSound);
            StopSound(failSound);
            PlayMusicStream(backgroundMusic);
    }

    if (gCurrentScene == gLevels[LEVEL_A] || gCurrentScene == gLevels[LEVEL_B] || gCurrentScene == gLevels[LEVEL_C] || gCurrentScene == gLevels[LEVEL_D])
    {
            if (gCurrentScene != gLevels[LEVEL_A] && IsKeyPressed(KEY_ONE)) switchToScene(gLevels[LEVEL_A]);
            if (gCurrentScene != gLevels[LEVEL_B] && IsKeyPressed(KEY_TWO)) switchToScene(gLevels[LEVEL_B]);
            if (gCurrentScene != gLevels[LEVEL_C] && IsKeyPressed(KEY_THREE)) switchToScene(gLevels[LEVEL_C]);
            if (gCurrentScene != gLevels[LEVEL_D] && IsKeyPressed(KEY_FOUR)) switchToScene(gLevels[LEVEL_D]);
    }

    Entity *player = gCurrentScene->getState().xochitl;

    // Prevent movement keys pressed during level switch from immediately moving player
    if (player != nullptr)
    {
            player->resetMovement();

            if (keyNotPressedAfterLevelSwitch)
            {
                    if (IsKeyDown(KEY_A)) player->moveLeft();
                    else if (IsKeyDown(KEY_D)) player->moveRight();
            }
            else
            {
                    if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D)) keyNotPressedAfterLevelSwitch = true;
            }

            // Play engine sound only when car starts moving
            bool carMoving = keyNotPressedAfterLevelSwitch && (IsKeyDown(KEY_A) || IsKeyDown(KEY_D));
            if (carMoving && !wasCarMoving) PlaySound(engineSound);
            wasCarMoving = carMoving;
    }
    else
    {
            wasCarMoving = false;
    }

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update()
{
    // Delta time
    float ticks     = static_cast<float>(GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    // Fixed timestep
    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        int previousLives = lives;

        gCurrentScene->update(FIXED_TIMESTEP);

        if (lives < previousLives) PlaySound(crashSound);

        deltaTime -= FIXED_TIMESTEP;

        // Follow player with camera
        if (gCurrentScene->getState().xochitl != nullptr)
        {
                Vector2 currentPlayerPosition = {
                    gCurrentScene->getState().xochitl->getPosition().x, ORIGIN.y
                };

                panCamera(&gCamera, &currentPlayerPosition);
        }
    }

    gTimeAccumulator = deltaTime;
}

void render()
{
    BeginDrawing();
    BeginMode2D(gCamera);

    gCurrentScene->render();

    EndMode2D();

    if (gCurrentScene == gLevels[LEVEL_A] || gCurrentScene == gLevels[LEVEL_B] || gCurrentScene == gLevels[LEVEL_C] || gCurrentScene == gLevels[LEVEL_D])
    {
            DrawText(TextFormat("Lives: %d", lives), SCREEN_WIDTH / 2 - 150, 10, 50, GREEN);
    }
    if (gCurrentScene == gLevels[LEVEL_A]) DrawText("Level 1", SCREEN_WIDTH / 2 + 150, 10, 50, RED);
    if (gCurrentScene == gLevels[LEVEL_B]) DrawText("Level 2", SCREEN_WIDTH / 2 + 150, 10, 50, ORANGE);
    if (gCurrentScene == gLevels[LEVEL_C]) DrawText("Level 3", SCREEN_WIDTH / 2 + 150, 10, 50, YELLOW);
    if (gCurrentScene == gLevels[LEVEL_D])
    {
            DrawText(TextFormat("Boss Level"), SCREEN_WIDTH / 2 + 150, 10, 50, MAGENTA);
            DrawText(TextFormat("One Hit = All Lives Lost"), SCREEN_WIDTH / 2 + 150, 60, 30, RED);
    }

    EndDrawing();
}

void shutdown()
{
    for (std::pair<const SceneID, Scene *> &entry : gLevels)
        delete entry.second;

    gLevels.clear();

    UnloadMusicStream(backgroundMusic);
    UnloadSound(engineSound);
    UnloadSound(crashSound);
    UnloadSound(cheeringSound);
    UnloadSound(failSound);
    CloseAudioDevice();
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        UpdateMusicStream(backgroundMusic);

        processInput();
        update();


        // When player wins or loses, stop background music and play appropriate sound effect.
        if (gCurrentScene->getState().nextSceneID != NO_SCENE)
        {
            SceneID id = gCurrentScene->getState().nextSceneID;
            if (id == WIN || id == LOSE)
            {
                    StopMusicStream(backgroundMusic);
                    StopSound(engineSound);
                    StopSound(crashSound);

                    if (id == WIN) PlaySound(cheeringSound);
                    else PlaySound(failSound);
            }

            switchToScene(gLevels[id]);
        }

        render();
    }

    shutdown();

    return 0;
}