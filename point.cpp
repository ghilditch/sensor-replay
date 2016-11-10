#include "point.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

Point::Point() : m_x(0), m_y(0), m_z(0)
{

}

Point::Point(const Point& other)
{
    m_x = other.x();
    m_y = other.y();
    m_z = other.z();
}

Point::Point(double x, double y, double z) : m_x(x), m_y(y), m_z(z)
{

}

double Point::x() const
{
    return m_x;
}

double Point::y() const
{
    return m_y;
}

double Point::z() const
{
    return m_z;
}

void Point::x(double x)
{
    m_x = x;
}

void Point::y(double y)
{
    m_y = y;
}

void Point::z(double z)
{
    m_z = z;
}

double Point::distanceBetweenPointsX (const Point& a,const  Point& b)
{
    return std::abs(b.x() - a.y());
}

double Point::distanceBetweenPointsY (const Point& a,const  Point& b)
{
    return std::abs(b.x() - a.y());
}

double Point::distanceBetweenPointsZ (const Point& a,const  Point& b)
{
    return std::abs(b.x() - a.y());
}

double Point::distanceBetweenPoints(const Point& a,const  Point& b)
{
    double xd = std::abs(b.x() - a.x());
    double yd = std::abs(b.y() - a.y());
    double zd = std::abs(b.z() - a.z());
    double distance = std::sqrt(xd * xd + yd * yd + zd * zd);
    return distance;
}

double Point::alphaAngleZY(const Point& a,const  Point& b)
{
    // calculate the length of the smaller sides
    double A = Point::distanceBetweenPointsZ(a, b);
    double B = Point::distanceBetweenPointsY(a, b);

    double angle = std::atan(A/B);
    return angle;

}

double Point::betaAngleZY(const Point& a,const  Point& b)
{
    // calculate the length of the smaller sides
    double A = Point::distanceBetweenPointsZ(a, b);
    double B = Point::distanceBetweenPointsY(a, b);

    double angle = std::atan(B/A);
    return angle;
}

double Point::radToDeg(double rad)
{
    return rad / M_PI * 180;
}

double Point::degToRad(double deg)
{
    return deg / 180 * M_PI;
}
