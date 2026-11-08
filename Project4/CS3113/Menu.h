#ifndef MENU_H
#define MENU_H

#include "LevelA.h"

class Menu : public Scene
{
private:
    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT];
    Texture2D mTitleTexture;

public:
    static constexpr float TILE_DIMENSION = 75.0f;

    Menu();
    Menu(Vector2 origin, const char *bgHexCode);
    ~Menu();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // MENU_H