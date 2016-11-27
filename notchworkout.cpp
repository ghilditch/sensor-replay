#include "notchworkout.h"
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <qmath.h>

#include "csvreader.h"
#include "vector.h"

NotchWorkout::NotchWorkout(){

}

void NotchWorkout::setRenderer(RenderContext* renderer){
    m_skeleton.setRenderer(renderer);
}

QDateTime NotchWorkout::startTime() const{
    return m_startTime;
}

QString NotchWorkout::workout_name() const{
    return m_workout_name;
}

int NotchWorkout::notch_count () const{
    return m_notch_count;
}

int NotchWorkout::sampleCount() const{
    return m_sampleCount;
}

void NotchWorkout::closeNotch(){
    // todo: delete all sample data; if we enable re-loading
}

void NotchWorkout::startPlayback(){
    m_playbackEngine.startPlayback();
}

void NotchWorkout::setPlaybackSpeed(double playbackSpeed){
    m_playbackEngine.setPlaybackSpeed(playbackSpeed);
}

void NotchWorkout::processNextSample(){
    int nextSampleIndex = m_playbackEngine.getNextSampleIndex();
    m_skeleton.popNextSetOfSamples(nextSampleIndex);
}

bool NotchWorkout::loadNotch(const QString &notchDir){ 
    QString skeleton = notchDir + "skeleton.js";
    skeleton = QDir::toNativeSeparators(skeleton);
    if (!loadSkeleton(skeleton))    {
        qWarning("Couldn't open notch skeleton file.");
        return false;
    }

    // build the path to the config directory
    QString config = notchDir + "config.js";
    config = QDir::toNativeSeparators(config);
    if (!loadNotchConfig(config))    {
        qWarning("Couldn't open notch config file.");
        return false;;

    }

    QString positions = notchDir + "positions.csv";
    positions = QDir::toNativeSeparators(positions);
    if (!loadNotchPositions(positions))    {
        qWarning("Couldn't open notch positions file.");
        return false;
    }

    QString orientations = notchDir + "orientations.csv";
    orientations = QDir::toNativeSeparators(orientations);
    if (!loadNotchOrientations(orientations))    {
        qWarning("Couldn't open notch orientations file.");
        return false;
    }

    QString angles = notchDir + "joint_angles.csv";
    angles = QDir::toNativeSeparators(angles);
    if (!loadNotchAngles(angles))    {
        qWarning("Couldn't open notch angles file.");
        return false;
    }

    // Now that notch is loaded, configure the playback engine
    m_playbackEngine.setSampleCount(m_sampleCount);

    // Post processing
    m_skeleton.postLoadProcessing();

    return true;
}

bool NotchWorkout::loadNotchConfig (const QString &fileName){
    QFile file(fileName);
    if (!file.exists())
        return false;

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open notch config file.");
        return false;
    }

    QByteArray saveData = file.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    loadJsonConfig(loadDoc.object());
    return true;
}

bool NotchWorkout::loadSkeleton (const QString &fileName){
    QFile file(fileName);
    if (!file.exists())
        return false;
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open notch config file.");
        return false;
    }

    QByteArray saveData = file.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    m_skeleton.loadFromJSON(loadDoc.object());
    return true;
}


void NotchWorkout::loadJsonConfig(const QJsonObject &json){
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


bool NotchWorkout::loadNotchPositions(const QString &csv){  
    QList<QStringList> readData = CsvReader::readToList(csv);
    for ( int i = 0; i < readData.size(); ++i )
    {
        // First row is a header row, scan from there
        if (i == 0){
            m_skeleton.loadOffsets(readData.at(i).join(","));
        }else{
            m_skeleton.loadPositionData(readData.at(i).join(","));
        }
    }

    // Set sample count equal to the number of rows minus the header
    m_sampleCount = readData.size() - 1;

    return true;
}

bool NotchWorkout::loadNotchOrientations(const QString &csv){
    QList<QStringList> readData = CsvReader::readToList(csv);
    for ( int i = 0; i < readData.size(); ++i )    {
        // First row is a header row, scan from there
        if (i == 0){
            m_skeleton.loadOffsets(readData.at(i).join(","));
        }else{
            m_skeleton.loadOrientationData(i-1, readData.at(i).join(","));
        }
    }

    return true;
}

bool NotchWorkout::loadNotchAngles(const QString &csv){
    QList<QStringList> readData = CsvReader::readToList(csv);
    for ( int i = 0; i < readData.size(); ++i ){
        // First row is a header row, scan from there
        if (i == 0)
            m_skeleton.loadAngleOffsets(readData.at(i).join(","));
        else
            m_skeleton.loadAngleData(i-1, readData.at(i).join(","));
    }

    return true;
}

