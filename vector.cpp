#include "vector.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

Vector::Vector() : m_x(0), m_y(0), m_z(0){

}

Vector::Vector(const Vector& other){
    m_x = other.x();
    m_y = other.y();
    m_z = other.z();
}

Vector::Vector(double x, double y, double z) : m_x(x), m_y(y), m_z(z){

}

double Vector::x() const{
    return m_x;
}

double Vector::y() const{
    return m_y;
}

double Vector::z() const{
    return m_z;
}

void Vector::x(double x){
    m_x = x;
}

void Vector::y(double y){
    m_y = y;
}

void Vector::z(double z){
    m_z = z;
}

double Vector::distanceBetweenVectorsX (const Vector& a,const  Vector& b){
    return std::abs(b.x() - a.y());
}

double Vector::distanceBetweenVectorsY (const Vector& a,const  Vector& b){
    return std::abs(b.x() - a.y());
}

double Vector::distanceBetweenVectorsZ (const Vector& a,const  Vector& b){
    return std::abs(b.x() - a.y());
}

double Vector::distanceBetweenVectors(const Vector& a,const  Vector& b){
    double xd = std::abs(b.x() - a.x());
    double yd = std::abs(b.y() - a.y());
    double zd = std::abs(b.z() - a.z());
    double distance = std::sqrt(xd * xd + yd * yd + zd * zd);
    return distance;
}

double Vector::alphaAngleZY(const Vector& a,const  Vector& b){
    // calculate the length of the smaller sides
    double A = Vector::distanceBetweenVectorsZ(a, b);
    double B = Vector::distanceBetweenVectorsY(a, b);

    double angle = std::atan(A/B);
    return angle;

}

double Vector::betaAngleZY(const Vector& a,const  Vector& b){
    // calculate the length of the smaller sides
    double A = Vector::distanceBetweenVectorsZ(a, b);
    double B = Vector::distanceBetweenVectorsY(a, b);

    double angle = std::atan(B/A);
    return angle;
}

double Vector::radToDeg(double rad){
    return rad / M_PI * 180;
}

double Vector::degToRad(double deg){
    return deg / 180 * M_PI;
}
