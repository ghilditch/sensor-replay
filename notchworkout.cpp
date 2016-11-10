#include "notchworkout.h"
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <qmath.h>

#include "csvreader.h"
#include "point.h"

NotchWorkout::NotchWorkout() : m_timeOffset(-1), m_currentIndex (-1), m_maxSamples(0)
{
    // Allocate the bones
    for (int i =0; i < TOTAL_BONES; i++)
    {
        Bone* bone = new Bone();
        m_bones.append(bone);
    }

    // Allocate joints
    for (int j =0; j < TOTAL_JOINTS; j++)
    {
        Joint* joint = new Joint();
        m_joints.append(joint);
    }
}

QDateTime NotchWorkout::startTime() const
{
    return m_startTime;
}

QString NotchWorkout::workout_name() const
{
    return m_workout_name;
}

int NotchWorkout::notch_count () const
{
    return m_notch_count;
}

void NotchWorkout::closeNotch()
{
    // todo: delete all sample data
}

void NotchWorkout::moveIndex(int step)
{
    m_currentIndex += step;
    if (m_currentIndex < 0 || m_currentIndex >= m_maxSamples)
        m_currentIndex = 0;
}

bool NotchWorkout::isTracked(BONES eBone)
{
    // Check the body bone
    return m_bones.at(eBone)->isEnabled();
}

BonePosition NotchWorkout::getPosition(BONES eBone) const
{
    Bone* bone = m_bones.at (eBone);
    const BonePosition& bp = bone->getPositionAt(m_currentIndex);
    return bp;
}

BoneOrientation NotchWorkout::getOrientation(BONES eBone) const
{
    Bone* bone = m_bones.at (eBone);
    return bone->getOrientationAt(m_currentIndex);
}

bool NotchWorkout::isAngleTracked(JOINTS eJoint)
{
    Joint* joint = m_joints.at(eJoint);
    return joint->isEnabled();
}

qreal NotchWorkout::getAngle(JOINTS eJoint) const
{
    Joint* joint = m_joints.at(eJoint);
    if (joint == NULL)
        return 0;

    // If the angle is tracked by notch
    if (joint->isEnabled())
        return joint->getAngleAt(m_currentIndex).angle();
    else
    {
        // Calculate the angle manually
        Bone* hipBone = m_bones.at (BONES::B_HIP);
        if (hipBone == NULL || hipBone->isEnabled())
            return 0;
        Bone* chestBone = m_bones.at (BONES::B_CHEST);
        if (chestBone == NULL || chestBone->isEnabled())
            return 0;

        // Get the positions
        const BonePosition& bpHip = hipBone->getPositionAt(m_currentIndex);
        const BonePosition& bpChest = chestBone->getPositionAt(m_currentIndex);


        // create a couple of points
        Point* a = new Point(bpHip.pos_x(), bpHip.pos_y(), bpHip.pos_z());
        Point* b = new Point(bpChest.pos_x(), bpChest.pos_y(), bpChest.pos_z());

        // Get the angle
        double angle = Point::betaAngleZY(*a, *b);

        // cleanup
        delete a;
        delete b;

        return angle;
    }
}

bool NotchWorkout::loadNotch(const QString &notchDir)
{
    // build the path to the config directory
    QString config = notchDir + "config.js";
    if (!loadNotchConfig(config))
    {
        qWarning("Couldn't open notch config file.");
        return false;
    }

    QString positions = notchDir + "positions.csv";
    if (!loadNotchPositions(positions))
    {
        qWarning("Couldn't open notch positions file.");
        return false;
    }

    QString orientations = notchDir + "orientations.csv";
    if (!loadNotchOrientations(orientations))
    {
        qWarning("Couldn't open notch orientations file.");
        return false;
    }

    QString angles = notchDir + "joint_angles.csv";
    if (!loadNotchAngles(angles))
    {
        qWarning("Couldn't open notch angles file.");
        return false;
    }

    return true;
}

bool NotchWorkout::loadNotchConfig (const QString &fileName)
{
    QFile file(fileName);
    if (!file.exists())
        return false;
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open notch config file.");
        return false;
    }

    QByteArray saveData = file.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    loadJson(loadDoc.object());
    return true;
}

