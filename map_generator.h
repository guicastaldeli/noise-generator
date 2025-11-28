#include "main.h"
#include "poisson_disk.h"
#include "point_generator.h"
#include "domain_warp.h"
#include "noise.h"
#include "erosion.h"

#define WORLD_SIZE 1024
#define CHUNK_SIZE 64
#define OCTAVES 6
#define PERSISTENCE 0.5
#define LACUNARITY 2.0

typedef struct {
    int x;
    int z;
    float heightMap[CHUNK_SIZE][CHUNK_SIZE];
    float waterMap[CHUNK_SIZE][CHUNK_SIZE];
    unsigned char pointId[CHUNK_SIZE][CHUNK_SIZE];
    unsigned char riverMap[CHUNK_SIZE][CHUNK_SIZE];
    unsigned char objMap[CHUNK_SIZE][CHUNK_SIZE];
} Chunk;

float generateEnhancedHeight(
    float worldX,
    float worldZ,
    PointCollection* collection
);
void generateChunk(
    Chunk* chunk,
    PointCollection* collection,
    PoissonCollection* objLocations
);
void generateMap(const char* filename);