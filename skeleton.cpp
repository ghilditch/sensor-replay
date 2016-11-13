#include "skeleton.h"
#include <QJsonArray>

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
                calculateAngleOfBone (bone);
            }
        }
    }
}

void Skeleton::calculateAngleOfBone(Bone* bone){
    // Get the parent
    Bone* parent = bone->parentBone();
    if (parent != NULL && parent->isEnabled()){
        int count = bone->getSampleCount();
        for (int index = 0; index < count; index++){
            const BonePosition& bpParent = parent->getSampleAt(index)->position();
            const BonePosition& bpBone = bone->getSampleAt(index)->position();

            // create a couple of Vectors
            Vector* a = new Vector(bpParent.pos_x(), bpParent.pos_y(), bpParent.pos_z());
            Vector* b = new Vector(bpBone.pos_x(), bpBone.pos_y(), bpBone.pos_z());

            // Get the angle
            double angle = Vector::betaAngleZY(*a, *b);
            bone->setAngle(index, angle);

            // cleanup
            delete a;
            delete b;

        }
    }
}

Bone* Skeleton::getBoneFromName(const QString &n){
    foreach (Bone* b, m_bones) {
        if (b->name() == n)
            return b;
    }
    return NULL;
}

double Skeleton::hipAngle(int index){
    // Calculate the angle manually
    Bone* hipBone = getBoneFromName(HIP);
    if (hipBone == NULL || !hipBone->isEnabled())
        return 0;
    Bone* chestBone = getBoneFromName(CHEST_BOTTOM);
    if (chestBone == NULL || !chestBone->isEnabled())
        return 0;

    // Get the positions
    const BonePosition& bpHip = hipBone->getSampleAt(index)->position();
    const BonePosition& bpChest = chestBone->getSampleAt(index)->position();

    // create a couple of Vectors
    Vector* a = new Vector(bpHip.pos_x(), bpHip.pos_y(), bpHip.pos_z());
    Vector* b = new Vector(bpChest.pos_x(), bpChest.pos_y(), bpChest.pos_z());

    // Get the angle
    double angle = Vector::betaAngleZY(*a, *b);

    // cleanup
    delete a;
    delete b;

    return angle;
}

double Skeleton::rThighAngle(int index){
    // Calculate the angle manually
    Bone* hipBone = getBoneFromName(HIP);
    if (hipBone == NULL || !hipBone->isEnabled())
        return 0;
    Bone* thighBone = getBoneFromName(RIGHT_THIGH);
    if (thighBone == NULL || !thighBone->isEnabled())
        return 0;

    // Get the positions
    const BonePosition& bpHip = hipBone->getSampleAt(index)->position();
    const BonePosition& bpThigh = thighBone->getSampleAt(index)->position();

    // create a couple of Vectors
    Vector* a = new Vector(bpHip.pos_x(), bpHip.pos_y(), bpHip.pos_z());
    Vector* b = new Vector(bpThigh.pos_x(), bpThigh.pos_y(), bpThigh.pos_z());

    // Get the angle
    double angle = Vector::betaAngleZY(*a, *b);

    // cleanup
    delete a;
    delete b;

    return angle;
}

double Skeleton::lThighAngle(int index){
    // Calculate the angle manually
    Bone* hipBone = getBoneFromName(HIP);
    if (hipBone == NULL || !hipBone->isEnabled())
        return 0;
    Bone* thighBone = getBoneFromName(LEFT_THIGH);
    if (thighBone == NULL || !thighBone->isEnabled())
        return 0;

    // Get the positions
    const BonePosition& bpHip = hipBone->getSampleAt(index)->position();
    const BonePosition& bpThigh = thighBone->getSampleAt(index)->position();

    // create a couple of Vectors
    Vector* a = new Vector(bpHip.pos_x(), bpHip.pos_y(), bpHip.pos_z());
    Vector* b = new Vector(bpThigh.pos_x(), bpThigh.pos_y(), bpThigh.pos_z());

    // Get the angle
    double angle = Vector::betaAngleZY(*a, *b);

    // cleanup
    delete a;
    delete b;

    return angle;
}
