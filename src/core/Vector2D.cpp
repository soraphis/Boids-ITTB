//
// Created by oliver on 15.06.17.
//

#include <cmath>
#include "Vector2D.h"


Vector2D::Vector2D(float x, float z) : values{x, z} {}

void Vector2D::operator+=(const Vector2D &rhs) {
    for(int i = 0; i < 2; ++i)
        this->values[i] += rhs.values[i];
}

void Vector2D::operator-=(const Vector2D &rhs) {
    for(int i = 0; i < 2; ++i)
        this->values[i] -= rhs.values[i];
}

float Vector2D::x() const {
    return this->values[0];
}

void Vector2D::x(float X) {
    this->values[0] = X;
}

float Vector2D::z() const {
    return this->values[1];
}

void Vector2D::z(float Z) {
    this->values[1] = Z;
}

float Vector2D::sqrmagnitude() {
    float s = 0;
    for(int i = 0; i < 2; ++i){
        s += this->values[i] * this->values[i];
    }

    return s;
}

void Vector2D::operator*=(const float &rhs) {
    for(int i = 0; i < 2; ++i)
        this->values[i] *= rhs;
}

bool Vector2D::equals(const Vector2D &rhs) const {
    return this->x() == rhs.x() && this->z() == rhs.z();
}
