#include "workouts.h"
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

Workouts::Workouts(QObject *parent) : QObject(parent)
{
}

QQmlListProperty<Workout> Workouts::workouts()
{
    return QQmlListProperty<Workout>(this, m_workouts);
}

int Workouts::workoutCount() const
{
    return m_workouts.count();
}

void Workouts::loadWorkout(const int workoutId)
{
    // load the workfile
    qWarning() << "Opening workout: " << workoutId;

    Workout* wo = getWorkoutById(workoutId);
    if (wo == NULL)
    {
        qWarning() << "Error opening workout: " << workoutId;
        emit workoutLoadError();
    }
    else
    {
        // load the workout
        if (wo->loadSamples())
        {
            emit workoutLoaded();
        }
        else
        {
            qWarning() << "Error loading workout: " << workoutId;
            emit workoutLoadError();
        }
    }
}

Workout* Workouts::getWorkoutById(int workoutId)
{
    foreach (Workout* wo, m_workouts) {
        if (wo->id() == workoutId)
            return wo;
    }
    return NULL ;
}

void Workouts::moveNotchSample(int workoutId, const int step)
{
    Workout* wo = getWorkoutById(workoutId);
    if (wo == NULL)
    {
        qWarning() << "Error accessing workout: " << workoutId;
        emit workoutLoadError();
    }
    // move to the next sample
    wo->Notch().moveIndex(step);

    // move everything
    moveBody(wo);
    upperArmL(wo);
    upperArmR(wo);
    forearmL(wo);
    forearmR(wo);
    thighL(wo);
    thighR(wo);
    shinL(wo);
    shinR(wo);
}

void Workouts::moveBody(Workout* wo)
{
    if (wo->Notch().isTracked(BONES::B_CHEST))
    {
        BonePosition& bp = wo->Notch().getPosition(BONES::B_CHEST);
        BoneOrientation& bo = wo->Notch().getOrientation(BONES::B_CHEST);
        emit moveBody(bp.pos_x(), bp.pos_y(), bp.pos_z(), bo.q1(), bo.q2(), bo.q3(), bo.q4());
    }
}

void Workouts::upperArmL(Workout* wo)
{
    if (wo->Notch().isTracked(BONES::B_LEFT_UPPER_ARM))
    {
        BonePosition bp = wo->Notch().getPosition(BONES::B_LEFT_UPPER_ARM);
        BoneOrientation bo = wo->Notch().getOrientation(BONES::B_LEFT_UPPER_ARM);
        emit moveupperArmL(bp.pos_x(), bp.pos_y(), bp.pos_z(), bo.q1(), bo.q2(), bo.q3(), bo.q4());
    }
}

void Workouts::upperArmR(Workout* wo)
{
    if (wo->Notch().isTracked(BONES::B_RIGHT_UPPER_ARM))
    {
        BonePosition bp = wo->Notch().getPosition(B_RIGHT_UPPER_ARM);
        BoneOrientation bo = wo->Notch().getOrientation(B_RIGHT_UPPER_ARM);
        emit moveupperArmR(bp.pos_x(), bp.pos_y(), bp.pos_z(), bo.q1(), bo.q2(), bo.q3(), bo.q4());
    }
}

void Workouts::forearmL(Workout* wo)
{
    if (wo->Notch().isTracked(BONES::B_LEFT_FOREARM))
    {
        BonePosition bp = wo->Notch().getPosition(BONES::B_LEFT_FOREARM);
        BoneOrientation bo = wo->Notch().getOrientation(BONES::B_LEFT_FOREARM);
        emit moveforearmL(bp.pos_x(), bp.pos_y(), bp.pos_z(), bo.q1(), bo.q2(), bo.q3(), bo.q4());
    }
}

void Workouts::forearmR(Workout* wo)
{
    if (wo->Notch().isTracked(BONES::B_RIGHT_FOREARM))
    {
        BonePosition bp = wo->Notch().getPosition(B_RIGHT_FOREARM);
        BoneOrientation bo = wo->Notch().getOrientation(B_RIGHT_FOREARM);
        emit moveforearmR(bp.pos_x(), bp.pos_y(), bp.pos_z(), bo.q1(), bo.q2(), bo.q3(), bo.q4());
    }
}

