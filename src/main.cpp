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


Vector3D inline avg(tuple<Vector3D, int> &t){
    return get<0>(t) * (1.0f / max(1, get<1>(t)));
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

//        if(! is_inrange(boid, other, ALLIGNMENT_RANGE)) continue;
//        align_count++;
//        align_vel += other.vel;
//
//        if(! is_inrange(boid, other, COHESION_RANGE)) continue;
//        cohes_count++;
//        cohes_pos += other.pos;
//
//        if(! is_inrange(boid, other, SEPARATION_RANGE)) continue;
//        separ_count++;
//        separ_dir -= (other.pos - boid.pos);
    }

//    if(cohes_count > 0){
//        cohes_pos *= (1.0f / cohes_count);
//        cohes_pos = (cohes_pos - boid.pos) * 0.01f;
//    }
//
//    if(align_count > 0){
//        align_vel *= (1.0f / align_count);
//        align_vel = (align_vel - boid.vel) * 0.125f;
//    }

    Vector3D home = Vector3D();
    if(chebyshev_distance(boid.pos, Vector3D()) > RETURN_HOME_RANGE) home = boid.pos * -1;

    Vector3D cohes_dir =  avg(cohesion_pos) - boid.pos;

    // "height" rule
    // Vector3D height = Vector3D(0, 1, 0) * - boid.pos.y();

    // boid.vel += cohes_pos + align_vel + separ_dir*12 + home;
    boid.vel += cohes_dir * 0.01f + avg(allignment_vel) * 0.125f + avg(separation_dir) * 12 + home;

    float _mag = sqrt(boid.vel.sqrmagnitude());
    if(_mag < min_speed)
        boid.vel *= min_speed / _mag;

    if(_mag > max_speed)
        boid.vel *= max_speed / _mag;
}

void swarm_update(vector<Boid> &swarm, float deltaTime){
    vector<Boid> tmp = vector<Boid>();

    for(auto &boid : swarm){
        boid_update(swarm, boid);
        Boid g = Boid(boid.pos + boid.vel * deltaTime, boid.vel);
        tmp.push_back(g);
    }

    swarm = tmp;
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


//    BoidView view = BoidView();
//    view.init();
//
//    auto sw = Stopwatch();
//
//    float deltaTime = 0;
//    for(;;){
//        sw.start();
//        swarm_update(swarm, deltaTime);
//        sw.stop();
//        deltaTime = min(0.0125f, sw.elapsedMilliseconds() / 1000.0f);
//        int i = view.update((int)sw.elapsedMilliseconds(), swarm);
//        if(i == 1){
//            view.close();
//            return 0;
//        }
//        //printf("%ldms\n", sw.elapsedMilliseconds());
//        //printf("(%.2f, %.2f) | (%.2f, %.2f)\n", swarm[0].pos.x(), swarm[0].pos.z(),  swarm[0].vel.x(), swarm[0].vel.z());
//    }
}
//#undef Vector3D