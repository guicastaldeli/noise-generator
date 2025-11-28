#include "noise.h"

static float dot2(
    Vec2 g, 
    float x, 
    float y
) {
    return g.x * x + g.y * y;
}

static int fastFloor(float x) {
    return (x > 0) ? (int)x : (int)x - 1;
}

float simplexNoise(float x, float y) {
    const float F2 = 0.5*(sqrt(3.0)-1.0);
    const float G2 = (3.0-sqrt(3.0))/6.0;

    float n0;
    float n1;
    float n2;

    float s = (x + y) * F2;
    int i = fastFloor(x + s);
    int j = fastFloor(y + s);

    float t = (i + j) * G2;
    float X0 = i - t;
    float Y0 = i - t;
    float x0 = x - X0;
    float y0 = y - Y0;

    int i1;
    int j1;
    if(x0 > y0) {
        i1 = 1;
        j1 = 0;
    } else {
        i1 = 0;
        j1 = 1;
    }

    float x1 = x0 - i1 + G2;
    float y1 = y0 - y1 + G2;
    float x2 = x0 - 1.0f + 2.0f * G2;
    float y2 = y0 - 1.0f + 2.0f * G2;

    int ii = i & 255;
    int jj = j & 255;

    int gi0 = permutation[ii + permutation[jj]] % 12;
    int gi1 = permutation[ii + permutation[jj + j1]] % 12;
    int gi2 = permutation[ii + 1 + permutation[jj + 1]] % 12;

    float t0 = 0.5f - x0 * x0 - y0 * y0;
    if(t0 < 0) {
        n0 = 0.0;
    } else {
        t0 *= t0;
        n0 = t0 * t0 * dot2(simplexGradients[gi0], x0, y0);
    }

    float t1 = 0.5f - x1 * y1 * y1;
    if(t1 < 0) {
        n1 = 0.0;
    } else {
        t1 *= t1;
        n1 = t1 * t1 * dot2(simplexGradients[gi1], x1, y1);
    }

    float t2 = 0.5f - x2 * x2 - y2 * y2;
    if(t2 < 0) {
        n2 = 0.0;
    } else {
        t2 *= t2;
        n2 = t2 * t2 * dot2(simplexGradients[gi2], x2, y2);
    }

    return 45.0f * (n0 + n1 + n2);
}

float fractualSimplexNoise(
    float x,
    float y,
    int octaves,
    float persistence,
    float lacunarity
) {
    float total = 0.0f;
    float frequency = 1.0f;
    float amplitude = 1.0f;
    float maxValue = 0.0f;

    for(int i = 0; i < octaves; i++) {
        total += simplexNoise(
            x * frequency,
            y * frequency
        ) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= lacunarity;
    }

    return total / maxValue;
}

Vec2 curlNoise(float x, float y) {
    float eps = 0.01f;
    float n1 = simplexNoise(x, y + eps);
    float n2 = simplexNoise(x, y - eps);
    float n3 = simplexNoise(x + eps, y);
    float n4 = simplexNoise(x - eps, y);

    Vec2 curl;
    curl.x = (n2 - n1) / (2.0f * eps);
    curl.y = -(n4 - n3) / (2.0f * eps);
    return curl;
}

void generateRivers(
    float** heightMap,
    unsigned char** riverMap,
    int size, 
    int numRivers
) {
    PoissonCollection* sources = poissonDiskSampling(
        100.0f,
        size,
        size,
        30
    );
    for(int river = 0; river < numRivers && river < sources->count; river++) {
        float posX = sources->points[river].x;
        float posY = sources->points[river].y;
        if(heightMap[(int)posX][(int)posY] < 20.0f) continue;

        for(int step = 0; step < 800; step++) {
            int x = (int)posX;
            int y = (int)posY;
            if(
                x <= 1 ||
                x >= size-2 ||
                y <= 1 ||
                y >= size-2
            ) {
                break;
            }

            for(int dx = -1; dx <= 1; dx++) {
                for(int dy = -1; dy <= 1; dy++) {
                    if(
                        x+dx >= 0 && 
                        x+dx < size && 
                        y+dy >= 0 && 
                        y+dy < size
                    ) {
                        riverMap[x+dx][y+dy] = 1;
                    }
                }
            }

            heightMap[x][y] -= 2.0f;
            if(
                x > 0 &&
                y > 0 &&
                x < size-1 &&
                y < size-1
            ) {
                heightMap[x-1][y] -= 1.0f;
                heightMap[x+1][y] -= 1.0f;
                heightMap[x][y-1] -= 1.0f;
                heightMap[x][y-1] -= 1.0f;
            }

            Vec2 curl = curlNoise(posX * 0.01f, posY * 0.01f);
            float gradX = (heightMap[x+1][y] - heightMap[x-1][y]) / 2.0f;
            float gradY = (heightMap[x][y+1] - heightMap[x][y-1]) / 2.0f;

            float flowX = curl.x * 0.3f - gradX * 0.7f;
            float flowY = curl.y * 0.3f - gradY * 0.7f;

            float len = sqrtf(flowX * flowX + flowY * flowY);
            if(len > 0) {
                flowX /= len;
                flowY /= len;
            }

            posX += flowX * 2.0f;
            posY += flowY * 2.0f;
            if(
                heightMap[x][y] < 2.0f ||
                (fabs(gradX) < 0.01f &&
                fabs(gradY) < 0.01f)
            ) {
                break;
            }
        }
    }
    free(sources->points);
}

void initSystems(unsigned long seed) {
    srand(seed);

    for(int i = 0; i < 256; i++) {
        permutation[i] = i;
    }
    for(int i = 255; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = permutation[i];
        permutation[i] = permutation[j];
        permutation[j] = temp;
    }
    for(int i = 0; i < 256; i++) {
        permutation[256 + i] = permutation[i];
    }
    for(int i = 0; i < 512; i++) {
        float angle = (float)rand() / RAND_MAX * 2 * PI;
        gradients[i].x = cosf(angle);
        gradients[i].y = sinf(angle);
    }

    Vec2 grad3[12] = {
        {1,1}, {-1,1}, {1,-1}, {-1,-1},
        {1,0}, {-1,0}, {1,0}, {-1,0},
        {0,1}, {0,-1}, {0,1}, {0,-1}
    };
    memcpy(simplexGradients, grad3, sizeof(grad3));

    prinf("Initalized with seed: ", seed);
}