#include "skeleton.h"
#include <QJsonArray>
#include <QDebug>

Skeleton::Skeleton() : m_timeOffset(-1), m_renderer(NULL){
}

Skeleton::~Skeleton(){
    foreach (Bone* b, m_bones) {
       delete b;
    }
}

void Skeleton::setRenderer(RenderContext* renderer){
    m_renderer = renderer;
}

void Skeleton::popNextSetOfSamples(int index){
    if (m_renderer != NULL)    {
        // The order of the bones is very important
        foreach (Bone* b, m_bones) {
           if (b->isEnabled())
           {
               // get the sample and notify the renderer
               NotchSensorSample* s = b->getSampleAt(index);
               m_renderer->OnRenderNotchFrame(s);
           }
        }
    }
}

void Skeleton::loadFromJSON(const QJsonObject &json){
    QJsonArray array = json["skeleton"].toArray();
    for (int i = 0; i < array.size(); ++i) {
        QJsonObject object = array[i].toObject();
        createBone(object);
    }
}

void Skeleton::createBone(const QJsonObject &json){
    double mass_ratio = json["mass_ratio"].toDouble();
    QString name = json["name"].toString();
    QString parent = json["parent_name"].toString();
    QJsonArray array = json["bone_vector"].toArray();
    double x = array[0].toDouble();
    double y = array[1].toDouble();
    double z = array[2].toDouble();

    // create the bone
    Bone* b = new Bone(name);
    b->setMassRatio(mass_ratio);
    b->setParentBone(getBoneFromName(parent));
    Vector v;
    v.x(x);
    v.y(y);
    v.z(z);
    b->setVector(v);

    // Add to list
    m_bones.append(b);
}

void Skeleton::loadOffsets(const QString &row){
    QStringList values = row.split(',');
    for ( int i = 0; i < values.size(); ++i ){
        Bone* b = NULL;
        QString str = values.at(i);

        // Find the mark
        if (str.indexOf("time") >= 0){
            m_timeOffset = i;
        }else if (str.indexOf(CHEST_BOTTOM) >= 0){
            b = getBoneFromName(CHEST_BOTTOM);
        }else if (str.indexOf(HIP) >= 0){
             b = getBoneFromName(HIP);
        }else if (str.indexOf(HEAD) >= 0){
             b = getBoneFromName(HEAD);
        }else if (str.indexOf(LEFT_FOREARM) >= 0){
            b = getBoneFromName(LEFT_FOREARM);
        }else if (str.indexOf(LEFT_LOWER_LEG) >= 0){
             b = getBoneFromName(LEFT_LOWER_LEG);
        }else if (str.indexOf(LEFT_THIGH) >= 0){
             b = getBoneFromName(LEFT_THIGH);
        }else if (str.indexOf(LEFT_UPPER_ARM) >= 0){
             b = getBoneFromName(LEFT_UPPER_ARM);
        }else if (str.indexOf(RIGHT_FOREARM) >= 0){
            b = getBoneFromName(RIGHT_FOREARM);
        }else if (str.indexOf(RIGHT_LOWER_LEG) >= 0){
             b = getBoneFromName(RIGHT_LOWER_LEG);
        }else if (str.indexOf(RIGHT_THIGH) >= 0){
            b = getBoneFromName(RIGHT_THIGH);
        }else if (str.indexOf(RIGHT_UPPER_ARM) >= 0){
             b = getBoneFromName(RIGHT_UPPER_ARM);
        }

        // Set the offset
        if (b != NULL){
            b->setOffset(i);
        }
    }
}

