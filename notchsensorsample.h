#ifndef NOTCHSENSORSAMPLE_H
#define NOTCHSENSORSAMPLE_H

#include <QObject>
#include <QDateTime>
#include "boneposition.h"
#include "boneorientation.h"

class NotchSensorSample : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString sensorName READ sensorName WRITE setSensorName NOTIFY sensorNameChanged)
    Q_PROPERTY(int index READ index WRITE setIndex)
    Q_PROPERTY(double timestamp READ timestamp WRITE setTimestamp NOTIFY timestampChanged)
    Q_PROPERTY(qreal pos_x READ pos_x)
    Q_PROPERTY(qreal pos_y READ pos_y)
    Q_PROPERTY(qreal pos_z READ pos_z)
    Q_PROPERTY(qreal q1 READ q1)
    Q_PROPERTY(qreal q2 READ q2)
    Q_PROPERTY(qreal q3 READ q3)
    Q_PROPERTY(qreal q4 READ q4)
    Q_PROPERTY(int angle READ angle WRITE setAngle NOTIFY angleChanged)
    Q_PROPERTY(int radian READ radian WRITE setRadian NOTIFY radianChanged)

public:
    explicit NotchSensorSample(QObject *parent = 0);

    QString sensorName() const;
    void setSensorName(const QString &);

    int index() const;
    void setIndex(int);

    double timestamp() const;
    void setTimestamp(double);

    // Problem with QML allow direct access to position
    qreal pos_x() const;
    qreal pos_y() const;
    qreal pos_z() const;

    // Problem with QML allow direct access to orientation
    qreal q1() const;
    qreal q2() const;
    qreal q3() const;
    qreal q4() const;

    BonePosition position() const;
    void setPosition(const BonePosition &);

    BoneOrientation orientation() const;
    void setOrientation(const BoneOrientation &);

    int angle() const;
    void setAngle(const int);

    qreal radian() const;
    void setRadian(const qreal);

signals:
    void sensorNameChanged();
    void timestampChanged();
    void positionChanged();
    void orientationChanged();
    void angleChanged();
    void radianChanged();

private:
    QString m_sensorName;
    double m_timestamp;
    BonePosition m_position;
    BoneOrientation m_orientation;
    int m_angle;
    qreal m_radian;
    int m_index;

};

#endif // NOTCHSENSORSAMPLE_H
