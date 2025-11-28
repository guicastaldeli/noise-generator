#include "poisson_disk.h"

/*
** Poisson Sampling
*/
PoissonCollection* poissonDiskSampling(
    float minDist,
    int width,
    int height,
    int maxAttempts
) {
    PoissonCollection* activeList = malloc(sizeof(PoissonCollection));
    PoissonCollection* points = malloc(sizeof(PoissonCollection));
    initPoissonCollection(activeList, 100);
    initPoissonCollection(points, 1000);

    addPoissonPoint(activeList, width * 0.5f, height * 0.5f);
    addPoissonPoint(points, width * 0.5f, height * 0.5f);
    while(activeList->count > 0 && points->count < 10000) {
        int randomIndex = rand() % activeList->count;
        PoissonPoint current = activeList->points[randomIndex];

        int found = 0;
        for(int attempt = 0; attempt < maxAttempts; attempt++) {
            float angle = 2.0f * PI * ((float)rand() / RAND_MAX);
            float distance = minDist + ((float)rand() / RAND_MAX) * minDist;

            float newX = current.x + cosf(angle) * distance;
            float newY = current.y + sinf(angle) * distance;
            if(
                newX < 0 ||
                newX >= width ||
                newY < 0 ||
                newY >= height
            ) {
                continue;
            }

            int valid = 1;
            for(int i = 0; i < points->count; i++) {
                float dx = newX - points->points[i].x;
                float dy = newY - points->points[i].y;
                if(dx * dx + dy * dy < minDist * minDist) {
                    valid = 0;
                    break;
                }
            }
            if(valid) {
                addPoissonPoint(activeList, newX, newY);
                addPoissonPoint(points, newX, newY);
                found = 1;
                break;
            }
        }
        if(!found) {
            activeList->points[randomIndex] = activeList->points[activeList->count - 1];
            activeList->count--;
        }
    }

    free(activeList->points);
    free(activeList);
    return points;
}

/*
** Add Poisson Point
*/
void addPoissonPoint(
    PoissonCollection* collection, 
    float x, 
    float y
) {
    if(collection->count >= collection->capacity) {
        collection->capacity *= 2;
        collection->points = realloc(
            collection->points,
            collection->capacity * sizeof(PoissonPoint)
        );
    }
    collection->points[collection->count].x = x;
    collection->points[collection->count].y = y;
    collection->count++;
}

void initPoissonCollection(PoissonCollection* collection, int initialCapacity) {
    collection->points = malloc(initialCapacity * sizeof(PoissonPoint));
    collection->count = 0;
    collection->capacity = initialCapacity;
}