void Workouts::thighL(Workout* wo)
{
    if (wo->Notch().isTracked(BONES::B_LEFT_THIGH))
    {
        BonePosition bp = wo->Notch().getPosition(BONES::B_LEFT_THIGH);
        BoneOrientation bo = wo->Notch().getOrientation(BONES::B_LEFT_THIGH);
        emit movethighL(bp.pos_x(), bp.pos_y(), bp.pos_z(), bo.q1(), bo.q2(), bo.q3(), bo.q4());
    }
}

void Workouts::thighR(Workout* wo)
{
    if (wo->Notch().isTracked(BONES::B_RIGHT_THIGH))
    {
        BonePosition bp = wo->Notch().getPosition(BONES::B_RIGHT_THIGH);
        BoneOrientation bo = wo->Notch().getOrientation(BONES::B_RIGHT_THIGH);
        emit movethighR(bp.pos_x(), bp.pos_y(), bp.pos_z(), bo.q1(), bo.q2(), bo.q3(), bo.q4());
    }
}

void Workouts::shinL(Workout* wo)
{
    if (wo->Notch().isTracked(BONES::B_LEFT_LOWER_LEG))
    {
        BonePosition bp = wo->Notch().getPosition(BONES::B_LEFT_LOWER_LEG);
        BoneOrientation bo = wo->Notch().getOrientation(BONES::B_LEFT_LOWER_LEG);
        emit moveshinL(bp.pos_x(), bp.pos_y(), bp.pos_z(), bo.q1(), bo.q2(), bo.q3(), bo.q4());
    }
}

void Workouts::shinR(Workout* wo)
{
    if (wo->Notch().isTracked(BONES::B_RIGHT_LOWER_LEG))
    {
        BonePosition bp = wo->Notch().getPosition(BONES::B_RIGHT_LOWER_LEG);
        BoneOrientation bo = wo->Notch().getOrientation(BONES::B_RIGHT_LOWER_LEG);
        emit moveshinR(bp.pos_x(), bp.pos_y(), bp.pos_z(), bo.q1(), bo.q2(), bo.q3(), bo.q4());
    }
}


void Workouts::loadWorkouts()
{
    QFile loadFile(QStringLiteral("workouts.json"));

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

void Workouts::saveWorkouts()
{
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

void Workouts::loadJson(const QJsonObject &json)
{
    QJsonArray array = json["workouts"].toArray();
    for (int i = 0; i < array.size(); ++i) {
        QJsonObject object = array[i].toObject();
        Workout* wo = new Workout();
        wo->read(object);
        m_workouts.append(wo);
    }
}

void Workouts::saveJson(QJsonObject &json)
{
    QJsonArray array;
    foreach (const Workout* wo, m_workouts) {
        QJsonObject object;
        wo->write(object);
        array.append(object);
    }
    json["workouts"] = array;
}

void Workouts::loadFakeData()
{
    // create a file
    SensorFile* sensorFile1 = new SensorFile();
    sensorFile1->setFilename("f:\\sensor_files\\test.txt");
    sensorFile1->setType(1);

    SensorFile* sensorFile2 = new SensorFile();
    sensorFile2->setFilename("f:\\sensor_files\\test2.txt");
    sensorFile2->setType(1);

    // create a workout
    Workout* workout1 = new Workout();
    workout1->setId(1);
    workout1->setName("Sufferfest-Angles Biking");
    workout1->setDate("Oct. 11 2016");
    workout1->setType("Biking");
    workout1->addSensorFile(sensorFile1);
    workout1->addSensorFile(sensorFile2);

    Workout* workout2 = new Workout();
    workout2->setId(2);
    workout2->setName("Ile-Bizard Running");
    workout2->setDate("Oct. 13 2016");
    workout2->setType("Running");
    workout2->addSensorFile(sensorFile1);
    workout2->addSensorFile(sensorFile2);

    Workout* workout3 = new Workout();
    workout3->setId(3);
    workout3->setName("Lap Swimming");
    workout3->setDate("Oct. 15 2016");
    workout3->setType("Swimming");
    workout3->addSensorFile(sensorFile1);
    workout3->addSensorFile(sensorFile2);

    // Add to the list
    m_workouts.append(workout1);
    m_workouts.append(workout2);
    m_workouts.append(workout3);

    // save file
    saveWorkouts();
}
