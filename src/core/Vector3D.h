//
// Created by oliver on 11.05.17.
//

#ifndef UNTITLED_VECTORF_H
#define UNTITLED_VECTORF_H


#include <iostream>


class Vector3D {
public:
    float values[3];

    float x() const;

    void x(float X);
    float y() const;

    void y(float Y);
    float z() const;

    void z(float Z);
    float sqrmagnitude() const;


    Vector3D(float x = 0, float y = 0, float z = 0);
    void operator +=(const Vector3D& rhs);
    void operator *=(const float& rhs);
    void operator -=(const Vector3D& rhs);

    bool equals(const Vector3D& rhs) const;
};

inline std::ostream& operator<<(std::ostream& os, const Vector3D& lhs){
    os << "(" << lhs.x() << ", " << lhs.y() << ", " << lhs.z() << ")";
    return os;
}

inline bool operator==(Vector3D const &lhs, Vector3D const &rhs) {
    return &lhs == &rhs;
}

inline bool operator!=(Vector3D const &lhs, Vector3D const &rhs) {
    return !(lhs == rhs);
}

inline Vector3D operator+(Vector3D lhs, const Vector3D &rhs) {
    lhs += rhs;
    return lhs;
}

inline Vector3D operator-(Vector3D lhs, const Vector3D &rhs) {
    lhs -= rhs;
    return lhs;
}

inline Vector3D operator*(Vector3D lhs, const float &rhs) {
    lhs *= rhs;
    return lhs;
}

#endif //UNTITLED_VECTORF_H
