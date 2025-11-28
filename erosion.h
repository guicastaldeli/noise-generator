#include "main.h"

typedef struct {
    float height;
    float water;
    float sediment;
} ErosionCell;

typedef struct {
    float posX;
    float posY;
    float speedX;
    float speedY;
    float water;
    float sediment;
    float volume;
} WaterDroplet;

void simulateHydraulicErosion(
    float** heightMap, 
    int size, 
    int droplets, 
    int iterations
);
void thermalErosion(
    float** heightMap,
    int size,
    float talusAngle,
    int iterations
);