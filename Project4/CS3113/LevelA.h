#ifndef LEVELA_H
#define LEVELA_H

#include "Scene.h"

constexpr int LEVEL_WIDTH  = 22,
              LEVEL_HEIGHT = 6;

class LevelA : public Scene
{
private:
    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT];

public:
    static constexpr float TILE_DIMENSION = 75.0f;

    LevelA();
    LevelA(Vector2 origin, const char *bgHexCode);
    ~LevelA();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // LEVELA_H