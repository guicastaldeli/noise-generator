#include "erosion.h"

/*
** Hydraulic Erosion
*/
void simulateHydraulicErosion(
    float** heightMap,
    int size,
    int droplets,
    int iterations
) {
    ErosionCell** erosionMap = malloc(size * sizeof(ErosionCell*));
    for(int i = 0; i < size; i++) {
        erosionMap[i] = malloc(size * sizeof(ErosionCell));
        for(int j = 0; j < size; j++) {
            erosionMap[i][j].height = heightMap[i][j];
            erosionMap[i][j].water = 0.0f;
            erosionMap[i][j].sediment = 0.0f;
        }
    }
    for(int iter = 0; iter < iterations; iter++) {
        for(int drop = 0; drop < droplets; drop++) {
            WaterDroplet droplet;
            droplet.posX = (rand() % (size - 20)) + 10;
            droplet.posY = (rand() % (size - 20)) + 10;
            droplet.speedX = 0.0f;
            droplet.speedY = 0.0f;
            droplet.water = 1.0f;
            droplet.sediment = 0.0f;
            droplet.volume = 0.01f;

            for(int step = 0; step < 100; step++) {
                int x = (int)droplet.posX;
                int y = (int)droplet.posY;
                if(
                    x <= 0 ||
                    x >= size-1 ||
                    y <= 0 ||
                    y >= size-1
                ) {
                    break;
                }

                float gradX = (erosionMap[x+1][y].height - erosionMap[x-1][y].height) / 2.0f;
                float gradY = (erosionMap[x][y+1].height - erosionMap[x][y-1].height) / 2.0;
                droplet.speedX = droplet.speedX * 0.9f - gradX * 0.1f;
                droplet.speedY = droplet.speedY * 0.9f - gradY * 0.1f;

                float speedLen = sqrtf(
                    droplet.speedX * droplet.speedX +
                    droplet.speedY * droplet.speedY
                );
                if(speedLen > 0) {
                    droplet.speedX /= speedLen;
                    droplet.speedY /= speedLen;
                }

                droplet.posX += droplet.speedX;
                droplet.posY += droplet.speedY;
                if(droplet.posX < 1 || droplet.posX >= size-1 ||
                droplet.posY < 1 || droplet.posY >= size-1) {
                    break;
                }
                x = (int)droplet.posX;
                y = (int)droplet.posY;

                float sedimentCapacity = fmaxf(
                    -gradX * droplet.speedX -
                    gradY * droplet.speedY,
                    0.0f
                ) *
                droplet.water * droplet.volume * 4.0f;
                if(droplet.sediment > sedimentCapacity || gradX > 0) {
                    float depositAmount = (droplet.sediment - sedimentCapacity) * 0.3;
                    erosionMap[x][y].height += depositAmount;
                    droplet.sediment -= depositAmount;
                } else {
                    float eroseAmount = fminf((sedimentCapacity - droplet.sediment) * 0.3f, 0.01f);
                    erosionMap[x][y].height -= eroseAmount;
                    droplet.sediment += eroseAmount;
                }
                droplet.water *= 0.95f;
                if(droplet.water < 0.01f) break;
            }
        }
        if((iter + 1) % 2 == 0) {
            printf("  Hydraulic erosion iteration %d/%d\n", iter + 1, iterations);
        }
    }

    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            heightMap[i][j] = erosionMap[i][j].height;
        }
        free(erosionMap[i]);
    }
    free(erosionMap);
}

/*
** Thermal Erosion
*/
void thermalErosion(
    float** heightMap,
    int size,
    float talusAngle,
    int iterations
) {
    float** tempHeightMap = malloc(size * sizeof(float*));
    for(int i = 0; i < size; i++) {
        tempHeightMap[i] = malloc(size * sizeof(float));
        memcpy(
            tempHeightMap[i],
            heightMap[i],
            size * sizeof(float)
        );
    }
    for(int iter = 0; iter < iterations; iter++) {
        for(int x = 1; x < size-1; x++) {
            for(int y = 1; y < size-1; y++) {
                float totalDiff = 0.0f;
                int steepNeighbors = 0;
                for(int dx = -1; dx <= 1; dx++) {
                    for(int dy = -1; dy <= 1; dy++) {
                        if(dx == 0 && dy == 0) continue;

                        float heightDiff = tempHeightMap[x][y] - tempHeightMap[x+dx][y+dy];
                        if(heightDiff > talusAngle) {
                            totalDiff += heightDiff;
                            steepNeighbors++;
                        }
                    }
                }
                if(steepNeighbors > 0) {
                    float avgMore = totalDiff / steepNeighbors * 0.1f;
                    heightMap[x][y] -= avgMore;

                    for(int dx = -1; dx <= 1; dx++) {
                        for(int dy = -1; dy <= 1; dy++) {
                            if(dx == 0 && dy == 0) continue;

                            float heightDiff = tempHeightMap[x][y] - tempHeightMap[x+dx][y+dy];
                            if(heightDiff > talusAngle) {
                                heightMap[x+dx][y+dy] += avgMore / steepNeighbors;
                            }
                        }
                    }
                }
            }
        }

        for(int i = 0; i < size; i++) {
            memcpy(
                tempHeightMap[i],
                heightMap[i],
                size * sizeof(float)
            );
        }
        if((iter + 1) % 5 == 0) {
            printf("  Thermal erosion iteration %d/%d\n", iter + 1, iterations);
        }
    }
    for(int i = 0; i < size; i++) {
        free(tempHeightMap[i]);
    }
    free(tempHeightMap);
}