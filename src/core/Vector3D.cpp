//
// Created by oliver on 11.05.17.
//

#include <cmath>
#include "Vector3D.h"

Vector3D::Vector3D(float x, float y, float z) : values{x, y, z} {}

void Vector3D::operator+=(const Vector3D &rhs) {
    for(int i = 0; i < 3; ++i)
        this->values[i] += rhs.values[i];
}

void Vector3D::operator-=(const Vector3D &rhs) {
    for(int i = 0; i < 3; ++i)
        this->values[i] -= rhs.values[i];
}

float Vector3D::x() const {
    return this->values[0];
}

void Vector3D::x(float X) {
    this->values[0] = X;
}

float Vector3D::y() const {
    return this->values[1];
}

void Vector3D::y(float Y) {
    this->values[1] = Y;
}

float Vector3D::z() const {
    return this->values[2];
}

void Vector3D::z(float Z) {
    this->values[2] = Z;
}

float Vector3D::sqrmagnitude() const{
    float s = 0;
    for(int i = 0; i < 3; ++i){
        s += this->values[i] * this->values[i];
    }
    return s;
}

void Vector3D::operator*=(const float &rhs) {
    for(int i = 0; i < 3; ++i)
        this->values[i] *= rhs;
}

bool Vector3D::equals(const Vector3D &rhs) const {
    return this->x() == rhs.x() && this->y() == rhs.y() && this->z() == rhs.z();
}
