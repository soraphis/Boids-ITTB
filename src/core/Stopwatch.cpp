//
// Created by oliver on 30.05.17.
//

#include "Stopwatch.h"
#include <chrono>

namespace cr = std::chrono;

void Stopwatch::start() {
    this->start_time = cr::high_resolution_clock::now();
    this->running = true;
}

void Stopwatch::stop() {
    if(! this->running) throw this; // to lazy to define an exception object
    this->end_time = cr::high_resolution_clock::now();
    this->running = false;
}

long Stopwatch::elapsedMilliseconds() {
    if(this->running) this->end_time = cr::high_resolution_clock::now();
    return cr::duration_cast<cr::milliseconds>(end_time - start_time).count();
}

bool Stopwatch::is_running() {
    return this->running;
}
