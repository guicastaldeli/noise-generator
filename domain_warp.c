#include "domain_warp.h"

void domainWarp(
    float* x,
    float* y,
    float strength,
    int octaves
) {
    float originalX = *x;
    float originalY = *y;

    float warpX = 0.0f;
    float warpY = 0.0f;
    float frequency = 1.0;
    float amplitude = 1.0f;
    float maxAmplitude = 0.0f;

    for(int i = 0; i < octaves; i++) {
        float warpNoiseX = fractualSimplexNoise(
            originalX * frequency * 0.01f + 1000.0f,
            originalY * frequency * 0.01f,
            2, 0.5f, 2.0f
        );
        float warpNoiseY = fractualSimplexNoise(
            originalX * frequency * 0.01f,
            originalY * frequency * 0.01f + 1000.0f,
            2, 0.5f, 2.0f
        );

        warpX += warpNoiseX * amplitude;
        warpY += warpNoiseY * amplitude;
        maxAmplitude += amplitude;

        frequency *= 2.0f;
        amplitude *= 0.5f;
    }

    warpX /= maxAmplitude;
    warpY /= maxAmplitude;
    *x = originalX + warpX * strength;
    *y = originalY + warpY * strength;
}