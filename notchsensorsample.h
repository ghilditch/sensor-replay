#ifndef NOTCHSENSORSAMPLE_H
#define NOTCHSENSORSAMPLE_H

#include <QObject>
#include <QDateTime>
#include "boneposition.h"
#include "boneorientation.h"
#include "jointangle.h"

class NotchSensorSample : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString sensorName READ sensorName WRITE setSensorName NOTIFY sensorNameChanged)
    Q_PROPERTY(QDateTime timestamp READ timestamp WRITE setTimestamp NOTIFY timestampChanged)
    Q_PROPERTY(BonePosition position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(BoneOrientation orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)
    Q_PROPERTY(qreal angle READ angle WRITE setAngle NOTIFY angleChanged)

public:
    explicit NotchSensorSample(QObject *parent = 0);

    QString sensorName() const;
    void setSensorName(const QString &);

    QDateTime timestamp() const;
    void setTimestamp(const QDateTime &);

    BonePosition position() const;
    void setPosition(const BonePosition &);

    BoneOrientation orientation() const;
    void setOrientation(const BoneOrientation &);

    qreal angle() const;
    void setAngle(const qreal);

signals:
    void sensorNameChanged();
    void timestampChanged();
    void positionChanged();
    void orientationChanged();
    void angleChanged();

private:
    QString m_sensorName;
    QDateTime m_timestamp;
    BonePosition m_position;
    BoneOrientation m_orientation;
    qreal m_angle;

};

#endif // NOTCHSENSORSAMPLE_H
