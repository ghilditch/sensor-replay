#ifndef BONEORIENTATION_H
#define BONEORIENTATION_H

#include <QObject>

class BoneOrientation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal q1 READ q1)
    Q_PROPERTY(qreal q2 READ q2)
    Q_PROPERTY(qreal q3 READ q3)
    Q_PROPERTY(qreal q4 READ q4)

public:
    explicit BoneOrientation(QObject *parent = 0);
    BoneOrientation(const BoneOrientation &);

    inline void operator = (const BoneOrientation &BO ) {
        m_q1 = BO.q1();
        m_q2 = BO.q2();
        m_q3 = BO.q3();
        m_q4 = BO.q4();
    }

    qreal q1() const;
    qreal q2() const;
    qreal q3() const;
    qreal q4() const;

    void setQuaternion(qreal q1, qreal q2, qreal q3, qreal q4);

private:
    qreal m_q1;
    qreal m_q2;
    qreal m_q3;
    qreal m_q4;

signals:

public slots:
};

#endif // BONEORIENTATION_H
