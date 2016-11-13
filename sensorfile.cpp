#include "sensorfile.h"

SensorFile::SensorFile(QObject *parent) : QObject(parent){

}

QString SensorFile::filename() const{
    return m_filename;
}

void SensorFile::setFilename(const QString &n){
    m_filename = n;
}

int SensorFile::type() const{
    return m_type;
}

void SensorFile::setType(int type){
    m_type = type;
}

void SensorFile::read(const QJsonObject &json){
    m_type = json["type"].toInt();
    m_filename = json["filename"].toString();
}

void SensorFile::write(QJsonObject &json) const{
    json["type"] = m_type;
    json["filename"] = m_filename;
}
