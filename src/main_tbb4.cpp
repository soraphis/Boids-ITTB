//
// Created by oliver on 14.06.17.
//
#include <vector>
#include "core/Vector3D.h"
// #include <tbb/tbb.h>
#include <tbb/parallel_sort.h>
#include <tbb/mutex.h>
#include <map>
#include <tbb/parallel_for_each.h>
#include <tbb/parallel_invoke.h>
#include "core/Stopwatch.h"
#include "core/Boid.h"
#include "view/BoidView.h"
#include "core/update_loop.h"

#include "core/constants.h"
using namespace std;

// y is up
// maptype swarm = maptype();
vector<Boid> swarm = vector<Boid>();

inline int next_pow_2(int v){
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

inline uint64_t splitBy3(unsigned int a){
    uint64_t x = a & 0x1fffff; // we only look at the first 21 bits
    x = (x | x << 32) & 0x1f00000000ffff;  // shift left 32 bits, OR with self, and 00011111000000000000000000000000000000001111111111111111
    x = (x | x << 16) & 0x1f0000ff0000ff;  // shift left 32 bits, OR with self, and 00011111000000000000000011111111000000000000000011111111
    x = (x | x << 8) & 0x100f00f00f00f00f; // shift left 32 bits, OR with self, and 0001000000001111000000001111000000001111000000001111000000000000
    x = (x | x << 4) & 0x10c30c30c30c30c3; // shift left 32 bits, OR with self, and 0001000011000011000011000011000011000011000011000011000100000000
    x = (x | x << 2) & 0x1249249249249249;
    return x;
}

inline uint64_t mortonEncode_magicbits(unsigned int x, unsigned int y, unsigned int z){
    uint64_t answer = 0;
    answer |= splitBy3(x) | splitBy3(y) << 1 | splitBy3(z) << 2;
    return answer;
}

uint64_t toMortonCode(Vector3D pos){
    return mortonEncode_magicbits( (unsigned int)(pos.x()/ CELL_SIZE + RETURN_HOME_CELLS),
                                   (unsigned int)(pos.y()/ CELL_SIZE + RETURN_HOME_CELLS),
                                   (unsigned int)(pos.z()/ CELL_SIZE + RETURN_HOME_CELLS)
    );
}

struct compare_morton{
    auto operator()(Boid &a, Boid &b) const
    noexcept(noexcept(toMortonCode(a.pos) < toMortonCode(b.pos)))
    -> decltype(toMortonCode(a.pos) < toMortonCode(b.pos)){
        return toMortonCode(a.pos) < toMortonCode(b.pos);
    }
} comperator;

Vector3D inline avg(tuple<Vector3D, int> &t){
    return get<0>(t) * (1.0f / max(1, get<1>(t)));
}

bool is_inrange(Boid &self, Boid &other, int range){
    return (self.pos - other.pos).sqrmagnitude() < range * range;
}

const float g = next_pow_2(max(COHESION_RANGE, max(ALLIGNMENT_RANGE, SEPARATION_RANGE)));
void boid_update(Boid &boid, int index){
    // float g = max(COHESION_RANGE, max(ALLIGNMENT_RANGE, SEPARATION_RANGE));

    tuple<Vector3D, int> cohesion_pos   = tuple<Vector3D, int>(Vector3D(), 0);
    tuple<Vector3D, int> allignment_vel = tuple<Vector3D, int>(Vector3D(), 0);
    tuple<Vector3D, int> separation_dir = tuple<Vector3D, int>(Vector3D(), 0);

    auto min_val = toMortonCode(boid.pos - Vector3D(1, 1, 1) * g);
    auto max_val = toMortonCode(boid.pos + Vector3D(1, 1, 1) * g);

    int start_index = index;
    int end_index = index;
//    tbb::parallel_invoke([&]{
        for(int i = index; i >= 0; --i){
         if(toMortonCode(swarm[i].pos) < min_val) break;
         start_index = i;
        }
//    },
//    [&]{
        for(int i = index; i < swarm.size(); ++i){
            if(toMortonCode(swarm[i].pos) > max_val) break;
            end_index = i;
        }
//    });


    for(int i = start_index; i < end_index; ++i){
        auto other = swarm[i];

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

    // Vector3D home = boid.pos * boid.pos.sqrmagnitude() * -0.0000001f;
    Vector3D home = Vector3D();
    if(chebyshev_distance(boid.pos, Vector3D()) > RETURN_HOME_RANGE) home = boid.pos * -1;

    // Vector3D height = Vector3D(0, 1, 0) * - boid.pos.y();

    Vector3D cohes_dir =  avg(cohesion_pos) - boid.pos;

    boid.vel += cohes_dir * 0.01f + avg(allignment_vel) * 0.125f + avg(separation_dir) * 12 + home;

//    if(boid.vel.sqrmagnitude() < min_speed*min_speed)
//        boid.vel *= (1.0f / sqrt(boid.vel.sqrmagnitude())) * min_speed;

    float _mag = sqrt(boid.vel.sqrmagnitude());
    if(_mag < min_speed)
        boid.vel *= min_speed / _mag;

    if(_mag > max_speed)
        boid.vel *= max_speed / _mag;

}


void insertion_sort(vector<Boid> &a, int L, int R){
    /**
     * siehe wikipedia - sentinel version zur reduktion von tausch operationen
     */
    bool b0K;
    Boid X, f;

    int k = -1;

    X = a[R];
    for(int i = R; i > L; --i){
        f = a[i - 1];
        if(comperator(X, f)){
            a[i - 1] = X;
            a[i] = f;
            k = i;
        }else{
            X = f;
        }
    }

    if (k < 0) return;

    for(int i = L + 2; i <= R; ++i){
        X = a[i];
        k = i;
        b0K = true;
        while(b0K){
            f = a[k - 1];
            if(comperator(X, f)){
                a[k] = f;
                k = k - 1;
            }else{
                b0K = false;
            }
        }
        if(k < i) a[k] = X;
    }


}


int sort_i = 0;
const int sort_intervall = 10;
void swarm_update(vector<Boid> &swarm, float deltaTime){

//    tbb::parallel_for_each(swarm.begin(), swarm.end(), [&](Boid &b){
//        boid_update(b);
//    });
    tbb::parallel_for(tbb::blocked_range<int>(0, (int)swarm.size()),
    [&](const tbb::blocked_range<int>& r){
        for(int i = r.begin(); i != r.end(); ++i){
            boid_update(swarm[i], i);
        }
    });


    //tbb::parallel_for_each(swarm.begin(), swarm.end(), [&](Boid &b){
    for(auto &b : swarm)
        b.pos += b.vel * deltaTime;
    //});

    sort_i = (++sort_i) % sort_intervall;
    if(sort_i == 0)
        // insertion_sort(swarm, 0, swarm.size()-1);
        tbb::parallel_sort(swarm.begin(), swarm.end(), compare_morton());
}


int main(){
    // tbb::task_scheduler_init init(2);

    long i = 1497957883l; //time(NULL);
    srand(i);

    for(int i = 0; i < SWARM_SIZE; ++i){
        auto b = Boid(Vector3D(rand()%50 - 25, 0, rand()%50 - 25));
        swarm.push_back(b);
    }

    tbb::parallel_sort(swarm.begin(), swarm.end(), compare_morton());

    if( update_loop<vector<Boid>>(swarm, swarm_update) == 0){
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