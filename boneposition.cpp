#include "boneposition.h"

BonePosition::BonePosition(QObject *parent) : QObject(parent){

}

BonePosition::BonePosition (const BonePosition &copy){
    m_pos_x = copy.pos_x();
    m_pos_y = copy.pos_y();
    m_pos_z = copy.pos_z();
}

qreal BonePosition::pos_x() const{
    return m_pos_x;
}

qreal BonePosition::pos_y() const{
    return m_pos_y;
}

qreal BonePosition::pos_z() const{
    return m_pos_z;
}

void BonePosition::setPosition(qreal x, qreal y, qreal z){
    m_pos_x = x;
    m_pos_y = y;
    m_pos_z = z;
}
