// Copyright 2024 Bernie Liu
// Author: Bernie Liu
// UWNetID: bliu29
// Homework 6
// 1/12/2024
// Class for a 3d vector

#include <iostream>
#include "Vector.h"
using std::ostream;

namespace vector374 {

Vector::Vector() {
    init(0, 0, 0);
}

// Construct the vector (x,y,z)
Vector::Vector(const float x, const float y, const float z) {
    init(x, y, z);
}

// Copy constructor
Vector::Vector(const Vector &v) {
    init(v.v_[0], v.v_[1], v.v_[2]);
}

// Destructor
Vector::~Vector() {
    delete[] v_;
}

// Assignment
Vector &Vector::operator=(const Vector &rhs) {
    if (this != &rhs) {
        delete[] v_;
        init(rhs.v_[0], rhs.v_[1], rhs.v_[2]);
    }
    return *this;
}

// dot-product: if a is (a,b,c) and b is (x,y,z),
// return ax+by+cz
double operator*(const Vector &a, const Vector &b) {
    return a.v_[0] * b.v_[0] + a.v_[1] * b.v_[1] + a.v_[2] * b.v_[2];
}

// scalar multiplication: if v is (a,b,c), return (ak,bk,ck)
Vector operator*(const double k, const Vector &v) {
    return Vector(k*v.v_[0], k*v.v_[1], k*v.v_[2]);
}
Vector operator*(const Vector &v, const double k) {
    return Vector(k*v.v_[0], k*v.v_[1], k*v.v_[2]);
}

// Stream output: define << for Vectors
ostream & operator<<(ostream &out, const Vector &v) {
    out << "(" << v.v_[0] << "," << v.v_[1] << "," << v.v_[2] << ")";
    return out;
}

void Vector::init(const float x, const float y, const float z) {
    v_ = new float[3]{x, y, z};
}

};  // namespace vector374


