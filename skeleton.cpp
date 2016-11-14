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
        if (bone->isEnabled()){
            bone->addPosition(ts, values);
        }
    }
}

void Skeleton::loadOrientationData(int index, const QString &row){
    QStringList values = row.split(',');

    for ( int i = 0; i < m_bones.count(); ++i ){
        Bone* bone = m_bones.at(i);
        if (bone->isEnabled()){
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
        if (bone->isEnabled()){
            // If the angle was not loaded, calculate
            if (!bone->isAngleEnabled()){
                calculateAngleOfBoneWithLogic (bone);
            }
        }
    }
}

void Skeleton::calculateAngleOfBone(Bone* bone){
    // Get the parent
    Bone* parent = getEnabledParentBone(bone);
    if (parent != NULL){
        int count = bone->getSampleCount();
        for (int index = 0; index < count; index++){
            const BonePosition& bpParent = parent->getSampleAt(index)->position();
            const BonePosition& bpBone = bone->getSampleAt(index)->position();

            // create a couple of Vectors
            Vector* a = new Vector(bpParent.pos_x(), bpParent.pos_y(), bpParent.pos_z());
            Vector* b = new Vector(bpBone.pos_x(), bpBone.pos_y(), bpBone.pos_z());

            // Get the angle
            double angleRad = Vector::betaAngleZY(*a, *b);

            // detect direction
            if (b->z() < a->z())
                angleRad *= -1;
            int angle = (int)angleRad;

            bone->setAngle(index, angle);

            // cleanup
            delete a;
            delete b;

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

    // Let's do this systematically and build the sensors
    if (bone->name() == CHEST_BOTTOM){
        setHipAngles();
    }else if (bone->name() == HEAD){
        setHeadAngles();
    }else if (bone->name() == LEFT_THIGH){
        setLeftThighAngles();
    }else if (bone->name() == LEFT_UPPER_ARM){
        setLeftUpperArmAngles();
    }else if (bone->name() == RIGHT_THIGH){
        setRightThighAngles();
    }else if (bone->name() == RIGHT_UPPER_ARM){
        setRightUpperArmAngles();
    }
}

Bone* Skeleton::getBoneFromName(const QString &n){
    foreach (Bone* b, m_bones) {
        if (b->name() == n)
            return b;
    }
    return NULL;
}

void Skeleton::setHipAngles(){
    // Calculate the angle manually
    Bone* hipBone = getBoneFromName(HIP);
    if (hipBone == NULL || !hipBone->isEnabled())
        return;
    Bone* chestBone = getBoneFromName(CHEST_BOTTOM);
    if (chestBone == NULL || !chestBone->isEnabled())
        return;
    Bone* tummyBone = getBoneFromName(TUMMY);
    if (tummyBone == NULL)
        return;

    int last_angle = 360;

    int count = chestBone->getSampleCount();
    for (int index = 0; index < count; index++){
        const BonePosition& bpParent = hipBone->getSampleAt(index)->position();
        const BonePosition& bpBone = chestBone->getSampleAt(index)->position();

        // create a couple of Vectors
        Vector* a = new Vector(bpParent.pos_x(), bpParent.pos_y(), bpParent.pos_z());
        Vector* b = new Vector(bpBone.pos_x(), bpBone.pos_y(), bpBone.pos_z());

        // default to no bend
        int angle = 0;
        // detect direction
        if (b->z() > a->z()){
            // Get the angle
            double angleRad = Vector::alphaAngleZY(*a, *b);
            angle = Vector::radToDeg(angleRad);
            // set the direction
            //angle *= -1;
        }else if(b->z() < a->z()){
            double angleRad = Vector::alphaAngleZY(*b, *a);
            angle = Vector::radToDeg(angleRad);
            angle *= -1;
        }

        // Set the angle and enable it
        // Smoothing
        int diff = std::abs(last_angle - angle);
        if (diff > ANGLE_SMOOTHING){
            last_angle = angle;
        }
        // Set the angle
        tummyBone->setAngle(index, last_angle);
        tummyBone->setOffset(1);

        // cleanup
        delete a;
        delete b;

    }
}

void Skeleton::setHeadAngles(){

}

void Skeleton::setRightUpperArmAngles(){

}

void Skeleton::setLeftUpperArmAngles(){

}

void Skeleton::setRightThighAngles(){
    // Calculate the angle manually
    Bone* hipBone = getBoneFromName(HIP);
    if (hipBone == NULL || !hipBone->isEnabled())
        return;
    Bone* thighBone = getBoneFromName(RIGHT_THIGH);
    if (thighBone == NULL || !thighBone->isEnabled())
        return;

    int last_angle = 360;

    int count = thighBone->getSampleCount();
    for (int index = 0; index < count; index++){
        const BonePosition& bpParent = hipBone->getSampleAt(index)->position();
        const BonePosition& bpBone = thighBone->getSampleAt(index)->position();

        // create a couple of Vectors
        Vector* a = new Vector(bpParent.pos_x(), bpParent.pos_y(), bpParent.pos_z());
        Vector* b = new Vector(bpBone.pos_x(), bpBone.pos_y(), bpBone.pos_z());

        // default to no bend
        int angle = 0;
        // detect direction
        if (b->z() > a->z()){
            // Get the angle
            double angleRad = Vector::alphaAngleZY(*a, *b);
            angle = Vector::radToDeg(angleRad);
            // set the direction
            angle *= -1;

        }else if(b->z() < a->z()){
            double angleRad = Vector::alphaAngleZY(*b, *a);
            angle = Vector::radToDeg(angleRad);
        }

        // Smoothing
        int diff = std::abs(last_angle - angle);
        if (diff > ANGLE_SMOOTHING){
            last_angle = angle;
        }
        // Set the angle
        thighBone->setAngle(index, last_angle);

        // cleanup
        delete a;
        delete b;

    }
}

void Skeleton::setLeftThighAngles(){
    // Calculate the angle manually
    Bone* hipBone = getBoneFromName(HIP);
    if (hipBone == NULL || !hipBone->isEnabled())
        return;
    Bone* thighBone = getBoneFromName(LEFT_THIGH);
    if (thighBone == NULL || !thighBone->isEnabled())
        return;

    int last_angle = 360;

    int count = thighBone->getSampleCount();
    for (int index = 0; index < count; index++){
        const BonePosition& bpParent = hipBone->getSampleAt(index)->position();
        const BonePosition& bpBone = thighBone->getSampleAt(index)->position();

        // create a couple of Vectors
        Vector* a = new Vector(bpParent.pos_x(), bpParent.pos_y(), bpParent.pos_z());
        Vector* b = new Vector(bpBone.pos_x(), bpBone.pos_y(), bpBone.pos_z());

        // default to no bend
        int angle = 0;
        // detect direction
        if (b->z() > a->z()){
            // Get the angle
            double angleRad = Vector::alphaAngleZY(*a, *b);
            angle = Vector::radToDeg(angleRad);
            // set the direction
            angle *= -1;
        }else if(b->z() < a->z()){
            double angleRad = Vector::alphaAngleZY(*b, *a);
            angle = Vector::radToDeg(angleRad);
        }

        //qDebug() << "set left thigh angle=" << Vector::radToDeg(angle) << " index=" << index;

        // Smoothing
        int diff = std::abs(last_angle - angle);
        if (diff > ANGLE_SMOOTHING){
            last_angle = angle;
        }
        // Set the angle
        thighBone->setAngle(index, last_angle);

        // cleanup
        delete a;
        delete b;

    }
}