void NotchWorkout::loadJson(const QJsonObject &json)
{
    QJsonObject custom = json["custom"].toObject();
    QString ts = custom["date"].toString();
    m_workout_name = custom["workout_name"].toString();
    m_notch_count = custom["notch_count"].toInt();
    ts.replace(".", "");
    m_startTime = QDateTime::fromString(ts,"yyyyMMdd hh:mm:ss");

    QJsonArray array = json["highlighted_bones"].toArray();
    for (int i = 0; i < array.size(); ++i) {
        QString str = array[i].toString();
        m_highlighted_bones.append(str);
    }
}

bool NotchWorkout::loadNotchPositions(const QString &csv)
{
    m_maxSamples = 0;
    QList<QStringList> readData = CsvReader::readToList(csv);
    for ( int i = 0; i < readData.size(); ++i )
    {
        // First row is a header row, scan from there
        if (i == 0)
            loadOffsets(readData.at(i).join(","));
        else
        {
            loadPositionData(readData.at(i).join(","));
            // increment the number of samples, based on the positions
            m_maxSamples++;
        }
    }

    return true;
}

bool NotchWorkout::loadNotchOrientations(const QString &csv)
{
    QList<QStringList> readData = CsvReader::readToList(csv);
    for ( int i = 0; i < readData.size(); ++i )
    {
        // First row is a header row, scan from there
        if (i == 0)
            loadOffsets(readData.at(i).join(","));
        else
            loadOrientationData(readData.at(i).join(","));
    }

    return true;
}

bool NotchWorkout::loadNotchAngles(const QString &csv)
{
    QList<QStringList> readData = CsvReader::readToList(csv);
    for ( int i = 0; i < readData.size(); ++i )
    {
        // First row is a header row, scan from there
        if (i == 0)
            loadAngleOffsets(readData.at(i).join(","));
        else
            loadAngleData(readData.at(i).join(","));
    }

    return true;
}

void NotchWorkout::loadOffsets(const QString &row)
{
    QStringList values = row.split(',');
    for ( int i = 0; i < values.size(); ++i )
    {
        QString str = values.at(i);
        if (str.indexOf("time") >= 0)
            m_timeOffset = i;
        else if (str.indexOf("Chest") >= 0)
        {
            m_bones.at(B_CHEST)->setOffset(i);
        }
        else if (str.indexOf("Hip") >= 0)
        {
            m_bones.at(B_HIP)->setOffset(i);
        }
        else if (str.indexOf("LeftForeArm") >= 0)
        {
           m_bones.at(B_LEFT_FOREARM)->setOffset(i);
        }
        else if (str.indexOf("LeftLowerLeg") >= 0)
        {
            m_bones.at(B_LEFT_LOWER_LEG)->setOffset(i);
        }
        else if (str.indexOf("LeftThigh") >= 0)
        {
            m_bones.at(B_LEFT_THIGH)->setOffset(i);
        }
        else if (str.indexOf("LeftUpperArm") >= 0)
        {
            m_bones.at(B_LEFT_UPPER_ARM)->setOffset(i);
        }
        else if (str.indexOf("RightForeArm") >= 0)
        {
           m_bones.at(B_RIGHT_FOREARM)->setOffset(i);
        }
        else if (str.indexOf("RightLowerLeg") >= 0)
        {
            m_bones.at(B_RIGHT_LOWER_LEG)->setOffset(i);
        }
        else if (str.indexOf("RightThigh") >= 0)
        {
             m_bones.at(B_RIGHT_THIGH)->setOffset(i);
        }
        else if (str.indexOf("RightUpperArm") >= 0)
        {
            m_bones.at(B_RIGHT_UPPER_ARM)->setOffset(i);
        }
    }
}

