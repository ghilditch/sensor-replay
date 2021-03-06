#ifndef SKELETON_H
#define SKELETON_H

// Define the names
#define ROOT "Root"
#define HIP "Hip"
#define TUMMY "Tummy"
#define CHEST_BOTTOM "ChestBottom"
#define CHEST_TOP "ChestTop"
#define LEFT_SHOULDER "LeftCollar"
#define LEFT_UPPER_ARM "LeftUpperArm"
#define LEFT_FOREARM "LeftForeArm"
#define LEFT_HAND "LeftHand"
#define RIGHT_SHOULDER "RightCollar"
#define RIGHT_UPPER_ARM "RightUpperArm"
#define RIGHT_FOREARM "RightForeArm"
#define RIGHT_HAND "RightHand"
#define NECK "Neck"
#define HEAD "Head"
#define LEFT_HIP "LeftHip"
#define LEFT_THIGH "LeftThigh"
#define LEFT_LOWER_LEG "LeftLowerLeg"
#define LEFT_TOP_FOOT "LeftTopFoot"
#define LEFT_FRONT_FOOT "LeftFrontFoot"
#define LEFT_HEEL "LeftHeel"
#define RIGHT_HIP "RightHip"
#define RIGHT_THIGH "RightThigh"
#define RIGHT_LOWER_LEG "RightLowerLeg"
#define RIGHT_TOP_FOOT "RightTopFoot"
#define RIGHT_FRONT_FOOT "RightFrontFoot"
#define RIGHT_HEEL "RightHeel"
#define ANGLE_SMOOTHING 0
#define MAX_HIP_ANGLE 45
#define CHEST_RISE_RATIO 0.02

#include <QList>
#include <QJsonObject>
#include "bone.h"
#include "notchsensorsample.h"
#include "rendercontext.h"

class Skeleton
{
public:
    Skeleton();
    ~Skeleton();

    void loadFromJSON(const QJsonObject &json);

public:
    void loadOffsets(const QString &);
    void loadAngleOffsets(const QString &);

    void loadPositionData(const QString &);
    void loadOrientationData(int index, const QString &);
    void loadAngleData(int index, const QString &);

    // Post processing
    void postLoadProcessing();

    // pop the next set of samples
    void popNextSetOfSamples(int index);

    // Set the renderer callbacl
    void setRenderer(RenderContext* renderer);

private:
    void createBone(const QJsonObject &json);
    Bone* getEnabledParentBone(Bone*);
    void calculateAngleOfBoneWithLogic(Bone*);
    Bone* getBoneFromName(const QString &);

    // Calculate the angles based on the position
    void positionBodyWithAngles();

    // Adjust the head and neck
    void setHeadAngles();

    // Move the limbs
    void setLimbAngle(const Vector& a, Bone* bone, int index, int offsetAngle);

    // Helpers
    int hipDropAngle(const Vector& a,const  Vector& b);

private:
    // Bones
    QList<Bone*> m_bones;
    int m_timeOffset;

    // Renderer
    RenderContext* m_renderer;
};

#endif // SKELETON_H
