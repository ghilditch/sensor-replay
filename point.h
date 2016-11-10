#ifndef POINT_H
#define POINT_H


class Point
{
public:
    Point();
    Point (const Point& );
    Point(double x, double y, double z);

    inline void operator = (const Point &other ) {
        m_x = other.x();
        m_y = other.y();
        m_z = other.z();
    }

    double x() const;
    double y() const;
    double z() const;

    void x(double x);
    void y(double y);
    void z(double z);

    // distances
    static double distanceBetweenPointsX (const Point& a,const  Point& b);
    static double distanceBetweenPointsY (const Point& a, const Point& b);
    static double distanceBetweenPointsZ (const Point& a, const Point& b);
    static double distanceBetweenPoints (const Point& a, const Point& b);

    // angles
    static double alphaAngleZY(const Point& a,const  Point& b);
    static double betaAngleZY(const Point& a,const  Point& b);
    static double radToDeg(double rad);
    static double degToRad(double deg);

private:
    double m_x;
    double m_y;
    double m_z;

};

#endif // POINT_H