void NotchWorkout::loadAngleOffsets(const QString &row)
{
    QStringList values = row.split(',');
    for ( int i = 0; i < values.size(); ++i )
    {
        QString str = values.at(i);
        if (str.indexOf("-LeftUpperArm") >= 0)
        {
            m_joints.at(J_LEFT_ELBOW)->setOffset(i);
        }
        else if (str.indexOf("-LeftThigh") >= 0)
        {
            m_joints.at(J_LEFT_KNEE)->setOffset(i);
        }
        else if (str.indexOf("-RightUpperArm") >= 0)
        {
            m_joints.at(J_RIGHT_ELBOW)->setOffset(i);
        }
        else if (str.indexOf("-RightThigh") >= 0)
        {
            m_joints.at(J_RIGHT_KNEE)->setOffset(i);
        }
    }
}

void NotchWorkout::loadPositionData(const QString &row)
{
    QStringList values = row.split(',');

    // Get the time
    QString str_time = values.at(m_timeOffset);
    double ts = str_time.toDouble();

    for ( int i = 0; i < m_bones.size(); ++i )
    {
        Bone* bone = m_bones.at(i);
        if (bone->isEnabled())
        {
            //qDebug() << "loading bone position=" << boneIndexToName(i);
            bone->addPosition(ts, values);
        }
    }
}

void NotchWorkout::loadOrientationData(const QString &row)
{
    QStringList values = row.split(',');

    // Get the time
    QString str_time = values.at(m_timeOffset);
    double ts = str_time.toDouble();

    for ( int i = 0; i < m_bones.size(); ++i )
    {
        Bone* bone = m_bones.at(i);
        if (bone->isEnabled())
        {
            //qDebug() << "loading bone orientaion=" << boneIndexToName(i);
            bone->addOrientation(ts, values);
        }
    }
}

void NotchWorkout::loadAngleData(const QString &row)
{
    QStringList values = row.split(',');

    // Get the time
    QString str_time = values.at(m_timeOffset);
    double ts = str_time.toDouble();

    for ( int i = 0; i < m_joints.size(); ++i )
    {
        Joint* joint = m_joints.at(i);
        if (joint->isEnabled())
        {
            qDebug() << "loading joint angle=" << jointIndexToName(i);
            joint->addJointAngle(ts, values);
        }
    }
}

QString NotchWorkout::boneIndexToName (int index)
{
    switch (index)
    {
    case B_HEAD:
        return "Head";
    case B_NECK:
        return "Neck";
    case B_CHEST:
        return "Chest";
    case B_TUMMY:
        return "Tummy";
    case B_HIP:
        return "Hip";
    case B_RIGHT_UPPER_ARM:
        return "Right Upper Arm";
    case B_RIGHT_FOREARM:
        return "Right Forearm";
    case B_RIGHT_HAND:
        return "Hand";
    case B_RIGHT_THIGH:
        return "Right Thigh";
    case B_RIGHT_LOWER_LEG:
        return "Right Lower Leg";
    case B_RIGHT_FOOT:
        return "Right Foot";
    case B_LEFT_UPPER_ARM:
        return "Left Upper Arm";
    case B_LEFT_FOREARM:
        return "Left Forearm";
    case B_LEFT_HAND:
        return "Left Hand";
    case B_LEFT_THIGH:
        return "Left Thigh";
    case B_LEFT_LOWER_LEG:
        return "Left Lower Leg";
    case B_LEFT_FOOT:
        return "Left Foot";
    default:
        return "Unknown??";
    }
}

QString NotchWorkout::jointIndexToName (int index)
{
    switch (index)
    {
    case  J_NECK:
        return "Neck";
    case J_HIP:
        return "Hip";
    case J_RIGHT_ELBOW:
        return "Right Elbow";
    case J_RIGHT_WRIST:
        return "Right Wrist";
    case J_RIGHT_KNEE:
        return "Right Knee";
    case J_RIGHT_ANKLE:
        return "Right Ankle";
    case J_LEFT_ELBOW:
        return "Left Elbow";
    case J_LEFT_WRIST:
        return "Left Wrist";
    case J_LEFT_KNEE:
        return "Left Knee";
    case J_LEFT_ANKLE:
        return "Left Ankle";
    default:
        return "Unknown??";
    }
}
