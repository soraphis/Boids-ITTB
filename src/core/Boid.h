//
// Created by oliver on 07.06.17.
//

#ifndef TBBTEST_BOID_H
#define TBBTEST_BOID_H

#include "Vector3D.h"
#include "Vector2D.h"

// #define Vector3D Vector2D

struct Boid{
    Vector3D pos, vel;

    Boid(){}
    Boid(const Vector3D &pos) : pos(pos), vel(Vector3D()) {}
    Boid(const Vector3D &pos, const Vector3D &vel) : pos(pos), vel(vel) {}

};

inline std::ostream& operator<<(std::ostream &strm, const Boid &a) {
    return strm << "Boid(" << a.pos << ")";
}

inline float chebyshev_distance(const Vector3D &a, const Vector3D &b){
    return std::max(abs(b.x() - a.x()), std::max(abs(b.z() - a.z()), abs(b.y() - a.y())));
    // return max(abs(b.x() - a.x()), abs(b.z() - a.z()));
}

inline float manhattan_distance(const Vector3D &a, const Vector3D &b){
    return abs(b.x() - a.x()) + abs(b.z() - a.z()) /* + abs(b.y() - a.y()) */;
}

// #undef Vector3D
#endif //TBBTEST_BOID_H
