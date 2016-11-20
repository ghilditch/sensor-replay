#ifndef Vector_H
#define Vector_H


class Vector
{
public:
    Vector();
    Vector (const Vector& );
    Vector(double x, double y, double z);

    inline void operator = (const Vector &other ) {
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

    void setCoordinates(double x, double y, double z);

    // distances
    static double distanceBetweenVectorsX (const Vector& a,const  Vector& b);
    static double distanceBetweenVectorsY (const Vector& a, const Vector& b);
    static double distanceBetweenVectorsZ (const Vector& a, const Vector& b);
    static double distanceBetweenVectors (const Vector& a, const Vector& b);

    // angles
    static int alphaAngle(double legA, double legB);
    static int betaAngle(double legA, double legB);
    static int alphaAngleZY(const Vector& a,const  Vector& b);
    static int betaAngleZY(const Vector& a,const  Vector& b);
    static double radToDeg(double rad);
    static double degToRad(double deg);

    // 2D movement
    static Vector getPointOnLine(double slope, const Vector& start, double length);

private:
    double m_x;
    double m_y;
    double m_z;

};

#endif // VECTOR_H
