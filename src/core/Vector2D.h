//
// Created by oliver on 15.06.17.
//

#ifndef TBBTEST_VECTOR2D_H
#define TBBTEST_VECTOR2D_H

#include <iostream>

class Vector2D {
public:
    float values[2];

    float x() const;
    void x(float X);

    float z() const;
    void z(float Z);

    float sqrmagnitude();

    Vector2D(float x = 0, float z = 0);
    void operator +=(const Vector2D& rhs);
    void operator *=(const float& rhs);
    void operator -=(const Vector2D& rhs);

    bool equals(const Vector2D& rhs) const;
};


inline std::ostream& operator<<(std::ostream& os, const Vector2D& lhs){
    os << "(" << lhs.x() << ", " << ", " << lhs.z() << ")";
    return os;
}

inline bool operator==(Vector2D const &lhs, Vector2D const &rhs) {
    return &lhs == &rhs;
}

inline bool operator!=(Vector2D const &lhs, Vector2D const &rhs) {
    return !(lhs == rhs);
}

inline Vector2D operator+(Vector2D lhs, const Vector2D &rhs) {
    lhs += rhs;
    return lhs;
}

inline Vector2D operator-(Vector2D lhs, const Vector2D &rhs) {
    lhs -= rhs;
    return lhs;
}

inline Vector2D operator*(Vector2D lhs, const float &rhs) {
    lhs *= rhs;
    return lhs;
}


#endif //TBBTEST_VECTOR2D_H
