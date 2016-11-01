#ifndef NOTCHWORKOUT_H
#define NOTCHWORKOUT_H

#include <QString>
#include <QDateTime>
#include <QList>
#include "bone.h"
#include "joint.h"

enum BONES{
    B_HEAD,
    B_NECK,
    B_CHEST,
    B_TUMMY,
    B_HIP,
    B_RIGHT_UPPER_ARM,
    B_RIGHT_FOREARM,
    B_RIGHT_HAND,
    B_RIGHT_THIGH,
    B_RIGHT_LOWER_LEG,
    B_RIGHT_FOOT,
    B_LEFT_UPPER_ARM,
    B_LEFT_FOREARM,
    B_LEFT_HAND,
    B_LEFT_THIGH,
    B_LEFT_LOWER_LEG,
    B_LEFT_FOOT
};

enum JOINTS {
    J_NECK,
    J_HIP,
    J_RIGHT_ELBOW,
    J_RIGHT_WRIST,
    J_RIGHT_KNEE,
    J_RIGHT_ANKLE,
    J_LEFT_ELBOW,
    J_LEFT_WRIST,
    J_LEFT_KNEE,
    J_LEFT_ANKLE
};

#define TOTAL_BONES 17
#define TOTAL_JOINTS 10

class NotchWorkout
{

public:
    NotchWorkout();

    QDateTime startTime() const;
    QString workout_name() const;
    int notch_count () const;

    // Load the samples
    bool loadNotch(const QString & );
    // Close samples
    void closeNotch();

    // Interaction
    void moveIndex(int step);

    // body
    bool isTracked(BONES eBone);
    BonePosition getPosition(BONES eBone) const;
    BoneOrientation getOrientation(BONES eBone) const;
    bool isAngleTracked(JOINTS eJoint);
    JointAngle getAngle(JOINTS eJoint) const;

private:
    QDateTime m_startTime;
    QString m_workout_name;
    int m_notch_count;
    QList<QString> m_highlighted_bones;
    QList<QString> m_disabled_bones;

    // Bones
    QList<Bone*> m_bones;
    QList<Joint*> m_joints;

    int m_timeOffset;

    // Current sample
    int m_currentIndex;
    // Max samples
    int m_maxSamples;

private:
    bool loadNotchConfig (const QString &);
    bool loadNotchPositions(const QString &);
    bool loadNotchOrientations(const QString &);
    bool loadNotchAngles(const QString &);
    void loadJson(const QJsonObject &json);

    // helpers
    void loadOffsets(const QString &);
    void loadAngleOffsets(const QString &);
    void loadPositionData(const QString &);
    void loadOrientationData(const QString &);
    void loadAngleData(const QString &);
    QString boneIndexToName (int index);
    QString jointIndexToName (int index);
};

#endif // NOTCHWORKOUT_H
