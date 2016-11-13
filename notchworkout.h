#ifndef NOTCHWORKOUT_H
#define NOTCHWORKOUT_H

#include <QString>
#include <QDateTime>
#include <QList>
#include "skeleton.h"
#include "bone.h"
#include "rendercontext.h"
#include "playbackengine.h"

class NotchWorkout
{

public:
    NotchWorkout();

    QDateTime startTime() const;
    QString workout_name() const;
    int notch_count () const;
    int sampleCount () const;

    // Load the samples
    bool loadNotch(const QString & );
    // Close samples
    void closeNotch();

    // Set the renderer callbacl
    void setRenderer(RenderContext* renderer);

    // start playback engine
    void startPlayback();
    void setPlaybackSpeed(double playbackSpeed);

    // Get the next sample
    void processNextSample();

private:
    QDateTime m_startTime;
    QString m_workout_name;
    int m_notch_count;
    QList<QString> m_highlighted_bones;
    QList<QString> m_disabled_bones;

    // Skeleton
    Skeleton m_skeleton;

    // Playback engine
    PlaybackEngine m_playbackEngine;

    // Sample count
    int m_sampleCount;

private:
    // Load data
    bool loadNotchConfig (const QString &);
    bool loadSkeleton (const QString &);
    bool loadNotchPositions(const QString &);
    bool loadNotchOrientations(const QString &);
    bool loadNotchAngles(const QString &);
    void loadJsonConfig(const QJsonObject &json);
};

#endif // NOTCHWORKOUT_H
