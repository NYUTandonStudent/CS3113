#ifndef CS3113_H
#define CS3113_H

#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <map>
#include <vector>
#include <string>

enum AppStatus   { TERMINATED, RUNNING };
enum SceneID     { NO_SCENE = -1, MENU, LEVEL_A, LEVEL_B, LEVEL_C, LEVEL_D, WIN, LOSE };

Color     ColorFromHex(const char *hex);
void      panCamera(Camera2D *camera, const Vector2 *targetPosition);

#endif // CS3113_H