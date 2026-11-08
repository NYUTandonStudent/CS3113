#ifndef LEVELD_H
#define LEVELD_H

#include "LevelA.h"

class LevelD : public Scene
{
private:
    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT];

public:
    static constexpr float TILE_DIMENSION = 75.0f;

    LevelD();
    LevelD(Vector2 origin, const char *bgHexCode);
    ~LevelD();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // LEVELD_H