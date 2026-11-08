#ifndef LOSE_H
#define LOSE_H

#include "LevelA.h"

class Lose : public Scene
{
private:
    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT];
    Texture2D mLoseTexture;

public:
    static constexpr float TILE_DIMENSION = 75.0f;

    Lose();
    Lose(Vector2 origin, const char *bgHexCode);
    ~Lose();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // LOSE_H