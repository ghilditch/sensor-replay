#ifndef BONEPOSITION_H
#define BONEPOSITION_H

#include <QObject>

class BonePosition : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal time_offset READ time_offset)
    Q_PROPERTY(qreal pos_x READ pos_x)
    Q_PROPERTY(qreal pos_y READ pos_y)
    Q_PROPERTY(qreal pos_z READ pos_z)

public:
    explicit BonePosition(QObject *parent = 0);
    BonePosition (const BonePosition& );

    inline void operator = (const BonePosition &BP ) {
        m_time_offset = BP.time_offset();
        m_pos_x = BP.pos_x();
        m_pos_y = BP.pos_y();
        m_pos_z = BP.pos_z();
    }

    qreal time_offset() const;
    qreal pos_x() const;
    qreal pos_y() const;
    qreal pos_z() const;

    void setPosition(qreal ts, qreal x, qreal y, qreal z);

private:
    qreal m_time_offset;
    qreal m_pos_x;
    qreal m_pos_y;
    qreal m_pos_z;

signals:

public slots:
};

#endif // BONEPOSITION_H
