//
// Created by oliver on 14.06.17.
//
#include <vector>
#include "core/Vector3D.h"
#include <tbb/tbb.h>
#include <tbb/mutex.h>
#include <map>
#include "core/Stopwatch.h"
#include "core/Boid.h"
#include "view/BoidView.h"
#include "core/update_loop.h"

#include "core/constants.h"

using namespace std;

// y is up
maptype swarm = maptype();

tuple<int, int, int> spatial_hash(const Vector3D &point){
    return tuple<int, int, int>( (int)(point.x() / CELL_SIZE),
                                 (int)(point.y() / CELL_SIZE),
                                 (int)(point.z() / CELL_SIZE));
}

void insert_boid(Boid &b, maptype &sw = swarm){
    auto hash = spatial_hash(b.pos);
    auto found = sw.find(hash);
    if(found == sw.end()){
        sw[hash] = vector<Boid>();
    }
    sw[hash].push_back(b);
}


Vector3D inline avg(tuple<Vector3D, int> &t){
    return get<0>(t) * (1.0f / get<1>(t));
}

bool is_inrange(Boid &self, Boid &other, int range){
    return (self.pos - other.pos).sqrmagnitude() < range * range;
}

void boid_update(Boid &boid, int3 cell){
    int g = max(COHESION_CELLS, max(ALLIGNMENT_CELLS, SEPARATION_CELLS));

    tuple<Vector3D, int> cohesion_pos   = tuple<Vector3D, int>(Vector3D(), 1);
    tuple<Vector3D, int> allignment_vel = tuple<Vector3D, int>(Vector3D(), 1);
    tuple<Vector3D, int> separation_dir = tuple<Vector3D, int>(Vector3D(), 1);

    for(int x = -g; x < g; ++x){
        for(int y = -g; y < g; ++y){
            for(int z = -g; z < g; ++z) {
                int chb_dist = max(abs(x), max(abs(y), abs(z)));
                int3 c = int3(get<0>(cell)+x, get<1>(cell)+y, get<2>(cell)+z);

                for(auto &other : swarm[cell]){
                    if(&other == &boid) continue;

                    if(chb_dist > ALLIGNMENT_CELLS) continue;
                    if(! is_inrange(boid, other, ALLIGNMENT_RANGE)) continue;
                    get<0>(allignment_vel) += other.vel;
                    get<1>(allignment_vel)++;

                    if(chb_dist > COHESION_CELLS) continue;
                    if(! is_inrange(boid, other, COHESION_RANGE)) continue;
                    get<0>(cohesion_pos) += other.pos;
                    get<1>(cohesion_pos)++;

                    if(chb_dist <= SEPARATION_CELLS) continue;
                    if(! is_inrange(boid, other, SEPARATION_RANGE)) continue;
                    get<0>(separation_dir) -= (other.pos - boid.pos);
                    get<1>(separation_dir)++;
                }
    }   }   }

    // Vector3D home = boid.pos * boid.pos.sqrmagnitude() * -0.0000001f;
    Vector3D home = Vector3D();
    if(chebyshev_distance(boid.pos, Vector3D()) > 55) home = boid.pos * -1;

    Vector3D height = Vector3D(0, 1, 0) * - boid.pos.y();

    Vector3D cohes_dir =  avg(cohesion_pos) - boid.pos;

    boid.vel += cohes_dir * 0.01f + avg(allignment_vel) * 0.125f + avg(separation_dir) * 12 + home;
//    boid.vel += cohes_dir * 0.25 + avg(allignment_vel) * 2 + avg(separation_dir) * 5 + home;
//    boid.vel *= 0.25f;

//    if(boid.vel.sqrmagnitude() < min_speed*min_speed)
//        boid.vel *= (1.0f / sqrt(boid.vel.sqrmagnitude())) * min_speed;

    float _mag = sqrt(boid.vel.sqrmagnitude());
    if(_mag < min_speed)
        boid.vel *= min_speed / _mag;

    if(_mag > max_speed)
        boid.vel *= max_speed / _mag;

}


void swarm_update(maptype &swarm, float deltaTime){

    maptype tmp = maptype();

//    tbb::parallel_for_each(swarm.begin(), swarm.end(), [&](auto &s){
//        for(auto b : s.second){
//            boid_update(b, s.first);
//            Boid g = Boid(b.pos + b.vel * deltaTime, b.vel);
//            insert_boid(g, tmp);
//        }
//    });


    for(auto s = swarm.begin(); s != swarm.end(); ++s){
        for(auto b : s->second){
            boid_update(b, s->first);
            Boid g = Boid(b.pos + b.vel * deltaTime, b.vel);
            insert_boid(g, tmp);
        }
    }

    swarm.swap(tmp);
}


int main(){
    // tbb::task_scheduler_init init(2);

    srand(time(NULL));

    for(int i = 0; i < SWARM_SIZE; ++i){
        auto b = Boid(Vector3D(rand()%50 - 25, 0, rand()%50 - 25));
        insert_boid(b);
    }

    if( update_loop<maptype>(swarm, swarm_update) == 0){
        return 0;
    }


//    BoidView view = BoidView();
//    view.init();
//
//    auto sw = Stopwatch();
//    float deltaTime = 0;
//    for(;;){
//        sw.start();
//        swarm_update(deltaTime);
//        sw.stop();
//        deltaTime = min(0.0125f, sw.elapsedMilliseconds() / 1000.0f);
//        int i = view.update((int)sw.elapsedMilliseconds(), swarm);
//        if(i == 1){
//            view.close();
//            return 0;
//        }
//    }

    // todo: delete all boids
    // .... but the programm ends here anyway
}