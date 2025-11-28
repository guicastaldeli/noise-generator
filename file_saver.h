#include "main.h"
#include "map_generator.h"

void saveMapToFile(
    const char* fileName,
    Chunk** chunks,
    int chunksX,
    int chunksZ,
    PointCollection* collection,
    PoissonCollection* objLocations,
    unsigned long seed
);