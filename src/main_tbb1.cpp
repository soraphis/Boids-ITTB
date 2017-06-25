//
// Created by oliver on 30.05.17.
//

#include <stdio.h>
#include <vector>
#include "core/Vector3D.h"
#include "core/Vector2D.h"
#include <tbb/tbb.h>
#include "core/Stopwatch.h"
#include "view/BoidView.h"
#include "core/Boid.h"
#include "core/update_loop.h"

#include "core/constants.h"

//#define Vector3D Vector2D

using namespace std;

bool is_inrange(Boid &self, Boid &other, int range){
    return manhattan_distance(self.pos, other.pos) < range;
    // return (self.pos - other.pos).sqrmagnitude() < range * range;
}

void boid_update(vector<Boid> &swarm, Boid &boid){


    tuple<Vector3D, int> cohesion_pos   = tuple<Vector3D, int>(Vector3D(), 0);
    tuple<Vector3D, int> allignment_vel = tuple<Vector3D, int>(Vector3D(), 0);
    tuple<Vector3D, int> separation_dir = tuple<Vector3D, int>(Vector3D(), 0);

    int         cohes_count = 0;
    Vector3D    cohes_pos = Vector3D();

    int         align_count = 0;
    Vector3D    align_vel = Vector3D();

    int         separ_count = 0;
    Vector3D    separ_dir = Vector3D();

    //*
    for(auto &other : swarm){
        if(&other == &boid) continue;


        if(! is_inrange(boid, other, ALLIGNMENT_RANGE)) continue;
        get<0>(allignment_vel) += other.vel;
        get<1>(allignment_vel)++;

        if(! is_inrange(boid, other, COHESION_RANGE)) continue;
        get<0>(cohesion_pos) += other.pos;
        get<1>(cohesion_pos)++;

        if(! is_inrange(boid, other, SEPARATION_RANGE)) continue;
        get<0>(separation_dir) -= (other.pos - boid.pos);
        get<1>(separation_dir)++;
    }
    /*/

    tbb::parallel_for_each(
            swarm.begin(),
            swarm.end(),
            [&](Boid &other) {
                //for(auto &other : swarm){
                if (&other == &boid) return;


                if (!is_inrange(boid, other, ALLIGNMENT_RANGE)) return;
                get<0>(allignment_vel) += other.vel;
                get<1>(allignment_vel)++;

                if (!is_inrange(boid, other, COHESION_RANGE)) return;
                get<0>(cohesion_pos) += other.pos;
                get<1>(cohesion_pos)++;

                if (!is_inrange(boid, other, SEPARATION_RANGE)) return;
                get<0>(separation_dir) -= (other.pos - boid.pos);
                get<1>(separation_dir)++;
                //}
            });
    //*/

    Vector3D home = Vector3D();
    if(chebyshev_distance(boid.pos, Vector3D()) > RETURN_HOME_RANGE) home = boid.pos * -1;

    Vector3D cohes_dir =  avg<Vector3D>(cohesion_pos) - boid.pos;

    boid.vel += cohes_dir * 0.01f + avg<Vector3D>(allignment_vel) * 0.125f + avg<Vector3D>(separation_dir) * 12 + home;

    float _mag = sqrt(boid.vel.sqrmagnitude());
    if(_mag < min_speed)
        boid.vel *= min_speed / _mag;

    if(_mag > max_speed)
        boid.vel *= max_speed / _mag;
}

void swarm_update(vector<Boid> &swarm, float deltaTime){

    tbb::parallel_for_each(swarm.begin(), swarm.end(),
    [&](auto &boid){
        boid_update(swarm, boid);
        boid.pos += boid.vel * deltaTime;
    });

//    vector<Boid> tmp = vector<Boid>();
//    for(auto &boid : swarm){
//        boid_update(swarm, boid);
//        Boid g = Boid(boid.pos + boid.vel * deltaTime, boid.vel);
//        tmp.push_back(g);
//    }

//    swarm = tmp;
}

int main(){
    vector<Boid> swarm = vector<Boid>();
    long i = 1497957883l; //time(NULL);
    srand(i);
    /* generate swarm */
    for(int i = 0; i < SWARM_SIZE; ++i) swarm.push_back(Boid(Vector3D(rand()%50 -25, 0, rand()%50-25)));

    if( update_loop<vector<Boid>>(swarm, swarm_update) == 0){
        return 0;
    }
}
//#undef Vector3D