void Skeleton::loadAngleOffsets(const QString &row){
    QStringList values = row.split(',');
    for ( int i = 0; i < values.size(); ++i )    {
        Bone* b = NULL;

        QString str = values.at(i);
        if (str.indexOf("-LeftUpperArm") >= 0){
            b = getBoneFromName(LEFT_FOREARM);
        }else if (str.indexOf("-LeftThigh") >= 0){
            b = getBoneFromName(LEFT_LOWER_LEG);
        }else if (str.indexOf("-RightUpperArm") >= 0){
            b = getBoneFromName(RIGHT_FOREARM);
        }else if (str.indexOf("-RightThigh") >= 0){
            b = getBoneFromName(RIGHT_LOWER_LEG);
        }

        // Set the offset
        if (b != NULL){
            b->setAngleOffset(i);
        }
    }
}

void Skeleton::loadPositionData(const QString &row){
    QStringList values = row.split(',');

    // Get the time
    QString str_time = values.at(m_timeOffset);
    double ts = str_time.toDouble();

    for ( int i = 0; i < m_bones.count(); ++i ){
        Bone* bone = m_bones.at(i);
        if (bone->isDataCaptured()){
            bone->addPosition(ts, values);
        }
    }
}

void Skeleton::loadOrientationData(int index, const QString &row){
    QStringList values = row.split(',');

    for ( int i = 0; i < m_bones.count(); ++i ){
        Bone* bone = m_bones.at(i);
        if (bone->isDataCaptured()){
            bone->addOrientation(index, values);
        }
    }
}

void Skeleton::loadAngleData(int index, const QString &row){
    QStringList values = row.split(',');

    for ( int i = 0; i < m_bones.count(); ++i ){
        Bone* bone = m_bones.at(i);
        if (bone->isAngleEnabled()){
            bone->addAngle(index, values);
        }
    }
}

void Skeleton::postLoadProcessing(){
    for ( int i = 0; i < m_bones.count(); ++i ){
        Bone* bone = m_bones.at(i);
        if (bone->hasData()){
            // If the angle was not loaded, calculate
            if (!bone->isAngleEnabled()){
                calculateAngleOfBoneWithLogic (bone);
            }else{
                bone->setEnabled(true);
            }
        }
    }
}

Bone* Skeleton::getEnabledParentBone(Bone* bone){

    Bone* parent = NULL;
    // go back to find parent bone
    do{
        parent = bone->parentBone();
        if (parent != NULL){
            if (parent->isEnabled())
                return parent;
            else
                bone = parent;
        }

    }while (parent != NULL);

    return NULL;
}

void Skeleton::calculateAngleOfBoneWithLogic(Bone* bone){

    // Let's do this systematically and build the angles starting from the hip
    if (bone->name() == HIP){
        positionBodyWithAngles();
    }
}

Bone* Skeleton::getBoneFromName(const QString &n){
    foreach (Bone* b, m_bones) {
        if (b->name() == n)
            return b;
    }
    return NULL;
}

