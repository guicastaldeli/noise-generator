#include "noise.h"

void domain_wrap(
    float* x,
    float* y,
    float strength,
    int octaves
) {
    float originalX = *x;
    float originalY = *y;

    float wrapX = 0.0f;
    float wrapY = 0.0f;
    float frequency = 1.0;
    float amplitude = 1.0f;
    float maxAmplitude = 0.0f;

    for(int i = 0; i < octaves; i++) {
        float wrapNoiseX = fractualSimplexNoise(
            originalX * frequency * 0.01f + 1000.0f,
            originalY * frequency * 0.01f,
            2, 0.5f, 2.0f
        );
        float wrapNoiseY = fractualSimplexNoise(
            originalX * frequency * 0.01f,
            originalY * frequency * 0.01f + 1000.0f,
            2, 0.5f, 2.0f
        );

        wrapX += wrapNoiseX * amplitude;
        wrapY += wrapNoiseY * amplitude;
        maxAmplitude += amplitude;

        frequency *= 2.0f;
        amplitude *= 0.5f;
    }

    wrapX /= maxAmplitude;
    wrapY /= maxAmplitude;
    *x = originalX + wrapX * strength;
    *y = originalY + wrapY * strength;
}