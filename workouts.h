#ifndef WORKOUTS_H
#define WORKOUTS_H

#include <QObject>
#include <QQmlListProperty>
#include "workout.h"
#include "boneorientation.h"
#include "boneposition.h"
#include "notchsensorsample.h"
#include "rendercontext.h"

class Workouts : public QObject, RenderContext
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

    Q_INVOKABLE void setPlaybackSpeed(const int workoutId, qreal speed);
    Q_INVOKABLE void startPlayback(int workoutId);
    Q_INVOKABLE void getNextFrame(int workoutId);

    // Render callback
    void OnRenderNotchFrame(NotchSensorSample* sample);

private:
    QList<Workout *> m_workouts;

private:
    void saveJson(QJsonObject &json);
    void loadJson(const QJsonObject &json);

    // Fake load
    void loadFakeData();

    // helpers
    Workout* getWorkoutById(int workoutId);

signals:
    // Workout events
    void workoutsChanged();
    void workoutLoaded(Workout* wo);
    void workoutLoadError();

    // Body movement events
    void updateMesh(NotchSensorSample* sample);
    void lastFrameReached();
    void playbackError();

};

#endif // WORKOUTS_H