void Skeleton::positionBodyWithAngles(){
    // Calculate the angle manually
    Bone* hipBone = getBoneFromName(HIP);
    if (hipBone == NULL || !hipBone->hasData())
        return;
    Bone* chestBone = getBoneFromName(CHEST_BOTTOM);
    if (chestBone == NULL || !chestBone->hasData())
        return;
    Bone* tummyBone = getBoneFromName(TUMMY);
    if (tummyBone == NULL)
        return;

    Vector a, b, c;

    // Activate both the hip and the chest
    hipBone->setEnabled(true);
    tummyBone->setEnabled(true);

    int count = chestBone->getSampleCount();
    for (int index = 0; index < count; index++){
        const BonePosition& bpParent = hipBone->getSampleAt(index)->position();
        const BonePosition& bpBone = chestBone->getSampleAt(index)->position();

        // create a couple of Vectors
        a.setCoordinates(bpParent.pos_x(), bpParent.pos_y(), bpParent.pos_z());
        b.setCoordinates(bpBone.pos_x(), bpBone.pos_y(), bpBone.pos_z());

        // default to no bend
        int hipAngle = hipDropAngle(a, b);
        int tummyAngle = 0;
        int beta = 0;

        // Control the movement of the spine
        if (hipAngle > MAX_HIP_ANGLE){

            // Cap the hip to the max
            hipAngle = MAX_HIP_ANGLE;

            // Update the new start position of the angle to the tip of the
            // hip bone
            double length = hipBone->vector().y() / 2;

            // we are using the slop of the line, which is the rise/fall not the drop, so flip it with 90 degrees
            c = Vector::getPointOnLine(90 - MAX_HIP_ANGLE, a, length);

            // Calculate the offset angle
            double A,B;
            int offset = 0;
            if (b.y() > c.y()){
                A = Vector::distanceBetweenVectorsZ(c, b);
                B = Vector::distanceBetweenVectorsY(c, b);
                offset = MAX_HIP_ANGLE;
            }else{
                A = Vector::distanceBetweenVectorsY(c, b);
                B = Vector::distanceBetweenVectorsZ(c, b);
                offset = 180 - MAX_HIP_ANGLE;
            }

            beta = Vector::betaAngle(A, B);

            // get the angle of the new point
            tummyAngle = offset - beta;

        }

        //qDebug() << "Hip angle=" << hipAngle << " Tummy angle=" << tummyAngle << " index=" << index;

        hipBone->setAngle(index, hipAngle);
        tummyBone->setAngle(index, tummyAngle);

        // Set the leg position
        Bone* pBone = getBoneFromName(RIGHT_THIGH);
        if (pBone != NULL && pBone->hasData()){
            setLimbAngle(a, pBone, index, hipAngle);
        }

        pBone = getBoneFromName(LEFT_THIGH);
        if (pBone != NULL && pBone->hasData()){
            setLimbAngle(a, pBone, index, hipAngle);
        }

        // Get the shoulder location
        double length = chestBone->vector().y();
        c = Vector::getPointOnLine(beta, a, length);
        pBone = getBoneFromName(LEFT_UPPER_ARM);
        if (pBone != NULL && pBone->hasData()){
            setLimbAngle(c, pBone, index, tummyAngle);
        }
        pBone = getBoneFromName(RIGHT_UPPER_ARM);
        if (pBone != NULL && pBone->hasData()){
            setLimbAngle(c, pBone, index, tummyAngle);
        }
    }
}

int Skeleton::hipDropAngle(const Vector& a,const  Vector& b){
    // default to no bend (if they point are parallel
    int angle = 0;

    // only deal with forward tilt
    if (b.z() > a.z()){
        // Check the level
        if (b.y() > a.y()){
            // leg A is the Z distance
            double A = Vector::distanceBetweenVectorsZ(a, b);
            // leg B is the Y distance
            double B = Vector::distanceBetweenVectorsY(a, b);

            // Get the alpha angle
            angle = Vector::alphaAngle(A, B);

        }else if(b.y() < a.y()){ // point b is lower

            // leg A is the Y distance
            double A = Vector::distanceBetweenVectorsY(a, b);
            // leg B is the Z distance
            double B = Vector::distanceBetweenVectorsZ(a, b);
            angle = Vector::alphaAngle(A, B);

            // Need to add the 90 degrees above parallel
            angle += 90;
        }else{
            angle = 90; // parallel drop
        }

    }

    return angle;
}

void Skeleton::setHeadAngles(){

}

void Skeleton::setLimbAngle(const Vector& a, Bone* bone, int index, int offsetAngle){
    // Enable it
    bone->setEnabled(true);

    Vector b;

    const BonePosition& bpBone = bone->getSampleAt(index)->position();

    // create a couple of Vectors
    b.setCoordinates(bpBone.pos_x(), bpBone.pos_y(), bpBone.pos_z());

    int angle = 0;

    // leg A is the Y distance
    double A = Vector::distanceBetweenVectorsY(a, b);
    // leg B is the Z distance
    double B = Vector::distanceBetweenVectorsZ(a, b);
    angle = Vector::betaAngle(A, B);

    // Correct for the hip movement (required due to the mesh parent bone semantic
    angle += offsetAngle;

    // Swing it the other way
    angle *= -1;

    // Set the angle
    bone->setAngle(index, angle);

}
