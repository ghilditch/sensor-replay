#include "workout.h"
#include <QJsonArray>
#include <QDebug>

Workout::Workout(QObject *parent) : QObject(parent), m_id(0){
}

QString Workout::name() const{
    return m_name;
}

void Workout::setName(const QString &n){
    m_name = n;
}

QString Workout::date() const{
    return m_date;
}

void Workout::setDate(const QString &d){
    m_date = d;
}

QString Workout::type() const{
    return m_type;
}

void Workout::setType(const QString &t){
    m_type = t;
}

int Workout::id() const{
    return m_id;
}

void Workout::setId(int id){
    m_id = id;
}

int Workout::sampleCount() const{
    return m_sampleCount;
}

void Workout::addSensorFile(SensorFile* s){
    m_sensorfiles.append(s);
}

int Workout::sensorfileCount() const{
    return m_sensorfiles.count();
}

void Workout::read(const QJsonObject &json){
    m_id = json["id"].toInt();
    m_name = json["name"].toString();
    m_date = json["date"].toString();
    m_type = json["type"].toString();

    QJsonArray array = json["sensor_files"].toArray();
    for (int i = 0; i < array.size(); ++i) {
        QJsonObject object = array[i].toObject();
        SensorFile* sf = new SensorFile();
        sf->read(object);
        m_sensorfiles.append(sf);
    }
}

void Workout::write(QJsonObject &json) const{
    json["id"] = m_type;
    json["name"] = m_name;
    json["date"] = m_date;
    json["type"] = m_type;

    QJsonArray array;
    foreach (const SensorFile* sf, m_sensorfiles) {
        QJsonObject object;
        sf->write(object);
        array.append(object);
    }
    json["sensor_files"] = array;
}

bool Workout::loadSamples(){
    // for each sensor file
    if (m_sensorfiles.count() == 0)
    {
        qWarning() << "  No sensor files configured: " << m_name;
        return false;
    }

    foreach (const SensorFile* sf, m_sensorfiles) {
        // load depending on type
        if (sf->type() == (int)SensorFileTypes::NOTCH_DIR){
            if (!m_notchWorkout.loadNotch(sf->filename())){
                qWarning() << "  Failed to load Notch data: " << sf->filename();
                return false;
            }
            // set the sample count
            m_sampleCount = m_notchWorkout.sampleCount();
        }
    }
    return true;
}


