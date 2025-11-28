#include "main.h"
#include "map_generator.h"

int main() {
    srand(time(NULL));
    generateMap("world_map.bin");
    return 0;
}