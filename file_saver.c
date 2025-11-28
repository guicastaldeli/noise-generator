#include "file_saver.h"

void saveMapToFile(
    const char* fileName, 
    Chunk** chunks, 
    int chunksX, 
    int chunksZ, 
    PointCollection* collection, 
    PoissonCollection* objLocations, 
    unsigned long seed
) {
    FILE* file = fopen(fileName, "wb");
    if(!file) {
        prinf("Error: could not create file", fileName);
        return;
    }

    fwrite("WORLDMAP", 11, 1, file);
    fwrite(&seed, sizeof(unsigned long), 1, file);
    fwrite(&chunksX, sizeof(int), 1, file);
    fwrite(&chunksZ, sizeof(int), 1, file);
    fwrite(&collection->count, sizeof(int), 1, file);
    fwrite(&objLocations->count, sizeof(int), 1, file);
    fwrite(collection->points, sizeof(Point), collection->count, file);
    fwrite(objLocations->points, sizeof(PoissonPoint), objLocations->count, file);

    for(int x = 0; x < chunksX; x++) {
        for(int z = 0; z < chunksZ; z++) {
            fwrite(&chunks[x][z].x, sizeof(int), 1, file);
            fwrite(&chunks[x][z].z, sizeof(int), 1, file);
            fwrite(chunks[x][z].heightMap, sizeof(float), CHUNK_SIZE * CHUNK_SIZE, file);
            fwrite(chunks[x][z].pointId, sizeof(unsigned char), CHUNK_SIZE * CHUNK_SIZE, file);
            fwrite(chunks[x][z].riverMap, sizeof(unsigned char), CHUNK_SIZE * CHUNK_SIZE, file);
            fwrite(chunks[x][z].objMap, sizeof(unsigned char), CHUNK_SIZE * CHUNK_SIZE, file);
        }
    }

    fclose(file);
    printf("Map saved to %s", fileName);
}