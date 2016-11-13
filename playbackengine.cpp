#include <QDebug>
#include <chrono>
#include "playbackengine.h"

PlaybackEngine::PlaybackEngine(){
    m_nanosPerSample = NANOSECOND / (double)NOTCH_SAMPLES_PER_SEC;
    m_playbackSpeed = 1;
    m_sampleCount = 0;
    m_currentSample = 0;

    // Start the timer
    m_timer.start();
}

void PlaybackEngine::startPlayback(){
    qint64 elasped = m_timer.nsecsElapsed();
    double sampleRewind = m_currentSample * (m_nanosPerSample / m_playbackSpeed);
    m_playbackStartNanos = elasped - (qint64)sampleRewind;
    //qDebug() << "Playback start=" << m_playbackStartNanos << " elasped=" << elasped;
}

void PlaybackEngine::setPlaybackSpeed(double speed){
    m_playbackSpeed = speed;
}

void PlaybackEngine::setSampleCount(int samples){
    m_sampleCount = samples;
}

int PlaybackEngine::getNextSampleIndex(){
    // current elasped time
    qint64 elasped = m_timer.nsecsElapsed();
    qint64 delta = elasped - m_playbackStartNanos;
    long nonLoopingSampleIndex = (long)((double)(delta) / m_nanosPerSample * m_playbackSpeed);
    m_currentSample =  (int)(nonLoopingSampleIndex % (long)m_sampleCount);

    //qDebug() << "Next sample index=" << m_currentSample << " elasped=" << elasped << "delta=" << delta << "speed=" << m_playbackSpeed;

    return m_currentSample;
}
