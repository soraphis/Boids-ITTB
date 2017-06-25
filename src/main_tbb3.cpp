//
// Created by oliver on 14.06.17.
//
#include <stdio.h>
#include <vector>
#include "core/Vector3D.h"
#include <tbb/tbb.h>
#include <tbb/parallel_reduce.h>
#include <tbb/parallel_invoke.h>
#include <tbb/mutex.h>
#include <tbb/task_scheduler_init.h>
#include <atomic>
#include <cassert>
#include "core/Stopwatch.h"
#include "core/Boid.h"
#include "view/BoidView.h"
#include <functional>

const int SWARM_SIZE = 50;
const int COHESION_RANGE = 1000;
const int ALLIGNMENT_RANGE = 300;
const int SEPARATION_RANGE = 16;

const float min_speed = 5;
const float max_speed = 25;

using namespace std;

// y is up

Boid* swarm[SWARM_SIZE][SWARM_SIZE][SWARM_SIZE];

void set_swarm_boid(int index, int d1, int d2, int dir, Boid* b){
    int x = dir == 0 ? index : dir == 1 ? d1 : d2;
    int y = dir == 1 ? index : dir == 2 ? d1 : d2;
    int z = dir == 2 ? index : dir == 0 ? d1 : d2;

    swarm[x][y][z] = b;
}
Boid* get_swarm_boid(int index, int d1, int d2, int dir){
    int x = dir == 0 ? index : dir == 1 ? d1 : d2;
    int y = dir == 1 ? index : dir == 2 ? d1 : d2;
    int z = dir == 2 ? index : dir == 0 ? d1 : d2;

    if(x >= SWARM_SIZE || y >= SWARM_SIZE || z >= SWARM_SIZE){
        return NULL;
    }

    // sei dir = 0: [index][d2][d1]; sei dir = 1: [d1][index][d2]
    return swarm[x][y][z];
}

void shellsort(int dir, int d1, int d2, int n /* stepsize */){

    int i, j , k, h;
    Boid* t;
    int spalten[23] = {2147483647, 1131376761, 410151271, 157840433,
                       58548857, 21521774, 8810089, 3501671, 1355339, 543749, 213331,
                       84801, 27901, 11969, 4711, 1968, 815, 271, 111, 41, 13, 4, 1};

    for (k = 0; k < 23; k++)
    {
        h = spalten[k];
        // Sortiere die "Spalten" mit Insertionsort
        for (i = h; i < n; i++)
        {
            t = get_swarm_boid(i, d1, d2, dir);

            j = i;
            Boid* t2 = get_swarm_boid(j-h, d1, d2, dir);
            bool b =  t == NULL || t2 != NULL && t2->pos.values[dir] > t->pos.values[dir];
            while (j >= h && b)
            {
                set_swarm_boid(j, d1, d2, dir, get_swarm_boid(j-h, d1, d2, dir));
                j = j - h;
            }
            set_swarm_boid(j, d1, d2, dir, t);
        }
    }
}

int sort_dir = 0;
void sort(){

    for(int a = 0; a < SWARM_SIZE; ++a){
        for(int b = 0; b < SWARM_SIZE; ++b) {
            shellsort(sort_dir, a, b, SWARM_SIZE);
        }
    }
    (++sort_dir) %= 3;
}

void moore_neighborhood_gather(void* grid, int radius, function<void (Boid&)> f){
    Boid b = Boid();
    f(b);
}

void update(){
    moore_neighborhood_gather(NULL, 3, [=](Boid &b){
        b.pos += Vector3D(1, 0, 0);
    });
}


int main(){
    tbb::task_scheduler_init init(2);

    srand(time(NULL));

    for(int x = 0; x < SWARM_SIZE; ++x){
        for(int y = 0; y < SWARM_SIZE; ++y){
            for(int z = 0; z < SWARM_SIZE; ++z) {
                swarm[x][y][z] = NULL;
    }   }   }

    for(int i = 0; i < SWARM_SIZE; ++i){
        auto b = new Boid(Vector3D(rand()%50, 0.0f, rand()%50));
        swarm[0][0][i] = b;
    }

    printf("test\n");

    sort();
    sort();
    sort();

    printf("test\n");

    ////////////
    // pick boid at random:
    Boid* b = NULL;
    int arr_pos1[3] = {-1, -1, -1};
    while(b == NULL){
        arr_pos1[0] = rand()%SWARM_SIZE;
        arr_pos1[1] = rand()%SWARM_SIZE;
        arr_pos1[2] = rand()%SWARM_SIZE;
        b = swarm[arr_pos1[0]][arr_pos1[1]][arr_pos1[2]]; // those numbers are decided by fair dice roll
    }


    // find closest to b, thats not b itself
    Boid* c = NULL;
    float min_dist = 900000;
    int arr_pos[3] = {-1, -1, -1};
    for(int x = 0; x < SWARM_SIZE; ++x){
        for(int y = 0; y < SWARM_SIZE; ++y){
            for(int z = 0; z < SWARM_SIZE; ++z) {
                if(! swarm[x][y][z] || swarm[x][y][z] == b) continue;
                Vector3D dif = swarm[x][y][z]->pos - b->pos;
                if(dif.sqrmagnitude() < min_dist){
                    min_dist = dif.sqrmagnitude();
                    c = swarm[x][y][z];
                    arr_pos[0] = x;
                    arr_pos[1] = y;
                    arr_pos[2] = z;
                }
    }   }   }

    BoidView view = BoidView();
    view.init();

    auto sw = Stopwatch();
    float deltaTime = 0;
    for(;;){
        sw.start();
        sw.stop();
        deltaTime = sw.elapsedMilliseconds() / 1000.0f;
        int i = view.update((int)sw.elapsedMilliseconds(), swarm[0][0], SWARM_SIZE);
        if(i == 1){
            view.close();
            return 0;
        }
    }

    // todo: delete all boids
    // .... but the programm ends here anyway
}