#ifndef BONE_H
#define BONE_H
#include <QObject>
#include <QList>
#include "boneposition.h"
#include "boneorientation.h"

class Bone
{
public:
    Bone(const QString &);

    bool isEnabled() const;
    void setOffset(int offset);

    void addPosition(double ts, QStringList values);
    void addOrientation(double ts, QStringList values);
    BonePosition getPositionAt(int index) const;
    BoneOrientation getOrientationAt(int index) const;
    QString name() const;

private:
    int m_offset;
    QList<BonePosition*> m_bonePositions;
    QList<BoneOrientation*> m_boneOrientations;
    QString m_name;

};

#endif // BONE_H
