#include "noise.h"

static float dot2(
    Vec2 g, 
    float x, 
    float y
) {
    return g.x * x + g.y * y;
}