#ifndef LEVELB_H
#define LEVELB_H

#include "LevelA.h"

class LevelB : public Scene
{
private:
    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT];

public:
    static constexpr float TILE_DIMENSION = 75.0f;

    LevelB();
    LevelB(Vector2 origin, const char *bgHexCode);
    ~LevelB();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // LEVELB_H