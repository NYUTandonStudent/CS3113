#ifndef WIN_H
#define WIN_H

#include "LevelA.h"

class Win : public Scene
{
private:
    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT];
    Texture2D mWinTexture;

public:
    static constexpr float TILE_DIMENSION = 75.0f;

    Win();
    Win(Vector2 origin, const char *bgHexCode);
    ~Win();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // WIN_H