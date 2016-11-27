#include "workouts.h"
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QDir>

Workouts::Workouts(QObject *parent) : QObject(parent){
}

QQmlListProperty<Workout> Workouts::workouts(){
    return QQmlListProperty<Workout>(this, m_workouts);
}

int Workouts::workoutCount() const{
    return m_workouts.count();
}

void Workouts::loadWorkout(const int workoutId){
    // load the workfile
    qWarning() << "Opening workout: " << workoutId;

    Workout* wo = getWorkoutById(workoutId);
    if (wo == NULL){
        qWarning() << "Error opening workout: " << workoutId;
        emit workoutLoadError();
    }else{
        // load the workout
        if (wo->loadSamples()){
            emit workoutLoaded(wo);
        }else{
            qWarning() << "Error loading workout: " << workoutId;
            emit workoutLoadError();
        }
    }
}

Workout* Workouts::getWorkoutById(int workoutId){
    foreach (Workout* wo, m_workouts) {
        if (wo->id() == workoutId)
            return wo;
    }
    return NULL;
}

void Workouts::setPlaybackSpeed (int workoutId, qreal speed){
    Workout* wo = getWorkoutById(workoutId);
    if (wo == NULL){
        qWarning() << "Error accessing workout: " << workoutId;
        emit playbackError();
    }
    wo->Notch().setPlaybackSpeed(speed);
}

void Workouts::startPlayback(int workoutId){
    Workout* wo = getWorkoutById(workoutId);
    if (wo == NULL){
        qWarning() << "Error accessing workout: " << workoutId;
        emit playbackError();
    }
    wo->Notch().startPlayback();
}

void Workouts::getNextFrame(int workoutId){
    Workout* wo = getWorkoutById(workoutId);
    if (wo == NULL){
        qWarning() << "Error accessing workout: " << workoutId;
        emit playbackError();
    }
    wo->Notch().processNextSample();
}

void Workouts::OnRenderNotchFrame(NotchSensorSample* sample){
    emit updateMesh(sample);
}

void Workouts::loadWorkouts(){

#ifdef Q_OS_MAC
    QString filename = QStringLiteral("/workouts.json");
#else
    QString filename = QStringLiteral("\\workouts.json");
#endif

    QFile loadFile(QDir::homePath() + filename);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open workout file.");
        // Just load some fake data
        loadFakeData();
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    loadJson(loadDoc.object());

    emit workoutsChanged();
}

void Workouts::saveWorkouts(){
    QFile sfile( QStringLiteral("workouts.json") );

    if (!sfile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QJsonObject workoutObject;
    saveJson(workoutObject);
    QJsonDocument saveDoc(workoutObject);
    sfile.write(saveDoc.toJson());
}

void Workouts::loadJson(const QJsonObject &json){
    QJsonArray array = json["workouts"].toArray();
    for (int i = 0; i < array.size(); ++i) {
        QJsonObject object = array[i].toObject();
        Workout* wo = new Workout();
        wo->read(object);
        wo->Notch().setRenderer(this);
        m_workouts.append(wo);
    }
}

void Workouts::saveJson(QJsonObject &json){
    QJsonArray array;
    foreach (const Workout* wo, m_workouts) {
        QJsonObject object;
        wo->write(object);
        array.append(object);
    }
    json["workouts"] = array;
}

void Workouts::loadFakeData(){
    // create a file
    SensorFile* sensorFile1 = new SensorFile();
    sensorFile1->setFilename("\\Notch\\");
    sensorFile1->setType(1);  

    // create a workout
    Workout* workout1 = new Workout();
    workout1->setId(1);
    workout1->setName("Sufferfest-Angles Biking");
    workout1->setDate("Oct. 11 2016");
    workout1->setType("Biking");
    workout1->addSensorFile(sensorFile1);

    // Add to the list
    m_workouts.append(workout1);


    // save file
    saveWorkouts();
}
