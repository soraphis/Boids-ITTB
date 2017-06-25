//
// Created by oliver on 30.05.17.
//

#ifndef TBBTEST_STOPWATCH_H
#define TBBTEST_STOPWATCH_H

#include <chrono>

namespace cr = std::chrono;

class Stopwatch {
private:
    cr::time_point<cr::high_resolution_clock> start_time, end_time;
    bool running = false;

public:
    void start();
    void stop();
    long elapsedMilliseconds();
    bool is_running();

};


#endif //TBBTEST_STOPWATCH_H
