#ifndef WORKOUTS_H
#define WORKOUTS_H

#include <QObject>
#include <QQmlListProperty>
#include "workout.h"
#include "boneorientation.h"
#include "boneposition.h"

class Workouts : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Workout> workouts READ workouts NOTIFY workoutsChanged)
public:
    explicit Workouts(QObject *parent = 0);
    QQmlListProperty<Workout> workouts();
    int workoutCount() const;

    // Methods
    Q_INVOKABLE void loadWorkout(const int workoutId);
    Q_INVOKABLE void loadWorkouts();
    Q_INVOKABLE void saveWorkouts();

    // move the notch sample, forward or backwards
    Q_INVOKABLE void moveNotchSample(int workoutId, const int step);

private:
    QList<Workout *> m_workouts;

private:
    void saveJson(QJsonObject &json);
    void loadJson(const QJsonObject &json);

    // Fake load
    void loadFakeData();

    // helpers
    Workout* getWorkoutById(int workoutId);

    // movement processing
    void moveBody(Workout* wo);
    void upperArmL(Workout* wo);
    void upperArmR(Workout* wo);
    void forearmL(Workout* wo);
    void forearmR(Workout* wo);
    void thighL(Workout* wo);
    void thighR(Workout* wo);
    void shinL(Workout* wo);
    void shinR(Workout* wo);

    // Angle movement
    void moveHipAngle(Workout* wo);
    void moveNeckAngle(Workout* wo);
    void moveupperArmRAngle(Workout* wo);
    void moveupperArmLAngle(Workout* wo);
    void moveforearmLAngle(Workout* wo);
    void moveforearmRAngle(Workout* wo);
    void movethighLAngle(Workout* wo);
    void movethighRAngle(Workout* wo);
    void moveshinLAngle(Workout* wo);
    void moveshinRAngle(Workout* wo);

signals:
    void workoutsChanged();
    void workoutLoaded();
    void workoutLoadError();

    // Body movements
    void moveBody(qreal x, qreal y, qreal z, qreal q1, qreal q2, qreal q3, qreal q4);
    void moveupperArmL(qreal x, qreal y, qreal z, qreal q1, qreal q2, qreal q3, qreal q4);
    void moveupperArmR(qreal x, qreal y, qreal z, qreal q1, qreal q2, qreal q3, qreal q4);
    void moveforearmL(qreal x, qreal y, qreal z, qreal q1, qreal q2, qreal q3, qreal q4);
    void moveforearmR(qreal x, qreal y, qreal z, qreal q1, qreal q2, qreal q3, qreal q4);
    void movethighL(qreal x, qreal y, qreal z, qreal q1, qreal q2, qreal q3, qreal q4);
    void movethighR(qreal x, qreal y, qreal z, qreal q1, qreal q2, qreal q3, qreal q4);
    void moveshinL(qreal x, qreal y, qreal z, qreal q1, qreal q2, qreal q3, qreal q4);
    void moveshinR(qreal x, qreal y, qreal z, qreal q1, qreal q2, qreal q3, qreal q4);

    // Body angles
    void moveHipToChestX(qreal angle);
    void moveHipToChestY(qreal angle);
    void moveHipToChestZ(qreal angle);


public slots:
};

#endif // WORKOUTS_H
