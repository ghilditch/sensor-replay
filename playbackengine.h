#ifndef PLAYBACKENGINE_H
#define PLAYBACKENGINE_H

#define NANOSECOND 1.0e9
#define NOTCH_SAMPLES_PER_SEC 40.0

#include <QElapsedTimer>


class PlaybackEngine
{
public:
    PlaybackEngine();

    // start the playback timer
    void startPlayback();

    // set the speed
    void setPlaybackSpeed(double speed);

    // get the next sample index
    int getNextSampleIndex();

    // set the max samples
    void setSampleCount(int samples);

private:
    // Playback timer
    QElapsedTimer m_timer;

    double m_nanosPerSample;
    double m_playbackSpeed;
    int m_sampleCount;
    int m_currentSample;
    qint64 m_playbackStartNanos;

};

#endif // PLAYBACKENGINE_H
