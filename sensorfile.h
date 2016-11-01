#ifndef SENSORFILE_H
#define SENSORFILE_H

#include <QObject>
#include <QJsonObject>

enum SensorFileTypes {
      NOTCH_DIR = 1,
      NOTCH_CONFIG = 2,
      NOTCH_BPOSITIONS = 3,
      NOTCH_BORIENTATIONS = 4
  };

class SensorFile : public QObject
{
    Q_OBJECT
public:
    explicit SensorFile(QObject *parent = 0);

    QString filename() const;
    void setFilename(const QString &);

    int type() const;
    void setType(int);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

private:
    QString m_filename;
    int m_type;

};

#endif // SENSORFILE_H
