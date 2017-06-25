//
// Created by oliver on 07.06.17.
//

#ifndef TBBTEST_BOID_VIZ_H
#define TBBTEST_BOID_VIZ_H

#include <SDL2/SDL.h>
#include <vector>
#include <unordered_map>
#include "../core/Stopwatch.h"
#include "../core/Boid.h"
#include "../core/Hashmap.h"

using namespace std;

class BoidView {
public:
    void init();
    int update(int ms, maptype &swarm);
    int update(int ms, std::vector<Boid> &swarm);
    int update(int ms, Boid** swarm, int SWARM_SIZE);
    void close();
private:
    SDL_Window* window = NULL;
    SDL_Surface* surface = NULL;
    SDL_Renderer* ren = NULL;
    Stopwatch sw;


};



#endif //TBBTEST_BOID_VIZ_H
