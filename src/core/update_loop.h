//
// Created by oliver on 21.06.17.
//

#ifndef TBBTEST_UPDATE_LOOP_H
#define TBBTEST_UPDATE_LOOP_H

#include "Boid.h"
#include "../view/BoidView.h"

using namespace std;

template <typename T>
T inline avg(tuple<T, int> &t){
    return get<0>(t) * (1.0f / max(1, get<1>(t)));
}

template <typename T = vector<Boid>>
int update_loop(T &swarm, function<void(T&, float)> swarm_update){
    BoidView view = BoidView();
    view.init();

    auto countdown = Stopwatch();
    auto sw = Stopwatch();

    tuple<float, int> avg_time = tuple<float, int>();
    float deltaTime = 0;
    countdown.start();
    for(;;){
        sw.start();
        swarm_update(swarm, deltaTime);
        sw.stop();
        deltaTime = min(0.0125f, sw.elapsedMilliseconds() / 1000.0f);
        get<0>(avg_time) += sw.elapsedMilliseconds();
        get<1>(avg_time)++;
        int i = view.update((int)avg<float>(avg_time), swarm);
        if(i == 1 || countdown.elapsedMilliseconds() > 25000){
            printf("average milliseconds: %.2f", avg<float>(avg_time));
            view.close();
            return 0;
        }
        //printf("%ldms\n", sw.elapsedMilliseconds());
        //printf("(%.2f, %.2f) | (%.2f, %.2f)\n", swarm[0].pos.x(), swarm[0].pos.z(),  swarm[0].vel.x(), swarm[0].vel.z());
    }
}


#endif //TBBTEST_UPDATE_LOOP_H
