#include "main.h"
#include "poisson_disk.h"

typedef struct {
    float x;
    float y;
} Vec2;

typedef struct {
    float x;
    float y;
    float z;
} Vec3;

static int permutation[512];
static Vec2 gradients[512];
static Vec2 simplexGradients[12];

static float dot2(
    Vec2 g, 
    float x, 
    float y
);
static int fastFloor(float x);
float simplexNoise(float x, float y);
float fractualSimplexNoise(
    float x,
    float y,
    int octaves,
    float perssistence,
    float lacunatiry
);
Vec2 curlNoise(float x, float y);
void generateRivers(
    float** heightMap,
    unsigned char** riverMap,
    int size,
    int numRivers
);
void initSystems(unsigned long seed);