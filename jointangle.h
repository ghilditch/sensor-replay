#ifndef JOINTANGLE_H
#define JOINTANGLE_H

#include <QObject>

class JointAngle : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal time_offset READ time_offset)
    Q_PROPERTY(qreal angle READ angle)

public:
    explicit JointAngle(QObject *parent = 0);
    JointAngle (const JointAngle &);

    inline void operator = (const JointAngle &JA ) {
        m_time_offset = JA.time_offset();
        m_angle = JA.angle();
    }

    qreal time_offset() const;
    qreal angle() const;

    void setAngle(qreal ts, qreal a);

private:
    qreal m_time_offset;
    qreal m_angle;

signals:

public slots:
};

#endif // JOINTANGLE_H
