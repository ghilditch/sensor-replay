#ifndef BONE_H
#define BONE_H
#include <QObject>
#include <QList>
#include "notchsensorsample.h"
#include "vector.h"

class Bone
{
public:
    Bone(const QString &);
    ~Bone();

    // Bone info
    bool isEnabled() const;
    bool isAngleEnabled() const;
    bool hasData() const;
    bool isDataCaptured() const;
    QString name() const;
    Bone* parentBone() const;
    double massRatio() const;
    Vector vector() const;

    // Per sample query
    NotchSensorSample* getSampleAt(int index) const;

    // Sample count
    int getSampleCount() const;

    // Updates
    void setOffset(int offset);
    void setEnabled(bool b);
    void setAngleOffset(int offset);
    void setName(const QString&);
    void setParentBone(Bone*);
    void setMassRatio(double);
    void setVector(const Vector&);
    void setAngle(int index, int angle);

    // NOTE: add position must be called first
    void addPosition(double ts, QStringList values);

    // Update only
    void addOrientation(int index, QStringList values);
    void addAngle(int index, QStringList values);

    // Data post-processing
    void applyRollingMeanFilter();

private:
    int m_offset;
    int m_angleOffset;
    bool m_enabled;
    QString m_name;
    Bone* m_parent;
    double m_massRatio;
    Vector m_vector;

    // Samples
    QList<NotchSensorSample*> m_samples;

};

#endif // BONE_H
