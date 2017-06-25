//
// Created by oliver on 30.05.17.
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

const int SWARM_SIZE = 2;
const int COHESION_RANGE = 1000;
const int ALLIGNMENT_RANGE = 300;
const int SEPARATION_RANGE = 16;

const float min_speed = 5;
const float max_speed = 25;

using namespace std;

//struct Boid{
//    Vector3D pos, vel;
//
//    Boid(){}
//    Boid(const Vector3D &pos) : pos(pos) {}
//    Boid(const Vector3D &pos, const Vector3D &vel) : pos(pos), vel(vel) {}
//};

bool is_inrange(const Boid &self, const Boid &other, int range){
    return (self.pos - other.pos).sqrmagnitude() < range * range;
}

struct BoidDatas{
    int size;
    Vector3D* cohes_pos;
    Vector3D* align_vel;
    Vector3D* separ_dir;

    BoidDatas(int size) {
        this->size = size;
        this->cohes_pos = new Vector3D[size];
        this->align_vel = new Vector3D[size];
        this->separ_dir = new Vector3D[size];
    }

    virtual ~BoidDatas() {
        delete this->cohes_pos;
        delete this->align_vel;
        delete this->separ_dir;
    }
};

void unzip_cohespos(vector<Boid> &swarm, Boid &boid, Vector3D* target){
    tbb::parallel_for( tbb::blocked_range<int>(0, (int)swarm.size()),
        [=](const tbb::blocked_range<int>& r) {
            for(int i=r.begin(); i!=r.end(); ++i)
                if(is_inrange(boid, swarm[i], COHESION_RANGE))
                    target[i] = swarm[i].pos;
                else
                    target[i] = boid.pos;
    });
}

void unzip_alignvel(vector<Boid> &swarm, Boid &boid, Vector3D* target){
    tbb::parallel_for( tbb::blocked_range<int>(0, (int)swarm.size()),
        [=](const tbb::blocked_range<int>& r) {
            for(int i=r.begin(); i!=r.end(); ++i)
                if(is_inrange(boid, swarm[i], ALLIGNMENT_RANGE))
                    target[i] = swarm[i].vel;
                else
                    target[i] = boid.vel;
        });
}

void unzip_separdir(vector<Boid> &swarm, Boid &boid, Vector3D* target){
    tbb::parallel_for( tbb::blocked_range<int>(0, (int)swarm.size()),
        [=](const tbb::blocked_range<int>& r) {
            for(int i=r.begin(); i!=r.end(); ++i)
                if(is_inrange(boid, swarm[i], SEPARATION_RANGE))
                    target[i] = swarm[i].pos - boid.pos;
                else
                    target[i] = 0;
        });
}


void boid_update(vector<Boid> &swarm, Boid &boid, float deltaTime){

    BoidDatas datas((int)swarm.size());
    tbb::parallel_invoke([&]{unzip_cohespos(swarm, boid, datas.cohes_pos);},
                         [&]{unzip_alignvel(swarm, boid, datas.align_vel);},
                         [&]{unzip_separdir(swarm, boid, datas.separ_dir);});


    for(int i = 0; i < swarm.size(); ++i){
        assert(swarm[i].pos.equals(datas.cohes_pos[i]));
        assert(swarm[i].vel.equals(datas.align_vel[i]));
    }

    typedef tbb::blocked_range<Vector3D*> range_type;
    Vector3D cohes_pos = tbb::parallel_reduce(
            range_type(datas.cohes_pos, datas.cohes_pos+datas.size), Vector3D(),
            [](range_type const &r, Vector3D init) -> Vector3D{
                for(Vector3D* a = r.begin(); a != r.end(); ++a){
                    init += *a;
                }
                return init;
            },
            std::plus<Vector3D>()
    );

    Vector3D align_vel = tbb::parallel_reduce(
            range_type(datas.align_vel, datas.align_vel+datas.size), Vector3D(),
            [](range_type const &r, Vector3D init) -> Vector3D{
                for(Vector3D* a = r.begin(); a != r.end(); ++a){
                    init += *a;
                }
                return init;
            },
            std::plus<Vector3D>()
    );

    Vector3D separ_dir = tbb::parallel_reduce(
            range_type(datas.separ_dir, datas.separ_dir+datas.size), Vector3D(),
            [](range_type const &r, Vector3D init) -> Vector3D{
                for(Vector3D* a = r.begin(); a != r.end(); ++a){
                    init += *a;
                }
                return init;
            },
            std::plus<Vector3D>()
    );


    cohes_pos *= (1.0f / swarm.size());
    align_vel *= (1.0f / swarm.size());
    separ_dir *= (1.0f / swarm.size());

    // "return home" rule
    Vector3D home = boid.pos * boid.pos.sqrmagnitude() * -0.0000001f;

    // "height" rule
    Vector3D height = Vector3D(0, 1, 0) * - boid.pos.y();

    boid.vel = (boid.vel + cohes_pos*0.25 + align_vel*2 + separ_dir*5 + home) * (0.25f);


    // boid.vel = (boid.vel + cohes_pos + align_vel - separ_dir) * (0.25f);

//    tbb::parallel_for_each(swarm.begin(), swarm.end(), [&](Boid &other){
//
//        if(&other == &boid) return;
//
//        if(! is_inrange(boid, other, COHESION_RANGE)) return;
//        v_mutex[0].lock();
//        cohes_count++;
//        cohes_pos += other.pos;
//        v_mutex[0].unlock();
//
//        // alignment: find all boids in range 300
//        if(! is_inrange(boid, other, ALLIGNMENT_RANGE)) return;
//        v_mutex[1].lock();
//        align_count++;
//        align_vel += other.vel;
//        v_mutex[1].unlock();
//
//        // separation has shorter range, thats why its later
//        if(! is_inrange(boid, other, SEPARATION_RANGE)) return;
//        v_mutex[2].lock();
//        separ_count++;
//        separ_dir -= (other.pos - boid.pos);
//        v_mutex[2].unlock();
//    });

//    if(cohes_count > 0){
//        cohes_pos *= (1.0f / cohes_count);
//        cohes_pos -= boid.pos;
//    }
//
//    if(align_count > 0){
//        align_vel *= (1.0f / align_count);
//    }
//
//    if(separ_count > 0){
//        separ_dir *= (1.0f / separ_count);
//    }
//
//    boid.vel = (boid.vel + cohes_pos + align_vel + separ_dir) * (0.25f);


}

void swarm_update(vector<Boid> &swarm, float deltaTime){
    tbb::parallel_for_each(swarm.begin(), swarm.end(), [&](Boid &boid){
        boid_update(swarm, boid, deltaTime);
        boid.pos += boid.vel * deltaTime;
    });
}

int main(){
    tbb::task_scheduler_init init(2);

    vector<Boid> swarm = vector<Boid>();

    for(int i = 0; i < SWARM_SIZE; ++i) swarm.push_back(Boid(Vector3D(rand()%5, 0.0f, rand()%5))); /* generate swarm */

    BoidView view = BoidView();
    view.init();

    auto sw = Stopwatch();
    float deltaTime = 0;
    for(;;){
        sw.start();
        swarm_update(swarm, deltaTime);
        sw.stop();
        deltaTime = sw.elapsedMilliseconds() / 1000.0f;
        int i = view.update((int)sw.elapsedMilliseconds(), swarm);
        if(i == 1){
            view.close();
            return 0;
        }

    }
}