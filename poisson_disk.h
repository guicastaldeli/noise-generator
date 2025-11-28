#include "main.h"

typedef struct {
    float x;
    float y;
} PoissonPoint;

typedef struct {
    PoissonPoint* points;
    int count;
    int capacity;
} PoissonCollection;

PoissonCollection* poissonDiskSampling(
    float minDist,
    int width,
    int height,
    int maxAttempts
);
void addPoissonPoint(
    PoissonCollection* collection, 
    float x, 
    float y
);
void initPoissonCollection(PoissonCollection* collection, int initialCapacity);
