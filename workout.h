#ifndef WORKOUT_H
#define WORKOUT_H

#include <Qobject>
#include <QList>
#include <QJsonObject>
#include "notchworkout.h"
#include "sensorfile.h"

class Workout : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString date READ date WRITE setDate NOTIFY dateChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(int sampleCount READ sampleCount NOTIFY sampleCountChanged)
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)

public:
    explicit Workout(QObject *parent = 0);

    int sensorfileCount() const;

    QString name() const;
    void setName(const QString &);

    QString date() const;
    void setDate(const QString &);

    QString type() const;
    void setType(const QString &);

    int sampleCount() const;

    int id() const;
    void setId(int);

    void addSensorFile(SensorFile*);

    // load and save workouts
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    // Access to the Notch Workout
    NotchWorkout& Notch()  {return m_notchWorkout;}

    // load the sensor samples
    bool loadSamples();

signals:
    void nameChanged();
    void dateChanged();
    void typeChanged();
    void idChanged();
    void sampleCountChanged();

private:
    int m_id;
    QString m_name;
    QString m_date;
    QString m_type;
    int m_sampleCount;
    QList<SensorFile *> m_sensorfiles;
    NotchWorkout m_notchWorkout;
};

#endif // WORKOUT_H
